
#include <stdio.h>
#include <stdlib.h>

int main()
{
	int *num,size,i,j,count=0;
	printf("�������ָ���:");
	scanf("%d",&size);
	num=(int *)malloc(size*sizeof(int));
	for(i=0;i<size;++i)
		scanf("%d",num+i);
	for(i=0;i<size-1;++i)
		for(j=i+1;j<size;++j)
			if(num[i]>num[j]) ++count;
	printf("������е�������Ϊ%d\n",count);
	free(num);
	return 0;
}
