;*******************************************************************************
;@file				 Main.s
;@project		     Microprocessor Systems Term Project
;@date
;
;@PROJECT GROUP
;@groupno
;@member1
;@member2
;@member3
;@member4
;@member5
;*******************************************************************************
;*******************************************************************************
;@section 		INPUT_DATASET
;*******************************************************************************

;@brief 	This data will be used for insertion and deletion operation.
;@note		The input dataset will be changed at the grading. 
;			Therefore, you shouldn't use the constant number size for this dataset in your code. 
				AREA     IN_DATA_AREA, DATA, READONLY
IN_DATA			DCD		0x10, 0x20, 0x15, 0x65, 0x25, 0x01, 0x01, 0x12, 0x65, 0x25, 0x85, 0x46, 0x10, 0x00
END_IN_DATA

;@brief 	This data contains operation flags of input dataset. 
;@note		0 -> Deletion operation, 1 -> Insertion 
				AREA     IN_DATA_FLAG_AREA, DATA, READONLY
IN_DATA_FLAG	DCD		0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x02
END_IN_DATA_FLAG


;*******************************************************************************
;@endsection 	INPUT_DATASET
;*******************************************************************************

;*******************************************************************************
;@section 		DATA_DECLARATION
;*******************************************************************************

;@brief 	This part will be used for constant numbers definition.
NUMBER_OF_AT	EQU		20									; Number of Allocation Table
AT_SIZE			EQU		NUMBER_OF_AT*4						; Allocation Table Size


DATA_AREA_SIZE	EQU		AT_SIZE*32*2						; Allocable data area
															; Each allocation table has 32 Cell
															; Each Cell Has 2 word (Value + Address)
															; Each word has 4 byte
ARRAY_SIZE		EQU		AT_SIZE*32							; Allocable data area
															; Each allocation table has 32 Cell
															; Each Cell Has 1 word (Value)
															; Each word has 4 byte
LOG_ARRAY_SIZE	EQU     AT_SIZE*32*3						; Log Array Size
															; Each log contains 3 word
															; 16 bit for index
															; 8 bit for error_code
															; 8 bit for operation
															; 32 bit for data
															; 32 bit for timestamp in us

;//-------- <<< USER CODE BEGIN Constant Numbers Definitions >>> ----------------------															
							


;//-------- <<< USER CODE END Constant Numbers Definitions >>> ------------------------	

;*******************************************************************************
;@brief 	This area will be used for global variables.
				AREA     GLOBAL_VARIABLES, DATA, READWRITE		
				ALIGN	
TICK_COUNT		SPACE	 4									; Allocate #4 byte area to store tick count of the system tick timer.
FIRST_ELEMENT  	SPACE    4									; Allocate #4 byte area to store the first element pointer of the linked list.
INDEX_INPUT_DS  SPACE    4									; Allocate #4 byte area to store the index of input dataset.
INDEX_ERROR_LOG SPACE	 4									; Allocate #4 byte aret to store the index of the error log array.
PROGRAM_STATUS  SPACE    4									; Allocate #4 byte to store program status.
															; 0-> Program started, 1->Timer started, 2-> All data operation finished.
;//-------- <<< USER CODE BEGIN Global Variables >>> ----------------------															
							

;//-------- <<< USER CODE END Global Variables >>> ------------------------															

;*******************************************************************************

;@brief 	This area will be used for the allocation table
				AREA     ALLOCATION_TABLE, DATA, READWRITE		
				ALIGN	
__AT_Start
AT_MEM       	SPACE    AT_SIZE							; Allocate #AT_SIZE byte area from memory.
__AT_END

;@brief 	This area will be used for the linked list.
				AREA     DATA_AREA, DATA, READWRITE		
				ALIGN	
__DATA_Start
DATA_MEM        SPACE    DATA_AREA_SIZE						; Allocate #DATA_AREA_SIZE byte area from memory.
__DATA_END

;@brief 	This area will be used for the array. 
;			Array will be used at the end of the program to transform linked list to array.
				AREA     ARRAY_AREA, DATA, READWRITE		
				ALIGN	
__ARRAY_Start
ARRAY_MEM       SPACE    ARRAY_SIZE						; Allocate #ARRAY_SIZE byte area from memory.
__ARRAY_END

;@brief 	This area will be used for the error log array. 
				AREA     ARRAY_AREA, DATA, READWRITE		
				ALIGN	
__LOG_Start
LOG_MEM       	SPACE    LOG_ARRAY_SIZE						; Allocate #DATA_AREA_SIZE byte area from memory.
__LOG_END

;//-------- <<< USER CODE BEGIN Data Allocation >>> ----------------------															
							


;//-------- <<< USER CODE END Data Allocation >>> ------------------------															

;*******************************************************************************
;@endsection 	DATA_DECLARATION
;*******************************************************************************

;*******************************************************************************
;@section 		MAIN_FUNCTION
;*******************************************************************************

			
;@brief 	This area contains project codes. 
;@note		You shouldn't change the main function. 				
				AREA MAINFUNCTION, CODE, READONLY
				ENTRY
				THUMB
				ALIGN 
__main			FUNCTION
				EXPORT __main
				BL	Clear_Alloc					; Call Clear Allocation Function.
				BL  Clear_ErrorLogs				; Call Clear ErrorLogs Function.
				BL	Init_GlobVars				; Call Initiate Global Variable Function.
				BL	SysTick_Init				; Call Initialize System Tick Timer Function.
				LDR R0, =PROGRAM_STATUS			; Load Program Status Variable Addresses.
LOOP			LDR R1, [R0]					; Load Program Status Variable.
				CMP	R1, #2						; Check If Program finished.
				BNE LOOP						; Go to loop If program do not finish.
STOP			B	STOP						; Infinite loop.
				
				ENDFUNC
			
;*******************************************************************************
;@endsection 		MAIN_FUNCTION
;*******************************************************************************				

;*******************************************************************************
;@section 			USER_FUNCTIONS
;*******************************************************************************

;@brief 	This function will be used for System Tick Handler
SysTick_Handler	FUNCTION			
;//-------- <<< USER CODE BEGIN System Tick Handler >>> ----------------------															
				EXPORT SysTick_Handler
				PUSH {LR}
				PUSH {r0, r1, r2, r3}
				LDR r0, =TICK_COUNT		;tickcount address to r0
				LDR r1, [r0]			;tickcount to r1
				PUSH {r1}				;tickcount pushed
				ADDS r1, r1, #1			;r1=r1+1
				STR r1, [r0]			;store incremented tickcount
				LDR r0, =INDEX_INPUT_DS	;index input address to r0
				LDR r0, [r0]			;index to r0
				LDR r1, =IN_DATA		;data begining address to r1
				LDR r3, [r1,r0]			;r3 equals to data[index]
				LDR r1, =IN_DATA_FLAG	;data flag begining address to r1
				LDR r2, [r1,r0]			;r2 equals to data_flag[index]
				PUSH {r0,r1,r3}			;index pushed
				MOVS r0, r3				;r0 is data[index] as parameter
				CMP r2, #0				;compare r2 with 0
				BNE skip_remove			;if r2 is 0 go to remove func
				BL Remove
				B cont
skip_remove		CMP r2, #1				;compare r2 with 1
				BNE skip_insert			;if r2 is 1 go to insert func
				BL Insert
				B cont
skip_insert		CMP r2, #2				;compare r2 with 2
				BNE no_op				;if r2 is 2 go to list2arr func
				BL LinkedList2Arr
				B cont
no_op			MOVS r0, #6				;error code is 6
cont			MOVS r1, r0				;r1 take error code to send as parameter
				POP {r0,r2,r3}			;parameters for writeerrorlog popped
				CMP r1, #0				;error check
				BEQ skip_error_log		;if not equal to 0 go to error log
				BL WriteErrorLog
skip_error_log	LDR r0, =END_IN_DATA	;data end address
				LDR r1, =IN_DATA		;data start address
				SUBS r1, r1, r0			;data size
				LSRS r1, r1, #2			;divide with 4
				ADDS r1, r1, #1			;size=+1
				POP {r0}				;tickcount popped
				CMP r1, r0				;compare with tickcount
				BNE skip_stop		;stop counter if all data readed
				BL SysTick_Stop
				POP {r0, r1, r2, r3}
				POP {PC}
skip_stop		LDR r0, =INDEX_INPUT_DS	;index input address to r0
				LDR r1, [r0]			;index to r0
				ADDS r1, r1, #4			;index+=1
				STR r1, [r0]			;store incremented index
				POP {r0, r1, r2, r3}
				POP {PC}
;//-------- <<< USER CODE END System Tick Handler >>> ------------------------				
				ENDFUNC

;*******************************************************************************				

;@brief 	This function will be used to initiate System Tick Handler
SysTick_Init	FUNCTION			
;//-------- <<< USER CODE BEGIN System Tick Timer Initialize >>> ----------------------															
				LDR r0, =0xE000E010		;load systick csr address to r0
				LDR r1, =3399			;load reload value to r1
				STR r1, [r0,#4]			;store reload value to reload value register
				MOVS r1, #0				;0 to r1
				STR r1, [r0,#8]			;clear current val register
				MOVS r1, #7				;keep clock,enable and flags value in r1
				STR r1, [r0]			;store r1 to systick csr
				LDR r0, =PROGRAM_STATUS	;load program status address to r0
				MOVS r1, #1				;1 to r1
				STR r1, [r0]			;program status set for timer started
				BX LR					;return if finished
;//-------- <<< USER CODE END System Tick Timer Initialize >>> ------------------------				
				ENDFUNC

;*******************************************************************************				

;@brief 	This function will be used to stop the System Tick Timer
SysTick_Stop	FUNCTION			
;//-------- <<< USER CODE BEGIN System Tick Timer Stop >>> ----------------------	
				LDR r0, =0xE000E010;          LOAD SYSTICK CONTROL REGISTER ADDRESS => 0xE000E010    SYST_CSR
                LDR r1, [r0];                LOAD CURRENT CONTROL REG VALUE 
                LSRS r1, #2;                SET TICKINT AND ENABLE TO ZERO
                LSLS r1, #2;                SHIFT BACK TO FIX THE VALUE    
                STR r1, [r0];                STORE NEW CONTROL REG VALUE
                MOVS r1, #2;                LOAD NEW PROGRAM STATUS VALUE => 2 SINCE PROGRAM IS FINISHED
                LDR r0, =PROGRAM_STATUS;    LOAD PROGRAM STATUS ADDRESS
                STR r1, [r0];                STORE NEW PROGRAM STATUS VALUE 
                BX LR;                        RETURN
;//-------- <<< USER CODE END System Tick Timer Stop >>> ------------------------				
				ENDFUNC

;*******************************************************************************				

;@brief 	This function will be used to clear allocation table
Clear_Alloc		FUNCTION			
;//-------- <<< USER CODE BEGIN Clear Allocation Table Function >>> ----------------------															
				PUSH {r0};				PUSH R0 TO STACK 
				LDR r0, =__AT_Start;	LOAD START ADDRESS OF ALLOCATION TABLE
				LDR r1, =NUMBER_OF_AT;	LOAD ALLOCATION TABLE SIZE
				MOVS r2, #0;			INDEX 
				MOVS r3, #0;			STORED VALUE
cla_loop		CMP r2, r1;				COMPARE INDEX WITH SIZE
				BEQ	cla_end;			JUMP TO END IF INDEX = SIZE
				LSLS r2, #2;			SHIFT INDEX LEFT BY 2
				STR r3, [r0, r2];		STORE 0 TO OFFSET ADDRESS
				LSRS r2, #2;			SHIFT INDEX RIGHT BY 2
				ADDS r2, r2, #1;	    INDEX = INDEX + 1
				B cla_loop;				JUMP BACK TO LOOP
cla_end			POP {r0};				POP R0 FROM STACK, R0 MUST STORE THE PROGRAM_STATUS
				BX LR;					RETURN IF FINISHED
;//-------- <<< USER CODE END Clear Allocation Table Function >>> ------------------------				
				ENDFUNC
				
;*******************************************************************************		

;@brief 	This function will be used to clear error log array
Clear_ErrorLogs	FUNCTION			
;//-------- <<< USER CODE BEGIN Clear Error Logs Function >>> ----------------------	
				PUSH {r0};				PUSH R0 TO STACK 
				LDR r0, =__LOG_Start;	LOAD START ADDRESS OF LOG ARRAY
				LDR r1, =LOG_ARRAY_SIZE;LOAD LOG ARRAY SIZE
				LSRS r1, #2;			DIVIDE SIZE BY 4
				MOVS r2, #0;			INDEX 
				MOVS r3, #0;			STORED VALUE
cle_loop		CMP r2, r1;				COMPARE INDEX WITH SIZE
				BEQ	cle_end;			JUMP TO END IF INDEX = SIZE
				LSLS r2, #2;			SHIFT INDEX LEFT BY 2
				STR r3, [r0, r2];		STORE 0 TO OFFSET ADDRESS
				LSRS r2, #2;			SHIFT INDEX RIGHT BY 2
				ADDS r2, r2, #1;	    INDEX = INDEX + 1
				B cle_loop;				JUMP BACK TO LOOP
cle_end			POP {r0};				POP R0 FROM STACK, R0 MUST STORE THE PROGRAM_STATUS
				BX LR;					RETURN IF FINISHED
;//-------- <<< USER CODE END Clear Error Logs Function >>> ------------------------				
				ENDFUNC
				
;*******************************************************************************

;@brief 	This function will be used to initialize global variables
Init_GlobVars	FUNCTION			
;//-------- <<< USER CODE BEGIN Initialize Global Variables >>> ----------------------
				PUSH {r0};				PUSH R0 TO STACK 
				MOVS r1, #0
				LDR r0, =TICK_COUNT
				STR r1, [r0]
				LDR r0, =FIRST_ELEMENT
				STR r1, [r0]
				LDR r0, =INDEX_INPUT_DS
				STR r1, [r0]
				LDR r0, =INDEX_ERROR_LOG
				STR r1, [r0]
				LDR r0, =PROGRAM_STATUS
				STR r1, [r0]
				POP {r0};				POP R0 FROM STACK, R0 MUST STORE THE PROGRAM_STATUS
				BX LR
;//-------- <<< USER CODE END Initialize Global Variables >>> ------------------------				
				ENDFUNC
				
;*******************************************************************************	

;@brief 	This function will be used to allocate the new cell 
;			from the memory using the allocation table.
;@return 	R0 <- The allocated area address
Malloc			FUNCTION			
;//-------- <<< USER CODE BEGIN System Tick Handler >>> ----------------------	
				PUSH {r1-r7}			;PUSH MODIFIED REGISTERS
				LDR r0, =__AT_Start		;LOAD START ADDRESS OF ALLOCATION TABLE
				LDR r1, =__DATA_Start	;LOAD ADDRESS OF HEAD OF LINKED LIST
				LDR r2, =NUMBER_OF_AT	;LOAD ALLOCATION TABLE SIZE
				MOVS r3, #0				;INDEX i
malloc_loop		CMP r2, r3				;COMPARE i WITH SIZE
				BEQ malloc_fail			;LINKED LIST IS FULL. JUMP TO malloc_fail
				LDRB r4, [r0,r3]		;LOAD BYTE FROM AT
				MOVS r5, #0				;INDEX j
				MOVS r6, #1				;BIT MASKING REGISTER
shift_loop		CMP r5, #8				;COMPARE j WITH 8
				BEQ shift_loop_end		;COMPLETED BYTE READ
				MOVS r7, r4				;MOVE BYTE TO R7
				ANDS r7, r6				;r7 <- r7 & r6
				CMP r7, #0				;COMPARE r7 WITH 0
				BEQ malloc_success		;FOUND EMPTY PLACE. JUMP TO malloc_success
				LSLS r6, #1				;SHIFT MASKING REGISTER LEFT BY ONE
				ADDS r5, r5, #1			;j = j + 1
				B shift_loop			;JUMP TO COMPARE FOR shift_loop
shift_loop_end	ADDS r3, r3, #1			;i = i + 1
				B malloc_loop			;JUMP TO COMPARE FOR malloc_loop
malloc_fail		MOVS r0, #0				;RETURN 0 SINCE LINKED LIST IS FULL
				B malloc_end			;JUMP TO END
malloc_success	ORRS r4, r6				;FORM BYTE TO STORE TO AT
				STRB r4, [r0, r3]		;STORE BYTE TO AT
				LSLS r3, #3				;i = 8 * i
				ADDS r4, r5, r3			;r4 <- j + 8 * i
				LSLS r4, #3				;r4 <- 8 * r4
				ADDS r1, r1, r4			;r1 <- __DATA_Start + r4. RETURN EMPTY PLACE'S ADDRESS
				MOVS r0, r1				;MOVE ADDRESS TO RETURN REGISTER
				POP {r1-r7}				;POP MODIFIED REGISTERS
malloc_end		BX LR					;RETURN FROM FUNCTION		
;//-------- <<< USER CODE END System Tick Handler >>> ------------------------				
				ENDFUNC
				
;*******************************************************************************				

;@brief 	This function will be used for deallocate the existing area
;@param		R0 <- Address to deallocate
Free			FUNCTION			
;//-------- <<< USER CODE BEGIN Free Function >>> ----------------------
				PUSH {r1-r6}			;PUSH MODIFIED REGISTERS
				LDR r1, =__AT_Start		;LOAD START ADDRESS OF ALLOCATION TABLE
				LDR r2, =__DATA_Start	;LOAD ADDRESS OF HEAD OF LINKED LIST
				SUBS r3, r0, r2			;r3 <- ADDRESS TO DEALLOCATE MINUS __DATA_Start. r3 STORES DIFFERENCE BETWEEN ADDRESS TO DEALLOCATE AND ADDRESS OF HEAD
				LSRS r3, #3				;DIVIDE ADDRESS DIFFERENCE BY 8. r3 STORES DISTANCE IN TERMS OF NODES. E.G. HOW MANY NODES AWAY THE NODE TO BE DEALLOCATED IS FROM HEAD
				MOVS r4, r3				;MOVE DISTANCE TO r4
				; TO FIND THE CORRECT POSITION IN ALLOCATION TABLE
				; DIVIDE DISTANCE BY 32 TO KNOW OFFSET IN TERMS OF WORDS
				; THEN MAKE MODULO OPERATION WITH 32
				; DISTANCE % 32 TO KNOW OFFSET IN TERMS OF BITS
				LSRS r4, #5				;DIVIDE DISTANCE BY 32
				MOVS r5, #32			;MOVE 32 TO r5
modulo_loop		CMP r3, r5				;COMPARE DISTANCE WITH 32
				BCC modulo_loop_end		;IF DISTANCE < 32, BRANCH TO END OF LOOP
				SUBS r3, r3, r5			;DISTANCE = DISTANCE - 32
				B modulo_loop			;WHILE LOOP
modulo_loop_end	LSLS r4,#2;
				LDR r6, [r1,r4]			;LOAD WORD FROM __AT_Start WITH OFFSET IN r4
				; CLEAR nth BIT IN WORD
				; WORD &= ~(0x0001 << n)
				; r3 STORES MODULO RESULT (n)
				MOVS r5, #1				;MOVE 1 to r5
				LSLS r5, r3				;SHIFT 1 LEFT BY OFFSET IN r3 AND STORE IN r5
				MVNS r5, r5				;MOVE NOT. r5 <- ~r5
				ANDS r6, r5				;CLEAR THE BIT IN WORD
				STR r6, [r1,r4]			;STORE WORD IN ALLOCATION TABLE WITH OFFSET IN r4
				POP {r1-r6}				;POP MODIFIED REGISTERS
				BX LR
;//-------- <<< USER CODE END Free Function >>> ------------------------				
				ENDFUNC
				
;*******************************************************************************				

;@brief 	This function will be used to insert data to the linked list
;@param		R0 <- The data to insert
;@return    R0 <- Error Code
Insert			FUNCTION			
;//-------- <<< USER CODE BEGIN Insert Function >>> ----------------------
			PUSH {LR};
			PUSH {r1, r2, r4, r6, r7};			PUSH REGISTERS TO STACK 
			LDR	 r6,=FIRST_ELEMENT;					LOAD FIRST ELEMENT POINTER TO THE R6
			LDR  r1,[r6];							LOAD R1 FROM ADDRESS OF R6
			MOVS r4,#0;								R4=0, R4 WILL STORE THE ADDRESS OF THE PREVIOUS NODE
insert_loop	
			CMP  r1,#0;								COMPARE WITH 0
			BEQ  add_to_end;						IF IT IS 0, THEN BRANCH TO LABEL
			LDR  r2,[r1];							LOAD R2 FROM ADDRESS OF R1
			CMP  r0,r2;								COMPARE R0,R2-----THE ELEMENT OF THE LINKEDLIST AND THE VALUE
			BEQ  same_data;							BRANCH TO SAME_DATA LABEL
			BMI	 add_to_middle;						IF VALUE IS SMALLER THEN BRANCH
			ADDS r1,#4;								R1=R1+4, NEXT POINTER OF THE NODE
			MOVS r4,r1;								R4 STORES THE NEXT POINTER OF PREVIOUS NODE
			LDR  r1,[r4];							LOAD R6 FROM ADDRESS OF R4, R6 POINTS TO NEXT NODE
			B	 insert_loop;						BRANCH TO INSERT_LOOP
linked_list_full
			MOVS r0,#1;								RETURN ERROR CODE 1 WHICH MEANS LINKED LIST IS FULL
			B    rtrn_insert;						BRANCH TO LABEL
same_data	
			MOVS r0,#2;								RETURN ERROR CODE 2 WHICH MEANS SAME VALUE IS ALREADY INCLUDED
			B    rtrn_insert;						BRANCH TO LABEL
add_to_end
			MOVS r7,r0;								MOVE r0->r7, r7 stores the value that will be inserted
			BL   Malloc;							CALL MALLOC FUNCTION
			CMP  r0,#0;								COMPARE WITH 0
			BEQ  linked_list_full;					IF EQUAL THAN BRANCH TO LABEL
			STR  r7,[r0];							STORE R7 IN THE R0 MEMORY ADDRESS
			CMP  r4,#0;								COMPARE R4 AND 0
			BEQ	 head_ptr;							BRANCH IF EQUAL, (THERE IS NO PREVIOUS NODE, ASSIGN NEW FIRST ELEMENT) 
			STR  r0,[r4];							STORE R0 IN THE R1 MEMORY ADDRESS
			MOVS r0,#0;								RETURN ERROR CODE 0 WHICH MEANS NO ERROR
			B	 rtrn_insert;						BRANCH TO RTRN_INSERT
add_to_middle
			MOVS r7,r0;								MOVE r0->r7, r7 stores the value that will be inserted
			BL   Malloc;							CALL MALLOC FUNCTION
			CMP  r0,#0;								COMPARE WITH 0
			BEQ  linked_list_full;					IF EQUAL THAN BRANCH TO LABEL
			STR  r7,[r0];							NEXT POINTER POINTS THE R7
			ADDS r0,#4;								ADD 4 to R0
			STR  r1,[r0];							STORE R1 IN THE R0 MEMORY ADDRESS
			SUBS r0,#4;
			CMP  r4,#0;								COMPARE R4 AND 0
			BEQ	 head_ptr;							BRANCH EQUAL IF, (THERE IS NO PREVIOUS NODE, ASSIGN NEW FIRST ELEMENT) 
			STR  r0,[r4];							STORE R0 IN THE R4 MEMORY ADDRESS, NEXT POINTER OF PREVIOUS NODE POINTS TO NEW NODE
			MOVS r0,#0;								RETURN ERROR CODE 0 WHICH MEANS NO ERROR
			B    rtrn_insert;						BRANCH TO RTRN_INSERT
			
head_ptr    STR  r0,[r6];							STORE R0 IN THE R6 MEMORY ADDRESS 
			MOVS r0,#0;								RETURN ERROR CODE 0 WHICH MEANS NO ERROR
rtrn_insert	POP  {r1, r2, r4, r6, r7};				POP RREGISTER FROM STACK, r0 returns the error code
			POP  {PC};								BRANCH WITH LINK REGISTER									
;//-------- <<< USER CODE END Insert Function >>> ------------------------				
				ENDFUNC
				
;*******************************************************************************				

;@brief 	This function will be used to remove data from the linked list
;@param		R0 <- the data to delete
;@return    R0 <- Error Code
Remove			FUNCTION			
;//-------- <<< USER CODE BEGIN Remove Function >>> ----------------------															
			;r3 is the paramater(value) 
			PUSH {LR};
			PUSH {r1, r2, r4, r5, r6, r7};			PUSH REGISTERS TO STACK 
			LDR	 r6,=FIRST_ELEMENT;					LOAD FIRST ELEMENT POINTER TO THE R0
			MOVS r4,#0;								R4=0, R4 WILL STORE THE ADDRESS OF THE PREVIOUS NODE
			LDR  r1,[r6];							LOAD R1 FROM ADDRESS OF R6
			CMP  r1,#0;								COMPARE WITH 0
			BEQ  empty_list;						IF IT IS 0, THEN BRANCH TO LABEL
remove_loop	
			LDR  r2,[r1];							LOAD R2 FROM ADDRESS OF R1
			CMP  r2,r0;								COMPARE R2,R0-----THE ELEMENT OF THE LINKEDLIST AND THE VALUE
			BEQ  rmw_found;							BRANCH TO SAME_DATA LABEL
			ADDS r1,#4;								R0=R0+4, NEXT POINTER OF THE NODE
			MOVS r4,r1;								MOVE r1->r4
			LDR  r1,[r4];							LOAD R1 FROM ADDRESS OF R4
			CMP  r1,#0;								COMPARE WITH 0
			BEQ  no_matching;						IF IT IS 0, THEN BRANCH TO LABEL
			B	 remove_loop;						BRANCH TO INSERT_LOOP
			
empty_list	
			MOVS r0,#3;								RETURN ERROR CODE 3 WHICH MEANS EMPTY LIST
			B    rtrn_remove;						BRANCH TO LABEL
no_matching	
			MOVS r0,#4;								RETURN ERROR CODE 4 WHICH MEANS THE ELEMENT IS NOT FOUND
			B    rtrn_remove;						BRANCH TO LABEL
rmw_found
			MOVS r0,r1;								R1->R0, R0 STORES THE ADDRESS FOR FREE() 
			ADDS r1,#4;								R1=R1+4, TO GET NEXT POINTER OF THE NODE THAT WILL BE REMOVED
			LDR  r5,[r1];							LOAD R5 FROM ADDRESS OF R1, SO, R5 STORES THE NEXT POINTER
			STR  r5,[r4];							R4 IS THE NEXT POINTER OF PREVIOUS NODE, PREVIOUS_NODE->NEXT=REMOVED_NODE->NEXT
			LDR  r7,[r6];
			CMP  r7,r0;
			BNE  not_head;
			ADDS r7,#4;
			LDR  r7,[r7];
			STR  r7,[r6];
not_head
			BL   Free;								CALL FREE FUNCTION
			MOVS r0,#0;								MOVE 0 TO R0 REGISTER, THAT MEANS NO ERROR
rtrn_remove	POP  {r1, r2, r4, r5, r6, r7};			POP RREGISTER FROM STACK, r0 returns the error code
			POP  {PC};								BRANCH WITH LINK REGISTER			
;//-------- <<< USER CODE END Remove Function >>> ------------------------				
				ENDFUNC
				
;*******************************************************************************				

;@brief 	This function will be used to clear the array and copy the linked list to the array
;@return	R0 <- Error Code
LinkedList2Arr	FUNCTION			
;//-------- <<< USER CODE BEGIN Linked List To Array >>> ----------------------															
				LDR r0, =FIRST_ELEMENT;        Load address of head of linked list
                LDR r0, [r0];                Load head of linked list
                CMP r0, #0;                    Check if head is null
                BEQ llist2arr_error;        If head is null then jump to llist2arr_error
                LDR r1, =ARRAY_MEM;            Load start address of array
                LDR r2, =ARRAY_SIZE;        Load size of array
                MOVS r3, #0;                Array index
                MOVS r4, #0;                Stored Value
array_clear     CMP r3, r2;                    Compare index with ARRAY_SIZE
                BEQ array_clear_end;        If equal, jump to array_clear_end
                STR r4, [r1, r3];            Store value from index
                ADDS r3, r3, #4;            Increment index of array
                B array_clear;                Jump back to start of loop
array_clear_end MOVS r2, #0;                Array index
llist2arr_loop     CMP r0, #0;                    Check if r0 is null
                BEQ llist2arr_end;            Jump to llist2arr_end if program reaches to end of linked list
                LDR r3, [r0];                Load value of node which r0 points
                STR r3, [r1, r2];            Store value loaded from linked list into array
                ADDS r2, #4;                Increment index of array
                LDR r0, [r0, #4];            Load next address of linked list
                B llist2arr_loop;            Jump back to loop
llist2arr_end    MOVS r0, #0;                Move success code into r0 (return register)
                BX LR
llist2arr_error MOVS r0, #5;                Move error code into r0 (return register)
                BX LR
;//-------- <<< USER CODE END Linked List To Array >>> ------------------------				
				ENDFUNC
				
;*******************************************************************************				

;@brief 	This function will be used to write errors to the error log array.
;@param		R0 -> Index of Input Dataset Array
;@param     R1 -> Error Code 
;@param     R2 -> Operation (Insertion / Deletion / LinkedList2Array)
;@param     R3 -> Data
WriteErrorLog	FUNCTION			
;//-------- <<< USER CODE BEGIN Write Error Log >>> ----------------------		
				PUSH {LR}
				LDR r4, =LOG_MEM;            Load start address of Error Log
                LDR r5, =INDEX_ERROR_LOG;    Load address of Error Log Index
                LDR r5, [r5];                Load error log index
                MOVS r7, r5;                Keep index value to increment it by 1
                MULS r5, r6, r5;            Multiply index by 12
                ADDS r4, r4, r5;            Add multiplied value into start address
                STRH r0, [r4];                Store Index of Input Dataset Array into Error Log
                ADDS r4, #2;                Increase address by 2
                STRB r1, [r4];                Store Error code
                ADDS r4, #1;                Increase address by 1
                STRB r2, [r4];                Store operation
                ADDS r4, #1;                Increase address by 1
                STR r3, [r4];                Store Data 
                ADDS r4, #4;                Increase by 4
                BL GetNow;                    Call GetNow function to get Timestamp
                STR r0, [r4];                Store Timestamp
                ADDS r7, #1;                Increment index by 1
                LDR r4, =INDEX_ERROR_LOG;    Load address of error log index
                STR r7, [r4];                Store new index
                POP {PC}
;//-------- <<< USER CODE END Write Error Log >>> ------------------------				
				ENDFUNC
				
;@brief 	This function will be used to get working time of the System Tick timer
;@return	R0 <- Working time of the System Tick Timer (in us).			
GetNow			FUNCTION			
;//-------- <<< USER CODE BEGIN Get Now >>> ----------------------
				PUSH {r1, r2}
				LDR r1, =850;                LOAD 850 TO USE IN MULTIPLICATION
                LDR r0, =TICK_COUNT;        LOAD TICK COUNT ADDRESS
                LDR r0, [r0];                LOAD TICK COUNT VALUE
                MULS r0, r1, r0;            TICK COUNT * 850 => TIME PASSED ONLY CONSIDERING TICK COUNT
                LDR r1, =0xE000E018;         LOAD SYSTICK CURRENT VALUE REGISTER ADDRESS
                LDR r1, [r1];                LOAD SYSTICK CURRENT VALUE 
                LDR r2, =0xE000E014;        LOAD RELOAD VALUE ADDRESS
                LDR r2, [r2];                LOAD RELOAD VALUE 
                SUBS r1, r2, r1;            RELOAD VALUE - CURRENT VALUE = TICKS PASSED SINCE LAST RELOAD
                LSRS r1, #2;                DIVIDE TICKS PASSED BY 4 => 4 TICKS = 1 uS 
                ADDS r0, r0, r1;            ADD R1 uS TO R0
				POP {r1, r2}
                BX LR;                        RETURN
;//-------- <<< USER CODE END Get Now >>> ------------------------
				ENDFUNC
				
;*******************************************************************************	

;//-------- <<< USER CODE BEGIN Functions >>> ----------------------															


;//-------- <<< USER CODE END Functions >>> ------------------------

;*******************************************************************************
;@endsection 		USER_FUNCTIONS
;*******************************************************************************
				ALIGN
				END		; Finish the assembly file
				
;*******************************************************************************
;@endfile 			main.s
;*******************************************************************************				
