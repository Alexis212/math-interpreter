#include <iostream>
#include <cstdlib>
#include <stack>
#include <map>
#include <cmath>

using namespace std;

const int ERR = -1, ACP= 999;
int idx;
string expr, token, lex;
bool error = false;
stack<double> pila;
map<string, double> tabsim;

void exp();
string lexico();

const string funcs[] ={"sen", "cos", "tan", "cot", "sqrt"};


int main()
{
    cout << "Expresion [.]=Salir: ";
    cin >> expr;

    while(expr != ".")
    {
       idx = 0;
       lex = lexico();
       tabsim.clear();
       exp();
       double res = pila.top();
       pila.pop();
       cout << "Resultado=" << res << endl;
       cout << "Expresion [.]=Salir: ";
       cin >> expr;
    }

    return EXIT_SUCCESS;
}


const int matran[7][6] =
{
//   let  dig   _    .   Ope  Del
   {  1,   2,   1, ERR,   5,   6}, // q0
   {  1,   1,   1, ACP, ACP, ACP}, // q1
   {ACP,   2, ACP,   3, ACP, ACP}, // q2
   {ERR,   4, ERR, ERR, ERR, ERR}, // q3
   {ACP,   4, ACP, ACP, ACP, ACP}, // q4
   {ACP, ACP, ACP, ACP, ACP, ACP}, // q5
   {ACP, ACP, ACP, ACP, ACP, ACP}, // q6
};

int colCar(char c)
{
    if (isalpha(c))
         return 0;

    if (isdigit(c))
         return 1;

    if (c == '_')
         return 2;

    if (c == '.')
         return 3;

    if (c == '+' or c == '-' or c == '*' or c == '/' or
        c == '%' or c == '^')
         return 4;

    if (c == '(' or c == ')')
        return 5;

    cout << "Simbolo no es valido en la expresion!!";
    return ERR;
}

bool esFuncion(string lex)
{
    for (int i=0; i < 5; i++)
        if (funcs[i] == lex)
            return true;

    return false;
}

string lexico()
{
    string lexema="";
    int estado = 0, estAnt;

    while (idx < expr.length() and estado != ERR and estado != ACP)
    {
        char c = expr[idx++];
        while(estado == 0 and (c == ' ' or c == '\t')) c = expr[idx++];
        if(estado > 0 and (c == ' ' or  c == '\t')) estado = ACP;
        int col = colCar(c);

        if(estado != ERR and estado != ACP and col >=0)
        {
            estAnt = estado;
            estado = matran[estado][col];

            if(estado != ACP and estado != ERR)
                lexema = lexema + c;
        }
    }

    if (estado == ACP)
        idx--;

    if (estado != ACP and estado != ERR)
        estAnt = estado;

    token = "NoTk";
    switch (estAnt)
    {
        case 1: token = "iden";
            if(esFuncion(lexema))
                token = "func";
            break;

        case 2: case 4: token = "nume";
            break;

        case 5: token = "oper";
            break;

        case 6: token = "deli";
            break;
    }

    return lexema;
}
//PARSER DE EXPRESIONES

double string_to_double(string s,unsigned short radix)
{
    double n = 0;

    for (unsigned short x = s.size(), y = 0;x>0;)
    {
      if(!(s[--x] ^ '.'))
          n /= pow(radix, s.size()-1-x), y += s.size()-x;

      else
          n += ((s[x]-(s[x] <= '9' ? '0':'0'+7)) * pow(radix, s.size() - 1-x - y));
    }

    return n;
}

void term()
{
    if(lex == "(")
    {
        lex = lexico();
        exp();
        if(lex != ")")
        {
            cout << "Se esperaba <)> y llego " << lex << endl;
            error = true;
        }
        lex = lexico();
    }

    else if(token == "nume")
    {
        pila.push(string_to_double(lex, 10));
        lex = lexico();
    }
    else if(token == "iden")
    {
        double val;
        val = tabsim[lex];
        if(val == 0)
        {
           cout << lex << ": ";
           cin >> val;
           tabsim[lex] = val;
        }
        pila.push(val);
        lex = lexico();
    }
    else if(token == "func")
    {
        string nomf=lex;
        lex = lexico();

        if(lex != "(")
        {
            cout << "Se esperaba <(> y llego " << lex << endl;
            error = true;
        }

        lex = lexico();
        exp();

        if(lex != ")")
        {
            cout << "Se esperaba <)> y llego " << lex << endl;
            error = true;
        }

        if(nomf == "sen")
        {
            double val = pila.top();
            pila.pop();
            pila.push(sin(val*3.141592/180));
        }

        else if(nomf == "cos")
        {
            double val = pila.top();
            pila.pop();
            pila.push(cos(val*3.141592/180));
        }

        else if(nomf == "tan")
        {
            double val = pila.top();
            pila.pop();
            pila.push(tan(val*3.141592/180));
        }

        else if(nomf == "cot")
        {
            double val = pila.top();
            pila.pop();
            pila.push(1 / tan(val*3.141592/180));
        }

        else if(nomf == "sqrt")
        {
            double val = pila.top();
            pila.pop();
            pila.push(sqrt(val));
        }
        
        lex = lexico();
    }

    else
    {
        cout << "Se esperaba (o numero o iden o funcion y llego "
             << lex << endl;
        error = true;
        lex = lexico();
    }
}

void signo()
{
    string ops = "";

    if(lex == "-")
    {
        ops=lex;
        lex = lexico();
    }

    term();

    if(ops == "-")
    {
          double vald = pila.top();
          pila.pop();
          pila.push(-vald);
    }
}

void expo()
{
    string ops="";

    do
    {
      if(lex == "^")
      {
          ops = lex;
          lex = lexico();
      }

      signo();

      if(ops == "^")
      {
          double vald = pila.top(), vali;
          pila.pop();
          vali = pila.top();
          pila.pop();
          pila.push(pow(vali,vald));
      }
    } while(lex == "^");
}

void multi()
{
    string ops="";
    do
    {
      if(lex == "*" or lex == "/" or lex == "%")
      {
          ops = lex;
          lex = lexico();
      }

      expo();

      if(ops == "*")
      {
          double vald = pila.top(), vali;
          pila.pop();
          vali = pila.top();
          pila.pop();
          pila.push(vali*vald);
      }

      else if(ops == "/")
      {
          double vald = pila.top(), vali;
          pila.pop();
          vali = pila.top();
          pila.pop();
          pila.push(vali/vald);
      }

      else if(ops == "%")
      {
          double vald = pila.top(), vali;
          pila.pop();
          vali = pila.top();
          pila.pop();
          pila.push((int)vali % (int) vald);
      }
    } while(lex == "*" or lex == "/" or lex == "%");
}

void exp()
{
    string ops="";

    do
    {
      if((lex == "+" or lex == "-") and pila.size() > 0)
      {
          ops = lex;
          lex = lexico();
      }

      multi();

      if(ops == "+")
      {
          double vald = pila.top(), vali;
          pila.pop();
          vali = pila.top();
          pila.pop();
          pila.push(vali+vald);
      }

      if(ops == "-")
      {
          double vald = pila.top(), vali;
          pila.pop();
          vali = pila.top();
          pila.pop();
          pila.push(vali-vald);
      }
    } while(lex == "+" or lex == "-");
}
