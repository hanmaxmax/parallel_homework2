// �����˹��ȥ��neon����
# include <arm_neon.h> // use Neon
# include <sys/time.h>

# include <iostream>
using namespace std;

unsigned int Act[200][18] = { 0 };
unsigned int Pas[200][18] = { 0 };

void init_A()
{
    unsigned int a;
	ifstream infile("act.txt");
	char fin[5000] = { 0 };
	int index = 0;
	//���ļ�����ȡ��
	while (infile.getline(fin, sizeof(fin)))
	{
		std::stringstream line(fin);

		//��������ȡ����������
		while (line >> a)
		{
			int k = a % 32;
			int j = a / 32;

			int temp = 1 << k;
			Act[index][17-j] += temp;
		}
		index++;
	}
}

void init_P()
{
    unsigned int a;
	ifstream infile("pas.txt");
	char fin[5000] = { 0 };
	int index = 0;
	//���ļ�����ȡ��
	while (infile.getline(fin, sizeof(fin)))
	{
		std::stringstream line(fin);

		//��������ȡ����������
		while (line >> a)
		{
			int k = a % 32;
			int j = a / 32;

			int temp = 1 << k;
			Pas[index][17-j] += temp;
		}
		index++;
	}
}




void f_ordinary()
{







}


void f_pro()
{




}



void getResult()
{
    for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			cout << A[i][j] << " ";
		}
		cout << endl;
	}
}



int main()
{
    init();
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






