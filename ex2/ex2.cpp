#include <iostream>
#include <cmath>
#include <chrono>
#include <typeinfo>
#include "transform.h"

#define COMPUTE_TIME_CONSUMPTION

using namespace std;

template<typename TRANS>
void Trans(vector<double> data);

/****************** 
 * Brief: (数字图像处理-->实验二) Fast Fourier transform and discrete cosine transform
 * Parameters:
 *     argv[1] -- data file name
 * Author:
 *     Ding Qingpeng (丁庆鹏, Student No. 18B903053)
 ******************/
int main(int argc, char const *argv[])
{
    vector<double> data = { 15, 32, 9, 222, 118, 151, 5, 7, 56, 233, 56, 121, 235, 89, 98, 111 };

    Trans<fft2>(data);

    return 0;
}

template<typename TRANS>
void Trans(vector<double> data)
{
    TRANS trans;
#ifdef COMPUTE_TIME_CONSUMPTION
    chrono::steady_clock::time_point t1 = chrono::steady_clock::now();
#endif
    trans.setData(data);
    trans.execute();
#ifdef COMPUTE_TIME_CONSUMPTION
    chrono::steady_clock::time_point t2 = chrono::steady_clock::now();
    chrono::duration<double> time_used = chrono::duration_cast<chrono::duration<double>>( t2-t1 );
    cout << "Time consumption: " << time_used.count()*1000 << " ms" << endl;
#endif
    cout << "-- " << typeid(trans).name() << " --" << endl;
    cout << trans.outputSequence << endl;
}
