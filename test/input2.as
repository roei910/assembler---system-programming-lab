;errors in first run file

;error - Hello exists twice
Hello:         inc r6
Hello:         inc r6
thisLabelIsWrong  : sub r1, r4

;error - symbol r1 is invalid (saved for register)
r1:         inc r6

;error - symbol inc is invalid (saved for command)
inc:         inc r6

;errors in command opperands
mov r1, #1
add r1, #1
sub r1, #1
lea r1, #1
lea #1, r1
lea r1, r2
clr #1
not #1
inc #1
dec #1
jmp #1
bne #1
jsr #1
red #1
jmp r1
bne r1
jsr r1

;data errors
.data
.data lost, 4, 8, 15, 16, 23, 42

;string errors
.string
.string za wardo
.string za wardo"

;unknown commands
COOK tasty, pie
.Data 12, 675
.sTring "Whoops"

;missing word
.extern
.entry

;comma errors
mov, r3, K
cmp K, , #-6
sub ,r1, r4 
add L3  L3  
add , L3,  L3
inc , r1  



;errors check - first run
;comma errors
myArray: .data 12,6, -9, 10,
myArray2: .data 12,6,-9,10,,
myArray3: .data ,12,6, -9, , 10
myArray4: .data 12,     ,    6,-9,10
myArray5: .data 12, 6, -9, 0, -10       ,    