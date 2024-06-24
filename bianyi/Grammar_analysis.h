#pragma once
#pragma warning( disable : 6386) 
#pragma warning( disable : 6001) 
#pragma warning( disable : 6385) 
#include "Generate_assembly_code.h"
//�����ַ��Ƿ�Ϊ���ʽ�е�ֵ
bool is_pro(int id) {
	switch (id) {
	case intconst:
	case ident:
	case plus1:
	case times:
	case becomes:
	case lparent:
	case rparent:
	case rop:
	case op_and:
	case op_or:
	case op_not:
		return true;
	default:
		return false;
	}
}

//��ñ��ʽ
int get_pro(int p_pro) {
	production.clear();  //���ʽ������
	//��ñ��ʽ
	do {
		production.push_back({ ResLex[p_pro].id ,ResLex[p_pro].value });
		p_pro++;
	} while (is_pro(ResLex[p_pro].id));
	production.push_back({ jinghao,0 });  //���#
	return p_pro - 1;  //���ر��ʽ�ĳ���
}

//ÿһ�ε��ã�����һ����������ʱ�������ֱ���
int newtemp() {
	static int new_id = 0;
	return ++new_id;
}

//��ȡ�������ʽ�ַ����ͣ���ӦArithmeticTable��
int getArithType(int id) {
	switch (id) {
	case ident:
	case intconst:
		return 0;  //������������
	case plus1:
		return 1;  //+
	case times:
		return 2;  //*
	case lparent:
		return 3;  //(
	case rparent:
		return 4;  //)
	case jinghao:
		return 5;  //#
	case tempsy:
		return 6;  //��ʱ����
	default:
		error(1);  //��ʾ���ս��/���������������ʽ�븳ֵ�����
		return -1;
	}
}

//��ȡ�������ʽ�ַ����ͣ���ӦBoolTable��
int getBoolType(int id) {
	switch (id) {
	case ident:
	case intconst:
		return 0;  //������������
	case rop:
		return 1;  //��ϵ�����
	case lparent:
		return 2;  //(
	case rparent:
		return 3;  //)
	case op_not:
		return 4;  //not
	case op_and:
		return 5;  //and
	case op_or:
		return 6;  //or
	case jinghao:
		return 7;  //#
	case tempsy:
		return 8;  //��ʱ����
	case EA:
		return 9;  //B��ȡ
	case EO:
		return 10; //B��ȡ
	default:
		error(2);  //��ʾ���ս����������ڲ������ʽ��
		return -1;
	}
}

//��p�����ӵ�ÿ����Ԫʽ��result��дΪ��ַaddress
void backpatch(int p, int address) {
	int q;
	q = p;
	while (q != -1) {
		int temp = ResSem[q].result;
		ResSem[q].result = address;
		q = temp;
	}
}

//����p1��p2Ϊ���׵��������ϲ�Ϊһ����p2Ϊ���׵�������������ֵp2��
int merge(int p1, int p2) {
	if (p2 == 0)
		return p1;
	else {
		int temp = p2;
		while (ResSem[temp].result >= 0) {  //resultΪ��Чֵ
			temp = ResSem[temp].result;
		}
		ResSem[temp].result = p1;
	}
	return p2;
}

//�����������ʽ�͸�ֵ����Ԫʽ�õ���Ӧ��ԭʽ��ֻ������ʾ��
void arithSentence(Binary b) {
	switch (b.id) {
	case ident:
		cout << VarTable[b.value];
		break;
	case intconst:
		cout << b.value;
		break;
	case plus1:
		cout << "+";
		break;
	case times:
		cout << "*";
		break;
	case lparent:
		cout << "(";
		break;
	case rparent:
		cout << ")";
		break;
	case becomes:
		cout << ":=";
		break;
	}
}

//���ݲ������ʽ��Ԫʽ�õ���Ӧ��ԭʽ��ֻ������ʾ��
void boolSentence(Binary b) {
	switch (b.id) {
	case ident:
		cout << VarTable[b.value];
		break;
	case intconst:
		cout << b.value;
		break;
	case rop:
		switch (b.value) {
		case 0:
			cout << "<";
			break;
		case 1:
			cout << "<=";
			break;
		case 2:
			cout << "<>";
			break;
		case 3:
			cout << ">";
			break;
		case 4:
			cout << ">=";
			break;
		case 5:
			cout << "=";
			break;
		}
		break;
	case lparent:
		cout << "(";
		break;
	case rparent:
		cout << ")";
		break;
	case op_not:
		cout << " not ";
		break;
	case op_and:
		cout << " and ";
		break;
	case op_or:
		cout << " or ";
		break;
	}
}

//�������ʽ
void SLR1Arithmetic(vector<Binary> pro) {
	cout << "\n\n****************************************�������ʽ�͸�ֵ���ķ�������****************************************\n";
	for (auto i : pro) arithSentence(i);
	cout << endl << "��Ӧ�Ķ�Ԫʽ���£�" << endl;
	for (auto i : pro) cout << "(" << i.id << "," << i.value << ")" << endl;
	int state_sk[SIZE_PRODUCTION];  //״̬ջ
	Binary symbol_sk[SIZE_PRODUCTION];  //����ջ
	int ACTION;  //ACTION��
	int GOTO;   //GOTO��	
	int SLR;	//SLR����
	int number = 0;  //���
	int p_pro = 2;	 //����ʽpro���±꣨��:=�������
	int p_stack = 0; //ջ���±�
	Binary left;     //����ʽ����ֵ
	//��ʼ����ֵ��״̬ջ�ͷ���ջ
	left = pro.front();
	state_sk[p_stack] = 0;
	symbol_sk[p_stack].id = jinghao;
	symbol_sk[p_stack].value = 0;
	p_stack++;
	//�����������
	cout << "\n���\t\t" << "����\t\t" << "״̬ջ\t\t\t\t\t\t\t" << "����ջ" << endl;
	cout << "[" << number++ << "]\t\t";  //���
	cout << "_\t\t";  //����
	cout << state_sk[0] << "\t\t\t\t\t\t\t"; //״̬ջ
	cout << "(" << symbol_sk[0].id << "," << symbol_sk[0].value << ")" << endl;  //����ջ

	while (true) {
		if (p_stack < 0)
			error(6);  //ջ����
		ACTION = getArithType(pro[p_pro].id);
		SLR = ArithmeticTable[state_sk[p_stack - 1]][ACTION];
		if (SLR >= 0 && SLR < 100) {  //�ƽ�
			cout << "[" << number++ << "]\t\t";  //���
			cout << "s" << SLR << "\t\t";  //����
			state_sk[p_stack] = SLR;  //״̬��ջ
			symbol_sk[p_stack].id = pro[p_pro].id;
			symbol_sk[p_stack].value = pro[p_pro++].value;  //���Ž�ջ
		}
		else if (SLR >= 100) {  //��Լ
			cout << "[" << number++ << "]\t\t";  //���
			cout << "r" << SLR - InitADD << "\t\t";  //����
			switch (SLR) {
			case 100:  //S'->E
				break;
			case 101:  //E->E + E
				p_stack -= 3;  //����3���ַ�
				E.address = newtemp();  //����һ����ʱ����
				/*��Ԫʽ*/
				ResSem.push_back({ {plus1,0},"+",symbol_sk[p_stack],symbol_sk[p_stack + 2],E.address });
				/*����ջ*/
				symbol_sk[p_stack].id = tempsy;
				symbol_sk[p_stack].value = E.address;
				break;
			case 102:  //E->E * E
				p_stack -= 3;  //����3���ַ�
				E.address = newtemp();  //����һ����ʱ����
				/*��Ԫʽ*/
				ResSem.push_back({ {times,0},"*",symbol_sk[p_stack],symbol_sk[p_stack + 2],E.address });
				/*����ջ*/
				symbol_sk[p_stack].id = tempsy;
				symbol_sk[p_stack].value = E.address;
				break;
			case 103:  //E->(E)
				p_stack -= 3;  //����3���ַ�
				E.address = symbol_sk[p_stack + 1].value;  //ֵ����
				/*����ջ*/
				symbol_sk[p_stack].id = tempsy;
				symbol_sk[p_stack].value = E.address;
				break;
			case 104:  //E->i
				p_stack--;  //����1���ַ�
				E.address = symbol_sk[p_stack].value;  //��ڵ�ַ
				break;
			default:
				break;
			}
			GOTO = getArithType(tempsy);  //��ȡGOTO��
			state_sk[p_stack] = ArithmeticTable[state_sk[p_stack - 1]][GOTO];
		}
		else if (SLR == acc) {
			//�����ɹ�
			ResSem.push_back({ {becomes,0},":=",symbol_sk[p_stack - 1],underline,left.value });
			cout << "�����ɹ�!" << endl;
			break;
		}
		else
			error(3);  //��ʾ�����������ʽʱ���ִ���

		/*״̬ջ��ʾ*/
		int cnt1 = 0;  //��¼���״̬ջ��λ�����������ݶ���
		for (int i = 0; i <= p_stack; i++) {
			cout << state_sk[i] << " ";
			if (state_sk[i] >= 10)
				cnt1 += 3;  //2λ+�ո�
			else
				cnt1 += 2;  //1λ+�ո�
		}
		cnt1++; //��\t���涨�������֮�����ټ��һ���ո񣬼�һλ
		/*���ݶ���*/
		for (int i = 0; i < (64 - cnt1) / 8; i++)
			cout << '\t';

		/*����ջ��ʾ*/
		for (int i = 0; i <= p_stack; i++) {
			cout << "(" << symbol_sk[i].id << "," << symbol_sk[i].value << ") ";
		}
		cout << endl;
		p_stack++;
	}
}

//�������ʽ
void SLR1Bool(vector<Binary> pro) {
	cout << "\n\n****************************************�������ʽ�ķ�������****************************************\n";
	for (auto i : pro) boolSentence(i);//��ʾ���ʽ
	cout << endl << "��Ӧ�Ķ�Ԫʽ���£�" << endl;
	for (auto i : pro) cout << "(" << i.id << "," << i.value << ")" << endl;
	int state_sk[SIZE_PRODUCTION];  //״̬ջ
	Binary symbol_sk[SIZE_PRODUCTION];  //����ջ
	int ACTION;  //ACTION��
	int GOTO;   //GOTO��
	int SLR;	//SLR����
	int number = 0;  //���
	int p_pro = 0;	 //����ʽpro���±�
	int p_stack = 0; //ջ���±�
	//��ʼ��״̬ջ�ͷ���ջ
	state_sk[p_stack] = 0;
	symbol_sk[p_stack].id = jinghao;
	symbol_sk[p_stack].value = 0;
	p_stack++;
	//�����������
	cout << "\n���\t\t" << "����\t\t" << "״̬ջ\t\t\t\t\t\t\t" << "����ջ" << endl;
	cout << "[" << number++ << "]\t\t";  //���
	cout << "_\t\t";  //״̬ջ
	cout << state_sk[0] << "\t\t\t\t\t\t\t"; //����ջ
	cout << "(" << symbol_sk[0].id << "," << symbol_sk[0].value << ")" << endl;  //����


	while (true) {
		if (p_stack < 0)
			error(6);  //ջ����
		ACTION = getBoolType(pro[p_pro].id);
		SLR = BoolTable[state_sk[p_stack - 1]][ACTION];
		if (SLR >= 0 && SLR < 100) {  //�ƽ�

			cout << "[" << number++ << "]\t\t";  //���
			cout << "s" << SLR << "\t\t";  //����
			state_sk[p_stack] = SLR;  //״̬��ջ
			symbol_sk[p_stack].id = pro[p_pro].id;
			symbol_sk[p_stack].value = pro[p_pro++].value;  //���Ž�ջ

		}
		else if (SLR >= 100) {  //��Լ
			cout << "[" << number++ << "]\t\t";  //���
			cout << "r" << SLR - InitADD << "\t\t";  //����
			switch (SLR) {
			case 100:  //S'->B'
				break;
			case 101:  //B->i
				p_stack--;  //����һ���ַ�
				B.ta = ResSem.size(); //��ڵ�ַ
				B.fa = ResSem.size() + 1; //�ٿڵ�ַ
				/*��Ԫʽ*/
				ResSem.push_back({ {-1,0},"jnz",symbol_sk[p_stack],underline,-1 });
				ResSem.push_back({ {-1,-1},"j",underline, underline,-1 });
				/*����ջ*/
				symbol_sk[p_stack].id = tempsy;
				symbol_sk[p_stack].value = 0;
				break;
			case 102:  //B->i rop i
				p_stack -= 3;  //���������ַ�
				B.ta = ResSem.size(); //��ڵ�ַ
				B.fa = ResSem.size() + 1; //�ٿڵ�ַ
				/*��Ԫʽ*/
				switch (symbol_sk[p_stack + 1].value) {
				case 0:  //<
					ResSem.push_back({ {rop,0},"j<",symbol_sk[p_stack],symbol_sk[p_stack + 2],-1 });
					break;
				case 1:  //<=
					ResSem.push_back({ {rop,1},"j<=",symbol_sk[p_stack],symbol_sk[p_stack + 2],-1 });
					break;
				case 2:  //<>
					ResSem.push_back({ {rop,2},"j<>",symbol_sk[p_stack],symbol_sk[p_stack + 2],-1 });
					break;
				case 3:  //>
					ResSem.push_back({ {rop,3},"j>",symbol_sk[p_stack],symbol_sk[p_stack + 2],-1 });
					break;
				case 4:  //>=
					ResSem.push_back({ {rop,4},"j>=",symbol_sk[p_stack],symbol_sk[p_stack + 2],-1 });
					break;
				case 5:  //=
					ResSem.push_back({ {rop,5},"j=",symbol_sk[p_stack],symbol_sk[p_stack + 2],-1 });
					break;
				default:
					break;
				}
				ResSem.push_back({ {-1,-1},"j",underline, underline,-1 });
				/*����ջ*/
				symbol_sk[p_stack].id = tempsy;
				symbol_sk[p_stack].value = 0;
				break;
			case 103:  //B->(B)
				p_stack = p_stack - 3;  //���������ַ�
				symbol_sk[p_stack].id = tempsy;
				symbol_sk[p_stack].value = 0;
				break;
			case 104:  //B->not B
				p_stack = p_stack - 2;  //���������ַ�
				swap(B.ta, B.fa);  //��ٿڵ�ַ����
				/*����ջ*/
				symbol_sk[p_stack].id = tempsy;
				symbol_sk[p_stack].value = 0;
				break;
			case 105:  //A->B and
				p_stack = p_stack - 2;  //���������ַ�
				backpatch(B.ta, ResSem.size() + InitADD); //B Ϊ�棬���� and �������������ж�
				A.fa = B.fa;  //�� B Ϊ�٣�����֪�������жϺ���������Ԫʽ�����Լٿڽ��д���
				/*����ջ*/
				symbol_sk[p_stack].id = EA;
				symbol_sk[p_stack].value = 0;
				break;
			case 106: //B->AB
				p_stack = p_stack - 2;  //���������ַ�
				B.fa = merge(A.fa, B.fa);  //ͬһ��������Ĳ������ʽ�ļٿ�����ͬ��
				/*����ջ*/
				symbol_sk[p_stack].id = tempsy;
				symbol_sk[p_stack].value = 0;
				break;
			case 107: //O->B or
				p_stack = p_stack - 2;  //���������ַ�
				backpatch(B.fa, ResSem.size() + InitADD);  //�� B Ϊ�٣���ô�϶����� or �������������ж�
				O.ta = B.ta;  //���� B Ϊ�棬����֪�������жϺ���������Ԫʽ��������ڽ��д���
				/*����ջ*/
				symbol_sk[p_stack].id = EO;
				symbol_sk[p_stack].value = 0;
				break;
			case 108:  //B->OB
				p_stack = p_stack - 2;  //���������ַ�
				B.ta = merge(O.ta, B.ta);  //ͬһ��������Ĳ������ʽ���������ͬ��
				/*����ջ*/
				symbol_sk[p_stack].id = tempsy;
				symbol_sk[p_stack].value = 0;
				break;
			default:
				break;
			}

			GOTO = getBoolType(symbol_sk[p_stack].id);  //��ȡGOTO��
			state_sk[p_stack] = BoolTable[state_sk[p_stack - 1]][GOTO];
		}
		else if (SLR == acc) {
			//�����ɹ�
			cout << "�����ɹ���" << endl;
			break;
		}
		else
			error(4);  //��ʾ�����������ʽʱ���ִ���

		/*״̬ջ��ʾ*/
		int cnt1 = 0;  //��¼���״̬ջ��λ�����������ݶ���
		for (int i = 0; i <= p_stack; i++) {
			cout << state_sk[i] << " ";
			if (state_sk[i] >= 10) cnt1 += 3;  //��λ+�ո�
			else cnt1 += 2;  //1λ+�ո�
		}
		cnt1++; //��\t���涨�������֮�����ټ��һ���ո񣬼�һλ
		/*���ݶ���*/
		for (int i = 0; i < (64 - cnt1) / 8; i++) cout << '\t';

		/*����ջ��ʾ*/
		for (int i = 0; i <= p_stack; i++) {
			cout << "(" << symbol_sk[i].id << "," << symbol_sk[i].value << ") ";
		}
		cout << endl;
		p_stack++;
	}
}

//�������
void SLR1() {
	cout << "\n\n*****************************************�������ķ�������****************************************\n";
	int state_sk[SIZE_PRODUCTION];  //״̬ջ
	int symbol_sk[SIZE_PRODUCTION];  //����ջ
	NT_semantic TF_export_sk[SIZE_PRODUCTION]; //��ٿ�ջ������ÿ���������ʽe����ٿ�
	int jmp_sk[SIZE_PRODUCTION]; //��Ԫʽ��ַջ�����ڴ�Ϊ������else��ĳ�����������ת��ָ����Ԫʽ��ַ���Ա����
	int ACTION;  //ACTION��
	int GOTO;   //GOTO��
	int SLR;	//SLR����
	int number = 0;  //���
	int p_stack = 0; //ջ���±�
	int p_tf = 0;  //��ٿ�ջ���±�
	int p_jm = 0;  //��Ԫʽ��ַջ�±�
	int cnt = 0;  //��Ԫʽ�±�
	//��ʼ��״̬ջ�ͷ���ջ
	state_sk[p_stack] = 0;
	symbol_sk[p_stack] = jinghao;
	p_stack++;
	//�����������
	cout << "���\t\t" << "����\t\t" << "״̬ջ\t\t\t\t\t\t\t" << "����ջ" << endl;
	cout << "[" << number++ << "]\t\t";  //���
	cout << "_\t\t";  //����
	cout << state_sk[0] << "\t\t\t\t\t\t\t"; //״̬ջ
	cout << symbol_sk[0] << endl;  //����ջ

	while (true) {
		if (p_stack < 0)
			error(6);  //ջ����
		ACTION = ResLex[cnt].id;
		if (is_pro(ResLex[cnt].id)) {  //�Ǳ��ʽ��ֵ
			int length = get_pro(cnt);  //�õ����ʽ�����س���
			//if��while
			if (cnt > 0 && ResLex[cnt - 1].id == sy_if || ResLex[cnt - 1].id == sy_while) {
				//�������ʽ
				TF_export_sk[p_tf].ta = ResSem.size();  //�����ʱ�����
				SLR1Bool(production);   //�������ʽ����
				cout << "\n\n***************************************�����������ķ�������***************************************\n";
				
				symbol_sk[p_stack] = e;  //�������ʽ�ķ���
				ACTION = e;
				backpatch(B.ta, ResSem.size() + InitADD);
				//�ٿڱ���
				TF_export_sk[p_tf].fa = B.fa;
				p_tf++;
			}
			else {
				//�������ʽ
				SLR1Arithmetic(production);  //�������ʽ�͸�ֵ������
				cout << "\n\n***************************************�����������ķ�������***************************************\n";
				symbol_sk[p_stack] = a;  //��ֵ���
				ACTION = a;
			}
			cnt = length;	//�������ʽ��ĵ�һ���ַ�
		}
		SLR = ProgramTable[state_sk[p_stack - 1]][ACTION];  //��ȡ����

		if (SLR >= 0 && SLR < 100) {  //�ƽ�
			cout << "[" << number++ << "]\t\t";  //���
			cout << "s" << SLR << "\t\t";  //����
			state_sk[p_stack] = SLR;  //״̬��ջ
			symbol_sk[p_stack] = ACTION;  //���Ž�ջ
			cnt++; //Ϊ������һ��������׼��
		}
		else if (SLR >= 100) {  //��Լ
			cout << "[" << number++ << "]\t\t";  //���
			cout << "r" << SLR - InitADD << "\t\t";  //����
			switch (SLR) {
			case 100:  //S'->S
				break;
			case 101:  //S->if e then S else S
				p_stack -= 6;  //����6���ַ�
				symbol_sk[p_stack] = S;  //��ԼΪS������ʽ�󲿵ģ�
				ResSem[jmp_sk[--p_jm]].result = ResSem.size() + InitADD;
				break;
			case 102:  //S->while e do S
				p_stack -= 4;  //����4���ַ�
				symbol_sk[p_stack] = S;  //��ԼΪS������ʽ�󲿵ģ�
				break;
			case 103:  //S->begin L end
				p_stack -= 3;  //����3���ַ�
				symbol_sk[p_stack] = S;  //��ԼΪS
				break;
			case 104:  //S->a
				p_stack--;  //����1���ַ�
				symbol_sk[p_stack] = S;  //��ԼΪS
				break;
			case 105:  //L->S
				p_stack--;  //����1���ַ�
				symbol_sk[p_stack] = L;  //��ԼΪL
				break;
			case 106:  //L->S;L
				p_stack -= 3;  //����3���ַ�
				symbol_sk[p_stack] = L;  //��ԼΪL
				break;
			default:
				break;
			}
			if (symbol_sk[p_stack] == S) {
				//S��ǰһ���ַ���do����while e do S
				if (symbol_sk[p_stack - 1] == sy_do) {
					ResSem.push_back({ {-1,-1},"j",underline,underline,TF_export_sk[--p_tf].ta + InitADD }); //S����������תӦ����S�����e��
					backpatch(TF_export_sk[p_tf].fa, ResSem.size() + InitADD);  //�ٿڻ���
				}
				//S��ǰһ���ַ���then����if e then S
				if (symbol_sk[p_stack - 1] == sy_then) {
					ResSem.push_back({ {-1,-1},"j",underline,underline,-1 }); //��������תָ�����else�����Ǵ�ʱ����֪��Ӧ��ת������
					backpatch(TF_export_sk[--p_tf].fa, ResSem.size() + InitADD);  //�ٿڻ���
					jmp_sk[p_jm++] = ResSem.size() - 1;  //�ݴ�������ת����Ԫʽ��ַ
				}
			}
			GOTO = symbol_sk[p_stack];  //���GOTO��
			state_sk[p_stack] = ProgramTable[state_sk[p_stack - 1]][GOTO]; //״̬��ջ
		}
		else if (SLR == acc) {  //�����ɹ�
			cout << "�����ɹ���" << endl;
			break;
		}
		else
			error(5);  //��ʾ�����������ʱ���ִ���

		/*״̬ջ��ʾ*/
		int cnt1 = 0;  //��¼���״̬ջ��λ�����������ݶ���
		for (int i = 0; i <= p_stack; i++) {
			cout << state_sk[i] << " ";
			if (state_sk[i] >= 10) cnt1 += 3;  //��λ+�ո�
			else cnt1 += 2;  //1λ+�ո�
		}
		cnt1++; //��\t���涨�������֮�����ټ��һ���ո񣬼�һλ
		/*���ݶ���*/
		for (int i = 0; i < (64 - cnt1) / 8; i++) cout << '\t';

		/*����ջ��ʾ*/
		for (int i = 0; i <= p_stack; i++) {
			cout << symbol_sk[i] << " ";
		}
		cout << endl;
		p_stack++;
	}
}


