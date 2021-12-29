INCLUDE "hardware.inc"

SECTION "Header", ROM0[$100]

	jp EntryPoint

	ds $150 - @, 0 ; Make room for the header

EntryPoint:
    LD HL, $C000
    LD A, [HLI]
    LD C, A
    LD A, [HL]
    ADD A, C
    JP $0000
Done:
	jp Done
