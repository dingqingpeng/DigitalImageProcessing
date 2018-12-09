#include <iostream>
#include <cmath>
#include <cstring>
#include "transform.h"

#define PI 3.1415926

/****************** 
 * Brief: (数字图像处理-->实验二) Fast Fourier transform and discrete cosine transform
 * Author:
 *     Ding Qingpeng (丁庆鹏, Student No. 18B903053)
 ******************/

using namespace std;

std::ostream& operator << ( std::ostream& out, const Complex& c)
{
    out << c.re;
    if(c.im >= 0)
        out << "+";
    out << c.im << "j" << endl;
    return out;
}

void dft::execute()
{
    ;
}

void fft2::truncate()
{
    layer = log2( inputSequence.size() );
    len2compute = pow(2, layer);
}

void fft2::sort()
{
    // Seq sortedSequence;
    for( size_t i = 0; i < len2compute; ++i )
    {
        Complex temp(inputSequence[binaryReverse(i)], 0);
        outputSequence.push_back( temp );
    }
}

size_t fft2::binaryReverse(size_t n)
{
    size_t reverse_n = 0;
    for( int i = 0; i < layer; ++i )
    {
        size_t lastBit = n & 1;
        n >>= 1;
        reverse_n <<= 1;
        reverse_n |= lastBit;
    }
    return reverse_n;
}

void fft2::execute()
{
    truncate();
    sort();

    OutSeq DataTemp;
    // DataTemp.assign( outputSequence.begin(), outputSequence.end() );
    for(size_t i = 1; i <= layer; i++)      // Compute each layer
    {
        DataTemp.assign( outputSequence.begin(), outputSequence.end() );
        setCore(pow(2, i));
        size_t sections = pow(2, layer - i);
        size_t jump     = len2compute/sections/2;
        for(size_t j = 0; j < sections; j++)    // Compute each section in every layer
        {
            size_t bias = j * jump * 2;
            for(size_t k = 0; k < jump; k++)  // Computes each element in every section
            {
                size_t start = bias + k;
                Complex F_even = DataTemp[start];
                Complex F_odd  = DataTemp[start + jump];
                outputSequence[start]        = F_even + F_odd * core.pow(k);
                outputSequence[start + jump] = F_even - F_odd * core.pow(k);
            }
        }
    }
}

std::ostream& operator << ( std::ostream& out, const std::vector<Complex>& v)
{
    
    for(size_t i = 0; i < v.size(); i++)
    {
        out << v[i] << endl;
    }
    return out;
}
