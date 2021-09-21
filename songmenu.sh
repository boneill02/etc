#!/bin/sh
# songmenu: a MPD client in dmenu

# should be changed if library is using different audio formats
EXT="mp3"

MENU="Menu"
CONTROLS="Choose\nSearch\nPlay\nPause\nPrevious\nNext"
STATE="menu"
ARTIST=""
ALBUM=""
SONG=""
PLAYING=""

choose_artist() { \
	res="$(echo "$MENU\n\n$(mpc ls | sort | uniq)" | dmenu -i -l 20 -p "artist:")"
	[ "$res" = "$MENU" ] && STATE="menu" && return
	[ -z "$res" ] && exit
	ARTIST="$res"
}

choose_album() { \
	res="$(echo "$MENU\nBack\n\n$(mpc ls "$ARTIST" | awk -F "/" '{print $2}' | sort | uniq)" | dmenu -i -l 20 -p "album:")"
	[ -z "$res" ] && exit
	ALBUM="$res"
}

choose_song() { \
	res="$(echo "$MENU\nBack\n\n$(mpc ls "$ARTIST/$ALBUM" | awk -F "/" '{print $3}' | sed "s/\.$EXT//g" | sort | uniq)"\
		| dmenu -i -l 20 -p "song:")"
	[ -z "$res" ] && exit
	SONG="$res.$EXT"
}

choose_state() { \
	[ -z "$ARTIST" ] && choose_artist
	[ "$res" = "$MENU" ] && STATE="menu" && return

	choose_album
	[ "$res" = "$MENU" ] && STATE="menu" && return
	[ "$res" = "Back" ] && ARTIST="" && return

	choose_song
	[ "$res" = "Back" ] && ALBUM="" && return
	[ "$res" = "$MENU" ] && STATE="menu" && return

	mpc insert "$ARTIST/$ALBUM/$SONG"
	mpc next
	ARTIST=""
	ALBUM=""
	SONG=""
}

search_state() { \
	phrase="$(echo "" | dmenu -p "search:")"
	[ -z "$phrase" ] && exit
	results="$(mpc search title "$phrase")"
	res="$(echo "$MENU\n\n$results" | sort | uniq | dmenu -i -l 20 -p "results:")"
	[ "$res" = "$MENU" ] && STATE="menu" && return
	[ -z "$res" ] && exit
	mpc insert "$res"
	mpc next
}

get_song() { \
	PLAYING="$(mpc status | grep "playing" >/dev/null && echo "Playing: $(mpc current)" \
	|| echo "Paused: $(mpc current)")"
}

menu_state() { \
	get_song
	res="$(echo "$PLAYING\n\n$CONTROLS" | dmenu -i -l 20 -p "menu:")"
	case $res in
		"Choose") STATE="choose" ;;
		"Search") STATE="search" ;;
		"Play") mpc play ;;
		"Pause") mpc pause ;;
		"Previous") mpc prev ;;
		"Next") mpc next ;;
		"") exit ;;
	esac
}

do_state() { \
	case $STATE in
		"menu") menu_state ;;
		"choose") choose_state ;;
		"search") search_state ;;
	esac
}

while true; do
	do_state
done
