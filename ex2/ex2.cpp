#include <iostream>
#include <cmath>
#include "transform.h"

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

    fft2 t;
    t.setData(data);
    t.execute();
    
    cout << "-----------------" << endl;
    for(size_t i = 0; i < t.outputSequence.size(); i++)
    {
        cout << t.outputSequence[i] << endl;
    }
    

    return 0;
}
