#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define ll long long
//change to long long?
typedef struct Node{
    ll number;
    int id;
}node;

int cmp(const void *a, const void *b){
    node *a_1 = (node *)a;
    node *b_1 = (node *)b;
    if(a_1->number < b_1->number) return -1;
    else if(a_1->number > b_1->number) return 1;
    else return 0;
}

int k, l, flag;
int index1, index2, find_similar = 0;
ll same = 0, similar_pair = 0;
ll big_prime = 237967551247;
ll power103[1000005], power97[1000005];
node hash1[1000005], hash2[1000005], newhash1[1000005], newhash2[1000005], extra[1000005];
char **str;

void ask_for_memory(){
    str = malloc((k + 10) * sizeof(char *));
    for(int i = 0 ; i < k + 10; i++){
        str[i] = malloc((l + 10) * sizeof(char));
    }

    power103[0] = 1;
    for(int i = 1 ; i <= l ; i++){
        power103[i] = (power103[i - 1] * 103) % big_prime;
    }
    power97[0] = 1;
    for(int i = 1 ; i <= l ; i++){
        power97[i] = (power97[i - 1] * 97) % big_prime;
    }
}

void cal_hash(){
    for(int i = 0 ; i < k ; i++){
        ll h = 0;
        for(int j = 0 ; j < l ; j++){
            h = ((h * 103) % big_prime + str[i][j] - '!' + 1) % big_prime;
        }
        node a;
        a.number = h, a.id = i, hash1[i] = a; 
    }

    for(int i = 0 ; i < k ; i++){
        ll h = 0;
        for(int j = 0 ; j < l ; j++){
            h = ((h * 97) % big_prime + str[i][j] - '!' + 1) % big_prime;
        }
        node a;
        a.number = h, a.id = i, hash2[i] = a;
    }

    for(int i = 0 ; i < k ; i++){
        extra[i] = hash1[i];
    }
    qsort(extra, k, sizeof(node), cmp);

    for(int i = 0 ; i < k - 1 ; i++){
        if(extra[i].number == extra[i + 1].number && hash2[extra[i].id].number == hash2[extra[i + 1].id].number){
            int diff = 0;
            for(int o = 0 ; o < l ; o++){
                if(str[extra[i].id][o] != str[extra[i + 1].id][o]) diff++;
            }
            if(diff == 0){
                find_similar = 1;
                index1 = extra[i].id, index2 = extra[i + 1].id;
                break;
            }
        }
    }

    ll cnt = 0;

    for(int i = 0 ; i < k - 1 ; i++){
        if(extra[i].number == extra[i + 1].number){
            cnt++;
        }else{
            same += cnt * (cnt + 1) / 2;
            cnt = 0;
        }
    }
    same += cnt * (cnt + 1) / 2;
}

void output_ans(){
    for(int j = 0 ; j < l ; j++){
        if(flag == 0 && find_similar == 1) break;

        for(int i = 0 ; i < k ; i++){
            ll h = 0;
            h = (hash1[i].number + big_prime - ((str[i][j] - '!' + 1) * power103[l - 1 - j]) % big_prime) % big_prime;
            node a; //becomes negative
            a.number = h, a.id = i, newhash1[i] = a;
        }

        for(int i = 0 ; i < k ; i++){
            ll h = 0;
            h = (hash2[i].number + big_prime - ((str[i][j] - '!' + 1) * power97[l - 1 - j]) % big_prime) % big_prime;
            node a;
            a.number = h, a.id = i, newhash2[i] = a;
        }

        // printf("before sort--------------\n");
        // for(int i = 0 ; i < k ; i++){
        //     printf("number = %lld, id = %d\n", newhash1[i].number, newhash1[i].id);
        // }
        qsort(newhash1, k, sizeof(node), cmp);
        // printf("after sort ---------------\n");
        // for(int i = 0 ; i < k ; i++){
        //     printf("number = %lld, id = %d\n", newhash1[i].number, newhash1[i].id);
        // }

        if(flag == 1){
            ll cnt = 0;
            for(int i = 0 ; i < k - 1 ; i++){
                if(newhash1[i].number == newhash1[i + 1].number && newhash2[newhash1[i].id].number == newhash2[newhash1[i + 1].id].number){
                    cnt++;
                }else{
                    similar_pair += cnt * (cnt + 1) / 2;
                    cnt = 0;
                }
            }
            similar_pair += cnt * (cnt + 1) / 2;

        }else{
            for(int i = 0 ; i < k - 1 ; i++){
                if(newhash1[i].number == newhash1[i + 1].number && newhash2[newhash1[i].id].number == newhash2[newhash1[i + 1].id].number){
                    int diff = 0;
                    for(int o = 0 ; o < l ; o++){
                        if(str[newhash1[i].id][o] != str[newhash1[i + 1].id][o]) diff++;
                    }
                    if(diff <= 1){
                        index1 = newhash1[i].id, index2 = newhash1[i + 1].id;
                        find_similar = 1;
                        break;
                    }
                }
            }
        }
    }

    if(flag == 1){
        if(similar_pair == 0) printf("No\n");
        else{
            printf("Yes\n");
            printf("%lld\n", similar_pair - (l - 1) * same);
        }
    }
    else{
        if(find_similar == 1){
            printf("Yes\n");
            printf("%d %d\n", index1, index2);
        }else{
            printf("No\n");
        }
    }
}

int main(){
    scanf("%d%d%d", &k, &l, &flag);
    ask_for_memory();
    for(int i = 0 ; i < k ; i++){
        scanf("%s", str[i]);
    }

    cal_hash();
    output_ans();
}
