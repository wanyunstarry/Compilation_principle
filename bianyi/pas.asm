**********************************************************************************
DSEG SEGMENT					;���ݶο�ʼ
 	a		DW	0
	b		DW	0
	m		DW	0
	n		DW	0
	k		DW	0
	h		DW	0
	x		DW	0
	y		DW	0
DSEG ENDS						;���ݶν���
**********************************************************************************
CSEG SEGMENT					;����ο�ʼ
	ASSUME CS: CSEG, DS: DSEG	;���öμĴ������߼��εĹ�ϵ
START:
		MOV	 AX, DSEG
		MOV	 DS, AX				;�������ݶζε�ַ
********************************
F100:	MOV	 AX, a
		CMP	 AX, b
		JG  F102
F101:	JMP	 F117
F102:	MOV	 AX, m
		CMP	 AX, n
		JGE  F104
F103:	JMP	 F107
F104:	MOV	 AX, a
		ADD	 AX, 0
		MOV	 BX, AX
F105:	MOV	 AX, BX
		MOV	 a, AX
F106:	JMP	 F112
F107:	MOV	 AX, k
		CMP	 AX, h
		JZ   F109
F108:	JMP	 F112
F109:	MOV	 AX, x
		ADD	 AX, 1
		MOV	 BX, AX
F110:	MOV	 AX, BX
		MOV	 x, AX
F111:	JMP	 F107
F112:	MOV	 AX, m
		ADD	 AX, y
		MOV	 BX, AX
F113:	MOV	 AX, x
		MUL  BX
		MOV	 CX, AX
F114:	MOV	 AX, n
		ADD	 AX, CX
		MOV	 DX, AX
F115:	MOV	 AX, DX
		MOV	 m, AX
F116:	JMP	 F100
********************************
F117:		MOV  AX, 4C00H		;ΪDOS���ܵ������ù��ܺź���ڲ���
		INT  21H				;DOS���ܵ��ã�����DOS����ϵͳ
		RET						;����
CSEG ENDS						;����ν���
	END START					;������
