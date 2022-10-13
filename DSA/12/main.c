#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct disjointSet{
    // TODO: Determine fields to use by your method
    int parent;
} DisjointSet;

DisjointSet ds[1 << 24];
int size[1 << 24];
bool set[1 << 24] = {};

int c2i(char c) { 
    if ('0' <= c && c <= '9') return c - '0';
    else if ('a' <= c && c <= 'z') return c - 'a' + 10;
    else if ('A' <= c && c <= 'Z') return c - 'A' + 36;
    return -1;
}

int hash(const char* s) {  
    int ret = 0;
    int mask = (1<<24)-1;
    int len = strlen(s);
    for (int i = 0; i < len; i++)
        ret = mask & (ret << 4) | c2i(s[i]);
    return ret;
}

void makeset(const char* s){
    // TODO: Initialize a set with hash value
    int i=hash(s);
    DisjointSet a;
    a.parent = i;
    ds[i] = a;
    size[i] = 1;
}

inline void static init(const char* s) {
    int i = hash(s);
    if (!set[i]) {
        makeset(s);
        set[i] = 1;
    }
}

int find_set(int i) {
    // TODO: Implement your find algorithm here
    if(i == ds[i].parent) return i;

    return ds[i].parent = find_set(ds[i].parent);
}

void group(const char *ra, const char *rb) {
    int a = find_set(hash(ra)), b = find_set(hash(rb));
    // TODO: Implement your union algorithm here
    if(a != b){
        if(size[a] > size[b]){
            ds[b].parent = a;
            size[a] += size[b];
        }else{
            ds[a].parent = b;
            size[b] += size[a];
        }
    }
}

bool same_set(const char*a, const char* b) {
    // TODO: Implement your algorithm here
    int first=find_set(hash(a)), second=find_set(hash(b));
    if(first==second) return true;
    else return false;
}

int main() {
    // TODO: Implement your algorithm here
    int n;
    char person1[20], person2[20], oper[10];
    scanf("%d",&n);
    for(int i = 1 ; i <= n ; i++){
        scanf("%s",oper);
        scanf("%s%s",person1, person2);
        if(oper[0]=='g'){ //group
            init(person1),init(person2);
            group(person1,person2);
        }else{ //test
            init(person1),init(person2);
            if(strcmp(person1, person2) == 0){
                printf("Positive\n");
            }
            else{
                if(same_set(person1,person2)==true){
                    printf("Positive\n");
                }else{
                    printf("Negative\n");
                }
            }
        }
    }
}
