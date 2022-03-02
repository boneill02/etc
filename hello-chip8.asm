start:
	CLS

	;hello
	LD I, sprite_h
	CALL draw
	LD I, sprite_e
	CALL draw
	LD I, sprite_l
	CALL draw
	CALL draw
	LD I, sprite_o
	CALL draw
	LD I, sprite_comma
	CALL draw

	; newline
	ADD V1, 8
	AND V0, V2

	;world
	LD I, sprite_w
	CALL draw
	LD I, sprite_o
	CALL draw
	LD I, sprite_r
	CALL draw
	LD I, sprite_l
	CALL draw
	LD I, sprite_d
	CALL draw
	LD I, sprite_exclamation
	CALL draw

	CALL loop

draw:
	DRW V0, V1, 5
	ADD V0, 8
	RET

loop:
	AND V1, V2 ;random inst
	JP loop

sprite_h: db
	%10010000,
	%10010000,
	%11110000,
	%10010000,
	%10010000

sprite_e: db
	%11110000,
	%10000000,
	%11110000,
	%10000000,
	%11110000

sprite_l: db
	%10000000,
	%10000000,
	%10000000,
	%10000000,
	%11110000

sprite_o: db
	%11110000,
	%10010000,
	%10010000,
	%10010000,
	%11110000

sprite_comma: db
	%00000000,
	%00000000,
	%00000000,
	%10000000,
	%10000000

sprite_w: db
	%10001000,
	%10001000,
	%10101000,
	%10101000,
	%11111000

sprite_r: db
	%11111000,
	%10001000,
	%11110000,
	%10001000,
	%10001000

sprite_d: db
	%11100000,
	%10010000,
	%10010000,
	%10010000,
	%11100000

sprite_exclamation: db
	%10000000,
	%10000000,
	%10000000,
	%00000000,
	%10000000
