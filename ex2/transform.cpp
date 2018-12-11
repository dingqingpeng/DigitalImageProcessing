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

template<typename InputType, typename OutputType, typename CoreType>
void FastTransformBase2<InputType, OutputType, CoreType>::truncate()
{
    layer = log2( this->inputSequence.size() );
    len2compute = 1 << layer;      // i.e. pow(2, layer)
}

template<typename InputType, typename OutputType, typename CoreType>
void FastTransformBase2<InputType, OutputType, CoreType>::sort()
{
    for( size_t i = 0; i < len2compute; ++i )
        this->outputSequence.push_back( OutputType(this->inputSequence[binaryReverse(i)]) );
}

template<typename InputType, typename OutputType, typename CoreType>
size_t FastTransformBase2<InputType, OutputType, CoreType>::binaryReverse(size_t n)
{
    size_t reverse_n = 0;
    for( size_t i = 0; i < layer; ++i )
    {
        size_t lastBit = n & 1;
        n >>= 1;
        reverse_n <<= 1;
        reverse_n |= lastBit;
    }
    return reverse_n;
}

void dft::execute()
{
    size_t M = inputSequence.size();
    setCore( M );
    for(size_t u = 0; u < M; u++)
    {
        ComplexDFT sum(0, 0);
        for(size_t x = 0; x < M; x++)
        {
            sum = sum + core.pow( u*x ) * inputSequence[x];
        }
        outputSequence.push_back( sum );
    }
}

// void fft2::sort()
// {
//     for( size_t i = 0; i < len2compute; ++i )
//     {
//         // ComplexDFT temp( inputSequence[binaryReverse(i)], 0 );
//         outputSequence.push_back( ComplexDFT(inputSequence[binaryReverse(i)]) );
//     }
// }

void fft2::execute()
{
    truncate();
    sort();

    OutputList DataTemp;
    for(size_t i = 1; i <= layer; i++)          // Compute each layer
    {
        DataTemp.assign( outputSequence.begin(), outputSequence.end() );
        setCore( 1 << i );                      // i.e. pow(2, i)
        size_t sections = 1 << ( layer - i );   // i.e. pow(2, layer - i)
        size_t jump     = len2compute / ( sections*2 );
        for(size_t j = 0; j < sections; j++)    // Compute each section in every layer
        {
            size_t bias = j * jump * 2;
            for(size_t k = 0; k < jump; k++)    // Computes each element in every section
            {
                size_t start = bias + k;
                ComplexDFT F_even = DataTemp[start];
                ComplexDFT F_odd  = DataTemp[start + jump] * core.pow(k);
                outputSequence[start]        = F_even + F_odd;
                outputSequence[start + jump] = F_even - F_odd;
            }
        }
    }
}

void dct::execute()
{
    size_t N = inputSequence.size();
    double sum = 0;

    // Compute C(0)
    for(size_t x = 0; x < N; x++)
        sum = sum + inputSequence[x];
    outputSequence.push_back( sqrt( 1.0/N ) * sum );

    // Compute C(1) to C(N-1)
    for(size_t u = 1; u < N; u++)
    {
        sum = 0;
        for(size_t x = 0; x < N; x++)
        {
            setCore( N, x, u );
            sum += inputSequence[x] * core;
        }
        outputSequence.push_back( sqrt( 2.0/N ) * sum );
    }
}

void idct::execute()
{
    size_t N = inputSequence.size();
    double ave = inputSequence[0] / sqrt( N );

    // Compute f(0) to f(N-1)
    for(size_t x = 0; x < N; x++)
    {
        double sum = 0;
        for(size_t u = 1; u < N; u++)
        {
            setCore( N, x, u );
            sum += inputSequence[u] * core;
        }
        outputSequence.push_back( ave + sqrt( 2.0/N ) * sum );
    }
}

void fdct::execute()
{
    // Using 4N FFT
    // Construct 4N input sequence
    size_t N = inputSequence.size();
    for(size_t i = 0; i < N; i++)
    {
        inputSequence_xN.push_back( 0.0 );
        inputSequence_xN.push_back( inputSequence[ i ] );
    }
    for(size_t i = 0; i < N; i++)
    {
        inputSequence_xN.push_back( 0.0 );
        inputSequence_xN.push_back( inputSequence[ N - i - 1 ] );
    }

    // New a fft2 object, and execute
    fft2 transform;
    transform.setData(this->inputSequence_xN);
    transform.execute();
    
    // Push to output sequence
    transform.outputSequence[0].re *= sqrt( 0.25/N );
    this->outputSequence.push_back(transform.outputSequence[0].re);
    for(size_t i = 1; i < N; i++)
    {
        transform.outputSequence[i].re *= sqrt( 0.5/N );
        this->outputSequence.push_back(transform.outputSequence[i].re);
    }

    /* // Using N FFT, faster than 4N FFT
    // Construct N input sequence
    size_t N = inputSequence.size();
    for(size_t i = 0; i < N; i += 2)
        inputSequence_xN.push_back( inputSequence[ i ] );
    for(size_t i = 1; i < N; i += 2)
        inputSequence_xN.push_back( inputSequence[ N - i - 1 ] );

    // New a fft2 object, and execute
    fft2 transform;
    transform.setData(this->inputSequence_xN);
    transform.execute();
    
    // Push to output sequence
    
    for(size_t i = 0; i < N; i++)
    {
        double theta = -i*PI / (2*N);
        Complexd factor( cos(theta), sin(theta) );
        transform.outputSequence[i] = transform.outputSequence[i] * 2 * factor;
    }
    transform.outputSequence[0].re *= sqrt( 0.25/N );
    this->outputSequence.push_back(transform.outputSequence[0].re);
    for(size_t i = 1; i < N; i++)
    {
        transform.outputSequence[i].re *= sqrt( 0.5/N );
        this->outputSequence.push_back(transform.outputSequence[i].re);
    } */
}

// template<typename T>
std::ostream& operator << ( std::ostream& out, const std::vector< Complexd >& v)
{
    for(size_t i = 0; i < v.size(); i++)
    {
        out << v[i];
    }
    return out;
}

std::ostream& operator << ( std::ostream& out, const std::vector< double >& v)
{
    for(size_t i = 0; i < v.size(); i++)
    {
        out << v[i] << endl;
    }
    return out;
}
