INDEX EQU 5; Again I assumed that the program calculates factorial(index) as the last value
	
			AREA arr, DATA, READWRITE
			ALIGN
array		SPACE INDEX * 4 + 4
array_end
			
			AREA fact_code, CODE, READONLY
			ALIGN
factorial	FUNCTION
			PUSH {lr};				Push lr to preserve the value in the next calls
			CMP r0, #2;				Check if n < 2
			BGE next_call;			If not smaller than 2 then do the next iteration
			B fact_end;				If is smaller than 2 jump to end
next_call	MULS r2, r0, r2;		r2 has the previous values so multiply it with current n which is r0
			SUBS r0, r0, #1;		Do n--
			BL factorial;			Call factorial again using link
fact_end	POP {pc};				Fix program counters value using stack
			ENDFUNC
			
			AREA main_code, CODE, READONLY
			ENTRY
			THUMB
			ALIGN
__main		FUNCTION
			EXPORT __main
			LDR r5, =array;			Store array address to r5
			MOVS r4, #0;			Initialize r4 as 0 to use as i
mloop		CMP r4, #INDEX;			i > INDEX test
			BGT stop;				If the loop is done jump to end
			MOV r0, r4;				Move the index value to r0 to pass it as an argument
			MOVS r2, #1;			Initialize the return value as 1
			BL factorial;			Call the function using branch with link
			LSLS r4, #2;			Left shift r4 by 2 to use as offset
			STR	r2, [r5, r4];		Store the result to the array using r2 register as return value
			LSRS r4, #2;			Fix r4 
			ADDS r4, #1;			i++
			B mloop;				Jump back to the start of the main loop
stop		B stop;					Infinite loop
			ENDFUNC
			END