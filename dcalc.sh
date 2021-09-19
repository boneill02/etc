#!/bin/sh
# dcalc: a calculator using dmenu
# usage: dcalc

eq="$(dmenu -p "calc:" </dev/null)"
[ ! -z "$eq" ] && notify-send "calc" "$(echo "$eq" | bc -l 2>&1 | sed 's/-/\\-/')"
