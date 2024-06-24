#pragma once
#pragma warning( disable : 6386) 
#pragma warning( disable : 6001) 
#pragma warning( disable : 6385) 
#include "Generate_assembly_code.h"
//测试字符是否为表达式中的值
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

//获得表达式
int get_pro(int p_pro) {
	production.clear();  //表达式表清零
	//获得表达式
	do {
		production.push_back({ ResLex[p_pro].id ,ResLex[p_pro].value });
		p_pro++;
	} while (is_pro(ResLex[p_pro].id));
	production.push_back({ jinghao,0 });  //添加#
	return p_pro - 1;  //返回表达式的长度
}

//每一次调用，返回一个代表新临时变量的种别码
int newtemp() {
	static int new_id = 0;
	return ++new_id;
}

//获取算术表达式字符类型（对应ArithmeticTable表）
int getArithType(int id) {
	switch (id) {
	case ident:
	case intconst:
		return 0;  //变量或整常量
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
		return 6;  //临时变量
	default:
		error(1);  //显示该终结符/变量不在算术表达式与赋值语句中
		return -1;
	}
}

//获取布尔表达式字符类型（对应BoolTable表）
int getBoolType(int id) {
	switch (id) {
	case ident:
	case intconst:
		return 0;  //变量或整常量
	case rop:
		return 1;  //关系运算符
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
		return 8;  //临时变量
	case EA:
		return 9;  //B合取
	case EO:
		return 10; //B析取
	default:
		error(2);  //显示该终结符或变量不在布尔表达式中
		return -1;
	}
}

//把p所链接的每个四元式的result改写为地址address
void backpatch(int p, int address) {
	int q;
	q = p;
	while (q != -1) {
		int temp = ResSem[q].result;
		ResSem[q].result = address;
		q = temp;
	}
}

//把以p1和p2为链首的两条链合并为一条以p2为链首的链（返回链首值p2）
int merge(int p1, int p2) {
	if (p2 == 0)
		return p1;
	else {
		int temp = p2;
		while (ResSem[temp].result >= 0) {  //result为有效值
			temp = ResSem[temp].result;
		}
		ResSem[temp].result = p1;
	}
	return p2;
}

//根据算术表达式和赋值语句二元式得到对应的原式（只用于提示）
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

//根据布尔表达式二元式得到对应的原式（只用于提示）
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

//算术表达式
void SLR1Arithmetic(vector<Binary> pro) {
	cout << "\n\n****************************************算术表达式和赋值语句的分析过程****************************************\n";
	for (auto i : pro) arithSentence(i);
	cout << endl << "对应的二元式如下：" << endl;
	for (auto i : pro) cout << "(" << i.id << "," << i.value << ")" << endl;
	int state_sk[SIZE_PRODUCTION];  //状态栈
	Binary symbol_sk[SIZE_PRODUCTION];  //符号栈
	int ACTION;  //ACTION列
	int GOTO;   //GOTO列	
	int SLR;	//SLR动作
	int number = 0;  //序号
	int p_pro = 2;	 //产生式pro的下标（从:=后分析）
	int p_stack = 0; //栈的下标
	Binary left;     //存表达式的左值
	//初始化左值、状态栈和符号栈
	left = pro.front();
	state_sk[p_stack] = 0;
	symbol_sk[p_stack].id = jinghao;
	symbol_sk[p_stack].value = 0;
	p_stack++;
	//输出分析过程
	cout << "\n序号\t\t" << "动作\t\t" << "状态栈\t\t\t\t\t\t\t" << "符号栈" << endl;
	cout << "[" << number++ << "]\t\t";  //序号
	cout << "_\t\t";  //动作
	cout << state_sk[0] << "\t\t\t\t\t\t\t"; //状态栈
	cout << "(" << symbol_sk[0].id << "," << symbol_sk[0].value << ")" << endl;  //符号栈

	while (true) {
		if (p_stack < 0)
			error(6);  //栈错误
		ACTION = getArithType(pro[p_pro].id);
		SLR = ArithmeticTable[state_sk[p_stack - 1]][ACTION];
		if (SLR >= 0 && SLR < 100) {  //移进
			cout << "[" << number++ << "]\t\t";  //序号
			cout << "s" << SLR << "\t\t";  //动作
			state_sk[p_stack] = SLR;  //状态进栈
			symbol_sk[p_stack].id = pro[p_pro].id;
			symbol_sk[p_stack].value = pro[p_pro++].value;  //符号进栈
		}
		else if (SLR >= 100) {  //规约
			cout << "[" << number++ << "]\t\t";  //序号
			cout << "r" << SLR - InitADD << "\t\t";  //动作
			switch (SLR) {
			case 100:  //S'->E
				break;
			case 101:  //E->E + E
				p_stack -= 3;  //回退3个字符
				E.address = newtemp();  //生成一个临时变量
				/*四元式*/
				ResSem.push_back({ {plus1,0},"+",symbol_sk[p_stack],symbol_sk[p_stack + 2],E.address });
				/*符号栈*/
				symbol_sk[p_stack].id = tempsy;
				symbol_sk[p_stack].value = E.address;
				break;
			case 102:  //E->E * E
				p_stack -= 3;  //回退3个字符
				E.address = newtemp();  //生成一个临时变量
				/*四元式*/
				ResSem.push_back({ {times,0},"*",symbol_sk[p_stack],symbol_sk[p_stack + 2],E.address });
				/*符号栈*/
				symbol_sk[p_stack].id = tempsy;
				symbol_sk[p_stack].value = E.address;
				break;
			case 103:  //E->(E)
				p_stack -= 3;  //回退3个字符
				E.address = symbol_sk[p_stack + 1].value;  //值不变
				/*符号栈*/
				symbol_sk[p_stack].id = tempsy;
				symbol_sk[p_stack].value = E.address;
				break;
			case 104:  //E->i
				p_stack--;  //回退1个字符
				E.address = symbol_sk[p_stack].value;  //入口地址
				break;
			default:
				break;
			}
			GOTO = getArithType(tempsy);  //获取GOTO列
			state_sk[p_stack] = ArithmeticTable[state_sk[p_stack - 1]][GOTO];
		}
		else if (SLR == acc) {
			//分析成功
			ResSem.push_back({ {becomes,0},":=",symbol_sk[p_stack - 1],underline,left.value });
			cout << "分析成功!" << endl;
			break;
		}
		else
			error(3);  //显示分析算术表达式时出现错误

		/*状态栈显示*/
		int cnt1 = 0;  //记录输出状态栈的位数，用于数据对齐
		for (int i = 0; i <= p_stack; i++) {
			cout << state_sk[i] << " ";
			if (state_sk[i] >= 10)
				cnt1 += 3;  //2位+空格
			else
				cnt1 += 2;  //1位+空格
		}
		cnt1++; //’\t’规定两个输出之间最少间隔一个空格，加一位
		/*数据对齐*/
		for (int i = 0; i < (64 - cnt1) / 8; i++)
			cout << '\t';

		/*符号栈显示*/
		for (int i = 0; i <= p_stack; i++) {
			cout << "(" << symbol_sk[i].id << "," << symbol_sk[i].value << ") ";
		}
		cout << endl;
		p_stack++;
	}
}

//布尔表达式
void SLR1Bool(vector<Binary> pro) {
	cout << "\n\n****************************************布尔表达式的分析过程****************************************\n";
	for (auto i : pro) boolSentence(i);//显示表达式
	cout << endl << "对应的二元式如下：" << endl;
	for (auto i : pro) cout << "(" << i.id << "," << i.value << ")" << endl;
	int state_sk[SIZE_PRODUCTION];  //状态栈
	Binary symbol_sk[SIZE_PRODUCTION];  //符号栈
	int ACTION;  //ACTION列
	int GOTO;   //GOTO列
	int SLR;	//SLR动作
	int number = 0;  //序号
	int p_pro = 0;	 //产生式pro的下标
	int p_stack = 0; //栈的下标
	//初始化状态栈和符号栈
	state_sk[p_stack] = 0;
	symbol_sk[p_stack].id = jinghao;
	symbol_sk[p_stack].value = 0;
	p_stack++;
	//输出分析过程
	cout << "\n序号\t\t" << "动作\t\t" << "状态栈\t\t\t\t\t\t\t" << "符号栈" << endl;
	cout << "[" << number++ << "]\t\t";  //序号
	cout << "_\t\t";  //状态栈
	cout << state_sk[0] << "\t\t\t\t\t\t\t"; //符号栈
	cout << "(" << symbol_sk[0].id << "," << symbol_sk[0].value << ")" << endl;  //动作


	while (true) {
		if (p_stack < 0)
			error(6);  //栈错误
		ACTION = getBoolType(pro[p_pro].id);
		SLR = BoolTable[state_sk[p_stack - 1]][ACTION];
		if (SLR >= 0 && SLR < 100) {  //移进

			cout << "[" << number++ << "]\t\t";  //序号
			cout << "s" << SLR << "\t\t";  //动作
			state_sk[p_stack] = SLR;  //状态进栈
			symbol_sk[p_stack].id = pro[p_pro].id;
			symbol_sk[p_stack].value = pro[p_pro++].value;  //符号进栈

		}
		else if (SLR >= 100) {  //规约
			cout << "[" << number++ << "]\t\t";  //序号
			cout << "r" << SLR - InitADD << "\t\t";  //动作
			switch (SLR) {
			case 100:  //S'->B'
				break;
			case 101:  //B->i
				p_stack--;  //回退一个字符
				B.ta = ResSem.size(); //真口地址
				B.fa = ResSem.size() + 1; //假口地址
				/*四元式*/
				ResSem.push_back({ {-1,0},"jnz",symbol_sk[p_stack],underline,-1 });
				ResSem.push_back({ {-1,-1},"j",underline, underline,-1 });
				/*符号栈*/
				symbol_sk[p_stack].id = tempsy;
				symbol_sk[p_stack].value = 0;
				break;
			case 102:  //B->i rop i
				p_stack -= 3;  //回退三个字符
				B.ta = ResSem.size(); //真口地址
				B.fa = ResSem.size() + 1; //假口地址
				/*四元式*/
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
				/*符号栈*/
				symbol_sk[p_stack].id = tempsy;
				symbol_sk[p_stack].value = 0;
				break;
			case 103:  //B->(B)
				p_stack = p_stack - 3;  //回退三个字符
				symbol_sk[p_stack].id = tempsy;
				symbol_sk[p_stack].value = 0;
				break;
			case 104:  //B->not B
				p_stack = p_stack - 2;  //回退三个字符
				swap(B.ta, B.fa);  //真假口地址交换
				/*符号栈*/
				symbol_sk[p_stack].id = tempsy;
				symbol_sk[p_stack].value = 0;
				break;
			case 105:  //A->B and
				p_stack = p_stack - 2;  //回退两个字符
				backpatch(B.ta, ResSem.size() + InitADD); //B 为真，跳到 and 后的语句进行真假判断
				A.fa = B.fa;  //如 B 为假，并不知道跳出判断后到了哪条四元式，所以假口进行传递
				/*符号栈*/
				symbol_sk[p_stack].id = EA;
				symbol_sk[p_stack].value = 0;
				break;
			case 106: //B->AB
				p_stack = p_stack - 2;  //回退两个字符
				B.fa = merge(A.fa, B.fa);  //同一条与运算的布尔表达式的假口是相同的
				/*符号栈*/
				symbol_sk[p_stack].id = tempsy;
				symbol_sk[p_stack].value = 0;
				break;
			case 107: //O->B or
				p_stack = p_stack - 2;  //回退两个字符
				backpatch(B.fa, ResSem.size() + InitADD);  //如 B 为假，那么肯定跳到 or 后的语句进行真假判断
				O.ta = B.ta;  //假如 B 为真，并不知道跳出判断后到了哪条四元式，所以真口进行传递
				/*符号栈*/
				symbol_sk[p_stack].id = EO;
				symbol_sk[p_stack].value = 0;
				break;
			case 108:  //B->OB
				p_stack = p_stack - 2;  //回退两个字符
				B.ta = merge(O.ta, B.ta);  //同一条或运算的布尔表达式的真口是相同的
				/*符号栈*/
				symbol_sk[p_stack].id = tempsy;
				symbol_sk[p_stack].value = 0;
				break;
			default:
				break;
			}

			GOTO = getBoolType(symbol_sk[p_stack].id);  //获取GOTO列
			state_sk[p_stack] = BoolTable[state_sk[p_stack - 1]][GOTO];
		}
		else if (SLR == acc) {
			//分析成功
			cout << "分析成功！" << endl;
			break;
		}
		else
			error(4);  //显示分析布尔表达式时出现错误

		/*状态栈显示*/
		int cnt1 = 0;  //记录输出状态栈的位数，用于数据对齐
		for (int i = 0; i <= p_stack; i++) {
			cout << state_sk[i] << " ";
			if (state_sk[i] >= 10) cnt1 += 3;  //两位+空格
			else cnt1 += 2;  //1位+空格
		}
		cnt1++; //’\t’规定两个输出之间最少间隔一个空格，加一位
		/*数据对齐*/
		for (int i = 0; i < (64 - cnt1) / 8; i++) cout << '\t';

		/*符号栈显示*/
		for (int i = 0; i <= p_stack; i++) {
			cout << "(" << symbol_sk[i].id << "," << symbol_sk[i].value << ") ";
		}
		cout << endl;
		p_stack++;
	}
}

//程序语句
void SLR1() {
	cout << "\n\n*****************************************控制语句的分析过程****************************************\n";
	int state_sk[SIZE_PRODUCTION];  //状态栈
	int symbol_sk[SIZE_PRODUCTION];  //符号栈
	NT_semantic TF_export_sk[SIZE_PRODUCTION]; //真假口栈，保存每个布尔表达式e的真假口
	int jmp_sk[SIZE_PRODUCTION]; //四元式地址栈，用于存为了跳过else后的程序块的无条件转移指令四元式地址，以便回填
	int ACTION;  //ACTION列
	int GOTO;   //GOTO列
	int SLR;	//SLR动作
	int number = 0;  //序号
	int p_stack = 0; //栈的下标
	int p_tf = 0;  //真假口栈的下标
	int p_jm = 0;  //四元式地址栈下标
	int cnt = 0;  //二元式下标
	//初始化状态栈和符号栈
	state_sk[p_stack] = 0;
	symbol_sk[p_stack] = jinghao;
	p_stack++;
	//输出分析过程
	cout << "序号\t\t" << "动作\t\t" << "状态栈\t\t\t\t\t\t\t" << "符号栈" << endl;
	cout << "[" << number++ << "]\t\t";  //序号
	cout << "_\t\t";  //动作
	cout << state_sk[0] << "\t\t\t\t\t\t\t"; //状态栈
	cout << symbol_sk[0] << endl;  //符号栈

	while (true) {
		if (p_stack < 0)
			error(6);  //栈错误
		ACTION = ResLex[cnt].id;
		if (is_pro(ResLex[cnt].id)) {  //是表达式的值
			int length = get_pro(cnt);  //得到表达式并返回长度
			//if或while
			if (cnt > 0 && ResLex[cnt - 1].id == sy_if || ResLex[cnt - 1].id == sy_while) {
				//布尔表达式
				TF_export_sk[p_tf].ta = ResSem.size();  //保存此时的真口
				SLR1Bool(production);   //布尔表达式分析
				cout << "\n\n***************************************继续控制语句的分析过程***************************************\n";
				
				symbol_sk[p_stack] = e;  //布尔表达式的符号
				ACTION = e;
				backpatch(B.ta, ResSem.size() + InitADD);
				//假口保存
				TF_export_sk[p_tf].fa = B.fa;
				p_tf++;
			}
			else {
				//算术表达式
				SLR1Arithmetic(production);  //算术表达式和赋值语句分析
				cout << "\n\n***************************************继续控制语句的分析过程***************************************\n";
				symbol_sk[p_stack] = a;  //赋值语句
				ACTION = a;
			}
			cnt = length;	//分析表达式后的第一个字符
		}
		SLR = ProgramTable[state_sk[p_stack - 1]][ACTION];  //获取动作

		if (SLR >= 0 && SLR < 100) {  //移进
			cout << "[" << number++ << "]\t\t";  //序号
			cout << "s" << SLR << "\t\t";  //动作
			state_sk[p_stack] = SLR;  //状态进栈
			symbol_sk[p_stack] = ACTION;  //符号进栈
			cnt++; //为分析下一个符号做准备
		}
		else if (SLR >= 100) {  //规约
			cout << "[" << number++ << "]\t\t";  //序号
			cout << "r" << SLR - InitADD << "\t\t";  //动作
			switch (SLR) {
			case 100:  //S'->S
				break;
			case 101:  //S->if e then S else S
				p_stack -= 6;  //回退6个字符
				symbol_sk[p_stack] = S;  //规约为S（产生式左部的）
				ResSem[jmp_sk[--p_jm]].result = ResSem.size() + InitADD;
				break;
			case 102:  //S->while e do S
				p_stack -= 4;  //回退4个字符
				symbol_sk[p_stack] = S;  //规约为S（产生式左部的）
				break;
			case 103:  //S->begin L end
				p_stack -= 3;  //回退3个字符
				symbol_sk[p_stack] = S;  //规约为S
				break;
			case 104:  //S->a
				p_stack--;  //回退1个字符
				symbol_sk[p_stack] = S;  //规约为S
				break;
			case 105:  //L->S
				p_stack--;  //回退1个字符
				symbol_sk[p_stack] = L;  //规约为L
				break;
			case 106:  //L->S;L
				p_stack -= 3;  //回退3个字符
				symbol_sk[p_stack] = L;  //规约为L
				break;
			default:
				break;
			}
			if (symbol_sk[p_stack] == S) {
				//S的前一个字符是do，即while e do S
				if (symbol_sk[p_stack - 1] == sy_do) {
					ResSem.push_back({ {-1,-1},"j",underline,underline,TF_export_sk[--p_tf].ta + InitADD }); //S的无条件跳转应到离S最近的e处
					backpatch(TF_export_sk[p_tf].fa, ResSem.size() + InitADD);  //假口回填
				}
				//S的前一个字符是then，即if e then S
				if (symbol_sk[p_stack - 1] == sy_then) {
					ResSem.push_back({ {-1,-1},"j",underline,underline,-1 }); //无条件跳转指令，跳过else，但是此时并不知道应跳转到哪里
					backpatch(TF_export_sk[--p_tf].fa, ResSem.size() + InitADD);  //假口回填
					jmp_sk[p_jm++] = ResSem.size() - 1;  //暂存无条件转移四元式地址
				}
			}
			GOTO = symbol_sk[p_stack];  //获得GOTO列
			state_sk[p_stack] = ProgramTable[state_sk[p_stack - 1]][GOTO]; //状态入栈
		}
		else if (SLR == acc) {  //分析成功
			cout << "分析成功！" << endl;
			break;
		}
		else
			error(5);  //显示分析控制语句时出现错误

		/*状态栈显示*/
		int cnt1 = 0;  //记录输出状态栈的位数，用于数据对齐
		for (int i = 0; i <= p_stack; i++) {
			cout << state_sk[i] << " ";
			if (state_sk[i] >= 10) cnt1 += 3;  //两位+空格
			else cnt1 += 2;  //1位+空格
		}
		cnt1++; //’\t’规定两个输出之间最少间隔一个空格，加一位
		/*数据对齐*/
		for (int i = 0; i < (64 - cnt1) / 8; i++) cout << '\t';

		/*符号栈显示*/
		for (int i = 0; i <= p_stack; i++) {
			cout << symbol_sk[i] << " ";
		}
		cout << endl;
		p_stack++;
	}
}


