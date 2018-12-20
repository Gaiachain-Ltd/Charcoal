#!/bin/bash
############################################
#
# MILO @ 2016
#
# Seafile File Uploader
#
############################################ 

function print_help() {
    echo "Seafile File Uploader";
    echo "MILO @ 2016"
    echo
    echo "Usage: $0 -f FILE -s SERVER -r REPO [-d DIR] -u NAME -p PASS"
    echo "OR"
    echo "Usage: $0 -f FILE -s SERVER -r REPO [-d DIR] -t TOKEN"
    echo "Parameters:"
    echo "  -f path    path to file to be uploaded"
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

  if [ ! -e $FILE ]; then
    echo "File not exists: $FILE"
    ERROR=3; return
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

function get_link() {
  echo "checking whether file exists"
  FILE_LIST=$(curl -H "Authorization: Token $TOKEN" -H 'Accept: application/json; indent=4' $DOMAIN/api2/repos/$REPO_ID/dir/?p=/$DIRECTORY)
  FILENAME=$(basename $FILE)
  OPERATION=upload-link
  if grep -q "$FILENAME" <<< "$FILE_LIST" ; then
    OPERATION=update-link
  fi

  echo "obtain upload link"
  UPLOAD_LINK=$(curl -H "Authorization: Token $TOKEN" $DOMAIN/api2/repos/$REPO_ID/$OPERATION/)
  #get rid of quotas
  UPLOAD_LINK=${UPLOAD_LINK:1:$(expr ${#UPLOAD_LINK} - 2)}
}

function upload() {
  if [ $OPERATION == update-link ] 
  then
    echo "Updating $FILE"
    curl -H "Authorization: Token $TOKEN" -F file=@$FILE -F filename=$FILENAME -F target_file=/$DIRECTORY/$FILENAME $UPLOAD_LINK
  else
    echo "Uploading $FILE"
    curl -H "Authorization: Token $TOKEN" -F file=@$FILE -F filename=$FILENAME -F parent_dir=/$DIRECTORY $UPLOAD_LINK
  fi
}

#main
ERROR=0
while getopts ":hf:s:r:d:t:u:p:" opt ;
do
    case $opt in
        h) print_help;
            exit 0;
            ;;
        f) FILE=$OPTARG
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
get_link
upload
echo "Done"

