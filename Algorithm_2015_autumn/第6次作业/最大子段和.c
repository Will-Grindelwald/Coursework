
#include <stdio.h> 
#include <stdlib.h>

int a[100], len, max, max_index;

void max_sub()
{
	int b = 0, i;
    for (i = 0; i < len; ++i)
        if((b = b > 0 ? b + a[i]: a[i]) > max) //b[j] = max{b[j-1] + a[j], a[j]} 
        {
        	max = b;
        	max_index = i;
        }
}

void print_sub()
{
	int i = 0, sum = 0;
	while(sum != max)
		sum += a[max_index - i++];
	printf("����Ӷκ�Ϊ: %d\n�Ӷ�Ϊ:", max);
	for(i = max_index - i + 1; i <= max_index; ++i)
	{
		printf("%d  ", a[i]);
	}
	puts("");
}

void init()
{
	printf("����������(�ո����,Ctrl+z����):"); 
	//freopen("sr.txt", "r", stdin); 
	while(scanf("%d", a + len++) != EOF); 
}

int main()
{
	init();
	max_sub();
	print_sub();
	return 0;
}
