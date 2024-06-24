#pragma once
#include "lexical_analysis.h"
//��ʾ������������������Ԫʽ��pas.med
void ShowAndSaveRes() {
	ofstream outfile("pas.med", ios_base::out);  //��д�뷽ʽ��
	if (!outfile.is_open()) {
		cout << "��pas1.med�ļ�����" << endl;
		return;
	}
	cout << "\n*************************��Ԫʽ�������*************************\n";
	for (unsigned int i = 0; i < ResSem.size(); i++) {
		cout << i + InitADD << " \t(" << ResSem[i].op << " , ";  //op
		outfile << i + InitADD << " \t(" << ResSem[i].op << " , ";  //op
		//arg1
		switch (ResSem[i].arg1.id) {
		case ident:  //����
			cout << VarTable[ResSem[i].arg1.value];
			outfile << VarTable[ResSem[i].arg1.value];
			break;
		case intconst:  //������
			cout << ResSem[i].arg1.value;
			outfile << ResSem[i].arg1.value;
			break;
		case tempsy:  //��ʱ����
			cout << "T" << ResSem[i].arg1.value;
			outfile << "T" << ResSem[i].arg1.value;
			break;
		case -1:
			cout << "_";
			outfile << "_";
		}
		cout << " , ";
		outfile << " , ";
		//arg2
		switch (ResSem[i].arg2.id) {
		case ident:  //����
			cout << VarTable[ResSem[i].arg2.value];
			outfile << VarTable[ResSem[i].arg2.value];
			break;
		case intconst:  //������
			cout << ResSem[i].arg2.value;
			outfile << ResSem[i].arg2.value;
			break;
		case tempsy:  //��ʱ����
			cout << "T" << ResSem[i].arg2.value;
			outfile << "T" << ResSem[i].arg2.value;
			break;
		case -1:
			cout << "_";
			outfile << "_";
		}
		cout << " , ";
		outfile << " , ";
		//result
		if (ResSem[i].op[0] == 'j') {
			cout << ResSem[i].result;
			outfile << ResSem[i].result;
		}
		else {
			if (ResSem[i].op == ":=") {
				cout << VarTable[ResSem[i].result];
				outfile << VarTable[ResSem[i].result];
			}
			else {//��ʱ����
				cout << "T" << ResSem[i].result;
				outfile << "T" << ResSem[i].result;
			}
		}
		cout << ")" << endl;
		outfile << ")" << endl;
	}
	cout << ResSem.size() + InitADD;
	outfile << ResSem.size() + InitADD;
	cout << "\n��Ԫʽ����ѱ�����pas.med�ļ�\n";
	cout << endl;
	outfile.close();
}