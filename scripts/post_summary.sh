#!/bin/bash

set -x

function fail {
    printf '%s\n' "$1" >&2 
    exit "${2-1}"  ## Return a code specified by $2 or 1 by default.
}

[ -n "${TOKEN}" ] && fail "TOKEN variable is not set. Exiting."

build_log=$1

header="CI Auto Message"
body="$(cat $build_log | grep -A 5 "Program: " | head -n 5 | sed -e ':a;N;$!ba;s/\n/\\n/g')"
footer=""
PR_NUMBER=$(echo $GITHUB_REF | awk 'BEGIN { FS = "/" } ; { print $3 }')

echo $PR_NUMBER
#
#   printf -v push_message '`%s`\\n---\\n```bash\\n%s\\n```\\n%s' "$header" "$body" "$footer"

#curl --fail -H "Authorization: token ${TOKEN}" -X POST \
#    -d "{\"body\": \"${push_message}\"}" \
#    "https://api.github.com/repos/${GITHUB_REPOSITORY}/issues/${TRAVIS_PULL_REQUEST}/comments"

exit $?
