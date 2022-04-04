// 特殊高斯消去的neon并行
# include <arm_neon.h> // use Neon
# include <sys/time.h>

#include <iostream>
#include <sstream>
#include <fstream>
using namespace std;

unsigned int Act[8399][264] = { 0 };
unsigned int Pas[8399][264] = { 0 };
//消元子初始化
void init_A()
{
    //每个消元子第一个为1位所在的位置，就是它所在二维数组的行号
    //例如：消元子（561，...）由Act[561][]存放
    unsigned int a;
    ifstream infile("act.txt");
    char fin[10000] = { 0 };
    int index;
    //从文件中提取行
    while (infile.getline(fin, sizeof(fin)))
    {
        std::stringstream line(fin);
        int biaoji = 0;

        //从行中提取单个的数字
        while (line >> a)
        {
            if (biaoji == 0)
            {
                //取每行第一个数字为行标
                index = a;
                biaoji = 1;
            }
            int k = a % 32;
            int j = a / 32;

            int temp = 1 << k;
            Act[index][262 - j] += temp;
            Act[index][263] = 1;//设置该位置记录消元子该行是否为空，为空则是0，否则为1
        }
    }
}

//被消元行初始化
void init_P()
{
    //直接按照磁盘文件的顺序存，在磁盘文件是第几行，在数组就是第几行
    unsigned int a;
    ifstream infile("pas.txt");
    char fin[10000] = { 0 };
    int index = 0;
    //从文件中提取行
    while (infile.getline(fin, sizeof(fin)))
    {
        std::stringstream line(fin);
        int biaoji = 0;

        //从行中提取单个的数字
        while (line >> a)
        {
            if (biaoji == 0)
            {
                //用Pas[ ][263]存放被消元行每行第一个数字，用于之后的消元操作
                Pas[index][263] = a;
                biaoji = 1;
            }

            int k = a % 32;
            int j = a / 32;

            int temp = 1 << k;
            Pas[index][262 - j] += temp;
        }
        index++;
    }
}




void f_ordinary()
{
    int i;
    for (i = 8398; i - 8 >= -1; i -= 8)
    {
        //每轮处理8个消元子，范围：首项在 i-7 到 i

        for (int j = 0; j < 4535; j++)
        {
            //看4535个被消元行有没有首项在此范围内的
            while (Pas[j][263] <= i && Pas[j][263] >= i - 7)
            {
                int index = Pas[j][263];
                if (Act[index][263] == 1)//消元子不为空
                {
                    //Pas[j][]和Act[（Pas[j][18]）][]做异或
                    for (int k = 0; k < 263; k++)
                    {
                        Pas[j][k] = Pas[j][k] ^ Act[index][k];
                    }

                    //更新Pas[j][18]存的首项值
                    //做完异或之后继续找这个数的首项，存到Pas[j][18]，若还在范围里会继续while循环
                    //找异或之后Pas[j][ ]的首项
                    int num = 0, S_num = 0;
                    for (num = 0; num < 263; num++)
                    {
                        if (Pas[j][num] != 0)
                        {
                            unsigned int temp = Pas[j][num];
                            while (temp != 0)
                            {
                                temp = temp >> 1;
                                S_num++;
                            }
                            S_num += num * 32;
                            break;
                        }
                    }
                    Pas[j][263] = S_num - 1;

                }
                else//消元子为空
                {
                    //Pas[j][]来补齐消元子
                    for (int k = 0; k < 263; k++)
                        Act[index][k] = Pas[j][k];

                    Act[index][263] = 1;//设置消元子非空
                    break;
                }

            }
        }
        cout << "3";
    }

    cout << "2";

    for (i = i + 8; i >= 0; i--)
    {
        //每轮处理1个消元子，范围：首项等于i

        for (int j = 0; j < 4535; j++)
        {
            //看53个被消元行有没有首项等于i的
            while (Pas[j][263] == i)
            {
                if (Act[i][263] == 1)//消元子不为空
                {
                    //Pas[j][]和Act[i][]做异或
                    for (int k = 0; k < 263; k++)
                    {
                        Pas[j][k] = Pas[j][k] ^ Act[i][k];
                    }

                    //更新Pas[j][18]存的首项值
                    //做完异或之后继续找这个数的首项，存到Pas[j][18]，若还在范围里会继续while循环
                    //找异或之后Pas[j][ ]的首项
                    int num = 0, S_num = 0;
                    for (num = 0; num < 263; num++)
                    {
                        if (Pas[j][num] != 0)
                        {
                            unsigned int temp = Pas[j][num];
                            while (temp != 0)
                            {
                                temp = temp >> 1;
                                S_num++;
                            }
                            S_num += num * 32;
                            break;
                        }
                    }
                    Pas[j][263] = S_num - 1;

                }
                else//消元子为空
                {
                    //Pas[j][]来补齐消元子
                    for (int k = 0; k < 263; k++)
                        Act[i][k] = Pas[j][k];

                    Act[i][263] = 1;//设置消元子非空
                    break;
                }

            }
        }


    }


}

void f_pro()
{




}




int main()
{
    init_A();
    init_P();
    struct timeval head,tail;

    gettimeofday(&head, NULL);//开始计时
    f_ordinary();
    gettimeofday(&tail, NULL);//结束计时

    double seconds = ((tail.tv_sec - head.tv_sec)*1000000 + (tail.tv_usec - head.tv_usec)) / 1000.0;//单位 ms
    cout<<"f_ordinary: "<<seconds<<" ms"<<endl;



    gettimeofday(&head, NULL);//开始计时
    f_pro();
    gettimeofday(&tail, NULL);//结束计时
    seconds = ((tail.tv_sec - head.tv_sec)*1000000 + (tail.tv_usec - head.tv_usec)) / 1000.0;//单位 ms
    cout<<"f_pro: "<<seconds<<" ms"<<endl;

    //getResult();

}






