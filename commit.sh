#!/bin/bash -l
#$ -S /bin/bash
echo $1
git add .
git status
git commit -m $1
echo "now u can type $ git commit something_cool"
