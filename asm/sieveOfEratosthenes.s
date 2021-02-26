LIMIT EQU 2
	
			AREA arrs, DATA, READWRITE
p_numbers	SPACE LIMIT * 4
p_end
is_p_number	SPACE LIMIT;		One byte (which is the smallest place I could set with a single instruction) is enough to symbolize true and false
is_p_end;						So I only used STRB and LDRB instructions whenever I used this array since they store a byte or load a byte.


			AREA sievecode, CODE, READONLY
			ALIGN
sieve		FUNCTION
			
			PUSH {r4};			Save r4 to stack to preserve its value after the operations
			MOVS r1, #0; 		Load 0 to r1 to use as i
			MOVS r2, #0;		Load 0 to r2 to use in the store instruction
			MOVS r3, #1;		Load 1 to r3 to use as true in the store instruction
first		CMP r1, r0;			Check if i < limit => limit is already in r0 since passed as a parameter
			BEQ first_end;		Branch to the end of the loop if it is done
			STRB r3, [r6, r1]; 	isPrimeNumber[i] = true
			LSLS r1, #2;		Left shift r1 by 2 to use i as the offset value
			STR	r2, [r5, r1];	primeNumbers[i] = 0 
			LSRS r1, #2;		Fix r1 value by shifting right
			ADDS r1, #1;		i++
			B first;			Branch back for the loop
first_end
			
			MOVS r1, #2;		Load 2 to r1 to use as i
second		MOV r2, r1;			Load r1 to r2 since MULS instruction wants the source and destination registers as the same
			MULS r2, r1, r2;	Load i * i to r2
			CMP r2, r0;			Check if i * i <= limit
			BGT second_end;		If done jump to end of outer loop
			LDRB r3, [r6, r1];	Load isPrimeNumber[i] to r3
			CMP r3, #1;			Check if it is a prime number using r3
			BNE second_back;	Jump to next iteration if not a prime
			
second_mid	CMP r2, r0;			Check if i * i, i * i + i ... sequence <= limit
			BGT second_back;	If not jump to next iteration
			MOVS r4, #0;		Move 0 to r4 to use in store instruction
			STRB r4, [r6, r2];	isPrimeNumber[j] = false
			ADDS r2, r1;		i*i + i, i * i + 2i => adding the i to r2
			B second_mid;		Branch back to mid loop
			
second_back	ADDS r1, #1;		i++
			B second;			Branch back to the start of the outer loop
second_end	

			MOVS r1, #0;		Load 0 to r1 to use as index
			MOVS r2, #2;		Load 2 to r2 to use as i
third		CMP r2, r0;			Check if i < limit => limis is already in r0 since passed as a parameter
			BEQ third_end;		Branch to the end if the loop is done
			LDRB r3, [r6, r2];	Load isPrimeNumber[i] to r3
			CMP r3, #1;			Check if it is a prime number using r3	
			BNE	third_back;		If not jump to the i++ line
			STR r2, [r5, r1];	primeNumbers[index] = i 
			ADDS r1, #4; 		Add 4 to index since we never use its literal value and always use it as a register offset
third_back	ADDS r2, #1;		i++
			B third;			Branch back to the start of the loop
third_end	POP {r4};			Restore r4
			BX lr
			ENDFUNC

			AREA main_code, CODE, READONLY
			ENTRY;					Define entry point	
			THUMB;					Switch to thumb mode
			ALIGN;					Align memory
__main		FUNCTION;				Declare main function
			EXPORT __main;			Make it global so that the linker can see it
			LDR r5, =p_numbers;		Load the address of primeNumbers to r5
			LDR r6, =is_p_number;	Load the address of the isPrimeNumber to r6
			MOVS r0, #LIMIT;		Load limit to r0 to pass as a parameter
			BL sieve;				Call the sieve function
stop		B stop;					Infinite loop
			ENDFUNC
			END