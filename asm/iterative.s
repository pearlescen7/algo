INDEX EQU 5;	Since I wasn't sure if the program should calculate factorial(index) as the last value I assumed that the last value should be included as well

			AREA arr_space, DATA, READWRITE
array		SPACE INDEX * 4 + 4
array_end	

			AREA iter_fact, CODE, READONLY
			ALIGN
factorial	FUNCTION
			PUSH {r4};					Push r4 to stack to preserve its value, no need to push r0-r3 since they are scratch registers
			LSLS r1, #2;				INDEX = INDEX * 4
			MOVS r3, #1;				Load 1 to r3 for the next line
			STR r3, [r0];				fact(0) = 1 => array[0] = 1
			MOVS r2, #4;				Load 4 to r2 to use as i in the loop
			MOVS r3, #0;				Load 0 to r3 to use as i - 1
loop		CMP r2, r1;					Loop test i > index
			BGT done;					If i > index jump to end
			LDR r4, [r0, r3];			Load array[i-1] to r4
			LSRS r2, #2;				r2 <= r2/4 to do the following calculation
			MULS r4, r2, r4;			Load array[i-1] * i to r4
			LSLS r2, #2;				r2 <= 4*r2 to do get back the index value
			STR r4, [r0, r2];			Store the result to array[i]
			ADDS r2, #4;				i++ => adding four since one integer is 4 bytes and we need to move that much for each index
			ADDS r3, #4;				(i-1)++
			B loop;						Branch back to keep looping
done		POP {r4};					Pop from stack to retrieve the old value of r4
			BX lr;						Return using link
			ENDFUNC
			
			AREA main_code, CODE, READONLY
			ENTRY
			THUMB
			ALIGN
__main		FUNCTION
			EXPORT __main
			LDR r5, =array; 	Load the array address to r5
			MOV r0, r5; 		Load the array address to r0 to pass to the function as a parameter
			MOVS r1, #INDEX;	Load the index to r1 to pass as a parameter
			BL factorial;		Call the factorial function with link
stop		B stop;				Infinite loop			
			ENDFUNC
			END