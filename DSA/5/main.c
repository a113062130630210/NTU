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

        swap(&arr[i], &arr[smallest]);
        // Recursively heapify the affected sub-tree
        heapify(arr, smallest);
    }
}

void heapify2(node arr[], int i)
{
    // Find parent
    int parent = (i - 1) / 2;
 
        // For Max-Heap
        // If current node is greater than its parent
        // Swap both of them and call heapify again
        // for the parent
    if (arr[i].value < arr[parent].value) {
        swap(&arr[i], &arr[parent]);
 
            // Recursively heapify the parent node
        heapify2(arr, parent);
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

    heapify2(arr,n-1);
    
}

void deleteRoot(node arr[]) //ok
{
    node lastElement = arr[n - 1]; // Get the last element

    arr[0] = lastElement; // Replace root with last element

    n = n - 1;  // Decrease size of heap by 1

    heapify(arr, 0); // heapify the root node
}

            
int result[1000005];
node arr[2000000];

void for_subtask2(){
    n = A * N;
    for(int i = 1 ; i <= A ; i++){
        for(int j = 1 ; j <= N ; j++){
            node tmp;
            tmp.value = price(s[i], j);
            tmp.id = i;
            arr[(i - 1) * N + j - 1] = tmp;
        }
    }
    buildHeap(arr);

    //printHeap(arr);
    int count_date[1030];
    for(int i = 1 ; i <= A ; i++){
        count_date[i] = N + 1;
    }

    for(int i = 1 ; i <= 1000000 ; i++){
        result[i] = arr[0].value;
        int id2 = arr[0].id;
        
        deleteRoot(arr);

        node tmp;
        tmp.value = price(s[id2], count_date[id2]);
        tmp.id = id2;
        count_date[id2]++;
        insertNode(arr, tmp);
    }
}

int binary_search(int arr[], int n, int key){ //Count of <= key elements in sorted array arr
    int left = 1;
    int right = n;
    int count = 0;
    while(left <= right){
        int mid = (left + right) / 2;
        if(arr[mid] < key){
            count = mid;
            left = mid + 1;
        }
        else right = mid - 1;
    }
    return count;
}

int min_of_two(int a, int b){
    if(a < b){
        return a;
    }else{
        return b;
    }
}

int subtask4(int k, int st1, int st2){
    if(st1 == 1000001) return price(extra_stock, st2+k-1);
    if(st2 == 1000001) return result[st1+k-1];
    if(k==1) return min_of_two(result[st1],price(extra_stock,st2));
    int curr=k/2;
    if(result[st1+curr-1]<price(extra_stock,st2+curr-1)){
        return subtask4(k-curr,st1+curr,st2);
    }else{
        return subtask4(k-curr,st1,st2+curr);
    }
}

node array[1000000];
node array1[1000000];

int kth(int arr1[], int arr2[], int m, int n, int k,int st1, int st2){
    if (st1 == m)
        return arr2[st2 + k - 1];
 
    // In case we have reached end of array 2
    if (st2 == n)
        return arr1[st1 + k - 1];
 
    // Compare first elements of arrays and return
    if (k == 1)
        return (arr1[st1] < arr2[st2]) ?
            arr1[st1] : arr2[st2];
    int curr = k / 2;
 
    // Size of array 1 is less than k / 2
    if (curr >= m +1- st1)
    {
        // Last element of array 1 is not kth
        // We can directly return the (k - m)th
        // element in array 2
        if (arr1[m] < arr2[st2 + curr - 1])
            return arr2[st2 + (k - (m - st1) - 1)];
        else
            return kth(arr1, arr2, m, n, k - curr,
                st1, st2 + curr);
    }
 
    // Size of array 2 is less than k / 2
    if (curr >= n+1-st2)
    {
        if (arr2[n] < arr1[st1 + curr - 1])
            return arr1[st1 + (k - (n - st2) - 1)];
        else
            return kth(arr1, arr2, m, n, k - curr,
                st1 + curr, st2);
    }
    else
    {
        // Normal comparison, move starting index
        // of one array k / 2 to the right
        if (arr1[curr + st1 - 1] < arr2[curr + st2 - 1])
            return kth(arr1, arr2, m, n, k - curr,
                st1 + curr, st2);
        else
            return kth(arr1, arr2, m, n, k - curr,
                st1, st2 + curr);
    }    
}


int subtask(int k){
    int l = 0, r = k;
    int mid1;
    int smaller1;
    while(r>l+3*N){
        int mid = (l+r)/2;
        int smaller=binary_search(result,k,price(extra_stock,mid));
        if(smaller+mid-N>k) //smaller than mid 
            r=mid-1;
        int smaller2=binary_search(result,k,price(extra_stock,mid+N));
        if(smaller2+mid<k){
            l=mid+N;
        }
        if(smaller+mid-N<=k&&smaller2+mid>=k){
            mid1=mid;
            smaller1=smaller;
            break;
        }
    }
    for(int i=0;i<4*N+1;i++){
        node newnode;
        newnode.value=price(extra_stock,mid1-2*N+i);
        newnode.id=mid1-2*N+i;
        array[i]=newnode;
    }
    n=4*N+1;
    buildHeap(array);
    for(int i=1;i<=4*N+1;i++){
        node newnode;
        newnode.value=array[0].value;
        newnode.id=array[0].id;
        array1[i]=newnode;
        deleteRoot(array);
    }

    for(int i=1;i<=4*N+1;i++){
        if(binary_search(result,k,array1[i].value)+mid1-2*N-1+i==k){
            return array1[i].value;
        }
        if(binary_search(result,k,array1[i+1].value)+mid1-2*N-1+i+1==k){
            return array1[i+1].value;
        } 
        if(binary_search(result,k,array1[i].value)+mid1-2*N-1+i<k&&binary_search(result,k,array1[i+1].value)+mid1-2*N-1+i+1>k){
            return result[k-(mid1-2*N-1+i)];
        }
    }
    
}

int output(int k){ //run Q times
    if(extra_stock == 0){
        if(A == 1 && N == 1){
            return price(s[1], k);
        }else{
            return result[k];
        }
    }else{
        if(N == 1){
            //int ans = kth(1000000,1000000,k,1,1);
            int ans = subtask4(k,1,1);
            return ans;
        }
        int ans = subtask(k);
        return ans;
    }
}

signed main(){
    scanf("%lld%lld%lld",&A,&Q,&N);
    for(int i = 0 ; i < A ; i++){
        scanf("%lld",&s[i + 1]);
    }
    for_subtask2();

    for(int i = 0 ; i < Q ; i++){
        scanf("%lld%lld",&extra_stock,&k);
        printf("%lld\n",output(k));
    }
}
