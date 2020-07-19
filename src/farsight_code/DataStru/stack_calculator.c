#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*===================stack==================================*/
#define SIZE 100

typedef struct stack{
	char data[SIZE];
	int top;
}STK;

STK *creat()
{
	STK *s = (STK *)malloc(sizeof(STK));
	memset(s->data,0,SIZE);
	s->top = -1;//full increase stack
	return s;
}

int isempty(STK *s)
{
	return ((s->top == -1) ? 1 : 0);
}

int isfull(STK *s)
{
	return ((s->top == SIZE - 1) ? 1 : 0);
}

void push(STK *s,char data)
{
	s->top++;
	s->data[s->top] = data;
}

char pop(STK *s)
{
	return s->data[s->top--];
}

char get_top(STK *s)
{
	return s->data[s->top];
}
/*======================================================*/

/*=====================deal================================*/
int GetPri(char op)
{
	switch( op )
	{
	case '+' : 
	case '-' :
			 return 1;
	case '*' :
	case '/' :
			 return 2;
	case '(' :
			 return 3;
	}
	return 0;
}

int Compute(int op1, int op2, char op)
{
	switch ( op )
	{
	case '+' : 
			return (op1 + op2);
	case '-' : 
			return (op1 - op2);
	case '*' :
			return (op1 * op2);
	case '/' :
			return (op1 / op2);
	}
	return 0;
}

void Deal(STK *operand, STK *operator, char new_op)
{
   char op;
   int op1, op2;
   
   if (! isempty(operator)) {
      op = (char)get_top(operator);
   }
   if (isempty(operator) || \
		(char)get_top(operator) == '(' || \
		((GetPri(new_op) - GetPri(op)) > 0)) {
      
	  push(operator, (int)new_op);
      return;
   } else {
      while ((GetPri(op) >= GetPri(new_op)) && (op != '(')) {
         op = (char)pop(operator);
         op2 = pop(operand);
         op1 = pop(operand);
         push(operand, Compute(op1, op2, op));
         op = (char)get_top(operator);
      }
      push(operator, (int)new_op);
   }
 
   return;  
}

void DealBracket(STK *operand, STK *operator)
{
   int op1, op2;
   char op;

   while ((op = (char)pop(operator)) != '(') {
      op2 = pop(operand);
      op1 = pop(operand);
      push(operand, Compute(op1, op2, op));
   }

   return;
}
/*===============================================================*/

int main(int argc, char *argv[])
{
	char str[30], *p = str, op;
	STK *operand, *operator;
	int op1, op2; 
	int flag = 0;//if previous one is number
	int sum = 0;

	printf("input:");
	fflush(stdout);   

	gets(str);
	operand = creat();
	operator = creat();
	while (*p)
	{
#if _DEBUG_
		printf("*p=%c\n", *p);
#endif
		if ((*p >= '0') && (*p <= '9')) {
			if (flag == 0) {
				flag = 1;
				sum = *p - '0';
			} else {
				sum = 10 * sum + *p - '0';
			}
		} else {
	//       push operand
			if (flag == 1) {
				if (! isfull(operand)) {
					push(operand, sum);
				} 
			}  
	//       process operator
			if (*p == ')') {
				DealBracket(operand, operator);
			} else {
				Deal(operand, operator, *p);
			} 

			flag = 0;
		}

		p++;
	}
	
	if (flag) //previous one is number
		push(operand, sum);

	while (! isempty(operator)) {
		op2 = pop(operand);
		op1 = pop(operand);
		op  = pop(operator);
		push(operand, Compute(op1, op2, op));
	}
	printf("%s = %d\n", str, get_top(operand));

	return 0;
}
