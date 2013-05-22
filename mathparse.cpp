#include "mathparse.h"

MathParse::MathParse(QWidget *parent)//создание окна
    : QWidget(parent)
{
    line = new QLineEdit;
    edit = new QTextEdit;
    generateBut = new QPushButton;
    connect(generateBut, SIGNAL(pressed()), this, SLOT(s_generate()));

    hLay = new QHBoxLayout;
    hLay->addWidget(line);
    hLay->addWidget(generateBut);

    vLay = new QVBoxLayout;
    vLay->addWidget(edit);
    vLay->addLayout(hLay);

    setLayout(vLay);
}
void MathParse::AppendToEdit(std::string text)
{
    edit->append(QString::fromStdString(text));
}

void MathParse::s_generate(){
    if(line->text().isEmpty())
    {
        return;
    }
    std::string str = line->text().toStdString();
    for (std::string::iterator it = str.begin() ; it<str.end(); ++it )
    {
        if (*it == ' ')
        {
            str.erase(it);//убираем пробелы
        }
    }
    std::string emptyString = "";
    std::string result = f_mathParse(str, emptyString, emptyString);
    int operators = countOperators(result);
    generate(result, operators);
}


bool MathParse::isOperator(char ch)
{
    std::string operators = "+-*/=|&><";
    bool isOperator = false;
    for(std::string::iterator it = operators.begin(); it < operators.end(); ++it)
    {
       if(*it == ch)
       {
           isOperator = true;
       }
    }
    return isOperator;
}


bool MathParse::isVariable(std::string input)
{
    int result;
    std::stringstream convert(input);
    if ( !(convert >> result) )
    {
        return true;
    }
    return false;
}

bool MathParse::operatorHasPriority(std::string ch)
{
    if(ch.at(0) == '*' || ch.at(0) == '/')
        return true;
    else
        return false;
}

bool MathParse::doGoingInto(std::string input)
{
//    cout << "is into? " <<input<< endl;
    int brackets = 0;
    int operators = 0;
    bool hasBrackets = false;
    for(std::string::iterator it = input.begin(); it < input.end(); ++it)
    {
//        cout << "is operator? "<< *it <<endl;
        if(isOperator(*it) && brackets == 0)
        {
//            cout<<"There is operator out of brackets"<<endl;
            return false;
        }
        if(*it == '(')
        {
//            cout << "HasBrackets" << endl;
            hasBrackets = true;
            brackets++;
        }
        if(*it == ')')
        {
            brackets--;
        }
    }
//    cout<<"is into "<< hasBrackets<<endl;
    return hasBrackets;
}

std::string MathParse::f_mathParse(std::string leftPart, std::string curOperator, std::string rightVar, bool isParsed)
{
//    cout << "new level " << leftPart<< " " << curOperator<<" "<< rightVar<<endl;
    if(leftPart == "" && rightVar == "")
    {
        return "";
    }
    else if(leftPart == "")
    {
        return curOperator + "("+rightVar+")";
    }
    bool isRightVar = rightVar.length() == 0;
//    cout << isRightVar << endl;
    std::string nextVar ="";
    std::string leftVar = "";
    std::string newPart = "";
    std::string tempOper;
    std::string temp = "";
    bool hasBrackets = false;
    bool priority = false;
    int brackets = 0;
    if(doGoingInto(leftPart))
    {
//        cout << "this is the end leftPart";
        isParsed = true;
        temp = std::string(leftPart.begin()+1, leftPart.end()-1) ;
        leftPart = f_mathParse(temp, "","");
    }
    else if (doGoingInto(rightVar))
    {
//        cout << "this is the end rightPart" <<endl;
        isParsed = true;
        temp = std::string(rightVar.begin()+1, rightVar.end()-1) ;
//        rightVar.erase(rightVar.end());
//        cout << temp << " without brackets" <<endl;
        rightVar = f_mathParse(temp, "","");
    }

    std::string::iterator it = leftPart.end();
    while(it > leftPart.begin())
    {
        if(isParsed)
        {
            leftVar = leftPart;
            break;
        }
        it--;
//        cout << "it " << *it << endl;
        if(*it == ')')
        {
            brackets++;
            hasBrackets = true;
            if (isRightVar){
                rightVar = *it + rightVar;
            }
            else
                leftVar = *it + leftVar;
        }
        else if(*it =='(')
        {
            brackets--;
            if (isRightVar)
                rightVar = *it + rightVar;
            else
                leftVar = *it + leftVar;
        }
        else if (isOperator(*it) && brackets == 0){
//            cout << *it << "is opersator" << endl;
            if(hasBrackets)
            {

            }
            if (curOperator == ""){
               curOperator = *it;
//               if(isOperator(*(it-1))
//               {
//                   curOperator = *(it-1) + curOperator;
//               }

               isRightVar = false;
            }
            else {
                tempOper="";
                if(operatorHasPriority(tempOper + *it) && !operatorHasPriority(curOperator))
                {
                    priority = true;
                    if (isRightVar)
                        rightVar = *it + rightVar;
                    else
                        leftVar = *it + leftVar;
                }
                else
                {
                    if(!priority)
                    {
                        rightVar = f_mathParse(leftVar, curOperator, rightVar);
                        leftVar = "";
                        curOperator = *it;
                    }
                    else
                    {
//                        cout << "woohoo" << endl;
//                        cout << "woohoo" << leftVar;
                        std::string tempPart = f_mathParse(leftVar, "","");
                        rightVar = f_mathParse(tempPart,curOperator,rightVar, true);
                        leftVar="";
                        curOperator = *it;
                    }
                }
            }
        }
        else {
//            char ch = c_str(*it);
//            rightPart.insert(0, it,it+1);
            if (isRightVar)
                rightVar = *it + rightVar;
            else
                leftVar = *it + leftVar;
        }
    }

    if(doGoingInto(leftVar))
    {
//        cout << "this is the end leftPart"<<endl;
        isParsed = true;
//        cout <<"hoh" <<endl;
//        cout <<leftVar <<endl;
        temp = std::string(leftVar.begin()+1, leftVar.end()-1) ;
//        cout <<"bob" <<endl;
        leftVar = f_mathParse(temp, "","");
    }

//    cout << "Level back " << curOperator + "("+leftVar+","+rightVar+")"<<endl;
    if(leftVar=="")
    {
        return curOperator + "("+rightVar+")";
    }
    else
    {
        return curOperator + "("+leftVar+","+rightVar+")";
    }

}

void MathParse::generate(std::string input, int operators)
{
    int brackets = 0;
    bool startCatString = false;
    bool wasComma = false;
    std::string rightVar ="";
    std::string leftVar ="";
    std::string curOper ="";
    std::string forGenerate = "";
    std::string command = "";
    std::string neg = "";
    std::string temp = "";
    bool isFirst = true;
    bool needXchg = false;
    bool uncommutateOperation = false;
    unsigned firstCatch;
    for(int i = 0; i < operators; i++)
    {
        temp = "";
        rightVar ="";
        leftVar ="";
        curOper ="";
        forGenerate = "";
        wasComma = false;
        uncommutateOperation = false;
        needXchg = false;
        command = "";

        // Ищем верхнюю операцию в дереве
        unsigned posOpenBrack = input.rfind("(") - 1;
        if(!isOperator(input.at(posOpenBrack)))
        {
            posOpenBrack++;
        }
        unsigned posCloseBrack = input.find(")", posOpenBrack) +1;
        unsigned length = posCloseBrack - posOpenBrack;
        forGenerate = input.substr(posOpenBrack, length);

        // Разбиваем строку на составляющие
        for(std::string::iterator it = forGenerate.begin(); it < forGenerate.end(); ++it)
        {
//            cout << "is operator? "<< *it <<endl;

            if(*it==',')
            {
                wasComma = true;
                continue;
            }
            if(isOperator(*it))
            {
                curOper += *it;
            }

            else if(*it!='(' && *it!=')')
            {
                    if (wasComma)
                        rightVar = rightVar + *it;
                    else
                        leftVar = leftVar + *it;
            }
        }
        // Унарный минус
//        cout <<"isUnar" << (rightVar!="" && leftVar=="" && curOper=="-") <<endl;
//        cout << rightVar<<" " << leftVar<< " " << curOper<<endl<<input<<endl<<forGenerate << endl;
        if(rightVar=="" && leftVar!="" && curOper=="-")
        {

            neg = leftVar;
            input.replace(input.find(forGenerate), forGenerate.size(), "NEG");
            continue;
        }


        if(curOper=="=")
        {
            AppendToEdit("mov " + leftVar + ",eax");
            break;
        }


        // Выводим строки асм
//        cout << leftVar << " "<< curOper<<" " << rightVar <<endl;
        firstCatch = input.find("just");
//        cout << input << endl;
//        cout << "forGenerate: " << forGenerate << " " << (firstCatch == std::std::string::npos)<<endl;
        if(firstCatch!=std::string::npos &&
           leftVar != "just" &&
           rightVar != "just")
        {
            input.replace(firstCatch, 4, "bob");
            AppendToEdit("push eax");
//            isFirst = true;
        }


        // Заполняем регистры значениями для вычисления
        if(leftVar != "just" &&
           rightVar != "just" &&
           leftVar != "bob" &&
           rightVar != "bob")
        {
            if(leftVar != "NEG")
            {
                AppendToEdit("mov eax," + leftVar);
            }
            else
            {
                AppendToEdit("mov eax," + neg );
                AppendToEdit("neg eax" );
            }
            if(rightVar !="NEG")
            {
                AppendToEdit("mov edx," + rightVar );
            }
            else
            {
                AppendToEdit("mov edx," + neg );
                AppendToEdit("neg edx" );
            }

        }
        else if(leftVar == "just" &&
                rightVar != "bob")
        {
            AppendToEdit("mov edx," + rightVar);
        }
        else if(leftVar != "bob" &&
                rightVar == "just")
        {
            AppendToEdit("mov edx," + leftVar );
            needXchg = true;
        }
        else if(leftVar == "just" &&
                rightVar == "bob")
        {
            AppendToEdit("pop edx");
        }
        else if(leftVar == "bob" &&
                rightVar == "just")
        {
            AppendToEdit("pop edx");
            needXchg = true;
        }
        else if(leftVar == "bob" && rightVar != "NEG")
        {
            AppendToEdit("pop eax");
            AppendToEdit("mov edx," + rightVar );
        }
        else if (rightVar == "bob" && leftVar != "NEG")
        {
            AppendToEdit("mov eax," + leftVar );
            AppendToEdit("pop edx");
        }
        else if(leftVar == "bob" && rightVar == "NEG")
        {
            AppendToEdit("pop eax");
            //edit->append("mov edx," << rightVar );
            AppendToEdit("mov edx," + rightVar );
            AppendToEdit("neg edx" );
        }
        else if (rightVar == "bob"&& leftVar == "NEG")
        {
            AppendToEdit("mov eax," + leftVar );
            AppendToEdit("neg eax" );
            AppendToEdit("pop edx");
        }

//        if(rightVar == "just")
//            cout << "pop edx " );
//        else
//            cout << "mov edx," << rightVar );

        if(curOper=="+")
            command = "add";
        else if(curOper=="-")
        {
            uncommutateOperation = true;
            command = "sub";
        }
        else if(curOper=="*")
            command = "imul";
        else if(curOper=="/")
        {
            uncommutateOperation = true;
            command = "idiv";
        }

//        edit->append("push eax";
        if(needXchg && uncommutateOperation)
        {
            AppendToEdit("xchg eax,edx" );
        }

        //cout << command + " eax, edx  //" << leftVar << " "<< curOper<<" " << rightVar <<endl;
        AppendToEdit(command + " eax, edx" );
//        if(curOperator=='')

        input.replace(posOpenBrack, length, "just");
//        edit->append("for " << forGenerate << " all " << input << endl;
        isFirst = false;
    }
}

int MathParse::countOperators(std::string input)
{
    int operators = 0;
    for(std::string::iterator it = input.begin(); it < input.end(); ++it)
    {
//        cout << "is operator? "<< *it <<endl;
        if(isOperator(*it))
        {
            operators++;
        }
    }
    return operators;
}

