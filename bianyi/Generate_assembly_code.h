#pragma once
#include"semantic_analysis.h"

//���ɻ�����
string selectRegister(int T_cnt) {
	string s;
	if (T_cnt == 1)
		s = "BX";
	else if (T_cnt == 2)
		s = "CX";
	else
		s = "DX";
	return s;
}

void Generate_Assembly_Program() {
	ofstream outfile("pas.asm", ios_base::out);  //��д�뷽ʽ��
	if (!outfile.is_open()) {
		cout << "��pas.asm�ļ�����" << endl;
		return;
	}
	outfile << "**********************************************************************************\n";
	cout << "**********************************************************************************\n";
	outfile << "DSEG SEGMENT\t\t\t\t\t;���ݶο�ʼ\n ";
	cout << "DSEG SEGMENT\t\t\t\t\t\t;���ݶο�ʼ\n ";
	for (unsigned i = 0; i < VarTable.size(); i++) {
		outfile << '\t' << VarTable[i] << "\t\tDW\t0\n";  //�����ֱ���
		cout << '\t' << VarTable[i] << "\tDW\t 0\n";  //�����ֱ���
	}

	outfile << "DSEG ENDS\t\t\t\t\t\t;���ݶν���\n";
	outfile << "**********************************************************************************\n";
	outfile << "CSEG SEGMENT\t\t\t\t\t;����ο�ʼ\n";
	//outfile << "MAIN PROC FAR\t\t\t\t\t;�����ִ�в���\n";
	outfile << "\tASSUME CS: CSEG, DS: DSEG\t;���öμĴ������߼��εĹ�ϵ\n";
	outfile << "START:\n";
	//outfile << "\t\tPUSH  DS\n";
	//outfile << "\t\tSUB  BX, BX\n";
	//outfile << "\t\tPUSH  BX\n";
	outfile << "\t\tMOV	 AX, DSEG\n";
	outfile << "\t\tMOV	 DS, AX\t\t\t\t;�������ݶζε�ַ\n";
	outfile << "********************************\n";

	cout << "DSEG ENDS\t\t\t\t\t\t;���ݶν���\n";
	cout << "**********************************************************************************\n";
	cout << "CSEG SEGMENT\t\t\t\t\t\t;����ο�ʼ\n";
	//cout << "MAIN PROC FAR\t\t\t\t\t\t;�����ִ�в���\n";
	cout << "\tASSUME CS: CSEG, DS: DSEG\t\t\t;���öμĴ������߼��εĹ�ϵ\n";
	cout << "START:\n";
	//cout << "\t\tPUSH     DS\n";
	//cout << "\t\tSUB      BX, BX\n";
	//cout << "\t\tPUSH     BX\n";
	cout << "\t\tMOV	 AX, DSEG\n";
	cout << "\t\tMOV	 DS, AX\t\t\t\t;�������ݶζε�ַ\n";
	cout << "********************************************************\n";

	/*���뿪ʼ*/
	unsigned int i;  //ResSem�±�
	for (i = 0; i < ResSem.size(); i++) {
		if (ResSem[i].op[0] != 'j') {  //�������ʽ�͸�ֵ����
			unsigned int j = i;
			int T_cnt = 0;  //��¼��ǰ����������ʱ������
			mp.clear();  //�����µĻ�����֮ǰ���֮ǰ����ʱ������¼
			while (j < ResSem.size() && ResSem[j].op[0] != 'j') {  //��ʼ�Ե�ǰ���������
				/*arg1*/
				switch (ResSem[j].arg1.id) {
				case ident:
					_arg1 = VarTable[ResSem[j].arg1.value];
					break;
				case intconst:
					_arg1 = to_string(ResSem[j].arg1.value);  //��ೣ��Ĭ��ʮ����
					break;
				case tempsy:
					if (!mp[ResSem[j].arg1.value]) {   //֮ǰû���ֹ�����ʱ����
						T_cnt++;
						mp[ResSem[j].arg1.value] = true;  //��ǳ��ֹ�
					}
					_arg1 = selectRegister(T_cnt);  //���ֹ���Tcnt����1
				case -1:
					break;
				}
				/*arg2*/
				switch (ResSem[j].arg2.id) {
				case ident:
					_arg2 = VarTable[ResSem[j].arg2.value];
					break;
				case intconst:
					_arg2 = to_string(ResSem[j].arg2.value);  //��ೣ��Ĭ��ʮ����
					break;
				case tempsy:
					if (!mp[ResSem[j].arg2.value]) {   //֮ǰû���ֹ�����ʱ����
						T_cnt++;
						mp[ResSem[j].arg2.value] = true;  //��ǳ��ֹ�
					}
					_arg2 = selectRegister(T_cnt);  //���ֹ���Tcnt����1
				case -1:
					break;
				}
				/*result*/
				if (ResSem[j].op_type.id == becomes)   //��ֵ���
					_result = VarTable[ResSem[j].result];
				else {  //�������ʽ
					if (!mp[ResSem[j].result]) {   //֮ǰû���ֹ�����ʱ����
						T_cnt++;
						mp[ResSem[j].result] = true;  //��ǳ��ֹ�
					}
					_result = selectRegister(T_cnt);  //���ֹ���Tcnt����1
				}
				//��ַ
				outfile << "F" << j + InitADD << ":";
				cout << "F" << j + InitADD << ":";

				//���
				switch (ResSem[j].op_type.id) {
				case plus1:  //�ӷ�
					outfile << "\tMOV	 AX, " << _arg1 << endl;
					outfile << "\t\tADD	 AX, " << _arg2 << endl;  //����λ�ӷ����������AX
					outfile << "\t\tMOV	 " << _result << ", AX" << endl;  //����浽_result
					cout << "\t\tMOV	 AX, " << _arg1 << endl;
					cout << "\t\tADD	 AX, " << _arg2 << endl;  //����λ�ӷ����������AX
					cout << "\t\tMOV	 " << _result << ", AX" << endl;  //����浽_result
					break;
				case times:  //�˷�
					outfile << "\tMOV	 AX, " << _arg1 << endl;
					outfile << "\t\tMUL  " << _arg2 << endl;
					outfile << "\t\tMOV	 " << _result << ", AX" << endl;  //����浽_result
					cout << "\t\tMOV	 AX, " << _arg1 << endl;
					cout << "\t\tMUL      " << _arg2 << endl;
					cout << "\t\tMOV	 " << _result << ", AX" << endl;  //����浽_result
					break;
				case becomes:  //��ֵ����
					outfile << "\tMOV	 AX, " << _arg1 << endl;
					outfile << "\t\tMOV	 " << _result << ", AX" << endl;
					cout << "\t\tMOV	 AX, " << _arg1 << endl;
					cout << "\t\tMOV	 " << _result << ", AX" << endl;
					break;
				}
				j++;
			}
			i = j - 1;
		}
		else {  //��ת���ж����
			/*arg1*/
			switch (ResSem[i].arg1.id) {
			case ident:
				_arg1 = VarTable[ResSem[i].arg1.value];
				break;
			case intconst:
				_arg1 = to_string(ResSem[i].arg1.value);  //��ೣ��Ĭ��ʮ����
				break;
			case -1:
				break;
			}
			/*arg2*/
			switch (ResSem[i].arg2.id) {
			case ident:
				_arg2 = VarTable[ResSem[i].arg2.value];
				break;
			case intconst:
				_arg2 = to_string(ResSem[i].arg2.value);  //��ೣ��Ĭ��ʮ����
				break;
			case -1:
				break;
			}
			/*result*/
			if (ResSem[i].op[0] == 'j')  //��תָ��
				jmpLocation = ResSem[i].result;
			//��ַ
			outfile << "F" << i + InitADD << ":";
			cout << "F" << i + InitADD << ":";

			//���
			switch (ResSem[i].op_type.id) {
			case rop:  //��ϵ�����
				outfile << "\tMOV	 AX, " << _arg1 << endl;
				outfile << "\t\tCMP	 AX, " << _arg2 << endl;  //�Ƚϣ�_arg1-_arg2����������
				cout << "\t\tMOV	 AX, " << _arg1 << endl;
				cout << "\t\tCMP	 AX, " << _arg2 << endl;  //�Ƚϣ�_arg1-_arg2����������
				switch (ResSem[i].op_type.value) {
				case 0:  //<
					outfile << "\t\tJL  F" << jmpLocation << endl;
					cout << "\t\tJL  F" << jmpLocation << endl;
					break;
				case 1:  //<=
					outfile << "\t\tJLE  F" << jmpLocation << endl;
					cout << "\t\tJLE  F" << jmpLocation << endl;
					break;
				case 2:  //<>
					outfile << "\t\tJNZ  F" << jmpLocation << endl;
					cout << "\t\tJNZ  F" << jmpLocation << endl;
					break;
				case 3:  //>
					outfile << "\t\tJG  F" << jmpLocation << endl;
					cout << "\t\tJG       F" << jmpLocation << endl;
					break;
				case 4:  //>=
					outfile << "\t\tJGE  F" << jmpLocation << endl;
					cout << "\t\tJGE      F" << jmpLocation << endl;
					break;
				case 5:  //=
					outfile << "\t\tJZ   F" << jmpLocation << endl;
					cout << "\t\tJZ       F" << jmpLocation << endl;
					break;
				}
				break;
			case -1:
				switch (ResSem[i].op_type.value) {
				case -1:  //j
					outfile << "\tJMP	 F" << jmpLocation << endl;
					cout << "\t\tJMP	 F" << jmpLocation << endl;
					break;
				case 0:  //jnz
					outfile << "\tJNZ	 F" << jmpLocation << endl;
					cout << "\tJNZ	 F" << jmpLocation << endl;
					break;
				}
			}
		}
	}
	outfile << "********************************\n";
	//outfile << "L" << i + InitADD << ":\tPOP  BX\n";
	//outfile << "\t\tPOP  DS\n";
	outfile << "F" << i + InitADD << ":\t\tMOV  AX, 4C00H\t\t\t;ΪDOS���ܵ������ù��ܺź���ڲ���\n";
	outfile << "\t\tINT  21H\t\t\t\t;DOS���ܵ��ã�����DOS����ϵͳ\n";
	outfile << "\t\tRET\t\t\t\t\t\t;����\n";
	//outfile << "MAIN ENDP\t\t\t\t\t\t;�ӳ������\n";
	outfile << "CSEG ENDS\t\t\t\t\t\t;����ν���\n";
	outfile << "\tEND START\t\t\t\t\t;������\n";

	cout << "********************************************************\n";
	//cout << "L" << i + InitADD << ":\t\tPOP      BX\n";
	//cout << "\t\tPOP      DS\n";
	cout << "F" << i + InitADD << ":\t\tMOV      AX, 4C00H\t\t\t;ΪDOS���ܵ������ù��ܺź���ڲ���\n";
	cout << "\t\tINT      21H\t\t\t\t;DOS���ܵ��ã�����DOS����ϵͳ\n";
	cout << "\t\tRET\t\t\t\t\t;����\n";
	//cout << "MAIN ENDP\t\t\t\t\t\t;�ӳ������\n";
	cout << "CSEG ENDS\t\t\t\t\t\t;����ζ������\n";
	cout << "\tEND START\t\t\t\t\t;������\n";
	cout << "���������ɻ����򲢱�����pas.asm�ļ�\n";
	outfile.close();
}