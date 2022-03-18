; file c.as

Hello:         inc r6
Hello:         inc r6
r1:         inc r6
inc:         inc r6
                macro   m1
                inc     r6
                mov     r3, W
                endm

                m1

                add r1, r3
                
                m1