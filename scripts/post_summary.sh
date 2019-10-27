#!/usr/bin/env bash
set -x #echo on

if [[ "${TRAVIS_PULL_REQUEST:-false}" = false ]]; then
    echo "Change is not a PR, nowhere to push summary to. Exiting."
    exit
fi

make_log=$1

header="CI Auto Message"
body="$(cat $make_log | grep -A 8 "miniboot.elf  :" | head -n 8 | sed -e ':a;N;$!ba;s/\n/\\n/g')"
footer=""

printf -v push_message '\`%s\`\\n---\\n\`\`\`bash\\n%s\\n\`\`\`\\n%s' "$header" "$body" "$footer"

curl --fail -H "Authorization: token ${GITHUB_TOKEN}" -X POST \
    -d "{\"body\": \"${push_message}\"}" \
    "https://api.github.com/repos/${TRAVIS_REPO_SLUG}/issues/${TRAVIS_PULL_REQUEST}/comments"

exit $?