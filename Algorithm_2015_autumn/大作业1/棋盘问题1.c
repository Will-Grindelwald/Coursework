
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef enum fill_type{ a = 0, b, c, d }fill_type;

char **checkerboard;
int che_l, m, n;
int count_fill[4];
//char graphics[4][4] = { { 0, 186, 205, 188 }, { 186, 0, 200, 205 }
//, { 205, 187, 0, 186 }, { 201, 205, 186, 0 } };

void print_che() //¥Ú”°∆Â≈Ã 
{
	int array_i, array_j;
	for (array_i = 0; array_i < che_l; ++array_i)
	{
		printf("\n\n\t");
		for (array_j = 0; array_j < che_l; ++array_j)
			printf("%c  ", checkerboard[array_i][array_j]);
	} // ‰≥ˆ
	puts("\n\n");
}

void fill_blank(int r_low, int r_high, int c_low, int c_high, fill_type type)
{
	int r_mid = (r_low + r_high) / 2, c_mid = (c_low + c_high) / 2
		, r_qtr1 = (r_low + r_mid) / 2, r_qtr3 = (r_mid + 1 + r_high) / 2
		, c_qtr1 = (c_low + c_mid) / 2, c_qtr3 = (c_mid + 1 + c_high) / 2
		, half_len = r_mid - r_low + 1, i;
	if (r_mid == r_low) //ÃÓ≥‰ 
	{
		++count_fill[type];
		for (i = 0; i < 4; ++i)
		{
			if (i == type) continue;
			//checkerboard[r_low + ((i & 2) >> 1)][c_low + (i & 1)] = graphics[type][i];
			checkerboard[r_low + ((i & 2) >> 1)][c_low + (i & 1)] = type + '1';
		}
		//print_che();
	}
	else //µ›πÈ 
	{
		fill_blank(r_qtr1 + 1, r_qtr3, c_qtr1 + 1, c_qtr3, type);
		for (i = 0; i < 4; ++i)
		{
			if (i == type) continue;
			fill_blank(r_low + ((i & 2) >> 1)*half_len, r_mid + ((i & 2) >> 1)*half_len
				, c_low + (i & 1)*half_len, c_mid + (i & 1)*half_len, 3 - i);
		}
	}
}

void find_blank(int r_low, int r_high, int c_low, int c_high)
{
	int r_mid = (r_low + r_high) / 2, c_mid = (c_low + c_high) / 2;
	fill_type type;
	if (r_low == r_high) //µ›πÈ≥ˆø⁄£¨’“µΩƒø±ÍøÈ 
		if (c_low != c_high || r_low != m || c_low != n)
		{
			printf("error"); system("pause"); exit(0);
		}
		else
			return;
	if (m > r_mid) //µ›πÈ
		if (n > c_mid) //3
		{
			find_blank(r_mid + 1, r_high, c_mid + 1, c_high);
			type = 3;
		}
		else //2
		{
			find_blank(r_mid + 1, r_high, c_low, c_mid);
			type = 2;
		}
	else
		if (n > c_mid) //1
		{
			find_blank(r_low, r_mid, c_mid + 1, c_high);
			type = 1;
		}
		else //0
		{
			find_blank(r_low, r_mid, c_low, c_mid);
			type = 0;
		}
	fill_blank(r_low, r_high, c_low, c_high, type); //µ›πÈ∑µªÿ ±ÃÓ≥‰∆Â≈Ã
}

int main()
{
	int array_i, array_j, k;

	printf("input k,m,n\n");
	scanf("%d,%d,%d", &k, &m, &n);
	che_l = (int)pow(2, k);
	//printf("k=%d,m=%d,n=%d,che_l=%d\n", k, m, n, che_l);
	m--; n--;

	checkerboard = (char**)malloc(che_l*sizeof(char*));
	for (array_i = 0; array_i < che_l; ++array_i)
		*(checkerboard + array_i) = (char*)malloc(che_l*sizeof(char));

	//for (array_i = 0; array_i < che_l; ++array_i)
	//	for (array_j = 0; array_j < che_l; ++array_j)
	//		checkerboard[array_i][array_j] = '0';//«Â¡„
	checkerboard[m][n] = '0';

	//puts("start");
	find_blank(0, che_l - 1, 0, che_l - 1); //ÃÓø’
	//puts("over");

	print_che(); //¥Ú”°∆Â≈Ã 

	for (array_i = 0; array_i < che_l; ++array_i)
		free(checkerboard[array_i]);
	free(checkerboard);
	system("pause");
	return 0;
}
