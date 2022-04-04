// �����˹��ȥ��neon����
# include <arm_neon.h> // use Neon
# include <sys/time.h>

#include <iostream>
#include <sstream>
#include <fstream>
using namespace std;

unsigned int Act[8399][264] = { 0 };
unsigned int Pas[8399][264] = { 0 };
//��Ԫ�ӳ�ʼ��
void init_A()
{
    //ÿ����Ԫ�ӵ�һ��Ϊ1λ���ڵ�λ�ã����������ڶ�ά������к�
    //���磺��Ԫ�ӣ�561��...����Act[561][]���
    unsigned int a;
    ifstream infile("act.txt");
    char fin[10000] = { 0 };
    int index;
    //���ļ�����ȡ��
    while (infile.getline(fin, sizeof(fin)))
    {
        std::stringstream line(fin);
        int biaoji = 0;

        //��������ȡ����������
        while (line >> a)
        {
            if (biaoji == 0)
            {
                //ȡÿ�е�һ������Ϊ�б�
                index = a;
                biaoji = 1;
            }
            int k = a % 32;
            int j = a / 32;

            int temp = 1 << k;
            Act[index][262 - j] += temp;
            Act[index][263] = 1;//���ø�λ�ü�¼��Ԫ�Ӹ����Ƿ�Ϊ�գ�Ϊ������0������Ϊ1
        }
    }
}

//����Ԫ�г�ʼ��
void init_P()
{
    //ֱ�Ӱ��մ����ļ���˳��棬�ڴ����ļ��ǵڼ��У���������ǵڼ���
    unsigned int a;
    ifstream infile("pas.txt");
    char fin[10000] = { 0 };
    int index = 0;
    //���ļ�����ȡ��
    while (infile.getline(fin, sizeof(fin)))
    {
        std::stringstream line(fin);
        int biaoji = 0;

        //��������ȡ����������
        while (line >> a)
        {
            if (biaoji == 0)
            {
                //��Pas[ ][263]��ű���Ԫ��ÿ�е�һ�����֣�����֮�����Ԫ����
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
        //ÿ�ִ���8����Ԫ�ӣ���Χ�������� i-7 �� i

        for (int j = 0; j < 4535; j++)
        {
            //��4535������Ԫ����û�������ڴ˷�Χ�ڵ�
            while (Pas[j][263] <= i && Pas[j][263] >= i - 7)
            {
                int index = Pas[j][263];
                if (Act[index][263] == 1)//��Ԫ�Ӳ�Ϊ��
                {
                    //Pas[j][]��Act[��Pas[j][18]��][]�����
                    for (int k = 0; k < 263; k++)
                    {
                        Pas[j][k] = Pas[j][k] ^ Act[index][k];
                    }

                    //����Pas[j][18]�������ֵ
                    //�������֮������������������浽Pas[j][18]�������ڷ�Χ������whileѭ��
                    //�����֮��Pas[j][ ]������
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
                else//��Ԫ��Ϊ��
                {
                    //Pas[j][]��������Ԫ��
                    for (int k = 0; k < 263; k++)
                        Act[index][k] = Pas[j][k];

                    Act[index][263] = 1;//������Ԫ�ӷǿ�
                    break;
                }

            }
        }
        cout << "3";
    }

    cout << "2";

    for (i = i + 8; i >= 0; i--)
    {
        //ÿ�ִ���1����Ԫ�ӣ���Χ���������i

        for (int j = 0; j < 4535; j++)
        {
            //��53������Ԫ����û���������i��
            while (Pas[j][263] == i)
            {
                if (Act[i][263] == 1)//��Ԫ�Ӳ�Ϊ��
                {
                    //Pas[j][]��Act[i][]�����
                    for (int k = 0; k < 263; k++)
                    {
                        Pas[j][k] = Pas[j][k] ^ Act[i][k];
                    }

                    //����Pas[j][18]�������ֵ
                    //�������֮������������������浽Pas[j][18]�������ڷ�Χ������whileѭ��
                    //�����֮��Pas[j][ ]������
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
                else//��Ԫ��Ϊ��
                {
                    //Pas[j][]��������Ԫ��
                    for (int k = 0; k < 263; k++)
                        Act[i][k] = Pas[j][k];

                    Act[i][263] = 1;//������Ԫ�ӷǿ�
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

    gettimeofday(&head, NULL);//��ʼ��ʱ
    f_ordinary();
    gettimeofday(&tail, NULL);//������ʱ

    double seconds = ((tail.tv_sec - head.tv_sec)*1000000 + (tail.tv_usec - head.tv_usec)) / 1000.0;//��λ ms
    cout<<"f_ordinary: "<<seconds<<" ms"<<endl;



    gettimeofday(&head, NULL);//��ʼ��ʱ
    f_pro();
    gettimeofday(&tail, NULL);//������ʱ
    seconds = ((tail.tv_sec - head.tv_sec)*1000000 + (tail.tv_usec - head.tv_usec)) / 1000.0;//��λ ms
    cout<<"f_pro: "<<seconds<<" ms"<<endl;

    //getResult();

}






