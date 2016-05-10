
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define N 1000

char num[N];
int arr_len, del_num;

void deletenum()
{
	char min;
	int slip_window = del_num + 1, current_front = 0, current_back = 0, max_index, i;
	while (num[current_front] != 0)
	{
		min = '9' + 1;
		for (i = 0; i < slip_window; ++i)
			if (num[current_front + i] < min)
			{
				min = num[current_front + i];
				max_index = current_front + i;
			}
		num[current_back++] = min;
		slip_window -= max_index - current_front;
		current_front = max_index + 1;
		num[current_back] = 0;
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
