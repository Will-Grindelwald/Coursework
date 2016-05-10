
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 0x7fff0000

typedef struct Graph
{
	int **matrix;		//�ڽӾ��� 
	int n;				//������ 
	int e;				//���� 
}Graph;

Graph g;
int source, *dist, *prior;

void Dijkstra()
{
	int i, j;
	int *visited = (int *)calloc(g.n, sizeof(int));
	visited[source] = 1;
	for (i = 0; i < g.n; ++i)		//��ʼ�� 
	{
		dist[i] = g.matrix[source][i];
		prior[i] = dist[i] < MAX ? source : -1;
	}
	prior[source] = -1;

	for (i = 1; i < g.n; ++i)		//ѭ����չn-1�� 
	{
		int min = MAX;
		int u;
		for (j = 0; j < g.n; ++j)	//Ѱ��δ����չ��Ȩֵ��С�Ķ��� 
			if (visited[j] == 0 && dist[j] <= min)
			{
				min = dist[j];
				u = j;
			}
		visited[u] = 1;
		for (j = 0; j < g.n; ++j)	//����dist�����ֵ��·����ֵ 
		{
			if (visited[j] == 0 && min + g.matrix[u][j] < dist[j])
			{
				dist[j] = min + g.matrix[u][j];
				prior[j] = u;
			}
		}
	}
}

void showPath()		//��ӡ���·���ϵĸ������� 
{
	int i, path_i, current;
	char *path = (char *)malloc(g.n * 2 * sizeof(char));
	puts("\nstart		end		length		nodes liet"); 
	for (i = 0; i < g.n; ++i)
		if (i != source)
		{
			memset(path, 0, g.n * 2);
			path_i = 0;
			current = i;
			path[path_i++] = current + 1 + 48;
			while (prior[current] != -1)
			{
				path[path_i++] = prior[current] + 1 + 48;
				current = prior[current];
			}
			printf("1		%d		%d		", i + 1, dist[i]);
			for (--path_i; path_i >= 0; --path_i)
				printf("%c ", path[path_i]);
			puts("\n");
		}
	//for (i = 0; i < g.n; ++i) printf("%d\t", prior[i] + 1);
}

void init()
{
	int i, j;
	int r, c, w;			//��ʾ����һ����s->t,ȨֵΪw
	printf("ͼ�Ķ�����:");
	scanf("%d", &g.n);								//g.n
	printf("ͼ�ı���:");
	dist = (int *)malloc(sizeof(int)*g.n);			//dist
	prior = (int *)malloc(sizeof(int)*g.n);			//prior
	g.matrix = (int **)malloc(sizeof(int *)*g.n);	//g.matrix
	for (i = 0; i < g.n; ++i)
	{
		g.matrix[i] = (int *)malloc(sizeof(int)*g.n);
		for (j = 0; j < g.n; ++j)
		{
			if (i == j) g.matrix[i][j] = 0;
			else g.matrix[i][j] = MAX;
		}
	}
	scanf("%d", &g.e);								//g.e
	printf("����ߵ�Ȩֵ(�������ϰ�����), ��ʽ: r,c,w\n");
	for (i = 0; i < g.e; ++i)
	{
		scanf("%d,%d,%d", &r, &c, &w);
		g.matrix[r - 1][c - 1] = g.matrix[c - 1][r - 1] = w;
	}
	printf("����Դ������:");
	scanf("%d", &source);
	--source;							//source
	/*for (i = 0; i < g.n; ++i)
	{
	putchar('\n');
	for (j = 0; j < g.n; ++j)
	printf("\t%d", g.matrix[i][j]);
	}*/
}

int main()
{
	//freopen("sr.txt", "r", stdin);
	//freopen("sc.txt", "w", stdout);
	init();
	Dijkstra();
	showPath();
	system("pause");
	return 0;
}
