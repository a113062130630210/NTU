#include "price.h"
#include<stdio.h>
#include<stdlib.h>
#define int long long

int A, Q, N;
int s[1030];
int extra_stock, k;

typedef struct Node{
    int id; //which stock
    int value;
}node;

void swap(node *xp, node *yp)
{
    node temp = *xp;
    *xp = *yp;
    *yp = temp;
}

int n;

void test(){
    node a[10];
    a[0].value = 1;
    a[1].value = 2;
    a[1].id = 2;
    a[0].id = 1;

    printf("a[0].value = %lld, a[0].id = %lld\n", a[0].value, a[0].id);
    printf("a[1].value = %lld, a[1].id = %lld\n", a[1].value, a[1].id);

    swap(&a[0],&a[1]);

    printf("a[0].value = %lld, a[0].id = %lld\n", a[0].value, a[0].id);
    printf("a[1].value = %lld, a[1].id = %lld", a[1].value, a[1].id);
}

// To heapify a subtree rooted with node i which is
// an index in arr[]. N is size of heap
void heapify(node arr[], int i)
{
    int smallest = i; // Initialize largest as root
    int l = 2 * i + 1; // left = 2*i + 1
    int r = 2 * i + 2; // right = 2*i + 2
 
    // If left child is larger than root
    if (l < n && arr[l].value < arr[smallest].value)
        smallest = l;
 
    // If right child is larger than largest so far
    if (r < n && arr[r].value < arr[smallest].value)
        smallest = r;
 
    // If largest is not root
    if (smallest != i) {
        //printf("before swap \n arr[i].value = %lld, arr[i].id = %lld\n", arr[i].value, arr[i].id);
        //printf("arr[smallest].value = %lld, arr[smallest].id = %lld\n", arr[smallest].value, arr[smallest].id);
        swap(&arr[i], &arr[smallest]);

        //printf("after swap \n arr[i].value = %lld, arr[i].id = %lld\n", arr[i].value, arr[i].id);
        //printf("arr[smallest].value = %lld, arr[smallest].id = %lld\n", arr[smallest].value, arr[smallest].id);
        // Recursively heapify the affected sub-tree
        heapify(arr, smallest);
    }
}

void buildHeap(node arr[]) //ok
{
    // Index of last non-leaf node
    int startIdx = (n / 2) - 1;
 
    // Perform reverse level order traversal
    // from last non-leaf node and heapify
    // each node
    for (int i = startIdx; i >= 0; i--) {
        heapify(arr, i);
    }
}

void printHeap(node arr[]) //ok
{
    printf("Array representation of Heap is:\n");
 
    for (int i = 0; i < n; ++i)
        printf("%lld ",arr[i].value);
    printf("\n");
}

void insertNode(node arr[], node Key) //ok
{
    n = n + 1;  // Increase the size of Heap by 1
 
    arr[n - 1] = Key; // Insert the element at end of Heap

    heapify(arr, n - 1); // Heapify the new node
}

void deleteRoot(node arr[]) //ok
{
    node lastElement = arr[n - 1]; // Get the last element
    //printf("before swap \narr[n-1].value = %lld, arr[n-1].id = %lld\n", arr[n-1].value, arr[n-1].id);
    //printf("arr[0].value = %lld, arr[0].id = %lld\n", arr[0].value, arr[0].id);
    arr[0] = lastElement; // Replace root with last element
    //printf("after swap \narr[n-1].value = %lld, arr[n-1].id = %lld\n", arr[n-1].value, arr[n-1].id);
    //printf("arr[0].value = %lld, arr[0].id = %lld\n", arr[0].value, arr[0].id);
    n = n - 1;  // Decrease size of heap by 1

    heapify(arr, 0); // heapify the root node
}

            
int result[1000005];
node arr[1000000];

void for_subtask2(){
    n = N;
    //int arr[1000000];
    for(int i = 1 ; i <= N ; i++){
        node tmp;
        tmp.value = price(s[1], i);
        tmp.id = s[1];
        arr[i-1] = tmp; 
    }

    buildHeap(arr);
    //printHeap(arr);
    int count_date = N + 1;
    for(int i = 1 ; i <= 1000000 ; i++){
        result[i] = arr[0].value;
        deleteRoot(arr);
        node tmp;
        tmp.value = price(s[1], count_date);
        tmp.id = s[1];
        insertNode(arr, tmp);
        count_date++;
    }
}

void for_subtask3(){
    n = A;
    for(int i = 1 ; i <= A ; i++){
        node tmp;
        tmp.value = price(s[i], 1);
        tmp.id = s[i];
        arr[i-1] = tmp; 
    }
    buildHeap(arr);
    int count_date[1030];
    for(int i = 1 ; i <= A ; i++){
        count_date[i] = 2;
    }

    for(int i = 1 ; i <= 1000000 ; i++){
        result[i] = arr[0].value;
        int _id = arr[0].id;
        deleteRoot(arr);

        node tmp;
        tmp.value = price(s[_id], count_date[_id]);
        tmp.id = _id;
        insertNode(arr, tmp);
        count_date[_id]++;
    }
}

void for_subtask34(){
    n = A * N;
    for(int i = 1 ; i <= A ; i++){
        for(int j = 1 ; j <= N ; j++){
            node tmp;
            //printf("value = %lld, id = %lld\n",price(s[i], j), i);
            tmp.value = price(s[i], j);
            tmp.id = s[i];
            arr[(i - 1) * N + j - 1] = tmp;
        }
    }
    buildHeap(arr);

    //printHeap(arr);
    int count_date[1030];
    for(int i = 1 ; i <= A ; i++){
        count_date[i] = N + 1;
    }
    //printf("-------------------------------------\n");
    for(int i = 1 ; i <= 1000000 ; i++){
        result[i] = arr[0].value;
        int _id = arr[0].id;
        //printf("value = %lld, id = %lld\n",result[i], _id);
        deleteRoot(arr);

        node tmp;
        tmp.value = price(s[_id], count_date[_id]);
        tmp.id = _id;
        insertNode(arr, tmp);
        count_date[_id]++;
    }
}


int output(int k){
    if(extra_stock == 0){
        if(A == 1 && N == 1){
            return price(s[1], k);
        }else{
            return result[k];
        }
    }
}

signed main(){
    scanf("%lld%lld%lld",&A,&Q,&N);
    for(int i = 0 ; i < A ; i++){
        scanf("%lld",&s[i + 1]);
    }
    if(A == 1) for_subtask2(); //calculate it first
    else if(N == 1) for_subtask3();
    else for_subtask34();

    for(int i = 0 ; i < Q ; i++){
        scanf("%lld%lld",&extra_stock,&k);
        printf("%lld\n",output(k));
    }
}
