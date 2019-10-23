#!/usr/bin/env bash
set -x #echo on

miniboot_elf=$(curl -s "https://api.travis-ci.org/jobs/${TRAVIS_JOB_ID}/log.txt?deansi=true" | grep -A 8 "miniboot.elf  :" | head -n 8)

curl -H "Authorization: token ${GITHUB_TOKEN}" -X POST \
    -d "{"body": "$miniboot_elf"}" \
    "https://api.github.com/repos/${TRAVIS_REPO_SLUG}/issues/${TRAVIS_PULL_REQUEST}/comments"