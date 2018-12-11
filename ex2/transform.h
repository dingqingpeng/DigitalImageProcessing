#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include <iostream>
#include <vector>
#include <cmath>

#define PI 3.1415926

/****************** 
 * Brief: Class definition
 * Author:
 *     Ding Qingpeng (丁庆鹏, Student No. 18B903053)
 ******************/

template<typename T>
class Complex
{
public:
    /* 
     * Brief: Default constructor
     * Parameter:
     *     None
     */
    Complex():re(0), im(0) {}

    /* 
     * Brief: Overload constructor
     * Parameter:
     *     r -- real part
     *     i -- imaginary part
     */
    Complex(T r, T i) { re = r; im = i; }

    /* 
     * Brief: Overload type conversion
     * Parameter:
     *     n -- A real number
     */
    Complex(T n) { re = n; im = 0; }

    /* 
     * Brief: Deconstructor
     * Parameter:
     *     None
     */
    ~Complex() {}

    /* 
     * Brief: Exponential of complex
     * Parameter:
     *     p -- power of complex
     * Return:
     *     complex
     */
    Complex pow(double p) {
        Complex temp(1, 0);
        for(size_t i = 0; i < p; i++)
            temp = (*this)*temp;
        return temp;
    }

    /* 
     * Brief: Overload operator +
     * Parameter:
     *     c -- Another complex
     * Return:
     *     (*this) + c
     */
    Complex operator + (const Complex c) {
        return Complex( this->re + c.re,
                        this->im + c.im );
    }

    /* 
     * Brief: Overload operator -
     * Parameter:
     *     c -- Another complex
     * Return:
     *     (*this) - c
     */
    Complex operator - (const Complex c) {
        return Complex( this->re - c.re,
                        this->im - c.im );
    }

    /* 
     * Brief: Overload operator *
     * Parameter:
     *     c -- Another complex
     * Return:
     *     (*this) * c
     */
    Complex operator * (const Complex c) {
        return Complex( this->re * c.re - this->im * c.im,
                        this->re * c.im + this->im * c.re );
    }

    /* 
     * Brief: Overload operator *
     * Parameter:
     *     n -- a real number
     * Return:
     *     (*this) * n
     */
    Complex operator * (const T n) {
        return Complex( this->re * n,
                        this->im * n );
    }

    /* 
     * Brief: Overload operator <<
     * Parameter:
     *     c -- Another complex
     * Return:
     *     Reference of ostream
     */
    friend std::ostream& operator << ( std::ostream& out, const Complex<T>& c)
    {
        out << c.re;
        if(c.im >= 0)
            out << "+";
        out << c.im << "j" << std::endl;
        return out;
    }

public:
    T re;  // Real part
    T im;  // Imaginary part
};

// typedef Complex<size_t> Complexl;
typedef Complex<float>  Complexf;
typedef Complex<double> Complexd;

template<typename InputType, typename OutputType, typename CoreType>
class Transform
{
public:
    typedef std::vector<InputType>   InputList;
    typedef std::vector<OutputType> OutputList;

    /* 
     * Brief: Default constructor
     * Parameter:
     *     None
     */
    Transform() {}

    /* 
     * Brief: Deconstructor
     * Parameter:
     *     None
     */
    ~Transform() {}

    /* 
     * Brief: Set input sequence
     * Parameter:
     *     list -- reference to data sequence
     */
    void setData(const InputList& list) {
        inputSequence.assign( list.begin(), list.end() );
    }

    /* 
     * Brief: Set transform core
     * Parameter:
     *     None
     */
    // void setCore(size_t M) = 0;

    /* 
     * Brief: Execute transform
     * Parameter:
     *     None
     * Return:
     *     None
     */
    virtual void execute() = 0;

public:
    InputList  inputSequence;
    OutputList outputSequence;
    CoreType   core;
};

template<typename InputType, typename OutputType, typename CoreType>
class FastTransformBase2: virtual public Transform<InputType, OutputType, CoreType>
{
public:
    /* 
     * Brief: Default constructor
     * Parameter:
     *     None
     */
    FastTransformBase2(): Transform<InputType, OutputType, CoreType>::Transform(), layer(0), len2compute(0) {}

    /* 
     * Brief: Truncate input sequence to the nearest number of 2^N
     * Parameter:
     *     None
     * Return:
     *     None
     */
    void truncate();

    /* 
     * Brief: Sort input sequence with reverse binary order
     * Parameter:
     *     None
     * Return:
     *     None
     */
    void sort();

    /* 
     * Brief: Reverse the last layer bits
     * Parameter:
     *     n -- number to be reversed
     * Return:
     *     reversed number
     */
    size_t binaryReverse(size_t n);
    
public:
    size_t layer;
    size_t len2compute; // Length of truncated input sequence
};

class dft: virtual public Transform<double, Complexd, Complexd>
{
public:
    typedef Complexd ComplexDFT;

    /* 
     * Brief: Default constructor
     * Parameter:
     *     None
     */
    dft(): Transform() {}

    /* 
     * Brief: Set discrete Fourier transform core
     * Parameter:
     *     None
     */
    void setCore(size_t M) { 
        double theta = -2.0*PI / M;
        core = ComplexDFT( cos(theta), sin(theta) );
    }

    /* 
     * Brief: Execute dft
     * Parameter:
     *     None
     * Return:
     *     None
     */
    void execute();
};

class fft2: public FastTransformBase2<double, Complexd, Complexd>, public dft
{
public:
    /* 
     * Brief: Default constructor
     * Parameter:
     *     None
     */
    fft2(): FastTransformBase2(), dft() {}

    /* 
     * Brief: Execute fft2
     * Parameter:
     *     None
     * Return:
     *     None
     */
    void execute();
};

class dct: virtual public Transform<double, double, double>
{
public:
    /* 
     * Brief: Default constructor
     * Parameter:
     *     None
     */
    dct(): Transform() {}

    /* 
     * Brief: Set discrete cosine transform core
     * Parameter:
     *     None
     */
    void setCore(size_t N, size_t x, size_t u) { 
        core = (u == 0)? ( 1.0 ): 
                         ( cos( (2.0*x+1.0)*u*PI / (2.0*N) ) );
    }

    /* 
     * Brief: Execute dct
     * Parameter:
     *     None
     * Return:
     *     None
     */
    void execute();
};

class idct: public dct
{
public:
    /* 
     * Brief: Default constructor
     * Parameter:
     *     None
     */
    idct(): dct() {}

    /* 
     * Brief: Execute idct
     * Parameter:
     *     None
     * Return:
     *     None
     */
    void execute();
};

class fdct: public FastTransformBase2<double, double, double>, public dct
{
public:
    /* 
     * Brief: Default constructor
     * Parameter:
     *     None
     */
    fdct(): FastTransformBase2(), dct() {}

    /* 
     * Brief: Execute fdct using 4N FFT
     * Parameter:
     *     None
     * Return:
     *     None
     */
    void execute();

public:
    InputList inputSequence_xN; 
};

/* 
 * Brief: Overload operator <<
 * Parameter:
 *     v -- vector of Complexd
 * Return:
 *     Reference of ostream
 */
// template<typename T>
std::ostream& operator << ( std::ostream& out, const std::vector< Complexd >& v);

/* 
 * Brief: Overload operator <<
 * Parameter:
 *     v -- vector of double
 * Return:
 *     Reference of ostream
 */
// template<typename T>
std::ostream& operator << ( std::ostream& out, const std::vector< double >& v);

#endif