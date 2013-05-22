#ifndef MATHPARSE_H
#define MATHPARSE_H

#include <QtGui>
#include <string>
#include <cstring>
#include <sstream>
#include <list>

class MathParse : public QWidget
{
    Q_OBJECT

public:
    MathParse(QWidget *parent = 0);
    ~MathParse(){};
private:
    QVBoxLayout *vLay;
    QHBoxLayout *hLay;
    QLineEdit *line;
    QTextEdit *edit;
    QPushButton *generateBut;
    void AppendToEdit(std::string);
    bool isOperator(char ch);
    bool isVariable(std::string input);
    bool operatorHasPriority(std::string ch);
    bool doGoingInto(std::string input);
    std::string f_mathParse(std::string leftPart, std::string curOperator, std::string rightVar, bool isParsed=false);
    void generate(std::string input, int operators);
    int countOperators(std::string input);
private slots:
    void s_generate();
};

#endif // MATHPARSE_H
