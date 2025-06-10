@ This ARM Assembler code should implement a matching function, for use in the MasterMind program, as
@ described in the CW2 specification. It should produce as output 2 numbers, the first for the
@ exact matches (peg of right colour and in right position) and approximate matches (peg of right
@ color but not in right position). Make sure to count each peg just once!
	
@ Example (first sequence is secret, second sequence is guess):
@ 1 2 1
@ 3 1 3 ==> 0 1
@ You can return the result as a pointer to two numbers, or two values
@ encoded within one number
@
@ -----------------------------------------------------------------------------

.text
@ this is the matching fct that should be called from the C part of the CW	
.global         matches
@ use the name `main` here, for standalone testing of the assembler code
@ when integrating this code into `master-mind.c`, choose a different name
@ otw there will be a clash with the main function in the C code
.global         main
main: 
	LDR  R2, =secret	@ pointer to secret sequence
	LDR  R3, =guess		@ pointer to guess sequence

	@ you probably need to initialise more values here

	@ ... COMPLETE THE CODE BY ADDING YOUR CODE HERE, you should use sub-routines to structure your code

exit:	@MOV	 R0, R4		@ load result to output register
	MOV 	 R7, #1		@ load system call code
	SWI 	 0		@ return this value

@ -----------------------------------------------------------------------------
@ sub-routines

@ this is the matching fct that should be callable from C	
matches:
    PUSH {LR}          @ Save the link register
    MOV R4, #0         @ Initialize the count for exact matches
    MOV R5, #0         @ Initialize the count for approximate matches

    LDR R2, =secret    @ Load the address of the secret sequence
    LDR R3, =guess     @ Load the address of the guess sequence

    MOV R6, #LEN       @ Load the length of the sequence
    MOV R7, #0         @ Initialize the loop counter

loop:
    CMP R7, R6         @ Check if we have reached the end of the sequence
    BEQ end            @ If yes, exit the loop

    LDR R8, [R2], #4   @ Load the next element from the secret sequence
    LDR R9, [R3], #4   @ Load the next element from the guess sequence

    CMP R8, R9         @ Compare the elements
    BEQ exact_match    @ If they are equal, it's an exact match
    BL check_approx    @ Otherwise, check for approximate match

    ADD R7, R7, #1     @ Increment the loop counter
    B loop             @ Repeat the loop

exact_match:
    ADD R4, R4, #10    @ Increment the count for exact matches
    B loop             @ Continue the loop

check_approx:
    MOV R10, R3        @ Copy the address of the guess sequence
    MOV R11, R8        @ Copy the value of the element from the secret sequence

inner_loop:
    CMP R11, R9        @ Compare the element from the secret sequence with each element from the guess sequence
    BEQ found_approx   @ If an approximate match is found, exit the inner loop

    ADD R10, R10, #4   @ Move to the next element in the guess sequence
    LDR R9, [R10]      @ Load the next element from the guess sequence

    CMP R10, R3        @ Check if we have reached the end of the guess sequence
    BEQ outer_loop     @ If yes, exit the inner loop

    B inner_loop       @ Repeat the inner loop

found_approx:
    ADD R5, R5, #1     @ Increment the count for approximate matches
    B loop             @ Continue the loop

outer_loop:
    ADD R7, R7, #1     @ Increment the loop counter
    B loop             @ Repeat the loop

end:
    MOV R0, R4         @ Put the count for exact matches in R0
    ADD R0, R0, R5, LSL #4  @ Combine the counts for exact and approximate matches
    POP {PC}           @ Return

@ show the sequence in R0, use a call to printf in libc to do the printing, a useful function when debugging 
showseq: 			@ Input: R0 = pointer to a sequence of 3 int values to show
	@ COMPLETE THE CODE HERE (OPTIONAL)
	
	
@ =============================================================================

.data

@ constants about the basic setup of the game: length of sequence and number of colors	
.equ LEN, 3
.equ COL, 3
.equ NAN1, 8
.equ NAN2, 9

@ a format string for printf that can be used in showseq
f4str: .asciz "Seq:    %d %d %d\n"

@ a memory location, initialised as 0, you may need this in the matching fct
n: .word 0x00
	
@ INPUT DATA for the matching function
.align 4
secret: .word 1 
	.word 2 
	.word 1 

.align 4
guess:	.word 3 
	.word 1 
	.word 3 

@ Not strictly necessary, but can be used to test the result	
@ Expect Answer: 0 1
.align 4
expect: .byte 0
	.byte 1

.align 4
secret1: .word 1 
	 .word 2 
	 .word 3 

.align 4
guess1:	.word 1 
	.word 1 
	.word 2 

@ Not strictly necessary, but can be used to test the result	
@ Expect Answer: 1 1
.align 4
expect1: .byte 1
	 .byte 1

.align 4
secret2: .word 2 
	 .word 3
	 .word 2 

.align 4
guess2:	.word 3 
	.word 3 
	.word 1 

@ Not strictly necessary, but can be used to test the result	
@ Expect Answer: 1 0
.align 4
expect2: .byte 1
	 .byte 0

