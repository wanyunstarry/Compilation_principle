#pragma once
#include "data.h"



//从input数组中读取一个字符到character中
char getch() {
    character = input[p_input++];
    return character;
}

//若character中的字符为空白，则调用继续扫描下一个字符，直至character为非空白字符为止
void getbe() // 预处理,滤除空格,注释
{
    while (character == ' ' || character == '\n' || character == '\r' || character == '\t')
        getch();
    if (character == '/')
    {
        getch();
        if (character == '/')
        {
            while (character != '\n')
                getch();
        }
        else if (character == '*')
        {
            do
            {
                while (character != '*')
                    getch();
                getch();
            } while (character != '/');
            getch();
        }
        else
            retract();
    }
}
//将token中的字符串与character中的字符连接并作为token中新的字符串
void concatenation() {
    //token.append(1, character);
    token += character;
}
//判断character是否为字母
bool letter() {
    if (isalpha(character))
        return true;
    return false;
}
//判断character是否为数字
bool digit() {
    if (isdigit(character))
        return true;
    return false;
}
//判断token数组中的字符串是否为保留字，若是则返回其种别码，否则返回-1
int reserve() {
    for (int i = 0; i < 7; i++) {
        if (token == reservedWords[i]) {
            return i;
        }
    }
    return -1;
}

//判断token数组中的字符串是否为布尔运算符，若是则返回其种别码，否则返回0
int isBoolOp() {
    for (int i = 0; i < 3; i++) 
    {
        if (token == boolOp[i]) {
            return i + 39;
        }
    }
    return 0;
}

//扫描指针回退一个字符，同时将character置为空白
void retract() {
    p_input--;
    character = '\0';
}

//查看当前变量是否已登记在变量表中
int findVar() {
    for (unsigned int i = 0; i < VarTable.size(); i++) {
        if (token == VarTable[i]) return i;
    }
    return -1;
}

//将标识符登录到变量表中或将常数登录到常数表中
void buildlist() {
    if (isdigit(token[0])||token[0]=='-')
        ConstNumTable.push_back(token);
    else
        VarTable.push_back(token);
}
void error() // 非法字符
{
    printf("非法字符:%c\n", character);
}
void error(const char* msg) // 错误信息
{
    printf("错误:%s\n", msg);
}
//词法分析器
//void LexicalAnalyzer() {
//    token = "";
//    getch();  //读入一个字符
//    getbe();  //滤除空格
//    switch (character) {
//    case 'a':
//    case 'b':
//    case 'c':
//    case 'd':
//    case 'e':
//    case 'f':
//    case 'g':
//    case 'h':
//    case 'i':
//    case 'j':
//    case 'k':
//    case 'l':
//    case 'm':
//    case 'n':
//    case 'o':
//    case 'p':
//    case 'q':
//    case 'r':
//    case 's':
//    case 't':
//    case 'u':
//    case 'v':
//    case 'w':
//    case 'x':
//    case 'y':
//    case 'z':
//        while (letter() || digit()) {
//            concatenation();  //将当前读入的字符送入token数组
//            getch();          //读入下一个字符
//        }
//        retract();  //扫描指针回退一个字符
//        c = reserve();
//        b = isBoolOp();
//        if (c != -1) {
//            ResLex.push_back({ c, 0 });  //保留字
//        }
//        else if (b != 0) {
//            ResLex.push_back({ b, 0 });  //布尔运算符
//        }
//        else {     //标识符
//            flag = findVar();
//            if (flag == -1) {
//                buildlist();  //将标识符登录到变量表中
//                ResLex.push_back({ ident, int(VarTable.size() - 1) });  //新登记变量 
//            }
//            else {
//                ResLex.push_back({ ident, flag });  //已登记变量 
//            }
//        }
//        break;
//    case '0':
//    case '1':
//    case '2':
//    case '3':
//    case '4':
//    case '5':
//    case '6':
//    case '7':
//    case '8':
//    case '9':
//        while (digit()) {
//            concatenation();
//            getch();
//        }
//        retract();
//        buildlist();
//        ResLex.push_back({ intconst, atoi(ConstNumTable.back().c_str()) });  //整常量（值为自身数值）
//        break;
//    case ';':
//        ResLex.push_back({ semicolon, 0 });
//        break;
//    case '#':
//        ResLex.push_back({ jinghao, 0 });
//        break;
//    case '+':
//        ResLex.push_back({ plus1, 0 });
//        break;
//    case '*':
//        ResLex.push_back({ times, 0 });
//        break;
//    case ':':
//        getch();
//        if (character == '=') {
//            ResLex.push_back({ becomes, 0 });  //:=
//        }
//        else {
//            retract();
//        }
//        break;
//        /*
//        * 关系运算符及其值：
//        * <    <=   <>  >   >=  =
//        * 0    1    2   3   4   5
//        */
//    case '<':
//        getch();
//        if (character == '=') {
//            ResLex.push_back({ rop, 1 });  //<=
//        }
//        else if (character == '>') {
//            ResLex.push_back({ rop, 2 });  //<>
//        }
//        else {
//            retract();
//            ResLex.push_back({ rop, 0 });  //<
//        }
//        break;
//    case '>':
//        getch();
//        if (character == '=') {
//            ResLex.push_back({ rop, 4 });  //>= 
//        }
//        else {
//            retract();
//            ResLex.push_back({ rop, 3 });  //>
//        }
//        break;
//    case '=':
//        ResLex.push_back({ rop, 5 });
//        break;
//    case '(':
//        ResLex.push_back({ lparent, 0 });
//        break;
//    case ')':
//        ResLex.push_back({ rparent, 0 });
//        break;
//    case '~':
//        over = true;  //扫描完毕，词法分析结束
//        break;
//    default:
//        break;
//    }
//}
//词法分析器
void LexicalAnalyzer() {
    token = "";
    getch();  //读入一个字符
    getbe();  //滤除空格
    if (letter())
    {
        while (letter() || digit())
        {
            concatenation();  //将当前读入的字符送入token数组
            getch();          //读入下一个字符
        }
        retract();  //扫描指针回退一个字符
        c = reserve();
        b = isBoolOp();
        if (c != -1) {
            ResLex.push_back({ c, 0 });  //保留字
        }
        else if (b != 0) {
            ResLex.push_back({ b, 0 });  //布尔运算符
        }
        else
        {     //标识符
            flag = findVar();
            if (flag == -1) {
                buildlist();  //将标识符登录到变量表中
                ResLex.push_back({ ident, int(VarTable.size() - 1) });  //新登记变量 
            }
            else {
                ResLex.push_back({ ident, flag });  //已登记变量 
            }
        }
    }
    else if (digit() || character == '-')  //首字符是数字
    {
        while (digit() || character == '-')
        {
            concatenation();
            getch();
        }
        if (character == '.')
        {
           
            getch();
            if (digit())
            {
                retract();
                retract();
                getch();
                concatenation();//说明是个有效小数号，先将小数点添加
                getch();
                while (digit())
                {
                    concatenation();
                    getch();
                }
                if (character == 'e'|| character == 'E')
                {
                    getch();
                    if (digit())
                    {
                        retract();
                        retract();
                        getch();
                        concatenation();//说明是个科学计数号，先将e添加
                        getch();
                        while (digit())
                        {
                            concatenation();
                            getch();
                        }
                        retract();
                        buildlist(); /*将常数登录到常数表中*/
                    }
                    else if (character == '-'|| character == '+')
                    {
                        getch();
                        if (digit())
                        {
                            retract();//考虑e后面只有一个负号的非法情况
                            retract();
                            retract();
                            getch();
                            concatenation();//说明是个有效e，先将e添加
                            getch();//ch定位到-
                            concatenation();//说明是个有效负号，先将负号添加
                            getch();//ch定位到负号后面的数字
                            while (digit())
                            {
                                concatenation();
                                getch();
                            }
                            retract();
                            buildlist(); /*将常数登录到常数表中*/
                        }
                        else//无效负号
                        {
                            error("非法的科学记数格式:无效负号");
                            retract();
                        }
                    }
                    else//e后面没有数字，无效e
                    {
                        error("非法的科学记数格式:无效e");
                        retract();
                        retract();
                        buildlist(); /*将常数登录到常数表中*/
                    }
                }
                else
                {
                    retract();
                    buildlist(); /*将常数登录到常数表中*/
                }
            }
            else//不是有效小数点，将前面的常数保存，小数点另外识别
            {
                retract();
                retract();
                buildlist(); /*将常数登录到常数表中*/
            }
        }
        else if (character == 'e' || character == 'E')
        {
            
            getch();
            if (digit())
            {
                retract();
                retract();
                getch();
                concatenation();//说明是个科学计数号，先将e添加
                getch();
                while (digit())
                {
                    concatenation();
                    getch();
                }
                retract();
                buildlist(); /*将常数登录到常数表中*/
                //cout << "( " << "num" << "," << ptr_present << ")num_table[" << ptr_present << "]=" << token << "\n";//是常数
            }
            else if (character == '-'|| character=='+')//考虑e后面可能为负数
            {
                getch();
                if (digit())
                {
                    retract();//考虑e后面只有一个负号的非法情况
                    retract();
                    retract();
                    getch();
                    concatenation();//说明是个有效e，先将e添加
                    getch();//ch定位到-
                    concatenation();//说明是个有效负号，先将负号添加
                    getch();//ch定位到负号后面的数字
                    while (digit())
                    {
                        concatenation();
                        getch();
                    }
                    retract();
                    buildlist(); /*将常数登录到常数表中*/
                    //cout << "( " << "num" << "," << ptr_present << ")num_table[" << ptr_present << "]=" << token << "\n";//是常
                }
                else//无效负号
                {
                    error("非法的科学记数格式:无效负号");
                    retract();
                }
            }
            else//无效e
            {
                retract();
                retract();
                buildlist(); /*将常数登录到常数表中*/
                //cout << "( " << "num" << "," << ptr_present << ")num_table[" << ptr_present << "]=" << token << "\n";//是常
            }
        }
        else
        {
            retract();
            buildlist(); /*将常数登录到常数表中*/
            //cout << "( " << "num" << "," << ptr_present << ")num_table[" << ptr_present << "]=" << token << "\n";//是常数
        }
        ResLex.push_back({ intconst,((int)ConstNumTable.size()-1)});  //整常量（值为自身数值）
    }
    else switch (character)
    {
    case ';':
        ResLex.push_back({ semicolon, 0 });
        break;
    case '#':
        ResLex.push_back({ jinghao, 0 });
        break;
    case '+':
        ResLex.push_back({ plus1, 0 });
        break;
    case '*':
        ResLex.push_back({ times, 0 });
        break;
    case ':':
        getch();
        if (character == '=') {
            ResLex.push_back({ becomes, 0 });  //:=
        }
        else {
            retract();
        }
        break;
        /*
        * 关系运算符及其值：
        * <    <=   <>  >   >=  =
        * 0    1    2   3   4   5
        */
    case '<':
        getch();
        if (character == '=') {
            ResLex.push_back({ rop, 1 });  //<=
        }
        else if (character == '>') {
            ResLex.push_back({ rop, 2 });  //<>
        }
        else {
            retract();
            ResLex.push_back({ rop, 0 });  //<
        }
        break;
    case '>':
        getch();
        if (character == '=') {
            ResLex.push_back({ rop, 4 });  //>= 
        }
        else {
            retract();
            ResLex.push_back({ rop, 3 });  //>
        }
        break;
    case '=':
        ResLex.push_back({ rop, 5 });
        break;
    case '(':
        ResLex.push_back({ lparent, 0 });
        break;
    case ')':
        ResLex.push_back({ rparent, 0 });
        break;
    case '~':
        over = true;  //扫描完毕，词法分析结束
        break;
    default:
        error();
        break;
    }
}

//显示词法分析结果
void Show_lex_res() {
    cout << "\n*************词法分析结果***************\n";
    cout << "序号\t种别码\t      值\n";
    for (unsigned int i = 0; i < ResLex.size(); i++) 
    {
        if(ResLex[i].id >=0 && ResLex[i].id <= 6)
			cout << "[" << i + 1 << "]" << '\t' << "（" << ResLex[i].id << '\t' << "," << "     " << reservedWords[ResLex[i].id] << "）" << endl;
		else if (ResLex[i].id >= 39 && ResLex[i].id <= 41)
			cout << "[" << i + 1 << "]" << '\t' << "（" << ResLex[i].id << '\t' << "," << "     " << boolOp[ResLex[i].id - 39] << "）" << endl;
		else if(ResLex[i].id != 56 && ResLex[i].id != 42&& ResLex[i].id != 57&&ResLex[i].value == 0)
            cout << "[" << i + 1 << "]" << '\t' << "（" << ResLex[i].id << '\t' << "," << "     " << "_" << "）" << endl;
        else if(ResLex[i].id == 57)
            cout << "[" << i + 1 << "]" << '\t' << "（" << ResLex[i].id << '\t' << "," << "     " << ConstNumTable[ResLex[i].value] << "）" << endl;
        else
            cout << "[" << i + 1 << "]" << '\t' << "（" << ResLex[i].id << '\t'<<"," <<"     " << ResLex[i].value << "）" << endl;
    }
    cout << "共产生" << ResLex.size() << "个二元式！" << endl;
    cout << "\n*************变量名表***************\n";
    cout << "序号\t\t变量" << endl;
    for (unsigned int i = 0; i < VarTable.size(); i++) {
        cout << "[" << i << "]" << "\t\t" << VarTable[i] << endl;
    }
}
