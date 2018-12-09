#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include <iostream>
#include <vector>

#define PI 3.1415926

/****************** 
 * Brief: Class definition
 * Author:
 *     Ding Qingpeng (丁庆鹏, Student No. 18B903053)
 ******************/

class Complex
{
public:
    /* 
     * Brief: Default constructor
     * Parameter:
     *     None
     */
    Complex():re(0.0), im(0.0) {}

    /* 
     * Brief: Overload constructor
     * Parameter:
     *     r -- real part
     *     i -- imaginary part
     */
    Complex(double r, double i) { re = r; im = i; }

    /* 
     * Brief: Overload constructor, if function has only one parameter,
     *        then reconstruct this complex with Euler equation
     * Parameter:
     *     theta -- exponential part
     */
    Complex(double theta) { re = cos(theta); im = sin(theta); }

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

    friend std::ostream& operator << ( std::ostream& out, const Complex& c);

private:
    double re;  // Real part
    double im;  // Imaginary part
};

class Transform
{
    
public:
    typedef std::vector<double> InSeq;

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
    void setData(const InSeq& list) {
        inputSequence.assign( list.begin(), list.end() );
    }

    /* 
     * Brief: Execute transform
     * Parameter:
     *     None
     * Return:
     *     None
     */
    virtual void execute() = 0;

protected:
    InSeq inputSequence;
};

class dft: public Transform
{
public:
    typedef std::vector<Complex> OutSeq;
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
    void setCore(size_t M) { core = Complex( -2.0*PI / M ); }

    /* 
     * Brief: Execute dft
     * Parameter:
     *     None
     * Return:
     *     None
     */
    void execute();

public:
    Complex core;
    OutSeq outputSequence;
};

class fft2: public dft
{
public:
    /* 
     * Brief: Default constructor
     * Parameter:
     *     None
     */
    fft2(): dft(), layer(0), len2compute(0) {}

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

    /* 
     * Brief: Execute fft2
     * Parameter:
     *     None
     * Return:
     *     None
     */
    void execute();

public:
    size_t layer;
    size_t len2compute; // Length of truncated input sequence
};

/* 
 * Brief: Overload operator <<
 * Parameter:
 *     c -- Another complex
 * Return:
 *     Reference of ostream
 */
std::ostream& operator << ( std::ostream& out, const Complex& c);

std::ostream& operator << ( std::ostream& out, const std::vector<Complex>& v);

#endif