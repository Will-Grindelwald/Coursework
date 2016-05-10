
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef enum fill_type{ a = 0, b, c, d }fill_type;

char **checkerboard;
int che_l, m, n;

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

void fill(int r_low, int c_low, fill_type type)
{
	int array_i;
	for (array_i = 0; array_i < 4; ++array_i)
	{
		if (array_i == type) continue;
		checkerboard[r_low + ((array_i & 2) >> 1)][c_low + (array_i & 1)] = type + '1';
	}
	//print_che();
}

void blank(int r_low, int r_high, int c_low, int c_high, int m, int n)
{
	int r_mid = (r_low + r_high) / 2, c_mid = (c_low + c_high) / 2
		, half_len = r_mid - r_low + 1;
	int array_i;
	fill_type type;
	if (r_low == r_high) return; //µ›πÈ≥ˆø⁄£¨’“µΩƒø±ÍøÈ 
	if (m > r_mid) //µ›πÈ
		if (n > c_mid) type = 3; else type = 2;
	else
		if (n > c_mid) type = 1; else type = 0;
	fill(r_mid, c_mid, type);
	for (array_i = 0; array_i < 4; ++array_i)
	{
		if (array_i == type) blank(r_low + ((array_i & 2) >> 1)*half_len
			, r_mid + ((array_i & 2) >> 1)*half_len
			, c_low + (array_i & 1)*half_len
			, c_mid + (array_i & 1)*half_len
			, m, n);
		else blank(r_low + ((array_i & 2) >> 1)*half_len
			, r_mid + ((array_i & 2) >> 1)*half_len
			, c_low + (array_i & 1)*half_len
			, c_mid + (array_i & 1)*half_len
			, r_mid + ((array_i & 2) >> 1), c_mid + (array_i & 1));
	}
}

int main()
{
	int array_i, array_j, k;
	printf("input k,m,n\n");
	scanf("%d,%d,%d", &k, &m, &n);
	che_l = (int)pow(2, k);
	m--; n--;
	checkerboard = (char**)malloc(che_l*sizeof(char*));
	for (array_i = 0; array_i < che_l; ++array_i)
		*(checkerboard + array_i) = (char*)malloc(che_l*sizeof(char));
	//for (array_i = 0; array_i < che_l; ++array_i)
	//	for (array_j = 0; array_j < che_l; ++array_j)
	//		checkerboard[array_i][array_j] = '0';//«Â¡„
	checkerboard[m][n] = '0';
	
	blank(0, che_l - 1, 0, che_l - 1, m, n); //ÃÓø’
	print_che(); //¥Ú”°∆Â≈Ã 
	
	for (array_i = 0; array_i < che_l; ++array_i)
		free(checkerboard[array_i]);
	free(checkerboard);
	return 0;
}
