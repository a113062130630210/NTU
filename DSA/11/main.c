#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#define int long long
#define INF 2147483647

//lazy tag: 先標記哪些要改 用到的時候在改 其他就不用改
int n, q; //number of machines, number of operations
int machine_time[300005];
typedef struct node_{
	int l;
	int r;
	int sum;
	int size;
	int value;
	int pri;
	bool lazytag_3;
	int lazytag_5;
	int ma;
	int se;
	int t;
	//node_(int k): l(0), r(0), lazytag_3(0), lazytag_5(0), value(k), pri(rand()), size(1), sum(k){}
}node;

node treap[300005];

int root, cnt;

void push_down(int cur){
	if(treap[cur].lazytag_3 == false) 
		return;
	int tmp;
	tmp = treap[cur].l;
	treap[cur].l = treap[cur].r;
	treap[cur].r = tmp;
	treap[treap[cur].l].lazytag_3 ^= 1;
	treap[treap[cur].r].lazytag_3 ^= 1;
	treap[cur].lazytag_3 = false;
}

void print(int cur){
	if(cur == 0) return;
	//push_down(cur);
	print(treap[cur].l);
	printf("cur_value = %lld, cur_lazytag5 = %lld, cur_pri = %lld, cur_ma = %lld, cur_se = %lld, cur_t = %lld, cur_l = %lld, cur_r = %lld, cur_sum = %lld\n", treap[cur].value, treap[cur].lazytag_5, treap[cur].pri, treap[cur].ma, treap[cur].se, treap[cur].t, treap[cur].l, treap[cur].r, treap[cur].sum);
	print(treap[cur].r);
}

int min_of_two(int a, int b){
	if(a <= b) return a;
	else return b;
}

int max_of_two(int a, int b){
	if(a >= b) return a;
	else return b;
}

void push_up(int cur){
	//int left = treap[cur].l, right = treap[cur].r;
	treap[cur].size = treap[treap[cur].l].size + treap[treap[cur].r].size + 1;
	treap[cur].sum = treap[treap[cur].l].sum + treap[treap[cur].r].sum + treap[cur].value;
	//5個東西放進陣列 問最大跟第二大以及最大有幾個
	// if(left == 0 && right == 0){
	// 	treap[cur].size = 1;
	// 	treap[cur].sum = treap[cur].value;
	// 	treap[cur].ma = treap[cur].value;
	// 	treap[cur].t = 1;
	// 	treap[cur].se = -INF;
	// }else if(right == 0){
	// 	treap[cur].size = 1 + treap[left].size;
	// 	treap[cur].sum = treap[cur].value + treap[left].sum;
	// 	if(treap[left].ma > treap[cur].value){
	// 		treap[cur].ma = treap[left].ma;
	// 		treap[cur].t = treap[left].t;
	// 		treap[cur].se = max_of_two(treap[cur].value, treap[left].se);
	// 	}else if(treap[left].ma == treap[cur].value){
	// 		treap[cur].ma = treap[cur].value;
	// 		treap[cur].t = 1 + treap[left].t;
	// 		treap[cur].se = treap[left].se;
	// 	}else{
	// 		treap[cur].ma = treap[cur].value;
	// 		treap[cur].t = 1;
	// 		treap[cur].se = treap[left].ma;
	// 	}
	// }else if(left == 0){
	// 	treap[cur].size = 1 + treap[right].size;
	// 	treap[cur].sum = treap[cur].value + treap[right].sum;
	// 	if(treap[right].ma > treap[cur].value){
	// 		treap[cur].ma = treap[right].ma;
	// 		treap[cur].t = treap[right].t;
	// 		treap[cur].se = max_of_two(treap[cur].value, treap[right].se);
	// 	}else if(treap[right].ma == treap[cur].value){
	// 		treap[cur].ma = treap[cur].value;
	// 		treap[cur].t = 1 + treap[right].t;
	// 		treap[cur].se = treap[right].se;
	// 	}else{
	// 		treap[cur].ma = treap[cur].value;
	// 		treap[cur].t = 1;
	// 		treap[cur].se = treap[left].ma;
	// 	}
	// }
	// else{
	// 	treap[cur].size = treap[treap[cur].l].size + treap[treap[cur].r].size + 1;
	// 	treap[cur].sum = treap[treap[cur].l].sum + treap[treap[cur].r].sum + treap[cur].value;
	// 	int a[5] = {treap[cur].value, treap[left].ma, treap[left].se, treap[right].ma, treap[right].se};
	// 	int max1 = a[0], max2 = -INF;
	// 	for(int i = 1 ; i < 5 ; i++){
	// 		if(a[i] > max1){
	// 			max2 = max1;
	// 			max1 = a[i];
	// 		}else if(a[i] != max1){
	// 			if(a[i] > max2) max2 = a[i];
	// 		}
	// 	}
	// 	treap[cur].ma = max1;
	// 	treap[cur].se = max2;
	// 	treap[cur].t = 0;
	// 	if(max1 == treap[cur].value){
	// 		treap[cur].t++;
	// 	}if(max1 == treap[left].ma){
	// 		treap[cur].t += treap[left].t;
	// 	}if(max1 == treap[right].ma){
	// 		treap[cur].t += treap[right].t;
	// 	}
	// }
	if(treap[cur].value == treap[treap[cur].l].ma && treap[cur].value == treap[treap[cur].r].ma){
		treap[cur].ma = treap[cur].value;
		treap[cur].t = 1 + treap[treap[cur].l].t + treap[treap[cur].r].t;
		treap[cur].se = max_of_two(treap[treap[cur].l].se, treap[treap[cur].r].se);
	}else if(treap[cur].value == treap[treap[cur].l].ma){
		if(treap[cur].value > treap[treap[cur].r].ma){
			treap[cur].ma = treap[cur].value;
			treap[cur].t = 1 + treap[treap[cur].l].t;
			treap[cur].se = max_of_two(treap[treap[cur].l].se, treap[treap[cur].r].ma);
		}else{
			treap[cur].ma = treap[treap[cur].r].ma;
			treap[cur].t = treap[treap[cur].r].t;
			treap[cur].se = max_of_two(treap[treap[cur].r].se, treap[treap[cur].l].ma);
		}
	}else if(treap[cur].value == treap[treap[cur].r].ma){
		if(treap[cur].value > treap[treap[cur].l].ma){
			treap[cur].ma = treap[cur].value;
			treap[cur].t = 1 + treap[treap[cur].r].t;
			treap[cur].se = max_of_two(treap[treap[cur].r].se, treap[treap[cur].l].ma);
		}else{
			treap[cur].ma = treap[treap[cur].l].ma;
			treap[cur].t = treap[treap[cur].l].t;
			treap[cur].se = max_of_two(treap[treap[cur].l].se, treap[treap[cur].r].ma);
		}
	}else if(treap[treap[cur].r].ma == treap[treap[cur].l].ma){
		if(treap[cur].value > treap[treap[cur].l].ma){
			treap[cur].ma = treap[cur].value;
			treap[cur].t = 1;
			treap[cur].se = treap[treap[cur].l].ma;
		}else{
			treap[cur].ma = treap[treap[cur].l].ma;
			treap[cur].t = treap[treap[cur].l].t + treap[treap[cur].r].t;
			treap[cur].se = max_of_two(treap[cur].value, max_of_two(treap[treap[cur].l].se, treap[treap[cur].r].se));
		}
	}else{
		if(treap[cur].value > treap[treap[cur].l].ma && treap[cur].value > treap[treap[cur].r].ma){
			treap[cur].ma = treap[cur].value;
			treap[cur].t = 1;
			treap[cur].se = max_of_two(treap[treap[cur].l].ma, treap[treap[cur].r].ma);
		}else if(treap[treap[cur].r].ma > treap[treap[cur].l].ma && treap[treap[cur].r].ma > treap[cur].value){
			treap[cur].ma = treap[treap[cur].r].ma;
			treap[cur].t = treap[treap[cur].r].t;
			treap[cur].se = max_of_two(treap[cur].value, max_of_two(treap[treap[cur].r].se, treap[treap[cur].l].ma));
		}else{
			treap[cur].ma = treap[treap[cur].l].ma;
			treap[cur].t = treap[treap[cur].l].t;
			treap[cur].se = max_of_two(treap[cur].value, max_of_two(treap[treap[cur].l].se, treap[treap[cur].r].ma));
		}
	}
	//維護ma, se, t
}

void another_push_down(int cur){
	if(treap[cur].lazytag_5 == 0 || cur <= 0) 
		return;
	// print(root);
	// printf("\n");
	treap[cur].value = min_of_two(treap[cur].value, treap[cur].lazytag_5);
	if(treap[cur].ma <= treap[cur].lazytag_5){
		treap[cur].lazytag_5 = 0;
		return;
	}
	else if(treap[cur].se < treap[cur].lazytag_5 && treap[cur].lazytag_5 < treap[cur].ma){
		treap[cur].sum -= treap[cur].t * (treap[cur].ma - treap[cur].lazytag_5);
		treap[cur].ma = treap[cur].lazytag_5;
		if(treap[cur].l != 0){
			if(treap[treap[cur].l].lazytag_5 > treap[cur].lazytag_5 || treap[treap[cur].l].lazytag_5 == 0)
				treap[treap[cur].l].lazytag_5 = treap[cur].lazytag_5;
		}
		if(treap[cur].r != 0){
			if(treap[treap[cur].r].lazytag_5 > treap[cur].lazytag_5 || treap[treap[cur].r].lazytag_5 == 0)
				treap[treap[cur].r].lazytag_5 = treap[cur].lazytag_5;
		}
		treap[cur].lazytag_5 = 0;
		return;
	}
	else{
		//跟之前懶標取min
		if(treap[cur].l != 0){
			if(treap[treap[cur].l].lazytag_5 > treap[cur].lazytag_5 || treap[treap[cur].l].lazytag_5 == 0)
				treap[treap[cur].l].lazytag_5 = treap[cur].lazytag_5;
		}
		if(treap[cur].r != 0){
			if(treap[treap[cur].r].lazytag_5 > treap[cur].lazytag_5 || treap[treap[cur].r].lazytag_5 == 0)
				treap[treap[cur].r].lazytag_5 = treap[cur].lazytag_5;
		}
		treap[cur].lazytag_5 = 0;
		another_push_down(treap[cur].l);
		another_push_down(treap[cur].r);
		push_up(cur);
	}
}

int merge(int x, int y){
	if(x == 0) return y;
	if(y == 0) return x;
	push_down(x);
	push_down(y);
	another_push_down(x);
	another_push_down(y);
	if(treap[x].pri > treap[y].pri){
		treap[x].r = merge(treap[x].r, y);
		another_push_down(treap[x].l);
		another_push_down(treap[x].r);
		push_up(x);
		return x;
	}else{
		treap[y].l = merge(x, treap[y].l);
		another_push_down(treap[y].l);
		another_push_down(treap[y].r);
		push_up(y);
		return y;
	}
}

void split(int cur, int k, int *x, int *y){ //要改成pointer
	push_down(cur);
	another_push_down(cur);
	if(cur == 0){
		*x = *y = 0;
		return;
	}
	if(treap[treap[cur].l].size >= k){
		*y = cur;
		split(treap[cur].l, k, x, &treap[*y].l);
		another_push_down(treap[*y].l);
		another_push_down(treap[*y].r);
		push_up(*y);
	}else{
		*x = cur;
		split(treap[cur].r, k - treap[treap[cur].l].size - 1, &treap[*x].r, y);
		another_push_down(treap[*x].l);
		another_push_down(treap[*x].r);
		push_up(*x);
	}
}

void init(){
	root = cnt = 0;
	treap[0].l = treap[0].r = 0;
	treap[0].size = 0;
	treap[0].lazytag_3 = false;
	treap[0].lazytag_5 = 0;
	treap[0].value = 0;
	treap[0].sum = 0;
	treap[0].ma = -INF;
	treap[0].se = -INF;
	treap[0].t = 0;
}

int create(int x){
	cnt++;
	treap[cnt].lazytag_3 = false;
	treap[cnt].lazytag_5 = 0;
    treap[cnt].size = 1;
    treap[cnt].pri = rand();
    treap[cnt].value = x;
    treap[cnt].sum = x;	
    treap[cnt].l = treap[cnt].r = 0;
    treap[cnt].ma = x;
    treap[cnt].se = -INF;
    treap[cnt].t = 1;
    return cnt;
}

void build(int x){
	root = merge(root, create(x));
}

void oper1(int p, int k){ //new machine which needs k seconds to reboot is placed between the p-th machine and the (p + 1)-th machine.
	int x, y;
	split(root, p, &x, &y);
	root = merge(merge(x, create(k)), y);
} //插入很多點

void oper2(int p){ //The p-th machine will be retired and discarded.
	int x, y, z;
	split(root, p - 1, &x, &y);
	split(y, 1, &y, &z);
	root = merge(x, z);
}

void oper3(int l, int r){
	int x, y, z;
	split(root, l - 1, &x, &y);
	int rt = y;
	split(rt, r - l + 1, &y, &z);
	treap[y].lazytag_3 ^= 1;
	rt = merge(y, z);
	root = merge(x, rt);
}

void oper4(int l, int r, int x, int y){
	int a, b, c, d, e;
	split(root, l - 1, &a, &b); //第一段
	int rt = b;
	split(rt, r - l + 1, &b, &c); //第二段
	int rt1 = c;
	split(rt1, x - r - 1, &c, &d); //第三段
	int rt2 = d;
	split(rt2, y - x + 1, &d, &e); //
	rt = merge(a, d);
	rt1 = merge(rt, c);
	rt2 = merge(rt1, b);
	root = merge(rt2, e);
}

void oper5(int l, int r, int k){
	int x, y, z;
	split(root, l - 1, &x, &y);
	int rt = y;
	split(rt, r - l + 1, &y, &z);
	treap[y].lazytag_5 = k;
	rt = merge(y, z);
	root = merge(x, rt);
}

void oper6(int l, int r){
	int first, second;
	if(l == 1){
		split(root, r, &first, &second);
		printf("%lld\n",treap[first].sum);
		root = merge(first, second);
	}else{
		int third, fourth;
		split(root, l - 1, &first, &second);
		split(second, r - l + 1, &third, &fourth);
		printf("%lld\n", treap[third].sum);
		second = merge(third, fourth);
		root = merge(first, second);
	}
}

signed main(){
	init();
	int p, l, r, x, y, k;
	scanf("%lld%lld",&n,&q);
	for(int i = 1 ; i <= n ; i++){
		scanf("%lld", &machine_time[i]);
	}
	for(int i = 1 ; i <= n ; i++){
		build(machine_time[i]);
	}
	// print(root);
	// printf("\n");
	for(int i = 1 ; i <= q ; i++){
		int oper;
		scanf("%lld",&oper);
		if(oper == 1){
			scanf("%lld%lld",&p, &k);
			oper1(p, k);
		}else if(oper == 2){
			scanf("%lld",&p);
			oper2(p);
		}else if(oper == 3){
			scanf("%lld%lld",&l,&r);
			oper3(l, r);
		}else if(oper == 4){
			scanf("%lld%lld%lld%lld",&l,&r,&x,&y);
			oper4(l, r, x, y);
		}else if(oper == 5){
			scanf("%lld%lld%lld",&l, &r, &k);
			oper5(l, r, k);
		}else if(oper == 6){
			scanf("%lld%lld", &l, &r);
			oper6(l, r);
		}
		//print(root);
	}
}

//每個維護好下面的總和
//每個存好下面的總和 sum key(無關) pri

//node: key(BST), pri(heap), value, 下面的sum, 下面的size, index = 左子樹size+1, reverse tag
//split by size not split by key
//根本不需要key 可以只用size跟pri完成treap
//key唯一的功能就是拿來做BST(不用維護BST，因為key不重要)
//不用key就可以建出treap 
//treap用merge跟split維護 因此根本不用key
//建treap: 用操作1 一次插入一個點 初始化就是插入很多點
