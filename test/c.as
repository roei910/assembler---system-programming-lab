; file c.as
.entry LIST
.extern W
MAIN:           add     r3,LIST
LOOP:           prn     #48
                macro   m1
                inc     r6
                mov     r3, W
                sub     r1, r4
                endm
                lea     STR, r6
                m1
                bne     END
                cmp     val1, #-6
                bne     END[r15]
                m1
                dec     K
.entry MAIN
                sub     LOOP[r10], r14
END:            stop
STR:            .string "abcd"
LIST:           .data   6, -9
                .data   -100
.entry K
K:              .data   31
.extern val1