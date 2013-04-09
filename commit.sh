#!/bin/bash -l
#$ -S /bin/bash
git add .
git status
echo "now u can type $ git commit something_cool"
echo "and $ git push origin master"
