#include <cstdio>
#include <unordered_set>
using namespace std;

const short OFFSET[4][3] = {(-1, 0, -4), (1, 0, 4), (0, 1, 1), (0, -1, -1)};

struct poi
{
    short x;
    short y;
    poi *operator=(const poi &a)
    {
        x = a.x;
        y = a.y;
        return this;
    }
};

unordered_set<unsigned int> wHash, bHash;
int sum[2];

void prt(short *a)
{
    for (int i = 0; i < 16; i++)
    {
        printf("%d ", a[i]);
        if (!((i + 1) % 4))
            printf("\n");
    }
    return;
}

void change(unsigned int &num, int position, int in)
{
    switch (in)
    {
    case 1:
        num |= (1 << (position - 1));
        break;
    case 2:
        num |= (1 << position);
        break;
    case 3:
        num |= (1 << (position - 1));
        num |= (1 << position);
        break;
    default:
        break;
    }
    return;
}

unsigned int convert(short *a) //每四位代表一个坐标
{
    unsigned int rt = 0;
    int pWhite = 31, pBlack = 15, x, y;
    for (int i = 0; i < 16; i++)
    {
        x = i / 4;
        y = i % 4;
        // printf("%d (%d,%d) ", *(a + i), x, y);
        if (*(a + i) == 0)
        {
            change(rt, pWhite, x);
            change(rt, pWhite - 2, y);
            pWhite -= 4;
        }
        if (*(a + i) == 1)
        {
            change(rt, pBlack, x);
            change(rt, pBlack - 2, y);
            pBlack -= 4;
        }
        // printf("%0x\n", rt);
    }
    while (pWhite != 15)
    {
        change(rt, pWhite, x);
        change(rt, pWhite - 2, y);
        pWhite -= 4;
    }
    while (pBlack != -1)
    {
        change(rt, pBlack, x);
        change(rt, pBlack - 2, y);
        pBlack -= 4;
    }
    return rt;
}

int move(short *a, bool player, int step)
{
    int key = convert(a);
    if (!player)
    {
        if (wHash.count(key))
            return 0;
        else
            wHash.insert(key);
    }
    else
    {
        if (bHash.count(key))
            return 0;
        else
            bHash.insert(key);
    }
    int x, y;
    for (int i = 0; i < 16; i++)
    {
        x = i / 4;
        y = i % 4;
        if (a[i] == player)
        {
            for (int j = 0; j < 4; j++)
            {
                if (x + OFFSET[j][0] >= 0 && x + OFFSET[j][0] < 4 && y + OFFSET[j][1] >= 0 && y + OFFSET[j][1] < 4 && a[i + OFFSET[j][2]] == -1)
                {
                    a[i] = -1;
                    a[i + OFFSET[j][2]] = player;

                    //判定吃
                    for (int k1 = 0; k1 < 2; k1++) //纵向
                    {
                        if (x + OFFSET[k1][0] >= 0 && x + OFFSET[k1][0] < 4 && a[i + OFFSET[k1][2]] == player)
                        {
                            bool op = false;
                            int opx, opi;
                            for (int m = y; m < 16; m += 4)
                            {
                                if (a[m] == !player)
                                {
                                    opi = m;
                                    opx = m / 4;
                                    op = !op;
                                }
                            }
                            if (op)
                            {
                                bool oop = false;
                                for (int n = 0; n < 2; n++)
                                {
                                    if (opx + OFFSET[n][0] >= 0 && opx + OFFSET[n][0] < 4 && a[opi + OFFSET[n][2]] == player)
                                    {
                                        oop = true;
                                    }
                                }
                                if (oop)
                                {
                                    a[opi] = -1;
                                    sum[player]++;
                                }
                            }
                        }
                    }
                    for (int k2 = 2; k2 < 4; k2++) //横向
                    {
                        if (y + OFFSET[k2][1] >= 0 && y + OFFSET[k2][1] < 4 && a[i + OFFSET[k2][2]] == player)
                        {
                            bool op = false;
                            int opy, opi;
                            for (int m = x * 4; m < x * 4 + 4; m++)
                            {
                                if (a[m] == !player)
                                {
                                    opi = m;
                                    opy = m % 4;
                                    op = !op;
                                }
                            }
                            if (op)
                            {
                                bool oop = false;
                                for (int n = 2; n < 4; n++)
                                {
                                    if (opy + OFFSET[n][0] >= 0 && opy + OFFSET[n][0] < 4 && a[opi + OFFSET[n][2]] == player)
                                    {
                                        oop = true;
                                    }
                                }
                                if (oop)
                                {
                                    a[opi] = -1;
                                    sum[player]++;
                                }
                            }
                        }
                    }
                    move(a,!player,step+1);
                    
                    a[i] = player;
                    a[i + OFFSET[j][2]] = -1;
                }
            }
        }
    }
    return 0;
}

int main()
{
    int sum = 0;
    unsigned int key;
    short board[16] = {0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, 1, 1, 1, 1}; //白0黑1
    short *p = board;
    key = convert(p);
    sum = move(p, 0, 1);

    return 0;
}