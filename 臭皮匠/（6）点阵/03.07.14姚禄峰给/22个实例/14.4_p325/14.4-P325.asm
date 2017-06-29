	  ORG  00H
	    LJMP LTB4
	    ORG 0026H  	
LTB4:	MOV R2,#0FFH	
        MOV DPTR,#TAB4
		MOV R0,#20h
LT4A:	 MOVC A,@A+DPTR
         JZ LNED 
        MOV A,R0
		MOV R1,A
 	   
 LT4B:   MOV A,@R1
        MOV B,A
		CLR A
	   MOVC A,@A+DPTR
       CLR F0
	   JBC ACC.7,LT4C
	   SETB F0
LT4C:  
       INC R2
       CJNE A,B,LT4D
       JNB F0,LYES
		;CJNE A,B,LT4D
	   INC R1
	   INC DPTR
	   SJMP LT4B
LT4D:  INC DPTR
        MOV A,@A+DPTR
		JZ LNED
LT4E:  JNB F0,LT4A
       CLR A
	   MOVC A,@A+DPTR
	   INC DPTR
		 CLR F0
		JBC ACC.7,LT4D
		SETB F0

		 SJMP LT4E
LYES:   MOV A,R2
        MOV DPTR,#TAB5
        MOVC A,@A+DPTR
 	    MOV P2,A
		SJMP DONE
LNED:   MOV A,5	 ;’“≤ªµΩœ‘ æ8
        MOVC A,@A+DPTR
	    MOV P2,A
done:   sjmp DONE
TAB4:	DB'RESE',0D4H
        DB'BEGI',0CEH
		DB'STOP',0D0H
		DB'SEND',0C4H
		DB 00H
TAB5:  DB 0C0H,0F9H,0A4H,0B0H,99H,92H,82H,0F8H
       
  	   END