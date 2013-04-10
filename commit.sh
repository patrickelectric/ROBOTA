#!/bin/bash -l
#$ -S /bin/bash
git add .
git status
echo ""
echo "now u can type $ git commit -m something_cool"
echo "and $ git push origin master"
