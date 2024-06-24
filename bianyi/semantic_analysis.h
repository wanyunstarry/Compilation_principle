#pragma once
#include "lexical_analysis.h"
//显示语义分析结果并保存四元式到pas.med
void ShowAndSaveRes() {
	ofstream outfile("pas.med", ios_base::out);  //以写入方式打开
	if (!outfile.is_open()) {
		cout << "打开pas1.med文件错误！" << endl;
		return;
	}
	cout << "\n*************************四元式分析结果*************************\n";
	for (unsigned int i = 0; i < ResSem.size(); i++) {
		cout << i + InitADD << " \t(" << ResSem[i].op << " , ";  //op
		outfile << i + InitADD << " \t(" << ResSem[i].op << " , ";  //op
		//arg1
		switch (ResSem[i].arg1.id) {
		case ident:  //变量
			cout << VarTable[ResSem[i].arg1.value];
			outfile << VarTable[ResSem[i].arg1.value];
			break;
		case intconst:  //整常量
			cout << ResSem[i].arg1.value;
			outfile << ResSem[i].arg1.value;
			break;
		case tempsy:  //临时变量
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
		case ident:  //变量
			cout << VarTable[ResSem[i].arg2.value];
			outfile << VarTable[ResSem[i].arg2.value];
			break;
		case intconst:  //整常量
			cout << ResSem[i].arg2.value;
			outfile << ResSem[i].arg2.value;
			break;
		case tempsy:  //临时变量
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
			else {//临时变量
				cout << "T" << ResSem[i].result;
				outfile << "T" << ResSem[i].result;
			}
		}
		cout << ")" << endl;
		outfile << ")" << endl;
	}
	cout << ResSem.size() + InitADD;
	outfile << ResSem.size() + InitADD;
	cout << "\n四元式结果已保存至pas.med文件\n";
	cout << endl;
	outfile.close();
}