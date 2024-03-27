#include <iostream>
#include <string.h>
#include <stack>
#include<stdio.h>
using namespace std;
#define MAX 100
#define INF 0x3f3f3f3f
#define me(a,b) memset(a,b,sizeof(b))
int dist[MAX], path[MAX];//������̾����·��

//����ͼ�ṹ
struct MGraph
{
    int edges[MAX][MAX];//�ڽӾ��󣬼�¼��������֮��ľ��룬Ҳ����Ȩֵ
    int e, n;//�����Ͷ�����
} G;

//��ʼ��ͼ
void init()
{
    memset(G.edges, INF, sizeof(G.edges));
    //me(G.edges,(INF));
    for (int i = 0; i < G.n; i++)
        G.edges[i][i] = 0;
}

//��ӡͼ���ڽӾ���
void printf_MG()
{
    printf("\n������ڵ���ڽӾ���\n");
    for (int i = 0; i < G.n; i++)
    {
        for (int j = 0; j < G.n; j++)
        {
            if (G.edges[i][j] == INF)
                printf("�� ");
            else
                printf("%2d ", G.edges[i][j]);
        }
        printf("\n");
    }
}

//ʵ��Dijkstra�㷨
//��Ҫ˼�����ÿ�δ�ʣ�ඥ����ѡһ�����������ĵ㣬Ȼ������������Χ�ĵ������ľ��룬ͬʱ�������㡣ֱ�����еĵ㶼����ǡ�
void Dijkstra(MGraph g, int u)
{
    int U[MAX], mmin;//�ֱ��ʾ�Ѿ��������ĵ㡢�൱ǰ��ʼ������ĵ�ľ���
    //�Ը�������г�ʼ��
    memset(U, 0, sizeof(U));
    memset(path, -1, sizeof(path));
    //me(dist,INF);
    for (int i = 0; i < g.n; i++)
    {
        dist[i] = g.edges[u][i];
        if (g.edges[u][i] < INF)
            path[i] = u;
    }
    dist[u] = 0;//������ľ���
    for (int i = 0; i < g.n; i++) //���Դ�㵽n�������̾���
    {
        mmin = INF;
        U[u] = 1;//��ѡ�����µ���ʼ�����U������
        for (int j = 0; j < g.n; j++)
            //���if�ж϶���u�ļ����Ƿ�����ͨ������j�ĸ��̵�·����������֣���ı�ԭ��·�����䳤�ȣ�����ʲô������
        {
            if (!U[j] && dist[u] + g.edges[u][j] < dist[j])
            {
                dist[j] = dist[u] + g.edges[u][j];//����·������
                path[j] = u;//���µ�����j��·��
            }
        }
        for (int j = 0; j < g.n; j++)
            //���ѭ��ÿ�δ�ʣ�ඥ����ѡ��һ�����㣬ͨ����������·����ͨ������ʣ�ඥ���·�����ǳ�����̵�
        {
            if (U[j] == 0 && dist[j] < mmin)
            {
                u = j;
                mmin = dist[j];
            }
        }
    }
}

//��ӡ��̵���·����·����
void printf_path(int u, int x)
{
    int a[MAX], cou = 0, ex = x;
    if (u == x)
        printf("\n�ڵ�%d->�ڵ�%d����·��%d-->%d��·�����ȣ�0\n", u, x, u, x);
    else if (path[x] == -1)
        printf("�ڵ�%d->�ڵ�%d����·��%d-->%d��·�����ȣ���(��û��·��)\n", u, x, u, x);//û��·��
    else
    {
        while (x != u)
        {
            a[cou++] = x;
            x = path[x];
        }
        a[cou] = x;
        printf("���ؽڵ�%d->�ڵ�%d����·��",u, a[0]);
        for (int i = cou; i > 0; i--)
            printf("%d-->", a[i]);
        printf("%d��·�����ȣ�%d\n", a[0], dist[ex]);
    }
    printf("\n");
}

//���·������OSPF��������Ҫʵ�����·���㷨������·�ɵ�ѡ�񣨼�����Դ�㵽���нڵ����̵�·����
int main()
{
    int v1, v2, w;
    printf("������·�ɽڵ������ڵ�������Լ��ڵ�֮�����ͨ�����\n");
    scanf_s("%d%d", &G.n, &G.e);//��������Ͷ�����
    init();
    for (int i = 0; i < G.e; i++)
    {
        scanf_s("%d%d%d", &v1, &v2, &w);
        G.edges[v1][v2] = w;
        G.edges[v2][v1] = w;
    }
    printf_MG();//����ڽӾ���
    int u;
    printf("\n�����뱾�ؽڵ㣨Դ�ڵ㣩��");
    scanf_s("%d", &u);//����Դ��
    Dijkstra(G, u);
    for (int i = 0; i < G.n; i++)//���Դ�㵽ÿ��������·���Լ�·����·
        printf_path(u, i);
    return 0;
}
/*
6 8
0 1 4
0 4 5
1 2 2
1 5 6
2 3 3
2 4 1
3 5 7
4 5 8
*/

