
//�����ܣ�û�кõ�����ʽ��Ϣ���޽纯����ֻ��Լ���������޽磬��ö�ٺò�����ȥ 
//�򿪵���1�����Կ���������ʽ��Ϣ����һ�����������򣬸���û�л��� 

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define M 8 
#define N 8 

struct move_node
{
	char dir;               //����
	char move_list_size;    //����չ�ڵ����
};
struct path_node
{
	char m, n, number
		, move_list_index //���ڵ���һ������չ�ڵ���move_list[]�е��±�, 0~move_list_size-1
		, move_list_size; //���ڵ����չ�ڵ�����
	struct move_node move_list[8];   //���ڵ����չ�ڵ��(�������չ�ڵ�����������)
}travel_path[M * N]; //ͬʱ��ջ������
char checkerboard[M][N];
int dir[2][8]= {{-2, -2, -1, 1, 2,  2,  1, -1}, {-1,  1,  2, 2, 1, -1, -2, -2}};

void print_path()
{
	char i;
	printf("travel_path:\n");
	for(i = 0; i < M * N; ++i)
		printf("%d\t", travel_path[i].number);
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
	return ((struct move_node*)a)->move_list_size - ((struct move_node*)b)->move_list_size;
}

int work(int m, int n)
{
	char i, j, step = 0; //stepָ��ջ����һ��,��Ҫ���ߵ��Ǹ�
	//int count = 0; //����1 
	struct path_node current = {m, n, m * M + n + 1, }, tmp; //����Ϊ��
	goto L;
	
	while(step != M * N && step)
	{
		//printf("count = %d\n", ++count); //����1 
		if(travel_path[step - 1].move_list_index == travel_path[step - 1].move_list_size)
		{
			travel_path[--step].number = 0;
			checkerboard[travel_path[step].m][travel_path[step].n] = -1;
			//printf("��%d������\n", step); print_che(); print_path(); //����1 
			//fflush(stdin); getchar();
			continue;
		}
		current.m = travel_path[step - 1].m + dir[0][travel_path[step - 1].move_list[travel_path[step - 1].move_list_index].dir];
		current.n = travel_path[step - 1].n + dir[1][travel_path[step - 1].move_list[travel_path[step - 1].move_list_index].dir];
L:		current.number = current.m * M + current.n + 1;
		current.move_list_index = current.move_list_size = 0;
		memset(current.move_list, 0, sizeof(current.move_list));
		++travel_path[step - 1].move_list_index;
		
		for(i = 0; i < 8; ++i)
		{
			tmp.m = current.m + dir[0][i];
			tmp.n = current.n + dir[1][i];
			if(tmp.m >= 0 && tmp.m < M && tmp.n >= 0 && tmp.n < N //�޽�1 - Ŀ�����
				&& checkerboard[tmp.m][tmp.n] == -1)              //�޽�2 - δ�߹�
			{
				//����һ����������ͳ�� 
				current.move_list[current.move_list_size].dir = i;
				current.move_list[current.move_list_size].move_list_size = 0;
				for(j = 0; j < 8; ++j)
					if(tmp.m + dir[0][j] >= 0 && tmp.m + dir[0][j] < M 
						&& tmp.n + dir[1][j] >= 0 && tmp.n + dir[1][j] < N 
						&& checkerboard[tmp.m + dir[0][j]][tmp.n + dir[1][j]] == -1)
						++current.move_list[current.move_list_size].move_list_size;
				++current.move_list_size;
			}
		}
		//����һ�������������� 
		qsort(current.move_list, current.move_list_size, sizeof(struct move_node), cmp);
		
		checkerboard[current.m][current.n] = step;
		travel_path[step++] = current;
		//printf("�߳���%d��\n", step - 1); print_che(); print_path(); //����1 
		//fflush(stdin); getchar();
	}
	return step;
}

int main()
{
	int m, n;
START:
	memset(*checkerboard, -1, M * N); //-1Ϊδ����
	printf("please input index of travel start point(m, n) 1 <= m <= %d, 1 <= n <= %d\n", M, N);
	fflush(stdin), scanf("%d, %d", &m, &n);
	--m; --n;
	if(m < 0 || m >= M||n < 0 || n >= N) { printf("input error\n"); goto START; }
	//freopen("result.txt", "w", stdout);

	if(!work(m, n)) printf("δ��ý�\n");
	print_che();
	print_path();
	goto START;
	system("pause");
	return 0;
}
