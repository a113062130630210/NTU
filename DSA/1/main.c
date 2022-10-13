#include <stdio.h>
#include <string.h>
#include <stdbool.h>
int num1[260], num2[260], num1_tmp[260], num2_tmp[260];
int length1 = 0, length2 = 0;
int ans = 1, count = 0, ansis[260], length_ansis = 0;
int comparator(int *biginteger1, int *biginteger2) // between two big integer
// return1 :biginteger1>biginteger2
// return2 :biginteger2>biginteger1
{
    if (length1 < length2)
        return 2;
    else if (length1 > length2)
        return 1;
    else
    {
        for (int i = length1 - 1; i >= 0; i--)
        {
            if (biginteger1[i] > biginteger2[i])
                return 1;
            else if (biginteger2[i] > biginteger1[i])
                return 2;
        }
        return 1;
    }
}

void substraction(int biginteger1[260], int biginteger2[260], int result[260]) // substraction between two big integer
{
    for (int i = 0; i < 260; i++)
        result[i] = biginteger1[i] - biginteger2[i];
    for (int i = 0; i < 259; i++)
    {
        if (result[i] < 0)
            result[i + 1]--, result[i] += 10;
    }
}

int callength(int *a)
{
    int ans;
    for (int i = 0; i < 260; i++)
    {
        if (a[i] != 0)
            ans = i + 1;
    }

    return ans;
}

void division(int biginteger[260], int length)
{
    int copy[260], borrow = 0;
    for (int i = 0; i < length; i++)
        copy[i] = biginteger[i];
    for (int i = length - 1; i >= 0; i--)
    {
        if (borrow == 0)
        {
            if (copy[i] % 2 == 0)
                biginteger[i] = copy[i] / 2;
            else
            {
                biginteger[i] = copy[i] / 2;
                borrow = 1;
            }
        }
        else
        {
            if (copy[i] % 2 == 0)
            {
                biginteger[i] = (copy[i] + 10) / 2;
                borrow = 0;
            }
            else
            {
                biginteger[i] = (copy[i] + 10) / 2;
                borrow = 1;
            }
        }
    }
}

void multiple(int biginteger[260], int k, int result[260])
{
    if (k == 0)
    {
        for (int i = 0; i < 260; i++)
        {
            result[i] = biginteger[i];
        }
    }
    else
    {
        for (int i = 0; i < 260; i++)
        {
            for (int j = 1; j <= k; j++)
            {
                if (j == 1)
                    result[i] = biginteger[i] * 2;
                else
                    result[i] = result[i] * 2;
            }
        }

        for (int i = 0; i < 260 - 1; i++)
        {
            result[i + 1] += result[i] / 10;
            result[i] %= 10;
        }
    }
}

bool iszero(int *biginteger)
{
    for (int i = 0; i < 260; i++)
    {
        if (biginteger[i] != 0)
            return false;
    }
    return true;
}

void swap(int *a, int *b, int n) // n means the max of two length arrays
{
    for (int i = 0; i < n; i++)
    {
        int tmp = a[i];
        a[i] = b[i], b[i] = tmp;
    }
    int temp = length1;
    length1 = length2, length2 = temp;
}

void test()
{
    printf("--------test--------\n");
    for (int i = length1 - 1; i >= 0; i--)
        printf("%d", num1[i]);
    printf("\n");
    for (int i = length2 - 1; i >= 0; i--)
        printf("%d", num2[i]);
    printf("\n");
}

void readinput()
{
    char c;
    int flag = 0;
    while (scanf("%c", &c) != EOF)
    {
        if (c == '\n')
            break;
        else if (c == ' ')
        {
            if (flag == 0)
                flag = 1;
            else
                break;
        }
        else
        {
            if (flag == 0)
                num1_tmp[length1] = c - '0', length1++;
            else
                num2_tmp[length2] = c - '0', length2++;
        }
    }
    for (int i = length1 - 1; i >= 0; i--)
    {
        num1[i] = num1_tmp[length1 - 1 - i];
    }
    for (int i = length2 - 1; i >= 0; i--)
    {
        num2[i] = num2_tmp[length2 - 1 - i];
    }
}

int main()
{
    readinput();

    // printf("test1\n");
    // printf("length of first number is %d ", length1);
    // for (int i = length1 - 1; i >= 0; i--)
    // {
    //     printf("%d", num1[i]);
    // }
    // printf("\nlength of first number is %d ", length2);

    // for (int i = length2 - 1; i >= 0; i--)
    // {
    //     printf("%d", num2[i]);
    // }
    // printf("\n");

    // printf("test 2\n");
    //   printf("after swap------------\n");
    //   if (comparator(num1, num2) == 2)
    //   {
    //       int length = (length1 > length2) ? length1 : length2;
    //       swap(num1, num2, length);
    //       int tmp_length = length1;
    //       length1 = length2, length2 = tmp_length;
    //   }

    // for (int i = length1 - 1; i >= 0; i--)
    // {
    //     printf("%d", num1[i]);
    // }
    // printf(" ");
    // for (int i = length2 - 1; i >= 0; i--)
    // {
    //     printf("%d", num2[i]);
    // }
    // printf("\n");

    // // test3
    // printf("after substraction---------\n");
    // int result[260], length_result;
    // substraction(num1, num2, result);
    // for (int i = 0; i < 260; i++)
    // {
    //     if (result[i] != 0)
    //         length_result = i + 1;
    // }
    // printf("length after substraction= %d\n", length_result);
    // for (int i = length_result - 1; i >= 0; i--)
    // {
    //     printf("%d", result[i]);
    // }
    // printf("\n");

    // test 4
    // printf("after division------------\n");
    // division(num1, length1);
    // for (int i = 0; i < 260; i++)
    // {
    //     if (num1[i] != 0)
    //         length1 = i + 1;
    // }
    // printf("length after division= %d\n", length1);
    // for (int i = length1 - 1; i >= 0; i--)
    // {
    //     printf("%d", num1[i]);
    // }
    // printf("\n");

    // // test 5
    // int after_multiple[260], length_multiple = 0, k;
    // printf("after division------------\n");
    // printf("What k do you want for multiple\n");
    // scanf("%d", &k);
    // multiple(num1, k, after_multiple);
    // for (int i = 0; i < 260; i++)
    // {
    //     if (after_multiple[i] != 0)
    //         length_multiple = i + 1;
    // }
    // printf("length after multiple= %d\n", length_multiple);
    // for (int i = length_multiple - 1; i >= 0; i--)
    // {
    //     printf("%d", after_multiple[i]);
    // }
    // printf("\n");

    if (comparator(num1, num2) == 1)
    {
        int length = (length1 > length2) ? length1 : length2;
        swap(num1, num2, length);
        // int tmp_length = length1;
        // length1 = length2, length2 = tmp_length;
    }

    // test();
    //  after this n = num1, m = num2 n is smaller m is bigger
    while (iszero(num1) == false && iszero(num2) == false)
    {
        // test();
        bool num1_is_even, num2_is_even;
        if (num1[0] % 2 != 0)
            num1_is_even = false;
        else
            num1_is_even = true;

        if (num2[0] % 2 != 0)
            num2_is_even = false;
        else
            num2_is_even = true;

        // test();
        // printf("\n");
        // if (num1_is_even == true)
        //     printf("num1 is even\n");
        // else
        //     printf("num1 is not even\n");
        // if (num2_is_even == true)
        //     printf("num2 is even\n");
        // else
        //     printf("num2 is not even\n");
        if (num1_is_even == true && num2_is_even == true)
        {
            // printf("enter here\n");
            // test();
            // printf("isthishere\n");
            ans *= 2, count++, division(num1, length1), division(num2, length2);
            length1 = callength(num1);
            length2 = callength(num2);
        }
        else if (num1_is_even == true)
        {
            division(num1, length1);
            length1 = callength(num1);
        }
        else if (num2_is_even == true)
        {
            division(num2, length2);
            length2 = callength(num2);
        }

        // printf("         -----------test after divsion-------------\n");
        // for (int i = length1 - 1; i >= 0; i--)
        //     printf("%d", num1[i]);
        // printf("\n");
        // for (int i = length2 - 1; i >= 0; i--)
        //     printf("%d", num2[i]);
        // printf("\n");

        // printf("length1= %d, length2= %d\n", length1, length2);
        if (comparator(num1, num2) == 1)
        {
            // printf("enter\n");
            int length_another = (length1 > length2) ? length1 : length2;
            swap(num1, num2, length_another);
            // int tmp_length = length1;
            // length1 = length2, length2 = tmp_length;
        }

        // printf("                   -----------test after swap-------------\n");
        // for (int i = length1 - 1; i >= 0; i--)
        //     printf("%d", num1[i]);
        // printf("\n");
        // for (int i = length2 - 1; i >= 0; i--)
        //     printf("%d", num2[i]);
        // printf("\n");

        int result[260];
        substraction(num2, num1, result);
        for (int i = 0; i < length2; i++)
            num2[i] = result[i];
        length2 = callength(num2);
    }
    // printf("----------------num1=---------------\n");
    //  for (int i = length1 - 1; i >= 0; i--)
    //      printf("%d", num1[i]);
    //  printf("\n");

    // printf("count=%d\n", count);

    multiple(num1, count, ansis);
    // printf("count= %d\n", count);

    int length_ansis = callength(ansis);
    // printf("length of ans= %d\n", length_ansis);
    for (int i = length_ansis - 1; i >= 0; i--)
        printf("%d", ansis[i]);
    printf("\n");
}
