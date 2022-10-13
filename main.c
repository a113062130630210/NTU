#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#define XOR(a, b) (XorNode *)((uintptr_t)(a)^(uintptr_t)(b))

int M, N, K;

typedef struct XorLinkedList
{
	int id, group;
	struct XorLinkedList *npx; // XOR of next and prev ptr
}XorNode;

int head_for_bathroom[1100000];   //開頭在哪裡
int tail_for_bathroom[1100000];  //結尾在哪裡
XorNode** head_for_group[1100000];
XorNode** tail_for_group[1100000];
bool closed[1100000]; //廁所關了
int close_bathroom = 0;
int* group_for_bathroom[1100000];

void prepare(){
	for(int i = 0 ; i < M + 2; i++){
		head_for_group[i] = calloc(K + 2, sizeof(XorNode*));
		//length[i] = calloc(K+2,sizeof(int));
	}


	for(int i = 0 ; i < M + 2; i++){
		for(int j = 0 ; j < K + 2; j++){
			head_for_group[i][j] = calloc(1, sizeof(XorNode));
			//length[i][j] = 0;
		}
	}

	for(int i = 0 ; i < M + 2 ; i++){
		tail_for_group[i] = calloc(K + 2, sizeof(XorNode*));
	}

	for(int i = 0 ; i < M + 2; i++){
		for(int j = 0 ; j < K + 2; j++){
			tail_for_group[i][j] = calloc(1, sizeof(XorNode));
			//length[i][j] = 0;
		}
	}

	for(int i = 0 ; i < M + 2 ; i++){
		group_for_bathroom[i] = calloc(K + 2, sizeof(int));
	}

	for(int i = 0 ; i < M + 2; i++){
		for(int j = 0 ; j < K + 2; j++){
			group_for_bathroom[i][j] = -1;
			//length[i][j] = 0;
		}
	}
}

void output(){
	for(int i = 0 ; i < M; i++){ //i_th 廁所
		if(closed[i] == true){
			printf("\n");
		}
		else{
			for(int j = head_for_bathroom[i] ; j <= tail_for_bathroom[i] ; j++){
				int gp = group_for_bathroom[i][j];
				if(gp != -1){
					XorNode* curr = head_for_group[i][gp];
					XorNode* prev = NULL;
					XorNode* next;
					while(NULL != curr){
						printf("%d ", curr -> id);
						next = XOR(prev, curr -> npx);
						prev = curr;
						curr = next;
					}
				}
			}
			printf("\n");
		}
		
	}
}

void removee(int m, bool from_tail){
	int i;
	XorNode* tmp;
	if(from_tail == 1){
		i = group_for_bathroom[m][tail_for_bathroom[m]];
		tmp = tail_for_group[m][i];
		XorNode* prev = XOR(tmp -> npx, NULL);
		if(prev == NULL){
			head_for_group[m][i] = tail_for_group[m][i] = NULL;
			group_for_bathroom[m][tail_for_bathroom[m]] = -1;
			tail_for_bathroom[m]--;
		}else{
			prev -> npx = XOR(tmp, XOR(prev -> npx, NULL));
			tail_for_group[m][i] = prev;
		}
	}else{
		i = group_for_bathroom[m][head_for_bathroom[m]];
		tmp = head_for_group[m][i];
		XorNode* next = XOR(tmp -> npx, NULL);
		if(next == NULL){
			//printf("haha\n");
			head_for_group[m][i] = tail_for_group[m][i] = NULL;
			// if(head_for_group[1][0] == NULL){
			// 	printf("head_for_group[1][0]=NULL\n");
			// }else{
			// 	printf("head_for_group[1][0] is not NULL\n");
			// }
			//printf("really here\n");
			group_for_bathroom[m][head_for_bathroom[m]] = -1;
			// if(head_for_group[1][0] == NULL){
			// 	printf("head_for_group[1][0]=NULL\n");
			// }else{
			// 	printf("head_for_group[1][0] is not NULL\n");
			// }
			head_for_bathroom[m]++;
			
		}else{
			next -> npx = XOR(tmp, XOR(NULL, next -> npx));
			head_for_group[m][i] = next;
		}
	}
	free(tmp);
	tmp = NULL;
}

void enter(int i, int j, int m){
	XorNode* tmp = (XorNode*)malloc(sizeof(XorNode));
	tmp -> id = j, tmp -> group = i;
	if(head_for_group[m][i] == NULL){
		tmp -> npx = NULL;
		head_for_group[m][i] = tail_for_group[m][i] = tmp;
		tail_for_bathroom[m]++;
		group_for_bathroom[m][tail_for_bathroom[m]] = i;
	}else{
		//會進入這裡
		//if(j == 37) printf("why here\n");
		tmp -> npx = XOR(tail_for_group[m][i], NULL);
		tail_for_group[m][i] -> npx = XOR(tmp, XOR(tail_for_group[m][i] -> npx, NULL));
		tail_for_group[m][i] = tmp;
	}
	//printf("why\n");
}

void leave(int m){
	removee(m, true);
}

void go(int m){
	removee(m, false);
}

void close(int m){
	closed[m] = true;
	close_bathroom++;
	int curr_bath;
	int i = m - 1; 
	while(close_bathroom < M){
		if(i < 0) i += M;
		if(closed[i] == false){
			curr_bath = i;
			break;
		}
		i--;
		
	}
	for(int i = tail_for_bathroom[m]; i >= head_for_bathroom[m]; i--){
		int flag = 0;
		for(int j = head_for_bathroom[curr_bath] ; j <= tail_for_bathroom[curr_bath]; j++){
			int gp1 = group_for_bathroom[m][i], gp2 = group_for_bathroom[curr_bath][j];
			//printf("       group = %d\n",group_for_bathroom[curr_bath][j]);
			if(group_for_bathroom[m][i] == group_for_bathroom[curr_bath][j]){
				tail_for_group[curr_bath][gp2] -> npx = XOR(tail_for_group[m][gp1], XOR(tail_for_group[curr_bath][gp2] -> npx, NULL));
				tail_for_group[m][gp1] -> npx = XOR(tail_for_group[curr_bath][gp2], XOR(tail_for_group[m][gp1] -> npx, NULL));
				tail_for_group[curr_bath][gp2] = head_for_group[m][gp1];
				flag = 1;
				break;
			}
		}
		if(flag == 0){ //插入到最後面
			tail_for_bathroom[curr_bath]++;
			int gp = group_for_bathroom[m][i];
			group_for_bathroom[curr_bath][tail_for_bathroom[curr_bath]] = gp;
			head_for_group[curr_bath][gp] = tail_for_group[m][gp];
			tail_for_group[curr_bath][gp] = head_for_group[m][gp];
		}
	}
	
}

int main(){
	scanf("%d%d%d",&M,&N,&K);
	prepare();
	for(int i = 0 ; i < M + 2 ; i++){
		for(int j = 0 ; j < K + 2 ; j++){
			head_for_group[i][j] = NULL;
			tail_for_group[i][j] = NULL;
			group_for_bathroom[i][j] = -1;
		}
	}
	for(int i = 0 ; i < M + 2; i++){
		head_for_bathroom[i] = 0;
		tail_for_bathroom[i] = -1;
		closed[i] = false;
	}

	for(int cnt = 0; cnt < N; cnt++){
		char c[10];
		scanf("%s",c);
		if(strcmp(c,"enter") == 0){
			int i, j, m;
			scanf("%d%d%d",&i,&j,&m); // id j, group i, bathroom m
			enter(i, j, m);
		}else if(strcmp(c,"leave") == 0){
			int m;
			scanf("%d",&m);
			leave(m);
		}else if(strcmp(c,"go") == 0){
			int m;
			scanf("%d",&m);
			go(m);
		}else if(strcmp(c,"close") == 0){
			int m;
			scanf("%d",&m);
			close(m);
		}
		// printf("current bathroom = -------------\n");
		// output();
		// printf("\n");
		// printf("head_for_bathroom = %d\n",head_for_bathroom[0]);
		// printf("tail_for_bathroom = %d\n",tail_for_bathroom[0]);
		// if(head_for_group[1][0] == NULL){
		// 	printf("head_for_group[1][0]=NULL\n");
		// }else{
		// 	printf("head_for_group[1][0] is not NULL\n");
		// }
	}
	output();
}