#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define ROW 140 // 140行（纵坐标单位个数）
#define AVG 10 // 计算AVG次取平均
#define MAXK 1 // 重复调用次数
#define ERROR -1
#define OVERFLOW -2

typedef struct{
    int len;  // 数组的长度（确定横坐标范围）
    double minData,maxData; // 数组的最小值与最大值（纵坐标范围）
    double *data;  // double类型的数组（存储图像信息）
}Coordinate;
// 操作坐标图
void InitCoor(Coordinate *C,double minN,double maxN,double add);
void DestroyCoor(Coordinate *C);
void TestToCoor(Coordinate *C,int f,double maxN,double add);
void DrawCoor(const Coordinate *C);
// 返回第f个函数在传入参数（自变量）为n时的平均运行时间（因变量）
double run(int f,double n);

double mypow(double x,double y);
// 若干被测函数（数量不确定）
double f1(double n);
double f2(double n);
double f3(double n);
double f4(double n);
double f5(double n);

int main()
{
    Coordinate C;
    int f; // 选择的第f个函数
    double minN,maxN,add; // 测试数据的范围和增幅
    scanf("%d%lf%lf%lf",&f,&minN,&maxN,&add);
    /* 操作坐标图 */
    InitCoor(&C,minN,maxN,add); // 构造一个空的坐标图
    TestToCoor(&C,f,maxN,add); // 测试数据并将结果赋值给坐标图
    DrawCoor(&C); // 画出坐标图（不修改）
    DestroyCoor(&C); // 销毁坐标图

    return 0;
}

double run(int f,double n)
{
    int i,avg;
    double all=0.0;
    clock_t start,stop;
    for( avg=0; avg<AVG; avg++ ) // 算AVG次取平均
    {
        start = clock();
        for( i=0; i<MAXK; i++ ) // 重复调用MAXK次
        {
            switch(f) // 选择第f个函数
            {
            case 1:
                f1(n); break;
            case 2:
                f2(n); break;
            case 3:
                f3(n); break;
            case 4:
                f4(n); break;
            case 5:
                f5(n); break;
            default:
                printf("未匹配到函数\n");
                exit(ERROR);
            }
        }
        stop=clock();
        all += stop-start;
    }
    return all/AVG;
}

// 构造一个空的坐标图
void InitCoor(Coordinate *C,double minN,double maxN,double add)
{
    C->len = (int)((maxN - minN) / add + 0.000001) + 1;
    C->data = (double*)malloc((C->len)*sizeof(double));
    if(C->data == NULL)
    {
        printf("数组创建失败");
        exit(OVERFLOW);
    }
}

// 销毁坐标图
void DestroyCoor(Coordinate *C)
{
    if(C->data) free(C->data);
}

// 测试数据并将结果赋值给坐标图
void TestToCoor(Coordinate *C,int f,double maxN,double add)
{
    int x; // 临时横坐标
    double n; // 临时自变量
    C->minData = 1.7976931348623158e+308; // double最大值
    C->maxData = 0; // 测试结果是时间，不会为负数
    // 从大到小遍历
    for( x=C->len-1,n=maxN; x>=0; x--,n-=add )
    {   // 测试结果赋值给数组
        C->data[x] = run(f,n);
        // 得到坐标图纵坐标范围
        if(C->data[x] < C->minData)
            C->minData = C->data[x];
        if(C->data[x] > C->maxData)
            C->maxData = C->data[x];
        // 输出数据
        printf("%d\t%.16f\t%f\n",x,n,C->data[x]);
    }
    printf("\nminData: %f",C->minData);
    printf("\nmaxData: %f\n\n",C->maxData);
}

// 画出坐标图
void DrawCoor(const Coordinate *C) // 不可修改
{
    if(C->maxData == C->minData)
    {
        printf("无法画出图像\n");
        exit(ERROR);
    }
    int x; // 横坐标
    double y,addy; // 纵坐标和纵坐标单位间隔
    addy = (C->maxData - C->minData) / ROW;
    for( y=C->maxData; y>=C->minData; y-=addy )
    {
        for( x=0; x<C->len; x++ )
        {
            if(y <= C->data[x]) // 判断该点是否满足数组数据
                printf("@ ");
            else
                printf("  ");
        }
        printf("\n");
    }
}

// 真正的累乘函数
double mypow(double x,double y)
{
    if(!y) return 1;
    int i;
    double r=x;
    for( i=1; i<y; i++ ) // 指数为y时应乘y-1次
        r *= x;
    return r;
}

// f1测试pow函数
double f1(double n)
{
    return pow(1.1,n);
}

// f2测试真正的累乘函数mypow
double f2(double n)
{
    return mypow(1.1,n);
}

// f3测试“求多项式的值”中用pow函数的时间复杂度
double f3(double n)
{
    int i;
    double p = 3.3;
    for( i=1; i<=n; i++ )
        p += (3.3 * pow(1.1,i));
    return p;
}

// f4测试“求多项式的值”中用mypow函数的时间复杂度
double f4(double n)
{
    int i;
    double p = 3.3;
    for( i=1; i<=n; i++ )
        p += (3.3 * mypow(1.1,i));
    return p;
}

// f5作为空白对照
double f5(double n)
{
    return 3.3;
}
