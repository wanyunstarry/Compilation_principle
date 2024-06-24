#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include<iomanip>
#include <map>
using namespace std;

//宏定义
constexpr auto acc = -2;				//接受
constexpr auto SIZE_PRODUCTION = 50;	//产生式的最大长度
constexpr auto InitADD = 100;			//四元式初始地址

//单词的内部定义
//保留字
constexpr auto sy_if = 0;		//if
constexpr auto sy_then = 1;		//then
constexpr auto sy_else = 2;		//else
constexpr auto sy_while = 3;	//while
constexpr auto sy_begin = 4;	//begin
constexpr auto sy_do = 5;		//do
constexpr auto sy_end = 6;		//end

constexpr auto a = 7;			//赋值语句
constexpr auto semicolon = 8;	//;
constexpr auto e = 9;			//布尔表达式
constexpr auto jinghao = 10;	//#
constexpr auto S = 11;			//语句
constexpr auto L = 12;			//复合语句
constexpr auto tempsy = 15;		//临时变量
constexpr auto EA = 18;			//B and（布尔表达式中的B∧）
constexpr auto EO = 19;			//B or（布尔表达式中的B∨）
//运算符
constexpr auto plus1 = 34;		//+
constexpr auto times = 36;		//*
constexpr auto becomes = 38;	//:=
constexpr auto op_and = 39;		//and
constexpr auto op_or = 40;		//or
constexpr auto op_not = 41;		//not
constexpr auto rop = 42;		//关系运算符
constexpr auto lparent = 48;	//(
constexpr auto rparent = 49;	//)
//变量和常量
constexpr auto ident = 56;		//变量
constexpr auto intconst = 57;	//整常量
constexpr auto kexue = -1;	//科学计数法

//词法分析
//保留字（种别码即下标）
string reservedWords[7] = { "if", "then", "else", "while", "begin", "do", "end" };
//变量表
vector<string> VarTable;
//变量表赋值
vector<int> VarValue;
//常数表
vector<string> ConstNumTable;
//布尔运算符（种别码为下标加39）
string boolOp[3] = { "and","or","not" };
//存放最新读入的源程序字符
char character;
//存放构成单词符号的字符串
string token;
//存放保留字种别码
int c;
//存放布尔运算符种别码
int b;
//保存从文件中读取的程序
string input;
//词法分析结束标志
bool over;
// input字符串的下标
int p_input;  //扫描指针
// 判断当前变量是否已登记在变量表中
int flag;

struct Binary {  //词法分析二元式类型
	int id;  //种别码
	int value; //值
};
//存储词法分析二元式结果
vector<Binary> ResLex;

//函数声明
char getch();				//从input数组中读取一个字符到character中
void getbe();				//若character中的字符为空白，则调用继续扫描下一个字符，直至character为非空白字符为止
void concatenation();		//将token中的字符串与character中的字符连接并作为token中新的字符串
bool letter();				//判断character是否为字母
bool digit();				//判断character是否为数字
int reserve();				//判断token数组中的字符串是否为保留字，若是则返回其种别码，否则返回-1
int isBoolOp();				//判断token数组中的字符串是否为布尔运算符，若是则返回其种别码，否则返回0
void retract();				//扫描指针回退一个字符，同时将character置为空白
int findVar();				//查看当前变量是否已登记在变量表中
void buildlist();			//将标识符登录到变量表中或将常数登录到常数表中
void LexicalAnalyzer();		//词法分析器
void Show_lex_res();		//显示词法分析结果
void error(); // 非法字符
void error(const char* msg); // 错误处理
/***************语法分析****************/
/*****************************************
		算术表达式的拓广文法
 G[S']：	(0) S'->E
			(1) E->E+E
			(2) E->E*E
			(3) E->(E)
			(4) E->i
 *****************************************/
static int ArithmeticTable[10][7] = {
	//error用-1表示，s从0开始，r从100开始，acc用-2表示
	/*0*/	{  3, -1, -1,  2, -1, -1,  1},
	/*1*/	{ -1,  4,  5, -1, -1,acc, -1},
	/*2*/	{  3, -1, -1,  2, -1, -1,  6},
	/*3*/	{ -1,104,104, -1,104,104, -1},
	/*4*/	{  3, -1, -1,  2, -1, -1,  7},
	/*5*/	{  3, -1, -1,  2, -1, -1,  8},
	/*6*/	{ -1,  4,  5, -1,  9, -1, -1},
	/*7*/	{ -1,101,  5, -1,101,101, -1},
	/*8*/	{ -1,102,102, -1,102,102, -1},
	/*9*/	{ -1,103,103, -1,103,103, -1}
};

/******************************************
		   布尔表达式的拓广文法
 G[S']：	(0) S'->B
			(1) B->i
			(2) B->i rop i
			(3) B->(B)
			(4) B->not B
			(5) A->B and
			(6) B->AB
			(7) O->B or
			(8) B->OB
 *****************************************/
static int BoolTable[16][11] = {
	/*0*/	{  1, -1,  4, -1,  5, -1, -1, -1, 13,  7,  8},
	/*1*/	{ -1,  2, -1,101, -1,101,101,101, -1, -1, -1},
	/*2*/	{  3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	/*3*/	{ -1, -1, -1,102, -1,102,102,102, -1, -1, -1},
	/*4*/	{  1, -1,  4, -1,  5, -1, -1, -1, 11,  7,  8},
	/*5*/	{  1, -1,  4, -1,  5, -1, -1, -1,  6,  7,  8},
	/*6*/	{ -1, -1, -1,104, -1,  9, 10,104, -1, -1, -1},
	/*7*/	{  1, -1,  4, -1,  5, -1, -1, -1, 14,  7,  8},
	/*8*/	{  1, -1,  4, -1,  5, -1, -1, -1, 15,  7,  8},
	/*9*/	{105, -1,105, -1,105, -1, -1, -1, -1, -1, -1},
	/*10*/	{107, -1,107, -1,107, -1, -1, -1, -1, -1, -1},
	/*11*/	{ -1, -1, -1, 12, -1,  9, 10, -1, -1, -1, -1},
	/*12*/	{ -1, -1, -1,103, -1,103,103,103, -1, -1, -1},
	/*13*/	{ -1, -1, -1, -1, -1,  9, 10,acc, -1, -1, -1},
	/*14*/	{ -1, -1, -1,106, -1,  9, 10,106, -1, -1, -1},
	/*15*/	{ -1, -1, -1,108, -1,  9, 10,108, -1, -1, -1}
};

/**************************************
				程序语句的拓广文法
 G[S']：	(0) S'->S
			(1) S->if e then S else S
			(2) S->while e do S
			(3) S->begin L end
			(4) S->a
			(5) L->S
			(6) L->S;L
 **************************************/
static int ProgramTable[19][13] = {
	/*0*/	{  2, -1, -1,  3,  4, -1, -1,  5, -1, -1, -1,  1, -1},
	/*1*/	{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,acc, -1, -1},
	/*2*/	{ -1, -1, -1, -1, -1, -1, -1,  6, -1,  6, -1, -1, -1},
	/*3*/	{ -1, -1, -1, -1, -1, -1, -1, -1, -1,  7, -1, -1, -1},
	/*4*/	{  2, -1, -1,  3,  4, -1, -1,  5, -1, -1, -1,  9,  8},
	/*5*/	{ -1, -1,104, -1, -1, -1,104, -1,104, -1,104, -1, -1},
	/*6*/	{ -1, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	/*7*/	{ -1, -1, -1, -1, -1, 11, -1, -1, -1, -1, -1, -1, -1},
	/*8*/	{ -1, -1, -1, -1, -1, -1, 12, -1, -1, -1, -1, -1, -1},
	/*9*/	{ -1, -1, -1, -1, -1, -1,105, -1, 13, -1, -1, -1, -1},
	/*10*/	{  2, -1, -1,  3,  4, -1, -1,  5, -1, -1, -1, 14, -1},
	/*11*/	{  2, -1, -1,  3,  4, -1, -1,  5, -1, -1, -1, 15, -1},
	/*12*/	{ -1, -1,103, -1, -1, -1,103, -1,103, -1,103, -1, -1},
	/*13*/	{  2, -1, -1,  3,  4, -1, -1,  5, -1, -1, -1,  9, 16},
	/*14*/	{ -1, -1, 17, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	/*15*/	{ -1, -1,102, -1, -1, -1,102, -1,102, -1,102, -1, -1},
	/*16*/	{ -1, -1, -1, -1, -1, -1,106, -1, -1, -1, -1, -1, -1},
	/*17*/	{  2, -1, -1,  3,  4, -1, -1,  5, -1, -1, -1, 18, -1},
	/*18*/	{ -1, -1,101, -1, -1, -1,101, -1,101, -1,101, -1, -1},
};

//产生式
vector<Binary> production;
//函数声明
bool is_pro(int id);			   //测试字符是否为产生式中的值
int get_pro(int p_pro);			   //获得产生式
int newtemp();					   //每一次调用，返回一个代表新临时变量的种别码
int getArithType(int id);		   //获取算术表达式字符类型（对应ArithmeticTable表）
int getBoolType(int id);		   //获取布尔表达式字符类型（对应BoolTable表）
void error(int errID);			   //错误处理
void arithSentence(Binary b);      //根据算术表达式和赋值语句二元式得到对应的原式（只用于提示）
void boolSentence(Binary b);	   //根据布尔表达式二元式得到对应的原式（只用于提示）

/***************语义分析****************/
Binary underline{ -1,-1 }; //下划线
//非终结符语义值
struct NT_semantic {
	int address; //存放非终结符值的变量名在符号表中的入口地址或临时变量名的整数码
	int ta;		 //记录非终结符所对应的四元式需要回填“真”出口的四元式地址所构成的链
	int fa;		 //记录非终结符所对应的四元式需要回填“假”出口的四元式地址所构成的链
}E, B, A, O; //非终结符 

//四元式
struct Quaternary {
	Binary op_type;  //运算符种类，(-1,-1)为j，(-1,0)为jnz
	string op;
	Binary arg1;
	Binary arg2;
	int result;
};
//存储语义分析结果
vector<Quaternary> ResSem;

//函数声明
void rewrite(int p, int address);  //把p所链接的每个四元式的result改写为地址address
int merge(int p1, int p2);	   //把以p1和p2为链首的两条链合并为一条以p2为链首的链（返回链首值p2）

//void SLR1Math_Assign(vector<Binary> pro);  //对算术表达式和赋值语句进行语法语义分析
void SLR1Bool(vector<Binary> pro);  //对布尔表达式进行语法语义分析
void SLR1();  //对程序语句进行语法语义分析
void ShowAndSaveRes();  //显示语义分析结果并保存四元式到pas.med


/***************生成汇编程序****************/
//变量定义
string _arg1, _arg2, _result; //分别对应第一个操作数，第二个操作数和结果
int jmpLocation = 0;  //跳转位置

//判断当前基本块内的临时变量是否出现过
map<int, bool> mp;

//函数声明
string selectRegister(int T_cnt);  //根据Tcnt的值选择寄存器
void Generate_Assembly_Program();  //根据四元式生成汇编程序写入pas.med

//错误处理
void error(int errID) {
	switch (errID) {
	case 1:
		cout << "该终结符/变量不在算术表达式与赋值语句的SLR中" << endl;
		break;
	case 2:
		cout << "该终结符/变量不在布尔表达式的SLR中" << endl;
		break;
	case 3:
		cout << "分析算术表达式时出现错误" << endl;
		break;
	case 4:
		cout << "分析布尔表达式时出现错误" << endl;
		break;
	case 5:
		cout << "分析控制语句时出现错误" << endl;
		break;
	case 6:
		cout << "栈溢出" << endl;
		break;
	default:
		cout << "未知出错" << endl;
		break;
	}
	exit(errID);
}

