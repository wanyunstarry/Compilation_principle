#pragma once
#include "data.h"
//生成汇编程序
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
	ofstream outfile("data1.asm", ios_base::out);  //以写入方式打开
	if (!outfile.is_open()) {
		cout << "打开data1.asm文件错误！" << endl;
		return;
	}
	outfile << ";**********************************************************************************\n";
	cout << ";**********************************************************************************\n";
	outfile << "DSEG SEGMENT\t\t\t\t\t;定义数据段\n ";
	cout << "DSEG SEGMENT\t\t\t\t\t\t;定义数据段\n ";
	for (unsigned i = 0; i < VarList.size(); i++) {
		outfile << '\t' << VarList[i] << "\t\tDW\t0\n";  //定义字变量
		cout << '\t' << VarList[i] << "\tDW\t 0\n";  //定义字变量
	}
	outfile << "DSEG ENDS\t\t\t\t\t\t;数据段定义结束\n";
	outfile << ";**********************************************************************************\n";
	outfile << "CSEG SEGMENT\t\t\t\t\t;定义代码段\n";
	outfile << "MAIN PROC FAR\t\t\t\t\t;程序的执行部分\n";
	outfile << "\tASSUME CS: CSEG, DS: DSEG\t;设置段寄存器与逻辑段的关系\n";
	outfile << "START:\n";
	outfile << "\t\tPUSH  DS\n";
	outfile << "\t\tSUB  BX, BX\n";
	outfile << "\t\tPUSH  BX\n";
	outfile << "\t\tMOV	 BX, DSEG\n";
	outfile << "\t\tMOV	 DS, BX\t\t\t\t;设置数据段段地址\n";
	outfile << ";********************************翻译语句由此开始\n";

	cout << "DSEG ENDS\t\t\t\t\t\t;数据段定义结束\n";
	cout << ";**********************************************************************************\n";
	cout << "CSEG SEGMENT\t\t\t\t\t\t;定义代码段\n";
	cout << "MAIN PROC FAR\t\t\t\t\t\t;程序的执行部分\n";
	cout << "\tASSUME CS: CSEG, DS: DSEG\t\t\t;设置段寄存器与逻辑段的关系\n";
	cout << "START:\n";
	cout << "\t\tPUSH     DS\n";
	cout << "\t\tSUB      BX, BX\n";
	cout << "\t\tPUSH     BX\n";
	cout << "\t\tMOV	 BX, DSEG\n";
	cout << "\t\tMOV	 DS, BX\t\t\t\t;设置数据段段地址\n";
	cout << ";********************************************************翻译语句由此开始\n";

	/*翻译开始*/
	unsigned int i;  //ResSem下标
	for (i = 0; i < ResSem.size(); i++) {
		if (ResSem[i].op[0] != 'j') {  //算术表达式和赋值语句块
			unsigned int j = i;
			int T_cnt = 0;  //记录当前基本块内临时变量数
			mp.clear();  //进入新的基本块之前清空之前的临时变量记录
			while (j < ResSem.size() && ResSem[j].op[0] != 'j') {  //开始对当前基本块操作
				/*arg1*/
				switch (ResSem[j].arg1.id) {
				case ident:
					_arg1 = VarList[ResSem[j].arg1.value];
					break;
				case intconst:
					_arg1 = to_string(ResSem[j].arg1.value);  //汇编常数默认十进制
					break;
				case tempsy:
					if (!mp[ResSem[j].arg1.value]) {   //之前没出现过该临时变量
						T_cnt++;
						mp[ResSem[j].arg1.value] = true;  //标记出现过
					}
					_arg1 = selectRegister(T_cnt);  //出现过则Tcnt不加1
				case -1:
					break;
				}
				/*arg2*/
				switch (ResSem[j].arg2.id) {
				case ident:
					_arg2 = VarList[ResSem[j].arg2.value];
					break;
				case intconst:
					_arg2 = to_string(ResSem[j].arg2.value);  //汇编常数默认十进制
					break;
				case tempsy:
					if (!mp[ResSem[j].arg2.value]) {   //之前没出现过该临时变量
						T_cnt++;
						mp[ResSem[j].arg2.value] = true;  //标记出现过
					}
					_arg2 = selectRegister(T_cnt);  //出现过则Tcnt不加1
				case -1:
					break;
				}
				/*result*/
				if (ResSem[j].op_type.id == becomes)   //赋值语句
					_result = VarList[ResSem[j].result];
				else {  //算术表达式
					if (!mp[ResSem[j].result]) {   //之前没出现过该临时变量
						T_cnt++;
						mp[ResSem[j].result] = true;  //标记出现过
					}
					_result = selectRegister(T_cnt);  //出现过则Tcnt不加1
				}
				//地址
				outfile << "L" << j + InitADD << ":";
				cout << "L" << j + InitADD << ":";

				//语句
				switch (ResSem[j].op_type.id) {
				case plus1:  //加法
					outfile << "\tMOV	 AX, " << _arg1 << endl;
					outfile << "\t\tADD	 AX, " << _arg2 << endl;  //带进位加法，结果存在AX
					outfile << "\t\tMOV	 " << _result << ", AX" << endl;  //结果存到_result
					cout << "\t\tMOV	 AX, " << _arg1 << endl;
					cout << "\t\tADD	 AX, " << _arg2 << endl;  //带进位加法，结果存在AX
					cout << "\t\tMOV	 " << _result << ", AX" << endl;  //结果存到_result
					break;
				case times:  //乘法
					outfile << "\tMOV	 AX, " << _arg1 << endl;
					outfile << "\t\tMUL  " << _arg2 << endl;
					outfile << "\t\tMOV	 " << _result << ", AX" << endl;  //结果存到_result
					cout << "\t\tMOV	 AX, " << _arg1 << endl;
					cout << "\t\tMUL      " << _arg2 << endl;
					cout << "\t\tMOV	 " << _result << ", AX" << endl;  //结果存到_result
					break;
				case becomes:  //赋值运算
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
		else {  //跳转和判断语句
			/*arg1*/
			switch (ResSem[i].arg1.id) {
			case ident:
				_arg1 = VarList[ResSem[i].arg1.value];
				break;
			case intconst:
				_arg1 = to_string(ResSem[i].arg1.value);  //汇编常数默认十进制
				break;
			case -1:
				break;
			}
			/*arg2*/
			switch (ResSem[i].arg2.id) {
			case ident:
				_arg2 = VarList[ResSem[i].arg2.value];
				break;
			case intconst:
				_arg2 = to_string(ResSem[i].arg2.value);  //汇编常数默认十进制
				break;
			case -1:
				break;
			}
			/*result*/
			if (ResSem[i].op[0] == 'j')  //跳转指令
				jmpLocation = ResSem[i].result;
			//地址
			outfile << "L" << i + InitADD << ":";
			cout << "L" << i + InitADD << ":";

			//语句
			switch (ResSem[i].op_type.id) {
			case rop:  //关系运算符
				outfile << "\tMOV	 AX, " << _arg1 << endl;
				outfile << "\t\tCMP	 AX, " << _arg2 << endl;  //比较，_arg1-_arg2，不保存结果
				cout << "\t\tMOV	 AX, " << _arg1 << endl;
				cout << "\t\tCMP	 AX, " << _arg2 << endl;  //比较，_arg1-_arg2，不保存结果
				switch (ResSem[i].op_type.value) {
				case 0:  //<
					outfile << "\t\tJL  L" << jmpLocation << endl;
					cout << "\t\tJL  L" << jmpLocation << endl;
					break;
				case 1:  //<=
					outfile << "\t\tJLE  L" << jmpLocation << endl;
					cout << "\t\tJLE  L" << jmpLocation << endl;
					break;
				case 2:  //<>
					outfile << "\t\tJNZ  L" << jmpLocation << endl;
					cout << "\t\tJNZ  L" << jmpLocation << endl;
					break;
				case 3:  //>
					outfile << "\t\tJG  L" << jmpLocation << endl;
					cout << "\t\tJG       L" << jmpLocation << endl;
					break;
				case 4:  //>=
					outfile << "\t\tJGE  L" << jmpLocation << endl;
					cout << "\t\tJGE      L" << jmpLocation << endl;
					break;
				case 5:  //=
					outfile << "\t\tJZ   L" << jmpLocation << endl;
					cout << "\t\tJZ       L" << jmpLocation << endl;
					break;
				}
				break;
			case -1:
				switch (ResSem[i].op_type.value) {
				case -1:  //j
					outfile << "\tJMP	 L" << jmpLocation << endl;
					cout << "\t\tJMP	 L" << jmpLocation << endl;
					break;
				case 0:  //jnz
					outfile << "\tJNZ	 L" << jmpLocation << endl;
					cout << "\tJNZ	 L" << jmpLocation << endl;
					break;
				}
			}
		}
	}
	outfile << ";********************************翻译语句结束\n";
	outfile << "L" << i + InitADD << ":\tPOP  BX\n";
	outfile << "\t\tPOP  DS\n";
	outfile << "\t\tMOV  AX, 4C00H\t\t\t;为DOS功能调用设置功能号和入口参数\n";
	outfile << "\t\tINT  21H\t\t\t\t;DOS功能调用，返回DOS操作系统\n";
	outfile << "\t\tRET\t\t\t\t\t\t;返回\n";
	outfile << "MAIN ENDP\t\t\t\t\t\t;子程序结束\n";
	outfile << "CSEG ENDS\t\t\t\t\t\t;代码段定义结束\n";
	outfile << "\tEND START\t\t\t\t\t;汇编结束\n";

	cout << ";********************************************************翻译语句结束\n";
	cout << "L" << i + InitADD << ":\t\tPOP      BX\n";
	cout << "\t\tPOP      DS\n";
	cout << "\t\tMOV      AX, 4C00H\t\t\t;为DOS功能调用设置功能号和入口参数\n";
	cout << "\t\tINT      21H\t\t\t\t;DOS功能调用，返回DOS操作系统\n";
	cout << "\t\tRET\t\t\t\t\t\t;返回\n";
	cout << "MAIN ENDP\t\t\t\t\t\t;子程序结束\n";
	cout << "CSEG ENDS\t\t\t\t\t\t;代码段定义结束\n";
	cout << "\tEND START\t\t\t\t\t;汇编结束\n";
	cout << "并且将编译生成汇编程序并保存至data1.asm文件\n";
	outfile.close();
}