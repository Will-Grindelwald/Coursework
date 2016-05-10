
//不知道什么问题，反正有问题 

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <setjmp.h>

#define M 8
#define N 8

struct point
{
	char m, n, number, next;
};
char travel_path[M * N];
char checkerboard[M][N];
char dir[2][8]= {{-2, -2, -1, 1, 2,  2,  1, -1}, {-1,  1,  2, 2, 1, -1, -2, -2}};
jmp_buf env_buf;

void print_path()
{
	char i;
	printf("travel_path:\n");
	for(i = 0; i < M * N; ++i)
		printf("%d\t", travel_path[i]);
	puts("");
}

void print_che()
{
	char i, j;
	for(i = 0; i < M; ++i)
	{
		for(j = 0; j < N; ++j)
			printf("%d\t", checkerboard[i][j]);
		puts("\n");
	}
}

int cmp(const void* a, const void* b)
{
	return ((struct point *)a)->next - ((struct point *)b)->next;
}

int work(int m, int n, int step)
{
	char i, j, size = 0;
	struct point tmp, current_next[8];
	if(step == M * N) longjmp(env_buf, 1);
	printf("%d\n", step);
	for(i = 0; i < 8; ++i)
	{
		tmp.m = m + dir[0][i];
		tmp.n = n + dir[1][i];
		tmp.number = tmp.m * M + tmp.n + 1;
		if(tmp.m >= 0 && tmp.m < M && tmp.n >= 0 && tmp.n < N //限界1 - 目标存在 
			&& checkerboard[tmp.m][tmp.n] == -1) //限界2 - 未走过 
		{
			for(tmp.next = 0, j = 0; j < 8; ++j)
                if(tmp.m + dir[0][i] >= 0 && tmp.m + dir[0][i] < M 
					&& tmp.n + dir[1][i] >= 0 && tmp.n + dir[1][i] < N 
					&& checkerboard[tmp.m + dir[0][j]][tmp.n + dir[1][j]] == -1)
					++tmp.next;
			current_next[size++] = tmp;
		}
	}
	qsort(current_next, size, sizeof(struct point), cmp);
	for(j = 0; j < size; ++j)
	{
		checkerboard[current_next[j].m][current_next[j].n] = step;
		travel_path[step] = current_next[j].number;
		work(current_next[j].m, current_next[j].n, step + 1);
		checkerboard[current_next[j].m][current_next[j].n] = -1;
	}
	return 0;
}

int main()
{
	int m, n, i, res = 1;
START:
	memset(*checkerboard, -1, M * N); //-1为未经过
	printf("please input index of travel start point(m, n) 1 <= m <= %d, 1 <= n <= %d\n", M, N);
	fflush(stdin), scanf("%d, %d", &m, &n);
	--m; --n;
	if(m < 0 || m >= M||n < 0 || n >= N) { printf("input error\n"); goto START; }
	
	checkerboard[m][n] = 0;
	travel_path[0] = m * M + n + 1;
	if(!setjmp(env_buf)) res = work(m, n, 1);
	if(res == 1) printf("success\n");
	else printf("fail\n");
	print_che();
	print_path();
	goto START; 
	system("pause");
	return 0;
}
