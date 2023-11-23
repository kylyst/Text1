#include<iostream>
#include<fstream>
#include<Windows.h>
#include<string>
using namespace std;

enum TokenList
{
	TK_undef = 0,

	Kw_Void,//void	
	Kw_Main,//main
	Kw_Int,//int
	Kw_Double,//double
	Kw_For,//for
	Kw_While,//while
	Kw_Switch,//Switch
	Kw_Case,//case
	Kw_If,//if
	Kw_Else,//else
	Kw_Return,//return
	Kw_String,//string

	Tk_Plus,//+
	Tk_Minus,//-
	Tk_Star,//*
	Tk_Divide,//除
	Tk_Assign,//=
	Tk_Eq,//==
	Tk_Lt,//<
	Tk_Leq,//<=	
	Tk_Gt,//>
	Tk_Geq,//>=	

	Tk_Openpa,//(
	Tk_Closepa,//)
	Tk_Openbr,//[
	Tk_Closebr,//]
	Tk_Begin,//{
	Tk_End,//}
	Tk_Comma,//,
	Tk_Semocolom,//;

	Tk_Int,//整数	
	Tk_Double,//浮点
	Tk_String,//字符

	Tk_Ident//标识符
};

TokenList code = TK_undef;
const int MAX = 12;
int row = 1;
string token = "";
char keyWord[][10] = { "void","main","int","double","for","while","switch","case","if","else","return","string" };

void print(TokenList List)
{
	switch (List)
	{
	case TK_undef:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
		cout << '(' << List << ',' << token << ")" << "未识别的符合在第" << row << "行." << endl;
		return;
		break;
	case Kw_Void:
	case Kw_Main:
	case Kw_Int:
	case Kw_Double:
	case Kw_For:
	case Kw_While:
	case Kw_Switch:
	case Kw_Case:
	case Kw_If:
	case Kw_Else:
	case Kw_Return:
	case Kw_String:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY|FOREGROUND_BLUE);
		break;

	case Tk_Plus:
	case Tk_Minus:
	case Tk_Star:
	case Tk_Divide:
	case Tk_Assign:
	case Tk_Eq:
	case Tk_Lt:
	case Tk_Leq:
	case Tk_Gt:
	case Tk_Geq:

	case Tk_Openpa:
	case Tk_Closepa:
	case Tk_Openbr:
	case Tk_Closebr:
	case Tk_Begin:
	case Tk_End:
	case Tk_Comma:
	case Tk_Semocolom:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
		break;

	case Tk_Int:
	case Tk_Double:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
		if(token.find('.')==token.npos)
		{
			cout << '(' << List << ',' << atoi(token.c_str()) << ")" << endl;
		}else{
			cout << '(' << List << ',' << atof(token.c_str()) << ")" << endl;
		}
		return;
		break;
	case Tk_String:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
		cout << '(' << List << ',' << token << ")" << endl;
	case Tk_Ident:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_INTENSITY);
		break;
	default:
		break;
	}
	cout << '(' << List << ',' << token << ")" << endl;
}


bool isKey(string token)
{
	for(int i=0;i<MAX;i++)
	{
		if(token.compare(keyWord[i])==0)
		{
			return true;
		}
	}
	return false;
}

int getKeyID(string token)
{
	for (int i = 0; i < MAX; i++)
	{
		if (token.compare(keyWord[i]) == 0)
		{
			return i + 1;
		}
	}
	return -1;
}

bool Letter(char letter) 
{
	if((letter >= 'a'&& letter <= 'z')|| (letter >= 'A'&& letter <= 'Z'))
	{
		return true;
	}
	return false;
}

bool Digit(char digit)
{
	if(digit>='0'&& digit<='9')
	{
		return true;
	}
	return false;
}

bool Underline(char underline)
{
	if(underline == '_')
	{
		return true;
	}
	return false;
}

void wordAnalysis(FILE *fp)
{
	ofstream fout;
	fout.open("text3.txt", ios::out | ios::trunc);
	if (!fout.is_open()) {
		cout << "写文件失败" << endl;
		return;
	}
	char ch;			
	while ((ch = fgetc(fp)) != EOF)	
	{
		token = ch;									
		if (ch == ' ' || ch == '\t' || ch == '\n')	
		{
			if (ch == '\n')							
				row++;
			continue;								
		}
		else if (Letter(ch))			
		{
			token = "";					
			while (Letter(ch) || Digit(ch))	
			{
				token.push_back(ch);	
				ch = fgetc(fp);			
			}
			fseek(fp, -1L, SEEK_CUR);
			if (isKey(token))
				code = TokenList(getKeyID(token));
			else
				code = Tk_Ident;
		}
		else if(Underline(ch))
		{
			token = "";
			token.push_back(ch);
			ch = fgetc(fp);
			while(Letter(ch)||Digit(ch))
			{
				token.push_back(ch);
				ch = fgetc(fp);
			}
			fseek(fp, -1L, SEEK_CUR);
			code = Tk_Ident;
		}
		else if (Digit(ch))
		{
			int isdouble = 0;
			token = "";			
			while (Digit(ch))
			{
				token.push_back(ch);
				ch = fgetc(fp);
				if (ch == '.'&& isdouble == 0)
				{
					if (Digit(fgetc(fp)))
					{
						isdouble = 1;		
						fseek(fp, -1L, SEEK_CUR);		
						token.push_back(ch);			
						ch = fgetc(fp);				
					}
				}
			}
			if (isdouble == 1)
				code = Tk_Double;	
			else
				code = Tk_Int;				
			fseek(fp, -1L, SEEK_CUR);
		}
		else if(ch == '\"')
		{
			token = "";
			ch = fgetc(fp);
			while (ch != '\"') 
			{
				ch = fgetc(fp);
				if(ch != '\"')
				{
					token.push_back(ch);
				}
			}
			code = Tk_String;
		}
		else if (ch == '\'')
		{
			ch = fgetc(fp);
			while (ch != '\'')
			{
				ch = fgetc(fp);
				token.push_back(ch);
			}
			continue;
		}
		else switch (ch)
		{

		case '+': code = Tk_Plus;			
			break;
		case '-': code = Tk_Minus;		
			break;
		case '*': code = Tk_Star;			
			break;
		case '/': code = Tk_Divide;		
			break;
		case '=':
		{
			ch = fgetc(fp);				
			if (ch == '=')				
			{
				token.push_back(ch);
				code = Tk_Eq;
			}
			else {
				code = Tk_Assign;
				fseek(fp, -1L, SEEK_CUR);
			}
		}
		break;
		case '<':
		{
			ch = fgetc(fp);
			if (ch == '=')
			{
				token.push_back(ch);
				code = Tk_Leq;
			}
			else {
				code = Tk_Lt;
				fseek(fp, -1L, SEEK_CUR);
			}
		}
		break;
		case '>':
		{
			ch = fgetc(fp);
			if (ch == '=')
			{
				token.push_back(ch);
				code = Tk_Geq;
			}
			else {
				code = Tk_Gt;
				fseek(fp, -1L, SEEK_CUR);
			}
		}
		break;
		case '(': code = Tk_Openpa;		
			break;
		case ')': code = Tk_Closepa;	
			break;
		case '[': code = Tk_Openbr;		
			break;
		case ']': code = Tk_Closebr;	
			break;
		case '{': code = Tk_Begin;		
			break;
		case '}': code = Tk_End;		
			break;
		case ',': code = Tk_Comma;		
			break;
		case ';': code = Tk_Semocolom;
			break;

		default: code = TK_undef;
		}
		print(code);
		if (code != TK_undef) 
		{
			fout << '(' << code << ',' << token << ")" << "\n";
		}
	}
}

int main()
{
	string filename;		
	FILE* fp;				
	cout << "请输入源文件名：" << endl;
	while (true) {
		cin >> filename;		
		if ((fopen_s(&fp, filename.c_str(), "r")) == 0)		
			break;
		else
			cout << "路径输入错误！" << endl;
	}
	cout << "/=***************************词法分析结果***************************=/" << endl;
	wordAnalysis(fp);		
	fclose(fp);					
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	return 0;
}