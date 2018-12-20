#!/usr/bin/env python3
import os
import stat
import shutil

PRE_COMMIT_HOOK_CONTENT = """#!/usr/bin/env python3
import sys
import subprocess

res = subprocess.call(["python3", "./milo/mscripts/hooks/mclang-format.py"])
if res is 0:
    res = subprocess.call(["python3", "./milo/mscripts/hooks/mclang-tidy.py"])
sys.exit(res)
"""

class GitHooksInstaller:
    def run(self):
        # create config file (not tracked by git)
        shutil.copy2("milo/mscripts/hooks/mconfig-sample.py", "milo/mscripts/hooks/mconfig.py")

        os.chdir(".git/hooks")
        
        # create 'pre-commit' file and write content of hook
        with open("pre-commit","w") as pre_commit_hook:
            pre_commit_hook.write(PRE_COMMIT_HOOK_CONTENT)
            
        # make it exec, see -> https://stackoverflow.com/questions/12791997/how-do-you-do-a-simple-chmod-x-from-within-python
        st = os.stat("pre-commit")
        os.chmod("pre-commit", st.st_mode | stat.S_IEXEC)
        
        print("pre-commit hook has been installed\n")
        print("Please update the configuration in milo/mscripts/hooks/mconfig.py!")

# main
if __name__ == "__main__":
    app = GitHooksInstaller()
    app.run()
