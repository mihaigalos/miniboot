#!/bin/bash

set -x

function fail {
    printf '%s\n' "$1" >&2 
    exit "${2-1}"  ## Return a code specified by $2 or 1 by default.
}

build_log=$1
POST_SUMMARY_TOKEN=$2

[ ! -f "${build_log}" ] && fail "Build log file not passed as argument to script. Exiting."
[ -z "${POST_SUMMARY_TOKEN}" ] && fail "POST_SUMMARY_TOKEN variable is not set. Exiting."

header="CI Auto Message"
body="$(cat $build_log | grep -A 5 "Program: " | head -n 5 | sed -e ':a;N;$!ba;s/\n/\\n/g')"
footer=""
PR_NUMBER=$(echo $GITHUB_REF | awk 'BEGIN { FS = "/" } ; { print $3 }')

printf -v push_message '`%s`\\n---\\n```bash\\n%s\\n```\\n%s' "$header" "$body" "$footer"

curl --fail -H "Authorization: token ${POST_SUMMARY_TOKEN}" -X POST \
    -d "{\"body\": \"${push_message}\"}" \
    "https://api.github.com/repos/${GITHUB_REPOSITORY}/issues/${PR_NUMBER}/comments"

exit $?
