#!/bin/sh
# compile markdown file and view in lynx(1)
# usage: mdview [file]

smu $1 | lynx -stdin
