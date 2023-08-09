#!/bin/bash
git add .
echo "Enter Commit message:"
read commitm
git commit -m "$commitm"
git push
