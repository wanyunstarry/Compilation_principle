#include "Grammar_analysis.h"



int main()
{
    /*���ļ�*/
    string fileName = "pas.dat";
    ifstream fin;
    fin.open(fileName);
    if (!fin) {
        cout << "��pas.dat����" << endl;
        return 0;
    }
    stringstream s;
    s << fin.rdbuf();
    input = s.str();
    cout << "����ĳ���:" << endl << input<<endl;
    while (!over) {
        LexicalAnalyzer();//�ʷ�����
    }
    Show_lex_res();  //��ʾ�ʷ������������������

    SLR1();  //SLR1��������,�﷨�������������
    ShowAndSaveRes();  //��ʾ�����������Ԫʽ��PAS.MED�ļ�
    Generate_Assembly_Program(); //���ɻ�����Generate Assembly Program
 
    fin.close();
}

