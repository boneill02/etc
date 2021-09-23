#!/bin/sh
# worduse: calculate word frequency from standard input
# usage: worduse

tr -cd "[:alpha:] \n" | tr '[:upper:] ' '[:lower:]\n' | sort | uniq -c | sed 1d | sort -n
