#include "Grammar_analysis.h"



int main()
{
    /*读文件*/
    string fileName = "pas.dat";
    ifstream fin;
    fin.open(fileName);
    if (!fin) {
        cout << "打开pas.dat错误！" << endl;
        return 0;
    }
    stringstream s;
    s << fin.rdbuf();
    input = s.str();
    cout << "读入的程序:" << endl << input<<endl;
    while (!over) {
        LexicalAnalyzer();//词法分析
    }
    Show_lex_res();  //显示词法分析结果及变量名表

    SLR1();  //SLR1分析程序,语法分析和语义分析
    ShowAndSaveRes();  //显示结果并保存四元式到PAS.MED文件
    Generate_Assembly_Program(); //生成汇编程序Generate Assembly Program
 
    fin.close();
}

