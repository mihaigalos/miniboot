#!/usr/bin/env bash
set -x #echo on

make_log=$1

push_message="\`[CI Auto Message]\`\n---\n\`\`\`bash\n$(cat $make_log | grep -A 8 "miniboot.elf  :" | head -n 8 | sed -e ':a;N;$!ba;s/\n/\\n/g')\n\`\`\`"

curl -H "Authorization: token ${GITHUB_TOKEN}" -X POST \
    -d "{\"body\": \"${push_message}\"}" \
    "https://api.github.com/repos/${TRAVIS_REPO_SLUG}/issues/${TRAVIS_PULL_REQUEST}/comments"