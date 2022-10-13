#include<stdio.h>
#include<string.h>
#include<stdlib.h>
char str[20000005];
int len;
int pi1[20000005];
int pi2[20000005];

char ans1[20000005]; //add from start
char ans2[20000005]; //add from end

char copy1[20000005]; //copy for add_front
char copy2[20000005]; //copy for add_end

//char reverse[20000005];

char* revstr(char* str1){
    int i, l;
    l = strlen(str1);
    char *reverse = malloc( sizeof(char) * ( l + 5 ) );
    for(i = 0 ; i < l ; i++){
        reverse[i] = str1[l - 1 - i];
    }
    return reverse;
}

int add_front(){
    strcpy(copy1, str);
    copy1[len] = '\t';
    strcat(copy1, revstr(str));
    // printf("reverse = %s\n", revstr(str));
    // printf("str1 = %s\n", copy1);
    //compute pi1 on copy1
    int length = 2 * len + 1;
    pi1[0] = 0;
    int q = 0;
    int i = 1;
    while(i < length){
        if(copy1[i] == copy1[q]){
            q++;
            pi1[i] = q;
            i++;
        }else{
            if(q != 0){
                q = pi1[q - 1];
            }else{
                pi1[i] = 0;
                i++;
            }
        }
    }
    // printf("-------------\n");
    // for(int i=0;i<length;i++){
    //     printf("%d ",pi1[i]);
    // }
    // printf("\n-------------\n");

    for(int j = 0 ; j < len - pi1[length - 1] ; j++){
        ans1[j] = str[len - 1 - j];
    }
    for(int j = 0; j < len; j++){
        ans1[len - pi1[length - 1] + j] = str[j];
    }

    return len - pi1[length - 1];
}

int add_end(){
    strcpy(copy2, revstr(str));
    copy2[len] = '\t';
    strcat(copy2, str);
    // printf("str2 = %s\n", copy2);
    //compute pi2 on copy2
    int length = 2 * len + 1;
    pi2[0] = 0;
    int q = 0;
    int i = 1;
    while(i < length){
        if(copy2[i] == copy2[q]){
            q++;
            pi2[i] = q;
            i++;
        }else{
            if(q != 0){
                q = pi2[q - 1];
            }else{
                pi2[i] = 0;
                i++;
            }
        }
    }
    // printf("-------------\n");
    // for(int i=0;i<length;i++){
    //     printf("%d ",pi2[i]);
    // }
    // printf("\n-------------\n");

    strcpy(ans2, str);

    for(int j = len + len - pi2[length - 1] - 1; j >= len; j--){
        ans2[j] = str[len + len - pi2[length - 1] - 1 - j];
    }
    return len - pi2[length - 1];
}

int checkpalindrome(){
    int flag = 1;
    for(int i = 0 ; i < len ; i++){
        if(str[i] != str[len - 1 - i]){
            flag = 0;
            break;
        }
    }
    return flag;
}

int main(){    
    scanf("%s",str);
    len = strlen(str);

    if(checkpalindrome() == 1){
        printf("0\n");
        printf("%s", str);
    }
    else{
        int add1 = add_front();
        int add2 = add_end();
        if(add1 < add2){
            printf("%d\n", add1);
            printf("%s", ans1);
        }else if(add2 < add1){
            printf("%d\n", add2);
            printf("%s", ans2);
        }else{ //the same
            printf("%d\n", add1);
            printf("%s\n", ans1);
            printf("%s", ans2);
        }
    }
    // answer can only be added at front or added at end
    // so only answer 2   

}
