#!/usr/bin/env bash
set -x #echo on

make_log=$1

miniboot_elf=$(cat $make_log | grep -A 8 "miniboot.elf  :" | head -n 8 | sed -e ':a;N;$!ba;s/\n//g')
markdown_ify="\`\`\`bash\n${miniboot_elf}\n\`\`\`"
push_message="[CI Auto Message]\n${markdown_ify}"


curl -H "Authorization: token ${GITHUB_TOKEN}" -X POST \
    -H "Content-Type: text; charset=utf-8" \
    -d "{"body": "${push_message}"}" \
    "https://api.github.com/repos/${TRAVIS_REPO_SLUG}/issues/${TRAVIS_PULL_REQUEST}/comments"