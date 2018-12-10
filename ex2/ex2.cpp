#include <iostream>
#include <cmath>
#include <chrono>
#include "transform.h"

#define COMPUTE_TIME_CONSUMPTION

using namespace std;

/****************** 
 * Brief: (数字图像处理-->实验二) Fast Fourier transform and discrete cosine transform
 * Parameters:
 *     argv[1] -- data file name
 * Author:
 *     Ding Qingpeng (丁庆鹏, Student No. 18B903053)
 ******************/
int main(int argc, char const *argv[])
{
    vector<double> data = { 15, 32, 9, 222, 118, 151, 5, 7, 56, 233, 56, 121, 235, 89, 98, 111 };//1, 2, 3, 4, 5, 6, 7, 8, 9

    fft2 trans1;
    dft  trans2;
    dct  trans3;
    idct trans4;

// -- FFT --
#ifdef COMPUTE_TIME_CONSUMPTION
    chrono::steady_clock::time_point t1 = chrono::steady_clock::now();
#endif

    trans1.setData(data);
    trans1.execute();

#ifdef COMPUTE_TIME_CONSUMPTION
    chrono::steady_clock::time_point t2 = chrono::steady_clock::now();
    chrono::duration<double> time_used = chrono::duration_cast<chrono::duration<double>>( t2-t1 );
    cout << "Time consumption: " << time_used.count()*1000 << " ms" << endl;
#endif
    cout << "-- FFT --" << endl;
    // cout << trans1.outputSequence << endl;

// -- DFT --
#ifdef COMPUTE_TIME_CONSUMPTION
    t1 = chrono::steady_clock::now();
#endif

    trans2.setData(data);
    trans2.execute();

#ifdef COMPUTE_TIME_CONSUMPTION
    t2 = chrono::steady_clock::now();
    time_used = chrono::duration_cast<chrono::duration<double>>( t2-t1 );
    cout << "Time consumption: " << time_used.count()*1000 << " ms" << endl;
#endif
    cout << "-- DFT --" << endl;
    // cout << trans2.outputSequence << endl;

// -- DCT --
#ifdef COMPUTE_TIME_CONSUMPTION
    t1 = chrono::steady_clock::now();
#endif

    trans3.setData(data);
    trans3.execute();

#ifdef COMPUTE_TIME_CONSUMPTION
    t2 = chrono::steady_clock::now();
    time_used = chrono::duration_cast<chrono::duration<double>>( t2-t1 );
    cout << "Time consumption: " << time_used.count()*1000 << " ms" << endl;
#endif
    cout << "-- DCT --" << endl;
    cout << trans3.outputSequence << endl;

// -- IDCT --
#ifdef COMPUTE_TIME_CONSUMPTION
    t1 = chrono::steady_clock::now();
#endif

    trans4.setData(trans3.outputSequence);
    trans4.execute();

#ifdef COMPUTE_TIME_CONSUMPTION
    t2 = chrono::steady_clock::now();
    time_used = chrono::duration_cast<chrono::duration<double>>( t2-t1 );
    cout << "Time consumption: " << time_used.count()*1000 << " ms" << endl;
#endif
    cout << "-- IDCT --" << endl;
    cout << trans4.outputSequence << endl;

    return 0;
}
