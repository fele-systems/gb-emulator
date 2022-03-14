; string-len

INCLUDE "hardware.inc"

SECTION "Header", ROM0[$100]

	jp EntryPoint
	ds $150 - @, 0 ; Make room for the header

EntryPoint:
    ; Stack:
    ; [0] WORD const char* src_string
    ; [1] WORD size_t string_size
    ; [2] WORD char* dst_string

    ; Variables:
    ; HL    @src_iterator
    ; BC    const src_iterator_end
    ; DE    @dst_iterator

    ; A     currentChar // accumulator
    ; BC    counter
    
    pop DE  ; Pop @dst_string
    pop BC  ; Pop @string_size

    ld H, D     ; Calculate the end address
    ld L, E     ; by add @src_string and
    add HL, BC  ; @string_str
    ld B, H     ;
    ld C, L     ;

    pop HL  ; Pop @src_string
Loop:
    ld A, L    ; Compare the lower word
    cp A, C
    jp z, Done
    ld A, H    ; Compare the higher word
    cp A, B     ;
    jp z, Done  ; while src_iterator_end == @src_string {

    ld A, [HLI] ;   Fetch next char
    ld [DE], A  ;   Write next char to @dst_string
    inc DE

    jp Loop     ; }

Done:
    ld H, D
    ld L, E
    ld [HL], 0  ; Null terminator
    JP $0000
