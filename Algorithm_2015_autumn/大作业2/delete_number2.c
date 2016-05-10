
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define N 1000

char num[N];
int arr_len, del_num;

void deletenum()
{
	int i, res_p;
	while (del_num-- != 0)
	{
		for(i = 0; i < arr_len; ++i)
			if (num[i] > num[i+1]) break;
		for(; i < arr_len; ++i) num[i] = num[i+1];
		--arr_len;
		//printf("result: %s\n\n", num);
	}
}

void init()
{
START_LABLE:
	printf("input the number:");
	arr_len = 0;
	memset(num, 0, N);
	
	//freopen("sr.txt", "r", stdin);
	scanf("%s", num);
	//freopen("CON", "r", stdin);
	
	while (num[arr_len] != 0)
		if (!isdigit(num[arr_len++]))
		{
			printf("input error\n");
			goto START_LABLE;
		}
	printf("要删除数字的个数：");
	scanf("%d", &del_num);
	if(del_num > arr_len)
	{
		printf("input error\n");
		goto START_LABLE;
	}
}

int main()
{
	while (1)
	{
		init();
		printf("number: %s\n\n", num);
		deletenum();
		printf("result: %s\n\n", num);
	}
	return 0;
}
