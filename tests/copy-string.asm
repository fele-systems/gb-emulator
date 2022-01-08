; string-len

INCLUDE "hardware.inc"

SECTION "Header", ROM0[$100]

	jp EntryPoint
	ds $150 - @, 0 ; Make room for the header

EntryPoint:
    ld SP, $CFFE    ; Initialize Stack Pointer
    ; Stack:
    ; [0xCFFF] const char* string_start
    ; [0xCFFE] 

    ; Registers:
    ; HL    iterator
    ; A     currentChar
    ; BC    counter
    
    pop HL
    ld BC, 0
   
Loop:
    ld A, (HLI) ; Next char
    cp A, 0     ; Check if null terminator
    jp z, Done  ; If 0, end
    inc BC      ; Increment counter
    jp Loop

Done:
    push BC     ; Return
    JP $0000
