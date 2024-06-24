**********************************************************************************
DSEG SEGMENT					;数据段开始
 	a		DW	0
	b		DW	0
	m		DW	0
	n		DW	0
	k		DW	0
	h		DW	0
	x		DW	0
	y		DW	0
DSEG ENDS						;数据段结束
**********************************************************************************
CSEG SEGMENT					;代码段开始
	ASSUME CS: CSEG, DS: DSEG	;设置段寄存器与逻辑段的关系
START:
		MOV	 AX, DSEG
		MOV	 DS, AX				;设置数据段段地址
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
F117:		MOV  AX, 4C00H		;为DOS功能调用设置功能号和入口参数
		INT  21H				;DOS功能调用，返回DOS操作系统
		RET						;返回
CSEG ENDS						;代码段结束
	END START					;汇编结束
