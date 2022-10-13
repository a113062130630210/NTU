#include<stdio.h>
#define int long long

char input[2000010];
char postfix[2000010];
int length = 0, postfix_length = 0;


typedef struct st1
{
	int a[2000010];
	int top;
}stack_for_evaluation;

stack_for_evaluation stack1;

void push_for_stack(int data){
	stack1.a[stack1.top] = data;
	stack1.top++;
}

int pop_for_stack(){
	stack1.top--;
	return stack1.a[stack1.top];
}

void print_for_stack(){
	for(int i = 0 ; i < stack1.top ; i++){
		printf("element in stack = %lld ", stack1.a[i]);
	}
	printf("\n");
}

typedef struct st2
{
	char store1[2000010];
	int top1;
}stack_for_infix;

stack_for_infix stack2;

void push_(char data){
	stack2.store1[stack2.top1] = data;
	stack2.top1++;
}

int pop_(){
	stack2.top1--;
	return stack2.store1[stack2.top1];
}

char top_(){
	return stack2.store1[stack2.top1 - 1];
}

void print_(){
	printf("element in stack = \n");
	for(int i = 0 ; i < stack2.top1 ; i++){
		printf("%c", stack2.store1[i]);
	}
	printf("\n");
}

void init(){
	stack1.top = 0;
	stack2.top1 = 0;
}


int precedence(char a){
	if(a == '*' || a == '/' || a == '%' ) return 2;
	else if(a == '+' || a == '-' ) return 1;
	else if(a == '(') return 0;
}

int evalution(){
	for(int i = 0;i < postfix_length;i++){
		if(postfix[i] <= 57 && postfix[i] >= 48 ){
			int num = 0;
			while(postfix[i] <= 57 && postfix[i] >= 48){
				int number = postfix[i] - '0';
				num = num * 10 + number;
				i++;
			}
			i--;
			push_for_stack(num);
		}
		else if(postfix[i] == ' '){
			continue;
		}else{
			int b = pop_for_stack();
			int a = pop_for_stack();
			int result;
			if(postfix[i] == '+'){
				result = a + b;
			}
			if(postfix[i] == '-'){
				result = a - b;
			}
			if(postfix[i] == '*'){
				result = a * b;
			}
			if(postfix[i] == '/'){
				if(a == 0){
					result = 0;
				}
				else if((a > 0 && b > 0)||(a < 0 && b < 0)){
					result = a / b;
				}else if((a < 0 && b > 0)||(a > 0 && b < 0)){
					if(a % b < 0){
						result = a / b - 1;
					}
				}
			}
			if(postfix[i] == '%'){
				result = a % b;
			}
			push_for_stack(result);
		}
	}
	return pop_for_stack();
}


char convert(int m){
	if(m < 10){
		postfix[postfix_length] = m - '0';
		postfix_length++;
	}else{
		convert(m / 10);
		postfix[postfix_length] = m % 10 - '0';
		postfix_length++;
	}
}



void infix_to_postfix(){
	int test = 0;
	for(int i = 0;i < length; i++){
		if(input[i] <= 57 && input[i] >= 48){
			while(input[i] <= 57 && input[i] >= 48){
				postfix[postfix_length] = input[i];
				postfix_length++;
				i++;
			}
			i--;
			postfix[postfix_length] = ' ';
			postfix_length++;
		}else if(input[i] == '('){
			push_('(');
		}else if(input[i] == ')'){
			while(stack2.top1 > 0 && top_() != '('){
				postfix[postfix_length] = pop_();
				postfix_length++;
				postfix[postfix_length] = ' ';
				postfix_length++;
			}
			pop_();
		}else if(input[i] == '='){
			while(stack2.top1 > 0){
				postfix[postfix_length] = pop_();
				postfix_length++;
				postfix[postfix_length] = ' ';
				postfix_length++;
			}

			// for(int i=0;i<postfix_length;i++){
			//  	printf("%c",postfix[i]);
			// }
			int ans = evalution();
			printf("Print: %lld\n",ans);


			//convert(ans);
			//postfix[postfix_length] = ' ';
			//postfix_length++;

		}else if(input[i] == 10){
			break;
		}else{
			while(stack2.top1 > 0 && precedence(top_()) >= precedence(input[i])){
				postfix[postfix_length] = pop_();
				postfix_length++;
				postfix[postfix_length] = ' ';
				postfix_length++;
			}
			push_(input[i]);
		}
	}
}

int main(){
	init();
	char c;
	while(scanf("%c",&c) != EOF){
		input[length] = c;
		length++;
	}
	infix_to_postfix();
}
