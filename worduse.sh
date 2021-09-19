#!/bin/sh
# worduse: calculate word frequency from standard input
# usage: worduse

sed 's/--/ /' | tr -cd '[:alpha:] \n' | tr '[:upper:]' '[:lower:]' | awk -v OFS='\n' '{$1=$1}1' | tr -d ' \t' | sort | uniq -c | sort -n
