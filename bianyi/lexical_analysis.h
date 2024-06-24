#pragma once
#include "data.h"



//��input�����ж�ȡһ���ַ���character��
char getch() {
    character = input[p_input++];
    return character;
}

//��character�е��ַ�Ϊ�հף�����ü���ɨ����һ���ַ���ֱ��characterΪ�ǿհ��ַ�Ϊֹ
void getbe() // Ԥ����,�˳��ո�,ע��
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
//��token�е��ַ�����character�е��ַ����Ӳ���Ϊtoken���µ��ַ���
void concatenation() {
    //token.append(1, character);
    token += character;
}
//�ж�character�Ƿ�Ϊ��ĸ
bool letter() {
    if (isalpha(character))
        return true;
    return false;
}
//�ж�character�Ƿ�Ϊ����
bool digit() {
    if (isdigit(character))
        return true;
    return false;
}
//�ж�token�����е��ַ����Ƿ�Ϊ�����֣������򷵻����ֱ��룬���򷵻�-1
int reserve() {
    for (int i = 0; i < 7; i++) {
        if (token == reservedWords[i]) {
            return i;
        }
    }
    return -1;
}

//�ж�token�����е��ַ����Ƿ�Ϊ����������������򷵻����ֱ��룬���򷵻�0
int isBoolOp() {
    for (int i = 0; i < 3; i++) 
    {
        if (token == boolOp[i]) {
            return i + 39;
        }
    }
    return 0;
}

//ɨ��ָ�����һ���ַ���ͬʱ��character��Ϊ�հ�
void retract() {
    p_input--;
    character = '\0';
}

//�鿴��ǰ�����Ƿ��ѵǼ��ڱ�������
int findVar() {
    for (unsigned int i = 0; i < VarTable.size(); i++) {
        if (token == VarTable[i]) return i;
    }
    return -1;
}

//����ʶ����¼���������л򽫳�����¼����������
void buildlist() {
    if (isdigit(token[0])||token[0]=='-')
        ConstNumTable.push_back(token);
    else
        VarTable.push_back(token);
}
void error() // �Ƿ��ַ�
{
    printf("�Ƿ��ַ�:%c\n", character);
}
void error(const char* msg) // ������Ϣ
{
    printf("����:%s\n", msg);
}
//�ʷ�������
//void LexicalAnalyzer() {
//    token = "";
//    getch();  //����һ���ַ�
//    getbe();  //�˳��ո�
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
//            concatenation();  //����ǰ������ַ�����token����
//            getch();          //������һ���ַ�
//        }
//        retract();  //ɨ��ָ�����һ���ַ�
//        c = reserve();
//        b = isBoolOp();
//        if (c != -1) {
//            ResLex.push_back({ c, 0 });  //������
//        }
//        else if (b != 0) {
//            ResLex.push_back({ b, 0 });  //���������
//        }
//        else {     //��ʶ��
//            flag = findVar();
//            if (flag == -1) {
//                buildlist();  //����ʶ����¼����������
//                ResLex.push_back({ ident, int(VarTable.size() - 1) });  //�µǼǱ��� 
//            }
//            else {
//                ResLex.push_back({ ident, flag });  //�ѵǼǱ��� 
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
//        ResLex.push_back({ intconst, atoi(ConstNumTable.back().c_str()) });  //��������ֵΪ������ֵ��
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
//        * ��ϵ���������ֵ��
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
//        over = true;  //ɨ����ϣ��ʷ���������
//        break;
//    default:
//        break;
//    }
//}
//�ʷ�������
void LexicalAnalyzer() {
    token = "";
    getch();  //����һ���ַ�
    getbe();  //�˳��ո�
    if (letter())
    {
        while (letter() || digit())
        {
            concatenation();  //����ǰ������ַ�����token����
            getch();          //������һ���ַ�
        }
        retract();  //ɨ��ָ�����һ���ַ�
        c = reserve();
        b = isBoolOp();
        if (c != -1) {
            ResLex.push_back({ c, 0 });  //������
        }
        else if (b != 0) {
            ResLex.push_back({ b, 0 });  //���������
        }
        else
        {     //��ʶ��
            flag = findVar();
            if (flag == -1) {
                buildlist();  //����ʶ����¼����������
                ResLex.push_back({ ident, int(VarTable.size() - 1) });  //�µǼǱ��� 
            }
            else {
                ResLex.push_back({ ident, flag });  //�ѵǼǱ��� 
            }
        }
    }
    else if (digit() || character == '-')  //���ַ�������
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
                concatenation();//˵���Ǹ���ЧС���ţ��Ƚ�С�������
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
                        concatenation();//˵���Ǹ���ѧ�����ţ��Ƚ�e���
                        getch();
                        while (digit())
                        {
                            concatenation();
                            getch();
                        }
                        retract();
                        buildlist(); /*��������¼����������*/
                    }
                    else if (character == '-'|| character == '+')
                    {
                        getch();
                        if (digit())
                        {
                            retract();//����e����ֻ��һ�����ŵķǷ����
                            retract();
                            retract();
                            getch();
                            concatenation();//˵���Ǹ���Чe���Ƚ�e���
                            getch();//ch��λ��-
                            concatenation();//˵���Ǹ���Ч���ţ��Ƚ��������
                            getch();//ch��λ�����ź��������
                            while (digit())
                            {
                                concatenation();
                                getch();
                            }
                            retract();
                            buildlist(); /*��������¼����������*/
                        }
                        else//��Ч����
                        {
                            error("�Ƿ��Ŀ�ѧ������ʽ:��Ч����");
                            retract();
                        }
                    }
                    else//e����û�����֣���Чe
                    {
                        error("�Ƿ��Ŀ�ѧ������ʽ:��Чe");
                        retract();
                        retract();
                        buildlist(); /*��������¼����������*/
                    }
                }
                else
                {
                    retract();
                    buildlist(); /*��������¼����������*/
                }
            }
            else//������ЧС���㣬��ǰ��ĳ������棬С��������ʶ��
            {
                retract();
                retract();
                buildlist(); /*��������¼����������*/
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
                concatenation();//˵���Ǹ���ѧ�����ţ��Ƚ�e���
                getch();
                while (digit())
                {
                    concatenation();
                    getch();
                }
                retract();
                buildlist(); /*��������¼����������*/
                //cout << "( " << "num" << "," << ptr_present << ")num_table[" << ptr_present << "]=" << token << "\n";//�ǳ���
            }
            else if (character == '-'|| character=='+')//����e�������Ϊ����
            {
                getch();
                if (digit())
                {
                    retract();//����e����ֻ��һ�����ŵķǷ����
                    retract();
                    retract();
                    getch();
                    concatenation();//˵���Ǹ���Чe���Ƚ�e���
                    getch();//ch��λ��-
                    concatenation();//˵���Ǹ���Ч���ţ��Ƚ��������
                    getch();//ch��λ�����ź��������
                    while (digit())
                    {
                        concatenation();
                        getch();
                    }
                    retract();
                    buildlist(); /*��������¼����������*/
                    //cout << "( " << "num" << "," << ptr_present << ")num_table[" << ptr_present << "]=" << token << "\n";//�ǳ�
                }
                else//��Ч����
                {
                    error("�Ƿ��Ŀ�ѧ������ʽ:��Ч����");
                    retract();
                }
            }
            else//��Чe
            {
                retract();
                retract();
                buildlist(); /*��������¼����������*/
                //cout << "( " << "num" << "," << ptr_present << ")num_table[" << ptr_present << "]=" << token << "\n";//�ǳ�
            }
        }
        else
        {
            retract();
            buildlist(); /*��������¼����������*/
            //cout << "( " << "num" << "," << ptr_present << ")num_table[" << ptr_present << "]=" << token << "\n";//�ǳ���
        }
        ResLex.push_back({ intconst,((int)ConstNumTable.size()-1)});  //��������ֵΪ������ֵ��
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
        * ��ϵ���������ֵ��
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
        over = true;  //ɨ����ϣ��ʷ���������
        break;
    default:
        error();
        break;
    }
}

//��ʾ�ʷ��������
void Show_lex_res() {
    cout << "\n*************�ʷ��������***************\n";
    cout << "���\t�ֱ���\t      ֵ\n";
    for (unsigned int i = 0; i < ResLex.size(); i++) 
    {
        if(ResLex[i].id >=0 && ResLex[i].id <= 6)
			cout << "[" << i + 1 << "]" << '\t' << "��" << ResLex[i].id << '\t' << "," << "     " << reservedWords[ResLex[i].id] << "��" << endl;
		else if (ResLex[i].id >= 39 && ResLex[i].id <= 41)
			cout << "[" << i + 1 << "]" << '\t' << "��" << ResLex[i].id << '\t' << "," << "     " << boolOp[ResLex[i].id - 39] << "��" << endl;
		else if(ResLex[i].id != 56 && ResLex[i].id != 42&& ResLex[i].id != 57&&ResLex[i].value == 0)
            cout << "[" << i + 1 << "]" << '\t' << "��" << ResLex[i].id << '\t' << "," << "     " << "_" << "��" << endl;
        else if(ResLex[i].id == 57)
            cout << "[" << i + 1 << "]" << '\t' << "��" << ResLex[i].id << '\t' << "," << "     " << ConstNumTable[ResLex[i].value] << "��" << endl;
        else
            cout << "[" << i + 1 << "]" << '\t' << "��" << ResLex[i].id << '\t'<<"," <<"     " << ResLex[i].value << "��" << endl;
    }
    cout << "������" << ResLex.size() << "����Ԫʽ��" << endl;
    cout << "\n*************��������***************\n";
    cout << "���\t\t����" << endl;
    for (unsigned int i = 0; i < VarTable.size(); i++) {
        cout << "[" << i << "]" << "\t\t" << VarTable[i] << endl;
    }
}
