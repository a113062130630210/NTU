#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int n, m, number_of_shops;
int visited[1000010];

int number_of_query = 0;
int dsu[1000010]; //存parent
int size[1000010]; 

typedef struct days_{
    int operation; //1: merge 2: query 3: boom
    int a, b;
}days;

days day_array[1000010];

typedef struct Ans{
    int date, number;
}ans;

ans answer[1000010];

struct node{
    int day;
    struct node* next;
    struct node* prev;
};

struct Graph{
    int numVertices;
    struct node** adjLists;
    struct node** tail;
};

struct node* createNode(int v){
    struct node* newnode = malloc(sizeof(struct node));
    newnode->day = v;
    newnode->next = NULL;
    newnode->prev = NULL;
    return newnode;
}

struct Graph* creategraph(int vertices){
    struct Graph* g = malloc(sizeof(struct Graph));
    g->numVertices = vertices;
    g->adjLists = malloc(vertices * sizeof(struct node*));
    g->tail = malloc(vertices * sizeof(struct node*));

    for(int i = 0 ; i < vertices ; i++){
        g->adjLists[i] = NULL;
        g->tail[i] = NULL;
    }
    return g;
}

void printgraph(struct Graph* g){
    for(int v = 0 ; v < g->numVertices ; v++){
        struct node* temp = g->adjLists[v];
        printf("\n Adjacency list of vertex %d\n ", v);
        while(temp){
            printf("%d -> ", temp->day);
            temp = temp->next;
        }
        printf("\n");
    }
}

void addEdge(struct Graph* g, int a, int b){
    struct node* newnode = createNode(a);

    if(g->adjLists[b]){
        newnode->prev = g->tail[b];
        g->tail[b]->next = newnode;
        g->tail[b] = newnode;
    }else{
        g->adjLists[b] = newnode;
        g->tail[b] = newnode; 
    }

    // newnode = createNode(b);
    // if(g->adjLists[a]){
    //  newnode->prev = g->tail[a];
    //  g->tail[a]->next = newnode;
    //  g->tail[a] = newnode;
    // }else{
    //  g->adjLists[a] = newnode;
    //  g->tail[a] = newnode; 
    // }
}

// void DFS_alter(struct Graph* graph, int vertex){
//  struct node* adjList = graph->adjLists[vertex];
//      struct node* temp = adjList;

//      visited[vertex] = 1;
//      printf("Visited %d \n", vertex);

//      while (temp != NULL) {
//      int connectedVertex = temp->day;

//      if (visited[connectedVertex] == 0) {
//              DFS(graph, connectedVertex);
//      }
//      temp = temp->next;
//      }
// }

int findset(int x){
    while(dsu[x] != x){
        x = dsu[x];
    }
    return x;
}

int union_(int a, int b){
    int aa = findset(a);
    int bb = findset(b);
    if(aa != bb){
        if(size[aa] > size[bb]){
            dsu[bb] = aa;
            size[aa] += size[bb];
        }else{
            dsu[aa] = bb;
            size[bb] += size[aa];
        }
        number_of_shops--;
    }
}

void traverse(struct Graph* graph, int vertex){
    struct node* adjList = graph->adjLists[vertex];
    struct node* temp =  adjList;
    visited[vertex] = 1;
    // printf("Visited %d \n", vertex);
    // printf("current_number_vertices=%d\n",number_of_shops);
    // for(int i = 1 ; i <= 10 ; i++){
    //     printf("dsu[%d]= %d ", i, dsu[i]);
    // }
    // printf("\n");

    while(temp != NULL){
        int connectedVertex = temp -> day;
        int first1, second1, size1, size2;
        //printf("connectedVertex = %d\n",connectedVertex);
        if(day_array[connectedVertex].operation == 2){ 
            ans x;
            x.number = number_of_shops, x.date = connectedVertex;
            answer[number_of_query] = x;
            number_of_query++;
        }else if(day_array[connectedVertex].operation == 3){

        }else{
            int first = day_array[connectedVertex].a, second = day_array[connectedVertex].b;
            first1 = findset(first); second1 = findset(second), size1 = size[first1], size2 = size[second1];
            union_(first, second);
        }

        if(visited[connectedVertex] == 0){
            traverse(graph, connectedVertex);
        }   

        if(day_array[connectedVertex].operation == 1){
            //int first = day_array[connectedVertex].a, second = day_array[connectedVertex].b;
            if(first1 != second1){
                number_of_shops++;
                if(size1 > size2){
                    dsu[first1] = first1, dsu[second1] = second1;
                    size[first1] -= size[second1];
                }else{
                    dsu[first1] = first1, dsu[second1] = second1;
                    size[second1] -= size[first1];
                }
            }
            
        }

        temp = temp->next;
    }
}

int cmp(const void *a, const void *b)
{
return (*(ans *)a).date - (*(ans *)b).date;
}

int main(){
    scanf("%d%d",&n,&m);
    number_of_shops = n;
    struct Graph* graph = creategraph(m + 1);

    char oper[10];

    for(int i = 0 ; i <= m ; i++){
        answer[i].number = 0;
        answer[i].date = 0;
        visited[i] = 0;
        dsu[i] = i;
        size[i] = 1;
    }

    for(int i = 1 ; i <= m ; i++){
        scanf("%s", oper);
        if(oper[0] == 'b'){
            int k;
            scanf("%d", &k);
            days newdays;
            newdays.operation = 3, newdays.a = k, newdays.b = 0;
            day_array[i] = newdays;
            addEdge(graph, i, k);
        }else if(oper[0] == 'q'){
            days newdays;
            newdays.operation = 2, newdays.a = 0, newdays.b = 0;
            day_array[i] = newdays;
            addEdge(graph, i, i - 1);
        }else{
            int shop_1, shop_2;
            scanf("%d%d",&shop_1,&shop_2);
            days newdays;
            newdays.operation = 1, newdays.a = shop_1, newdays.b = shop_2; 
            day_array[i] = newdays;
            addEdge(graph, i, i - 1);
        }
    }
    //printgraph(graph);
    traverse(graph, 0);


    // for(int i = 0 ; i < number_of_query ; i++){
    //     printf("%d\n", answer[i].number);
    // }

    qsort(answer, number_of_query, sizeof(ans), cmp);

    for(int i = 0 ; i < number_of_query ; i++){
        printf("%d\n", answer[i].number);
    }
}

//最後依照query順序排序
