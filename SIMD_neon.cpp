# include <arm_neon.h> // use Neon
# include <sys/time.h>

# include <iostream>
using namespace std;

int n=1000;
float **A;

void init()
{
	A = new float* [n];
	for (int i = 0; i < n; i++)
	{
		A[i] = new float[n];
	}


	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			A[i][j] = 0;
		}
		A[i][i] = 1.0;
		for (int j = i + 1; j < n; j++)
			A[i][j] = rand();
	}

	for (int k = 0; k < n; k++)
	{
		for (int i = k + 1; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				A[i][j] += A[k][j];
			}
		}
	}
}


void f_ordinary()
{
    for (int k = 0; k < n; k++)
	{
		for (int j = k + 1; j < n; j++)
		{
			//-------------------------------------------------------
			A[k][j] = A[k][j] * 1.0 / A[k][k];
		}
		A[k][k] = 1.0;

		for (int i = k + 1; i < n; i++)
		{
			for (int j = k + 1; j < n; j++)
			{
				//-----------------------------------------------------------
				A[i][j] = A[i][j] - A[i][k] * A[k][j];
			}
			A[i][k] = 0;
		}
	}
}


void f_pro()
{
    for (int k = 0; k < n; k++)
	{
	    float32x4_t vt=vmovq_n_f32(A[k][k]); //vt ← dupTo4Float(A[k,k]);

	    int j;
		for (j = k + 1; j+4 <= n; j+=4)
		{
		    float32x4_t va=vld1q_f32(&(A[k][j]) );

			va= vdivq_f32(va,vt);

			vst1q_f32(&(A[k][j]), va);

		}

		for(j = j + 4; j<n; j++)
        {
            A[k][j]=A[k][j]*1.0 / A[k][k];

        }

		A[k][k] = 1.0;

		for (int i = k + 1; i < n; i++)
		{
		    float32x4_t vaik=vmovq_n_f32(A[i][k]);

			for (int j = k + 1; j+4 <= n; j+=4)
			{
				float32x4_t vakj=vld1q_f32(&(A[k][j]));
				float32x4_t vaij=vld1q_f32(&(A[i][j]));
				float32x4_t vx=vmulq_f32(vakj,vaik);
				vaij=vsubq_f32(vaij,vx);

				vst1q_f32(&A[i][j], vaij);
			}

			for(j = j + 4; j<n; j++)
            {
                A[i][j] = A[i][j] - A[i][k] * A[k][j];
            }

			A[i][k] = 0;
		}
	}
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






