
#include <stdio.h>
#include <stdlib.h>

int main()
{
	int *num,size,i,j,count=0;
	printf("输入数字个数:");
	scanf("%d",&size);
	num=(int *)malloc(size*sizeof(int));
	for(i=0;i<size;++i)
		scanf("%d",num+i);
	for(i=0;i<size-1;++i)
		for(j=i+1;j<size;++j)
			if(num[i]>num[j]) ++count;
	printf("这个序列的逆序数为%d\n",count);
	free(num);
	return 0;
}
