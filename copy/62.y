%{
	#include <stdlib.h>
	#include <stdio.h>
	#include <string.h>
	//#include "lex.yy.c"
	int n;
	#define DUMMY "a"
	#define _INT 1
	#define	_ID 2
	#define _READ 3
	#define _WRITE 4
	#define _IF 5
	#define _THEN 6
	#define _WHILE 7
	#define _DO 8
	#define _EQEQ 9
	#define _INTD 10
	#define _ARRAY 11
	#define _Varlist 12
	#define _StmtList 13
	#define _Var 14
	#define _GDefList 15
	#define _GBOOL 16
	#define _GINT 17
	#define _BOOLD 18
	#define _Program 19
	#define _Truth 20
	#define _TRUE 1
	#define _FALSE 0
	#define _LE 21
	#define _GE 22
	#define _NE 23
	#define _NOT 24
	#define _mod 25
	#define _myAND 26
	#define _myOR 27

	#include "table2.c"
	#include "tree2.c"
	struct node* t;
	/*
	
	Prog: GDefblock Fdeflist Mainblock
	FdefList :

			| FdefList Fdef

	Fdef : Type ID ( Arglist ) { LDefblock Body }
	LDefblock :

	Mainblock : INTEGER MAIN '(' ')' '{'  Body '}'
		*/
	
%}


%union {
	int val;
	char* id;
	struct node *ptr;
}


%token <id>  ID


%token <val> INT
%token <val> READ
%token <val> WRITE
%token <val> IF
%token <val> THEN ENDIF
%token <val> WHILE DO ENDWHILE
%token <val> EQEQ
%token <val> INTD
%token <val> EXIT
%token <val> INTEGER
%token <val> MAIN
%token <val> SILBEGIN
%token <val> END
%token <val> DECL
%token <val> ENDDECL
%token <val> GBOOL
%token <val> GINT
%token <val> BOOLD
%token <val> TRUE FALSE
%token <val> LE GE NE
%token <val> AND OR



%type <ptr> Program 
%type <ptr> Mainblock
%type <ptr> StmtList
%type <ptr> Stmt
%type <ptr> Relexp
%type <ptr> Expr
%type <ptr> Var
%type <ptr> Varlist
%type <ptr> GDecl
%type <ptr> GDefList
%type <ptr> GDefblock
%type <val> Truth

%left '!'
%left OR
%left AND 
%left '+' '-'
%left '*' '/' '%'
%nonassoc '<'
%nonassoc '>' LE NE GE
%nonassoc EQEQ 


%%

Program: GDefblock  Mainblock	{$$=makenode($1,$2,_Program,0,DUMMY);
									evaltree($$,-1);
									print_table();
									exit(1);
								}
	;

GDefblock : DECL GDefList ENDDECL	{$$=$2;}
		;

GDefList : GDefList GDecl 	{$$=makenode($1,$2,_GDefList,0,DUMMY);}

		| GDecl				{$$=$1;}

		;

GDecl   : GINT Varlist ';'	{$$=makenode($2,NULL,_GINT,0,DUMMY);}		//type int
		
		| GBOOL Varlist ';' {$$=makenode($2,NULL,_GBOOL,0,DUMMY);}	//type bool
		
		;

Mainblock : SILBEGIN StmtList END  	{$$ = $2;}

		;

StmtList: Stmt 			{$$=$1;}

	| StmtList Stmt 	{$$=makenode($1,$2,_StmtList,0,DUMMY);}

	;

Stmt : WRITE '(' Expr ')' ';'
	
	{$$=makenode($3,NULL,_WRITE,0,DUMMY);}

	| READ '(' Var ')' ';'

	{$$=makenode($3,NULL,_READ,0,DUMMY);}
	
	| IF '(' Relexp ')' THEN StmtList ENDIF ';'

	{$$=makenode($3,$6,_IF,0,DUMMY);}

	| IF '(' Var ')' THEN StmtList ENDIF ';'

	{$$=makenode($3,$6,_IF,0,DUMMY);}

	| WHILE '(' Relexp ')' DO StmtList ENDWHILE ';'
	
	{$$=makenode($3,$6,_WHILE,0,DUMMY);}

	| Var '=' Expr ';'

	{$$=makenode($1,$3,'=',0,DUMMY);}

	| Var '=' Relexp ';'

	{$$=makenode($1,$3,'=',0,DUMMY);}

	| INTD Varlist ';'

	{$$=makenode($2,NULL,_INTD,0,DUMMY);}
	
	;

Varlist :	Varlist ',' Var  	{$$=makenode($1,$3,_Varlist,0,DUMMY);}

		| Var 					{$$=makenode(NULL,$1,_Varlist,0,DUMMY);}

		
		;

Relexp  : Expr '<' Expr    	{$$=makenode($1,$3,'<',0,DUMMY);}

		| Expr '>' Expr    	{$$=makenode($1,$3,'>',0,DUMMY);}

		| Expr GE Expr   	{$$=makenode($1,$3,_GE,0,DUMMY);}

		| Expr LE Expr    	{$$=makenode($1,$3,_LE,0,DUMMY);}
		
		| Expr NE Expr   	{$$=makenode($1,$3,_NE,0,DUMMY);}

		| Expr EQEQ Expr   	{$$=makenode($1,$3,_EQEQ,0,DUMMY);}

		| '!' Relexp  		{$$=makenode($2,NULL,_NOT,0,DUMMY);}

		| Relexp AND Relexp	{$$=makenode($1,$3,_myAND,0,DUMMY);}

		| Relexp OR Relexp	{$$=makenode($1,$3,_myOR,0,DUMMY);}

		| Truth				{$$=makenode(NULL,NULL,_Truth,$1,DUMMY);}

		| '(' Relexp ')'	{$$=$2;}


		| Var 				{$$=$1;}

		;

Expr: Expr '+' Expr	{$$=makenode($1,$3,'+',0,DUMMY);}

	| Expr '-' Expr	{$$=makenode($1,$3,'-',0,DUMMY);}

	| Expr '*' Expr	{$$=makenode($1,$3,'*',0,DUMMY);}

	| Expr '/' Expr	{$$=makenode($1,$3,'/',0,DUMMY);}

	| Expr '%' Expr	{$$=makenode($1,$3,_mod,0,DUMMY);}

	| INT 			{$$=makenode(NULL,NULL,_INT,$1,DUMMY);}

	| Var 			{$$=$1;}

	;


Var : ID 				{$$=makenode(NULL,NULL,_ID,0,$1);}

	| ID '[' Expr ']'	{$$=makenode($3,NULL,_ARRAY,0,$1);}

	;

Truth : FALSE 	{$$=0;}

		| TRUE	{$$=1;}

		;

%%

