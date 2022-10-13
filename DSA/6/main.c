//Adjency list reference from: https://www.programiz.com/dsa/graph-adjacency-list
//https://www.programiz.com/dsa/graph-bfs
#include<stdio.h>
#include<stdlib.h>
int N, Q, S, R, a, b, c;
//1. S to R path write first
//2. 
struct node {
	int vertex;
	struct node* next;
};
struct node* createNode(int);

struct Graph {
	int numVertices;
	struct node** adjLists;
    int* visited;
};

// Create a node
struct node* createNode(int v) {
	struct node* newNode = malloc(sizeof(struct node));
	newNode->vertex = v;
	newNode->next = NULL;
	return newNode;
}

// Create a graph
struct Graph* createAGraph(int vertices) {
	struct Graph* graph = malloc(sizeof(struct Graph));
	graph->numVertices = vertices;

  	graph->adjLists = malloc(vertices * sizeof(struct node*));
    graph->visited = malloc(vertices * sizeof(int));

  	int i;
  	for (i = 0; i < vertices; i++)
    	graph->adjLists[i] = NULL, graph->visited[i]=0;

  	return graph;
}

// Add edge
void addEdge(struct Graph* graph, int s, int d) {
  // Add edge from s to d
  	struct node* newNode = createNode(d);
  	newNode->next = graph->adjLists[s];
  	graph->adjLists[s] = newNode;

  // Add edge from d to s
  	newNode = createNode(s);
  	newNode->next = graph->adjLists[d];
  	graph->adjLists[d] = newNode;
}

// Print the graph
void printGraph(struct Graph* graph) {
  	int v;
  	for (v = 0; v < graph->numVertices; v++) {
    	struct node* temp = graph->adjLists[v];
    	printf("\n Vertex %d\n: ", v);
    	while (temp) {
      	printf("%d -> ", temp->vertex);
      	temp = temp->next;
    	}
    	printf("\n");
  	}
}

struct queue {
    int items[1000010];
    int front;
    int rear;
};

struct queue* createQueue(){
    struct queue* q = malloc(sizeof(struct queue));
    q->front = -1;
    q->rear = -1;
    return q;
}
int isEmpty(struct queue* q) {
    if (q->rear == -1)
        return 1;
    else
        return 0;
}
void enqueue(struct queue* q, int value) {
    if (q->front == -1)
        q->front = 0;
    q->rear++;
    q->items[q->rear] = value;
}
int dequeue(struct queue* q){
    int item;
    if (isEmpty(q)) {
        item = -1;
    }else{
        item = q->items[q->front];
        q->front++;
        if (q->front > q->rear) {
            q->front = q->rear = -1;
        }
    }
    return item;
}

void printQueue(struct queue* q) {
    int i = q->front;

    if (isEmpty(q)) {
    }else{
        printf("\nQueue contains \n");
        for (i = q->front; i < q->rear + 1; i++) {
            printf("%d ", q->items[i]);
        }
    }
}

int pi[1000010];
int path[1000010]; //path from S to R
int dis_to_S[1000010];
int order[1000010];
int start = 0;
void bfs(struct Graph* graph, int startVertex) {

    for(int i = 1 ; i <= N ; i++){
        dis_to_S[i] = 0;
    }

    struct queue* q = createQueue();
    dis_to_S[startVertex] = 0;
    graph->visited[startVertex] = 1;
    enqueue(q, startVertex);

    order[start] = startVertex;

    while (!isEmpty(q)){
        //printQueue(q);
        int currentVertex = dequeue(q);
        //printf("Visited %d\n", currentVertex);
        start++;
        order[start] = currentVertex;
        struct node* temp = graph->adjLists[currentVertex];

        while (temp){
            int adjVertex = temp->vertex;

            if(graph->visited[adjVertex] == 0) {
                graph->visited[adjVertex] = 1;
                pi[adjVertex] = currentVertex;
                dis_to_S[adjVertex] = dis_to_S[currentVertex] + 1;
                enqueue(q, adjVertex);
            }
            temp = temp->next;
        }
    }
}

int distance = 0;
void pathfunction(){
    int current = R;
    while(current != S){
        path[distance] = current;
        distance++;
        current = pi[current];
    }
    path[distance] = S;
}

int anschart[1000010];

void constructanschart(struct Graph* graph){
    for(int i = 1 ; i <= N ; i++){
        anschart[i] = 0;
    }
    for(int i = 0 ; i <= distance ; i++){
        anschart[path[i]] = path[i];
    }

    for(int i = 0 ; i <= start ; i++){
        int vertex = order[i];
        if(anschart[vertex] == 0){
            anschart[vertex] = anschart[pi[vertex]];
        }
    }
}



int main(){
	scanf("%d%d%d%d",&N,&Q,&S,&R);
	struct Graph* graph = createAGraph(N + 5);
	for(int i = 1 ; i <= N - 1 ; i++){
		scanf("%d%d",&a,&b);
		addEdge(graph,a,b);
	}
    
    //printGraph(graph);
    bfs(graph, S);
    pathfunction();
    constructanschart(graph);
	for(int i = 1 ; i <= Q ; i++){
		scanf("%d",&c);
		printf("%d\n",anschart[c]);
	} 
}
