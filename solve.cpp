#include <cstdio>
#include <map>
#include<cmath>
using namespace std;

const short OFFSET[4][3] = {{-1, 0, -4}, {1, 0, 4}, {0, 1, 1}, {0, -1, -1}};
const short INCLINED_OFFSET[4][3]={{-1,-1,-5},{-1,1,-3},{1,-1,3},{1,1,5}};

typedef map<unsigned int, pair<int, int>> chart;
chart wMap, bMap;

template <class Ty1, class Ty2>
pair<Ty1, Ty2> operator+(const pair<Ty1, Ty2> &p1, const pair<Ty1, Ty2> &p2)
{
    pair<Ty1, Ty2> rt;
    rt.first = p1.first + p2.first;
    rt.second = p1.second + p2.second;
    return rt;
}

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

unsigned int convert(const short *a) //每四位代表一个坐标
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

int estimate(const short *a){//估值函数
    int wnum=0,bnum=0,est=0;
    int b[16];//b中 白1黑-1
    for (int i = 0; i < 16; i++)
    {
        b[i]=0;
        if(a[i]==0){
            b[i]=1;
            wnum++;
        }
        if(a[i]==1){
            b[i]=-1;
            bnum++;
        }
    }
    if(wnum==1){
        return -100;
    }else if(bnum==1){
        return 100;
    }

    if(abs(wnum-bnum)==1){
        est=(wnum-bnum)*30;
    }else if(abs(wnum-bnum)==2){
        est=(wnum-bnum)*70;
    }

    for (int i = 0; i < 16; i++)
    {
        
    }
    
}

pair<int, int> move(short *a, bool player, int step)
{

    int x, y, opx, opy, opi1, opi2;
    bool op = false, oop1 = false, oop2 = false;
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
                            op = false;
                            for (int m = y; m < 16; m += 4)
                            {
                                if (a[m] == !player)
                                {
                                    opi1 = m;
                                    opx = m / 4;
                                    op = !op;
                                }
                            }
                            if (op)
                            {
                                for (int n = 0; n < 2; n++)
                                {
                                    if (opx + OFFSET[n][0] >= 0 && opx + OFFSET[n][0] < 4 && a[opi1 + OFFSET[n][2]] == player)
                                    {
                                        oop1 = true;
                                    }
                                }
                                if (oop1)
                                {
                                    a[opi1] = -1;
                                }
                            }
                        }
                    }
                    for (int k2 = 2; k2 < 4; k2++) //横向
                    {
                        if (y + OFFSET[k2][1] >= 0 && y + OFFSET[k2][1] < 4 && a[i + OFFSET[k2][2]] == player)
                        {
                            op = false;
                            for (int m = x * 4; m < x * 4 + 4; m++)
                            {
                                if (a[m] == !player)
                                {
                                    opi2 = m;
                                    opy = m % 4;
                                    op = !op;
                                }
                            }
                            if (op)
                            {
                                for (int n = 2; n < 4; n++)
                                {
                                    if (opy + OFFSET[n][0] >= 0 && opy + OFFSET[n][0] < 4 && a[opi2 + OFFSET[n][2]] == player)
                                    {
                                        oop2 = true;
                                    }
                                }
                                if (oop2)
                                {
                                    a[opi2] = -1;
                                }
                            }
                        }
                    }

                    int key = convert(a);
                    pair<int, int> rt(0, 0);
                    if (!player)
                    {
                        chart::iterator it = wMap.find(key);
                        if (it != wMap.end())
                        {
                            wMap.insert(pair<unsigned int, pair<int, int>>(key, rt));
                            rt = rt + move(a, !player, step + 1);
                            return rt;
                        }
                        else
                        {
                            rt = make_pair(it->second.first, it->second.second);
                            return rt;
                        }
                    }
                    else
                    {
                        chart::iterator it = bMap.find(key);
                        if (it != bMap.end())
                        {
                            bMap.insert(pair<unsigned int, pair<int, int>>(key, rt));
                            rt = rt + move(a, !player, step + 1);
                            return rt;
                        }
                        else
                        {
                            rt = make_pair(it->second.first, it->second.second);
                            return rt;
                        }
                    }

                    a[i] = player;
                    a[i + OFFSET[j][2]] = -1;
                    if (oop1)
                    {
                        a[opi1] = !player;
                    }
                    if (oop2)
                    {
                        a[opi2] = !player;
                    }
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