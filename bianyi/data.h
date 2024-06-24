#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include<iomanip>
#include <map>
using namespace std;

//�궨��
constexpr auto acc = -2;				//����
constexpr auto SIZE_PRODUCTION = 50;	//����ʽ����󳤶�
constexpr auto InitADD = 100;			//��Ԫʽ��ʼ��ַ

//���ʵ��ڲ�����
//������
constexpr auto sy_if = 0;		//if
constexpr auto sy_then = 1;		//then
constexpr auto sy_else = 2;		//else
constexpr auto sy_while = 3;	//while
constexpr auto sy_begin = 4;	//begin
constexpr auto sy_do = 5;		//do
constexpr auto sy_end = 6;		//end

constexpr auto a = 7;			//��ֵ���
constexpr auto semicolon = 8;	//;
constexpr auto e = 9;			//�������ʽ
constexpr auto jinghao = 10;	//#
constexpr auto S = 11;			//���
constexpr auto L = 12;			//�������
constexpr auto tempsy = 15;		//��ʱ����
constexpr auto EA = 18;			//B and���������ʽ�е�B�ģ�
constexpr auto EO = 19;			//B or���������ʽ�е�B�ţ�
//�����
constexpr auto plus1 = 34;		//+
constexpr auto times = 36;		//*
constexpr auto becomes = 38;	//:=
constexpr auto op_and = 39;		//and
constexpr auto op_or = 40;		//or
constexpr auto op_not = 41;		//not
constexpr auto rop = 42;		//��ϵ�����
constexpr auto lparent = 48;	//(
constexpr auto rparent = 49;	//)
//�����ͳ���
constexpr auto ident = 56;		//����
constexpr auto intconst = 57;	//������
constexpr auto kexue = -1;	//��ѧ������

//�ʷ�����
//�����֣��ֱ��뼴�±꣩
string reservedWords[7] = { "if", "then", "else", "while", "begin", "do", "end" };
//������
vector<string> VarTable;
//������ֵ
vector<int> VarValue;
//������
vector<string> ConstNumTable;
//������������ֱ���Ϊ�±��39��
string boolOp[3] = { "and","or","not" };
//������¶����Դ�����ַ�
char character;
//��Ź��ɵ��ʷ��ŵ��ַ���
string token;
//��ű������ֱ���
int c;
//��Ų���������ֱ���
int b;
//������ļ��ж�ȡ�ĳ���
string input;
//�ʷ�����������־
bool over;
// input�ַ������±�
int p_input;  //ɨ��ָ��
// �жϵ�ǰ�����Ƿ��ѵǼ��ڱ�������
int flag;

struct Binary {  //�ʷ�������Ԫʽ����
	int id;  //�ֱ���
	int value; //ֵ
};
//�洢�ʷ�������Ԫʽ���
vector<Binary> ResLex;

//��������
char getch();				//��input�����ж�ȡһ���ַ���character��
void getbe();				//��character�е��ַ�Ϊ�հף�����ü���ɨ����һ���ַ���ֱ��characterΪ�ǿհ��ַ�Ϊֹ
void concatenation();		//��token�е��ַ�����character�е��ַ����Ӳ���Ϊtoken���µ��ַ���
bool letter();				//�ж�character�Ƿ�Ϊ��ĸ
bool digit();				//�ж�character�Ƿ�Ϊ����
int reserve();				//�ж�token�����е��ַ����Ƿ�Ϊ�����֣������򷵻����ֱ��룬���򷵻�-1
int isBoolOp();				//�ж�token�����е��ַ����Ƿ�Ϊ����������������򷵻����ֱ��룬���򷵻�0
void retract();				//ɨ��ָ�����һ���ַ���ͬʱ��character��Ϊ�հ�
int findVar();				//�鿴��ǰ�����Ƿ��ѵǼ��ڱ�������
void buildlist();			//����ʶ����¼���������л򽫳�����¼����������
void LexicalAnalyzer();		//�ʷ�������
void Show_lex_res();		//��ʾ�ʷ��������
void error(); // �Ƿ��ַ�
void error(const char* msg); // ������
/***************�﷨����****************/
/*****************************************
		�������ʽ���ع��ķ�
 G[S']��	(0) S'->E
			(1) E->E+E
			(2) E->E*E
			(3) E->(E)
			(4) E->i
 *****************************************/
static int ArithmeticTable[10][7] = {
	//error��-1��ʾ��s��0��ʼ��r��100��ʼ��acc��-2��ʾ
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
		   �������ʽ���ع��ķ�
 G[S']��	(0) S'->B
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
				���������ع��ķ�
 G[S']��	(0) S'->S
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

//����ʽ
vector<Binary> production;
//��������
bool is_pro(int id);			   //�����ַ��Ƿ�Ϊ����ʽ�е�ֵ
int get_pro(int p_pro);			   //��ò���ʽ
int newtemp();					   //ÿһ�ε��ã�����һ����������ʱ�������ֱ���
int getArithType(int id);		   //��ȡ�������ʽ�ַ����ͣ���ӦArithmeticTable��
int getBoolType(int id);		   //��ȡ�������ʽ�ַ����ͣ���ӦBoolTable��
void error(int errID);			   //������
void arithSentence(Binary b);      //�����������ʽ�͸�ֵ����Ԫʽ�õ���Ӧ��ԭʽ��ֻ������ʾ��
void boolSentence(Binary b);	   //���ݲ������ʽ��Ԫʽ�õ���Ӧ��ԭʽ��ֻ������ʾ��

/***************�������****************/
Binary underline{ -1,-1 }; //�»���
//���ս������ֵ
struct NT_semantic {
	int address; //��ŷ��ս��ֵ�ı������ڷ��ű��е���ڵ�ַ����ʱ��������������
	int ta;		 //��¼���ս������Ӧ����Ԫʽ��Ҫ����桱���ڵ���Ԫʽ��ַ�����ɵ���
	int fa;		 //��¼���ս������Ӧ����Ԫʽ��Ҫ����١����ڵ���Ԫʽ��ַ�����ɵ���
}E, B, A, O; //���ս�� 

//��Ԫʽ
struct Quaternary {
	Binary op_type;  //��������࣬(-1,-1)Ϊj��(-1,0)Ϊjnz
	string op;
	Binary arg1;
	Binary arg2;
	int result;
};
//�洢����������
vector<Quaternary> ResSem;

//��������
void rewrite(int p, int address);  //��p�����ӵ�ÿ����Ԫʽ��result��дΪ��ַaddress
int merge(int p1, int p2);	   //����p1��p2Ϊ���׵��������ϲ�Ϊһ����p2Ϊ���׵�������������ֵp2��

//void SLR1Math_Assign(vector<Binary> pro);  //���������ʽ�͸�ֵ�������﷨�������
void SLR1Bool(vector<Binary> pro);  //�Բ������ʽ�����﷨�������
void SLR1();  //�Գ����������﷨�������
void ShowAndSaveRes();  //��ʾ������������������Ԫʽ��pas.med


/***************���ɻ�����****************/
//��������
string _arg1, _arg2, _result; //�ֱ��Ӧ��һ�����������ڶ����������ͽ��
int jmpLocation = 0;  //��תλ��

//�жϵ�ǰ�������ڵ���ʱ�����Ƿ���ֹ�
map<int, bool> mp;

//��������
string selectRegister(int T_cnt);  //����Tcnt��ֵѡ��Ĵ���
void Generate_Assembly_Program();  //������Ԫʽ���ɻ�����д��pas.med

//������
void error(int errID) {
	switch (errID) {
	case 1:
		cout << "���ս��/���������������ʽ�븳ֵ����SLR��" << endl;
		break;
	case 2:
		cout << "���ս��/�������ڲ������ʽ��SLR��" << endl;
		break;
	case 3:
		cout << "�����������ʽʱ���ִ���" << endl;
		break;
	case 4:
		cout << "�����������ʽʱ���ִ���" << endl;
		break;
	case 5:
		cout << "�����������ʱ���ִ���" << endl;
		break;
	case 6:
		cout << "ջ���" << endl;
		break;
	default:
		cout << "δ֪����" << endl;
		break;
	}
	exit(errID);
}

