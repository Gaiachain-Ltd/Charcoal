#!/usr/bin/env python3
import sys
import os
from shutil import which
import subprocess
import urllib.request
import json
import mconfig

class ClangFormat:
    def run(self):
        self.check_dependencies()
        sha = self.sha_to_compare()

        print("Running mclang-format...")
        diff = subprocess.check_output(["git", mconfig.FORMAT_TOOL, "--diff", "--commit={0}".format(sha)], universal_newlines=True).strip()
        print(diff)
        # validate diff and provide summary
        self.validate(diff)

    # check for software which is needed to clang-format, see -> https://stackoverflow.com/questions/11210104/check-if-a-program-exists-from-a-python-script
    def check_dependencies(self):
        # first check for clang-format-3.8
        if which(mconfig.FORMAT_TOOL) is None:
            print(mconfig.FORMAT_TOOL + " is not installed or does not included in PATH")
            sys.exit(1)

    # retrieve sha to compare based on the place where we run script (gitlabCI/locally)
    def sha_to_compare(self):
        sha = None
        # in case when we run script from GitLab CI
        if "GITLAB_CI" in os.environ:
            # retrieve sha of last successful pipeline
            sha = self.successful_pipeline_sha()
            if sha is None:
                # retrieve sha of initial commit, see -> https://stackoverflow.com/questions/5188914/how-to-show-first-commit-by-git-log
                sha = subprocess.check_output(["git", "rev-list", "--max-parents=0", "HEAD"], universal_newlines=True).strip()
        else:
            # in case when we run script locally
            sha = subprocess.check_output(["git", "rev-list", "--no-merges", "-n 1", "HEAD"], universal_newlines=True).strip()
        return sha

    # validate output of clang-format and provide summary
    def validate(self, diff):
        # retrieve errors
        errors = 0
        for line in diff.split('\n'):
            if line and line.split(' ', 1)[0].startswith('+') and line.split(' ', 1)[0].count('+') is 1:
                errors += 1
        print("\n=== " + str(errors) + " error(s) was found\n")
        if errors:
            sys.exit(1)
        sys.exit(0)

    # retrieve 20 pipelines from page 'page' ( default == 1)
    # we should not change number of pipelines because probably in most cases in the latest 20 pipelines we will be able 
    # to find at least one which finished with success (better performance) 
    def pipelines(self, page=1):
        # url to retrieve Gitlab variables
        url = '{0}/api/v4/projects/{1}/pipelines??per_page=20&page={1}'.format(mconfig.GIT_URL).format(os.getenv("CI_PROJECT_ID"), page)
        # create the request object and set headers
        request = urllib.request.Request(url)
        request.add_header('PRIVATE-TOKEN', mconfig.API_TOKEN)
        # make the request object
        response = urllib.request.urlopen(request)
        # convert bytes to string
        data = response.read().decode('utf-8')
    
        # return as json
        return json.loads(data)

    # check whether in 'pipelines' exists at least one which finished with success
    def successful_pipeline(self, pipelines):
        for pipeline in pipelines:
            if pipeline['status'] == 'success' and pipeline['ref'] == os.getenv("CI_COMMIT_REF_NAME"):
                return pipeline
        return None

    # if 'pipeline' is valid, retrieve and return 'sha', otherwise return 'None'
    def retrieve_sha(self, pipeline):
        if pipeline:
            return pipeline['sha']
        else:
            return None

    # go through all pipelines in current branch and return the first which finished with success or 'None'
    def successful_pipeline_sha(self):
        pipelines = self.pipelines()
        sha = self.retrieve_sha(self.successful_pipeline(pipelines))
        # search until sha is not 'None' or until we have no more pipelines
        pipelines = 0
        page = 1
        while sha is None:
            page += 1
            pipelines = self.pipelines(page)
            if not pipelines:
                break
            sha = self.retrieve_sha(self.successful_pipeline(pipelines))
        return sha

# main
if __name__ == "__main__":
    app = ClangFormat()
    app.run()
