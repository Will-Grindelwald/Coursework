
#include <iostream>
#include <vector>
#include <memory.h>
#include <algorithm>

using namespace std;

struct point
{
    int x;
    int y;
    int totaltonext;
}; 
int board[10][10];
int dx[8]= {-2, -2, -1, 1, 2,  2,  1, -1};
int dy[8]= {-1,  1,  2, 2, 1, -1, -2, -2};
int path[100];

bool cmp(point a, point b)
{
    return a.totaltonext < b.totaltonext;
}

bool dfs(point p, int current)
{
    point n;
    if(current == 64)
    {
        for(int i = 0; i < 63; i++)
            cout << path[i] << " ";
        cout << path[63] << endl;
        return true;
    }
    vector<point> tmp;
    for(int i = 0; i < 8; i++)
    {
        n.x = p.x + dx[i];
        n.y = p.y + dy[i];
        if((n.x >= 1 && n.x <= 8 && n.y >= 1 && n.y <= 8 && !board[n.x][n.y]))
        {
            point k;
            n.totaltonext = 0;
			for(int j = 0; j < 8; j++)
            {
                k.x = n.x + dx[j];
                k.y = n.y + dy[j];
                if(k.x >= 1 && k.x <= 8 && k.y >= 1 && k.y <= 8 && !board[k.x][k.y])
                    n.totaltonext++;
            }
            tmp.push_back(n);
        }
    }
    sort(tmp.begin(), tmp.end(), cmp);
    for(int i = 0; i < tmp.size(); i++)
    {
        board[tmp[i].x][tmp[i].y] = 1;
        path[current] = (tmp[i].x - 1) * 8 + tmp[i].y;
        if(dfs(tmp[i], current+1)) return true;
        board[tmp[i].x][tmp[i].y] = 0;
    }
    return false;
}

int main()
{
    int n;
    point p;
    while(cin >> n && n != -1)
    {
        memset(board, 0, 10*10*sizeof(int));
        p.x = (n - 1) / 8 + 1;
        p.y = n - ((p.x-1) * 8);
        path[0] = n;
        board[p.x][p.y] = 1;
        dfs(p, 1);
    }
    return 0;
}
