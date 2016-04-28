.data $interrupt
.data $systrap
JUMP $startup__
POP
HALT
.dataseg
BP_REG:
.data 0
.codeseg
.dataseg
LP_REG:
.data 1
.codeseg
.dataseg
IP_REG:
.data 2
.codeseg
.dataseg
SP_REG:
.data 3
.codeseg
.dataseg
FP_REG:
.data 4
.codeseg
.dataseg
FLAG_REG:
.data 5
.codeseg
.dataseg
WORD_SIZE:
.data 4
.codeseg
.dataseg
FL_HALTED:
.data 1
.codeseg
.dataseg
FL_USER_MODE:
.data 2
.codeseg
.dataseg
FL_INT_MODE:
.data 4
.codeseg
.dataseg
FL_INT_DIS:
.data 8
.codeseg
.dataseg
FL_INT_PENDING:
.data 16
.codeseg
.dataseg
FL_VMEM:
.data 32
.codeseg
; 0 strlen

strlen:
ADJSP 8
PUSH 0
DUP
PUSH 0
PUSHFP
PLUS
POPVARIND
POP
LABEL_2:
PUSHVAR -12
PUSHCVARIND
PUSH 0
NE
JUMPE $LABEL_3
PUSHVAR 0
PUSH 1
PLUS
DUP
PUSH 0
PUSHFP
PLUS
POPVARIND
POP
PUSHVAR -12
PUSH 1
PLUS
DUP
PUSH -12
PUSHFP
PLUS
POPVARIND
POP
JUMP $LABEL_2
LABEL_3:
PUSHVAR 0
RETURNV
PUSH 0
RETURNV
; 0 strcpy

strcpy:
ADJSP 8
PUSHVAR -12
DUP
PUSH 0
PUSHFP
PLUS
POPVARIND
POP
LABEL_4:
PUSHVAR -16
PUSHCVARIND
PUSH 0
NE
JUMPE $LABEL_5
PUSHVAR -16
PUSHCVARIND
DUP
PUSHVAR 0
POPCVARIND
POP
PUSHVAR 0
PUSH 1
PLUS
DUP
PUSH 0
PUSHFP
PLUS
POPVARIND
POP
PUSHVAR -16
PUSH 1
PLUS
DUP
PUSH -16
PUSHFP
PLUS
POPVARIND
POP
JUMP $LABEL_4
LABEL_5:
PUSH 0
DUP
PUSHVAR 0
POPCVARIND
POP
PUSHVAR -12
RETURNV
PUSH 0
RETURNV
; 0 strncpy

strncpy:
ADJSP 12
PUSHVAR -12
DUP
PUSH 0
PUSHFP
PLUS
POPVARIND
POP
PUSH 0
DUP
PUSH 4
PUSHFP
PLUS
POPVARIND
POP
LABEL_6:
PUSHVAR -16
PUSHCVARIND
PUSH 0
NE
DUP
JUMPE $LABEL_8
PUSHVAR 4
PUSHVAR -20
LT
AND
LABEL_8:
JUMPE $LABEL_7
PUSHVAR -16
PUSHCVARIND
DUP
PUSHVAR 0
POPCVARIND
POP
PUSHVAR 0
PUSH 1
PLUS
DUP
PUSH 0
PUSHFP
PLUS
POPVARIND
POP
PUSHVAR -16
PUSH 1
PLUS
DUP
PUSH -16
PUSHFP
PLUS
POPVARIND
POP
PUSHVAR 4
PUSH 1
PLUS
DUP
PUSH 4
PUSHFP
PLUS
POPVARIND
POP
JUMP $LABEL_6
LABEL_7:
PUSHVAR 4
DUP
PUSH 4
PUSHFP
PLUS
POPVARIND
POP
LABEL_9:
PUSHVAR 4
PUSHVAR -20
LT
JUMPE $LABEL_10
PUSH 0
DUP
PUSHVAR -12
PUSHVAR 4
PLUS
POPCVARIND
POP
PUSHVAR 4
PUSH 1
PLUS
DUP
PUSH 4
PUSHFP
PLUS
POPVARIND
POP
JUMP $LABEL_9
LABEL_10:
PUSHVAR -12
RETURNV
PUSH 0
RETURNV
; 0 strchr

strchr:
LABEL_11:
PUSHVAR -12
PUSHCVARIND
PUSHVAR -16
NE
DUP
JUMPE $LABEL_13
PUSHVAR -12
PUSHCVARIND
AND
LABEL_13:
JUMPE $LABEL_12
PUSHVAR -12
PUSH 1
PLUS
DUP
PUSH -12
PUSHFP
PLUS
POPVARIND
POP
JUMP $LABEL_11
LABEL_12:
PUSHVAR -12
PUSHCVARIND
PUSHVAR -16
NE
JUMPE $LABEL_14
PUSH 0
RETURNV
LABEL_14:
PUSHVAR -12
RETURNV
PUSH 0
RETURNV
; 0 strcmp

strcmp:
ADJSP 12
LABEL_15:
PUSHVAR -12
PUSHCVARIND
PUSH 0
NE
DUP
JUMPE $LABEL_18
JUMP $LABEL_17
LABEL_18:
PUSHVAR -16
PUSHCVARIND
PUSH 0
NE
OR
LABEL_17:
JUMPE $LABEL_16
PUSHVAR -12
PUSHCVARIND
PUSHVAR -16
PUSHCVARIND
MINUS
DUP
PUSH 4
PUSHFP
PLUS
POPVARIND
POP
PUSHVAR 4
PUSH 0
NE
JUMPE $LABEL_19
PUSHVAR 4
RETURNV
LABEL_19:
PUSHVAR -12
PUSH 1
PLUS
DUP
PUSH -12
PUSHFP
PLUS
POPVARIND
POP
PUSHVAR -16
PUSH 1
PLUS
DUP
PUSH -16
PUSHFP
PLUS
POPVARIND
POP
JUMP $LABEL_15
LABEL_16:
PUSH 0
RETURNV
PUSH 0
RETURNV
; 0 strrev

strrev:
ADJSP 16
PUSHVAR -12
PUSH 0
EQ
JUMPE $LABEL_20
PUSH 0
RETURNV
LABEL_20:
PUSHVAR -12
DUP
PUSH 4
PUSHFP
PLUS
POPVARIND
POP
PUSHVAR -12
DUP
PUSH 0
PUSHFP
PLUS
POPVARIND
POP
LABEL_21:
PUSHVAR 4
PUSHCVARIND
JUMPE $LABEL_22
PUSHVAR 4
PUSH 1
PLUS
DUP
PUSH 4
PUSHFP
PLUS
POPVARIND
POP
JUMP $LABEL_21
LABEL_22:
PUSHVAR 4
PUSH 1
MINUS
DUP
PUSH 4
PUSHFP
PLUS
POPVARIND
POP
LABEL_23:
PUSHVAR 0
PUSHVAR 4
LT
JUMPE $LABEL_24
PUSHVAR 0
PUSHCVARIND
DUP
PUSH 8
PUSHFP
PLUS
POPVARIND
POP
PUSHVAR 4
PUSHCVARIND
DUP
PUSHVAR 0
POPCVARIND
POP
PUSHVAR 8
DUP
PUSHVAR 4
POPCVARIND
POP
PUSHVAR 0
PUSH 1
PLUS
DUP
PUSH 0
PUSHFP
PLUS
POPVARIND
POP
PUSHVAR 4
PUSH 1
MINUS
DUP
PUSH 4
PUSHFP
PLUS
POPVARIND
POP
JUMP $LABEL_23
LABEL_24:
PUSHVAR -12
RETURNV
PUSH 0
RETURNV
; 0 itostr

itostr:
ADJSP 12
PUSH 0
DUP
PUSH 4
PUSHFP
PLUS
POPVARIND
POP
PUSHVAR -16
DUP
PUSH 0
PUSHFP
PLUS
POPVARIND
POP
PUSHVAR -12
PUSH 0
EQ
JUMPE $LABEL_25
PUSH 48
DUP
PUSHVAR -16
PUSH 0
PLUS
POPCVARIND
POP
PUSH 0
DUP
PUSHVAR -16
PUSH 1
PLUS
POPCVARIND
POP
PUSHVAR -16
RETURNV
LABEL_25:
PUSHVAR -12
PUSH 0
LT
JUMPE $LABEL_26
PUSH 1
DUP
PUSH 4
PUSHFP
PLUS
POPVARIND
POP
PUSHVAR -12
NEG
DUP
PUSH -12
PUSHFP
PLUS
POPVARIND
POP
LABEL_26:
LABEL_27:
PUSHVAR -12
PUSH 0
GT
JUMPE $LABEL_28
PUSHVAR -12
PUSH 10
MOD
PUSH 48
PLUS
DUP
PUSHVAR 0
POPCVARIND
POP
PUSHVAR 0
PUSH 1
PLUS
DUP
PUSH 0
PUSHFP
PLUS
POPVARIND
POP
PUSHVAR -12
PUSH 10
DIVIDE
DUP
PUSH -12
PUSHFP
PLUS
POPVARIND
POP
JUMP $LABEL_27
LABEL_28:
PUSHVAR 4
JUMPE $LABEL_29
PUSH 45
DUP
PUSHVAR 0
POPCVARIND
POP
PUSHVAR 0
PUSH 1
PLUS
DUP
PUSH 0
PUSHFP
PLUS
POPVARIND
POP
LABEL_29:
PUSH 0
DUP
PUSHVAR 0
POPCVARIND
POP
PUSHVAR -16
CALL $strrev
POP
PUSHVAR -16
RETURNV
PUSH 0
RETURNV
.dataseg
PRINTS_CALL:
.data 1
.codeseg
.dataseg
EXIT_CALL:
.data 2
.codeseg
.dataseg
GETS_CALL:
.data 5
.codeseg
.dataseg
GETI_CALL:
.data 7
.codeseg
; 0 interrupt

interrupt:
PUSH 0
RETURNV
; 0 systrap

systrap:
PUSH -20
PUSHFP
PLUS
PUSH 0
PLUS
PUSHVARIND
PUSH 1
EQ
JUMPE $LABEL_30
PUSH -20
PUSHFP
PLUS
PUSH 4
PLUS
PUSHVARIND
OUTS
NOP
JUMP $LABEL_31
LABEL_30:
PUSH -20
PUSHFP
PLUS
PUSH 0
PLUS
PUSHVARIND
PUSH 2
EQ
JUMPE $LABEL_32
HALT
JUMP $LABEL_33
LABEL_32:
PUSH $LABEL_34
.dataseg
LABEL_34:
.string "Invalid operator"
.codeseg
OUTS
LABEL_33:
LABEL_31:
RTI
PUSH 0
RETURNV
; 0 syscall

syscall:
PUSHVAR -12
PUSH 0
PLUS
PUSHVARIND
PUSH 1
EQ
JUMPE $LABEL_35
PUSHVAR -12
PUSH 4
PLUS
PUSHVARIND
OUTS
JUMP $LABEL_36
LABEL_35:
PUSHVAR -12
PUSH 0
PLUS
PUSHVARIND
PUSH 2
EQ
JUMPE $LABEL_37
HALT
JUMP $LABEL_38
LABEL_37:
PUSHVAR -12
PUSH 0
PLUS
PUSHVARIND
PUSH 5
EQ
JUMPE $LABEL_39
PUSH -12
PUSHFP
PLUS
INP
LABEL_40:
PUSHVAR -12
PUSH 0
PLUS
PUSHVARIND
PUSH 0
GE
JUMPE $LABEL_41
JUMP $LABEL_40
LABEL_41:
JUMP $LABEL_42
LABEL_39:
PUSHVAR -12
PUSH 0
PLUS
PUSHVARIND
PUSH 7
EQ
JUMPE $LABEL_43
PUSH -12
PUSHFP
PLUS
INP
LABEL_44:
PUSHVAR -12
PUSH 0
PLUS
PUSHVARIND
PUSH 0
GE
JUMPE $LABEL_45
JUMP $LABEL_44
LABEL_45:
JUMP $LABEL_46
LABEL_43:
PUSH $LABEL_47
.dataseg
LABEL_47:
.string "Invalid operator"
.codeseg
OUTS
LABEL_46:
LABEL_42:
LABEL_38:
LABEL_36:
PUSH 0
RETURNV
PUSH 0
RETURNV
; 0 startup__

startup__:
CALL $main
POP
HALT
PUSH 0
RETURNV
; 0 sys_prints

sys_prints:
PUSHVAR -12
OUTS
PUSH 0
RETURNV
PUSH 0
RETURNV
; 0 halt

halt:
PUSH 0
PUSH 2
CALL $syscall
SWAP
POP
SWAP
POP
POP
PUSH 0
RETURNV
; 0 main

main:
ADJSP 12
PUSH $LABEL_48
.dataseg
LABEL_48:
.string "Testing Prints\n"
.codeseg
CALL $prints
POP
PUSH $LABEL_49
.dataseg
LABEL_49:
.string "Hello World\n"
.codeseg
CALL $prints
POP
PUSH 10
CALL $printi
SWAP
POP
POP
PUSH $LABEL_50
.dataseg
LABEL_50:
.string "\n"
.codeseg
CALL $prints
POP
PUSH 0
RETURNV
PUSH 0
RETURNV
; 0 prints

prints:
ADJSP 16
PUSH 1
DUP
PUSH 0
PUSHFP
PLUS
PUSH 0
PLUS
POPVARIND
POP
PUSHVAR -12
DUP
PUSH 0
PUSHFP
PLUS
PUSH 4
PLUS
POPVARIND
POP
PUSH 0
PUSHFP
PLUS
CALL $syscall
SWAP
POP
SWAP
POP
SWAP
POP
POP
PUSH 0
RETURNV
PUSH 0
RETURNV
; 0 printi

printi:
ADJSP 36
PUSH 12
PUSHFP
PLUS
PUSHVAR -12
CALL $itostr
SWAP
POP
SWAP
POP
POP
PUSH 1
DUP
PUSH 0
PUSHFP
PLUS
PUSH 0
PLUS
POPVARIND
POP
PUSH 12
PUSHFP
PLUS
DUP
PUSH 0
PUSHFP
PLUS
PUSH 4
PLUS
POPVARIND
POP
PUSH 0
PUSHFP
PLUS
CALL $syscall
SWAP
POP
SWAP
POP
SWAP
POP
POP
PUSH 0
RETURNV
PUSH 0
RETURNV
; 0 gets

gets:
ADJSP 16
PUSH 5
DUP
PUSH 0
PUSHFP
PLUS
PUSH 0
PLUS
POPVARIND
POP
PUSHVAR -12
DUP
PUSH 0
PUSHFP
PLUS
PUSH 4
PLUS
POPVARIND
POP
PUSH 0
PUSHFP
PLUS
CALL $syscall
SWAP
POP
SWAP
POP
SWAP
POP
POP
PUSH 0
PUSHFP
PLUS
PUSH 4
PLUS
PUSHVARIND
DUP
PUSH -12
PUSHFP
PLUS
POPVARIND
POP
PUSH 0
RETURNV
PUSH 0
RETURNV
