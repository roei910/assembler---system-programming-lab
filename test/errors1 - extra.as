;errors check - first run

;label errors
myLabel: .string "This should be fine"
thisLabelIsErroneous  : sub r1, r4
myLabel: .string "This label was already used on first line"

;comma errors
myArray: .data 12,6, -9, 10,
myArray2: .data 12,6,-9,10,,
myArray3: .data ,12,6, -9, , 10
myArray4: .data 12,     ,    6,-9,10
myArray5: .data 12, 6, -9, 0, -10       ,    
mov, r3, K
cmp K, , #-6
sub ,r1, r4 
add , L3  L3  
add , L3,  L3
inc , r1  

;data errors
.data
.data lost, 4, 8, 15, 16, 23, 42

;string errors
.string
.string za wardo
.string za wardo"

;error with commands
mov A, r1, r2
mov A, r1 r2 
cmp A, r1, r2
add A, r0, r1
sub #3, r1, r5
lea HAHAHA, r3, r4
clr r2, r6
inc r1, r4 
dec Count, Monte, Cristo
jmp ROOF, FLOOR
bne Bob, Sponge
jsr BAT, MAN
red r3, r5
prn r1, r2
mov A
cmp A
add A
sub #3
lea HAHAHA
clr
inc
dec
jmp
bne
jsr
red
prn

;missing word
.data 
.string 
.extern
.entry 

;unknown words
COOK tasty, pie
.Data 12, 675
.sTring "Whoops"

;operand addressing errors
mov ohno, #1
mov ohno, ohyes
mov ohno, someLabel47
mov ohno, r3
mov #-1, #5
mov #-1, ohyes
cmp ohno, ohyes
add ohno, #1
add ohno, ohyes
add ohno, someLabel47
add ohno, r3
add #-1, #5
add #-1, ohyes
sub ohno, #1
sub ohno, ohyes
sub ohno, someLabel47
sub ohno, r3
sub #-1, #5
sub #-1, ohyes
lea #-1, #8
lea #-1, someLabel47
lea #-1, ohno
lea #-1, r4
lea ohno, #-1
lea ohno, ohyes
lea ohno, someLabel47
lea ohno, r0
lea r1, #-1
lea r1, ohno
lea r1, someLabel47
lea r1, r0
lea someLabel47, #-1
lea someLabel47, ohno
clr #-1
clr ohno
not #-1
not ohno
inc #-1
inc ohno
dec #-1
dec ohno
jmp #-1
jmp r1
bne #-1
bne r1
jsr #-1
jsr r1
red #-1
red ohno
prn ohno
rts #-1
rts someLabel47
rts ohno
rts r1
stop #-1
stop someLabel47
stop ohno
stop r1