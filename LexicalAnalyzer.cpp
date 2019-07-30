/*Brandin Dennin's Lexical Analyzer*/


/* Filename: LexicalAnalyzer.cpp
CSC 340 - Programming Languages
Dr. Tu

A lexical analyzer system for simple arithmetic expressions.

Operator/keyword: +, -, * /, (, )
Identifer: (a+b+...+z+A+B+...Z)(a+b+...+z+A+B+...Z+0+1+2+...+9)*
Integer: (0+1+2+...+9)(0+1+2+...+9)*

*/

#include <iostream>
#include <fstream>
#include <cctype>
using namespace std;

/* Global declarations */
/* Variables */

int charClass;
char lexeme[100];
char nextChar;
int lexLen;
int token;
int nextToken;
ifstream in_fp("syntax.txt");

/* Function declarations */
void getChar();
void addChar();
void getNonBlank();
void expr();
void term();
void factor();
void assign();
void boolfactor();
void boolterm();
void boolexpr();
void ifstmt();
int lex();      // to get the nextToken


/* Character classes */
#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99

/* Token codes */
#define INT_LIT 10
#define IDENT 11
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 26

/* Added Token codes */
#define MOD_OP 27
#define SEMICOLON 28
#define TRUE_OP 29
#define FALSE_OP 30
#define OR_OP 31
#define AND_OP 32
#define NOT_OP 33
#define IF_OP 34
#define ELSE_OP 35
#define LEFT_BRACE 36
#define RIGHT_BRACE 37
#define ASSIGN_OP 38


/******************************************************/
/* main driver */
void main()
{
	/* Open the input data file and process its contents */

	if (in_fp.fail())
	{
		cout << "File could not be opened\n";
		cin.get();
		exit(1);
	}
	else {
		getChar();
		do {
			ifstmt();
			lex();  // Getting the nextToken
		} while (nextToken != EOF);
	}
	in_fp.close();

	system("PAUSE");
}

/*****************************************************/
/* lookup - a function to lookup operators and parentheses
and return the token */
int lookup(char ch)
{
	switch (ch) {
	case '(':
		addChar();
		nextToken = LEFT_PAREN;
		break;
	case ')':
		addChar();
		nextToken = RIGHT_PAREN;
		break;
	case '+':
		addChar();
		nextToken = ADD_OP;
		break;
	case '-':
		addChar();
		nextToken = SUB_OP;
		break;
	case '*':
		addChar();
		nextToken = MULT_OP;
		break;
	case '/':
		addChar();
		nextToken = DIV_OP;
		break;
		/* Added lookup operators */
	case '%':
		addChar();
		nextToken = MOD_OP;
		break;
	case ';':
		addChar();
		nextToken = SEMICOLON;
		break;
	case '!':
		addChar();
		nextToken = NOT_OP;
		break;
	case '{':
		addChar();
		nextToken = LEFT_BRACE;
		break;
	case '}':
		addChar();
		nextToken = RIGHT_BRACE;
		break;
	case '=':
		addChar();
		nextToken = ASSIGN_OP;
		break;
	case '|':
		addChar();
		getChar();
		addChar();
		if (lexeme[0] == '|' && lexeme[1] == '|') {
			nextToken = OR_OP;
		}
		else {
			nextToken = EOF;
		}
		break;
	case '&':
		addChar();
		getChar();
		addChar();
		if (lexeme[0] == '&' && lexeme[1] == '&') {
			nextToken = AND_OP;
		}
		else {
			nextToken = EOF;
		}
		break;
	default:
		addChar();
		nextToken = EOF;
		break;
	}
	return nextToken;
}

/*****************************************************/
/* addChar - a function to add nextChar to lexeme */
void addChar()
{
	if (lexLen <= 98) {
		lexeme[lexLen++] = nextChar;
		lexeme[lexLen] = 0;
	}
	else
		cout << " Error - lexeme is too long \n";
}

/*****************************************************/
/* getChar - a function to get the next character of
input and determine its character class */
void getChar()
{
	in_fp.get(nextChar);
	if (in_fp.eof())   // if no more character in the file
		nextChar = EOF;

	if (nextChar != EOF) {
		if (isalpha(nextChar))
			charClass = LETTER;
		else if (isdigit(nextChar))
			charClass = DIGIT;
		else charClass = UNKNOWN;
	}
	else
		charClass = EOF;
}

/*****************************************************/
/* getNonBlank - a function to call getChar until it
returns a non-whitespace character */
void getNonBlank()
{
	while (isspace(nextChar))
		getChar();
}

/***************************************************** /
/* lex - a simple lexical analyzer for arithmetic expressions */
int lex()
{
	lexLen = 0;
	getNonBlank();
	switch (charClass) {
		/* Parse identifiers */
	case LETTER:
		addChar();
		getChar();
		while (charClass == LETTER || charClass == DIGIT) {
			addChar();
			getChar();
		}
		if (lexeme[0] == 'T' && lexeme[1] == 'R' && lexeme[2] == 'U' && lexeme[3] == 'E') {
			nextToken = TRUE_OP;
		}
		else if (lexeme[0] == 'F' && lexeme[1] == 'A' && lexeme[2] == 'L' && lexeme[3] == 'S' && lexeme[4] == 'E') {
			nextToken = FALSE_OP;
		}
		else if (lexeme[0] == 'i' && lexeme[1] == 'f') {
			nextToken = IF_OP;
		}
		else if (lexeme[0] == 'e' && lexeme[1] == 'l' && lexeme[2] == 's' && lexeme[3] == 'e') {
			nextToken = ELSE_OP;
		}
		else {
			nextToken = IDENT;
		}
		break;
		/* Parse integer literals */
	case DIGIT:
		addChar();
		getChar();
		while (charClass == DIGIT) {
			addChar();
			getChar();
		}
		nextToken = INT_LIT;
		break;
		/* Parentheses and operators */
	case UNKNOWN:
		lookup(nextChar);
		getChar();
		break;
		/* EOF */
	case EOF:
		nextToken = EOF;
		lexeme[0] = 'E';
		lexeme[1] = 'O';
		lexeme[2] = 'F';
		lexeme[3] = 0;
		break;
	} /* End of switch */
	cout << "Next token is: " << nextToken
		<< "       Next lexeme is " << lexeme << "\n";
	return nextToken;
} /* End of function lex */

/* Function expr
Parses strings in the language
generated by the rule:
<expr> -> <term> {(+ | -) <term>}
*/
void expr() {
	/* Parse the first term */
	term();
	/* As long as the next token is + or -, call
	lex to get the next token and parse the
	next term */
	while (nextToken == ADD_OP ||
		nextToken == SUB_OP) {
		lex(); // to get the nextToken
		term();
	}
}
/* term
Parses strings in the language generated by
the rule:
<term> -> <factor> {(* | /) <factor>}
*/
void term() {
	/* Parse the first factor */
	factor();
	/* As long as the next token is * or /,
	next token and parse the next factor */
	while (nextToken == MULT_OP || nextToken == DIV_OP || nextToken == MOD_OP)
	{
		lex();
		factor();
	}
} /* End of function term */
void factor() {
	/* Determine which RHS */
	if (nextToken == IDENT || nextToken == INT_LIT)
		/* For the RHS id or int, just call lex */
		lex();
	/* If the RHS is (<expr>) – call lex to pass over
	the left parenthesis, call expr, and check for
	the right parenthesis */
	else if (nextToken == LEFT_PAREN) {
		lex();
		expr();
		if (nextToken == RIGHT_PAREN)
			lex();
		else
			cout << "Error - Missing Right Parenthese(factor)" << endl;
	} /* End of else if (nextToken == ... */
	else cout << "Error - Something in factor went wrong!" << endl; /* Neither RHS matches */
}
/*<assign> -> id = <expr>; | id = <expr>; <assign>*/
void assign() {
	if (nextToken == IDENT) {
		lex();
		if (nextToken == ASSIGN_OP) {
			lex();
			expr();
			if (nextToken != SEMICOLON) {
				cout << "Error - Missing ; OPERATOR(assign)" << endl;
			}
			else {
				lex();
				assign();
			}


		}
		else {
			cout << "Error - Missing = OPERATOR(assign)" << endl;
		}
	}

	else {
		//cout << "Error - Something in assignment went wrong" << endl;
	}
}
/*<boolfactor> -> TRUE | FALSE | ! <boolfactor> | (<boolexpr>)*/
void boolfactor() {
	if (nextToken == TRUE_OP) {
		lex();
	}
	else if (nextToken == FALSE_OP) {
		lex();
	}
	else if (nextToken == NOT_OP) {
		lex();
		if (nextToken == TRUE_OP || nextToken == FALSE_OP) {
			lex();
			boolfactor();
		}
		else {
			cout << "Error - Invalid use of the NOT operator" << endl;
		}
	}
	else if (nextToken == LEFT_PAREN) {
		lex();
		boolexpr();
		if (nextToken == RIGHT_PAREN) {
			lex();
		}
		else {
			cout << "error - missing right parenthese" << endl;
		}
	}
}
/*<boolterm> -> <boolfactor> {&& <boolfactor>}*/
void boolterm() {
	boolfactor();
	while (nextToken == AND_OP) {
		lex();
		boolfactor();
	}
}
/*<boolexpr> -> <boolterm> {|| <boolterm>}*/
void boolexpr() {
	boolterm();
	while (nextToken == OR_OP) {
		lex(); // to get the nextToken
		boolterm();
	}
}
/*<ifstmt> -> if (<boolexpr>) '{'<assign>'}' [else '{'<assign>'}']*/
void ifstmt() {
	if (nextToken == IF_OP) {
		lex();
		if (nextToken == LEFT_PAREN) {
			lex();
			boolexpr();
			if (nextToken == RIGHT_PAREN) {
				lex();
				if (nextToken == LEFT_BRACE) {
					lex();
					assign();
					if (nextToken == RIGHT_BRACE) {
						lex();
						if (nextToken == ELSE_OP) {
							lex();
							if (nextToken == LEFT_BRACE) {
								lex();
								assign();
								if (nextToken == RIGHT_BRACE) {
									lex();
								}
								else {
									cout << "Error - Missing RIGHT BRACE" << endl;
								}
							}
							else {
								cout << "Error - Missing LEFT BRACE(2)" << endl;
							}
						}

					}
					else {
						cout << "Error - Missing RIGHT BRACE" << endl;
					}

				}
				else {
					cout << "Error - Missing LEFT BRACE(1)" << endl;
				}
			}
			else {
				cout << "Error - Missing RIGHT PARENTHESE" << endl;
			}
		}
		else {
			cout << "Error - Missing LEFT PARENTHESE" << endl;
		}
	}
}