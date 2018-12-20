#!/bin/bash
############################################
#
# MILO @ 2018
#
# Seafile File Deleter
#
#
############################################

function print_help() {
    echo "Seafile File Deleter";
    echo "MILO @ 2018"
    echo
    echo "This script will remove files in a given Seafile repository,"
    echo "you just need to provide a file list (you can get it from"
    echo "get_files_from_seafile.sh script)."
    echo
    echo "Usage: $0 -f FILES -s SERVER -r REPO [-d DIR] -u NAME -p PASS"
    echo "OR"
    echo "Usage: $0 -f FILES -s SERVER -r REPO [-d DIR] -t TOKEN"
    echo "Parameters:"
    echo "  -f names   files to delete, separated by colons file:file2:file3"
    echo "  -s path    seafile server address"
    echo "  -r name    seafile repository id"
    echo "  -d path    library subdirectory for upload (default: /)"
    echo "  -t token   seafile auth token (use token or user/password pair)"
    echo "  -u name    seafile user (use token or user/password pair)"
    echo "  -p pass    user password (use token or user/password pair)"
    echo
}

# check if tools and paths are valid
# will stop on first error
function sanity_check() {

  type curl
  if [ $? -ne 0 ]; then
    echo "Cannot run curl. Check if it's installed."
    ERROR=1; return
  fi

  type basename
  if [ $? -ne 0 ]; then
    echo "Cannot run basename. Check if it's installed."
    ERROR=2; return
  fi

  if [ -z $DOMAIN ]; then
    echo "Server address not set (-s)"
    ERROR=4; return
  fi

  if [ -z $REPO_ID ]; then
    echo "Repository id not set (-r)"
    ERROR=5; return
  fi

  if [ -z $TOKEN ]; then
    if [ -z $USER ]; then
      echo "Use Token (-t) or User (-u) and Password (-p) pair"
      ERROR=6; return
    fi

    if [ -z $PASSWORD ]; then
      echo "Use Token (-t) or User (-u) and Password (-p) pair"
      ERROR=7; return
    fi
    echo "Token not set. Will try to get it automatically"
  fi
}

function get_token() {
  echo "obtaining token"
  TOKEN=$(curl -d "username=$USER&password=$PASSWORD" $DOMAIN/api2/auth-token/)
  #check if token keyword is in answer
  echo $TOKEN | grep token
  if [ $? -ne 0 ]; then
    echo "Could not get token. Server answer: $TOKEN"
    return
  fi
  #extract same token from answer - we now exactly where it is
  TOKEN=${TOKEN:11:$(expr ${#TOKEN} - 13)}
}

function remove_files() {
  echo "Removing $FILES"
  curl -d "file_names=$FILES" -H "Authorization: Token $TOKEN" $DOMAIN/api2/repos/$REPO_ID/fileops/delete/?p=/$DIRECTORY
}

#main
ERROR=0
while getopts ":hf:s:r:d:t:u:p:" opt ;
do
    case $opt in
        h) print_help;
            exit 0;
            ;;
        f) FILES=$OPTARG
            ;;
        s) DOMAIN=$OPTARG
            ;;
        r) REPO_ID=$OPTARG
            ;;
        d) DIRECTORY=$OPTARG
            ;;
        t) TOKEN=$OPTARG
            ;;
        u) USER=$OPTARG
            ;;
        p) PASSWORD=$OPTARG
            ;;
        *) echo "Wrong parameter";
           echo "For help run $0 -h";
           exit 1
            ;;
    esac
done

sanity_check
if [ $ERROR != 0 ]; then
  exit $ERROR
fi

if [ -z $TOKEN ]; then
  get_token
  if [ -z $TOKEN ]; then
    exit 10
  fi
fi
remove_files
echo "Done"

