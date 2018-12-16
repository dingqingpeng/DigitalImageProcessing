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
     * Brief: Overload type conversion
     * Parameter:
     *     n -- A real number
     */
    operator T() { return re; }

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
     * Brief: Overload operator /
     * Parameter:
     *     n -- a real number
     * Return:
     *     (*this) / n
     */
    Complex operator / (const T n) {
        return Complex( this->re / n,
                        this->im / n );
    }

    Complex conjugate() {
        return Complex( re, -im );
    }

    T abs() {
        return sqrt( re * re + im * im );
    }

    T pha() {
        return atan2( im, re );
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
typedef Complex<float>  Complexf;
typedef Complex<double> Complexd;
typedef Complexd ComplexDFT;

template<typename InputType, typename OutputType, typename CoreType>
class Transform
{
public:
    typedef std::vector<InputType>   InputList;
    typedef std::vector<OutputType> OutputList;

    Transform() {}
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
     * Brief: Execute transform
     * Parameter:
     *     None
     * Return:
     *     None
     */
    virtual void execute() = 0;

    /*
     * Brief: Display transform result
     * Parameter:
     *     None
     * Return:
     *     None
     */
    void displayResult() {
        std::cout << outputSequence << std::endl;
    }

public:
    InputList  inputSequence;
    OutputList outputSequence;
    CoreType   core;
};

template<typename InputType, typename OutputType, typename CoreType>
class FastTransformBase2: virtual public Transform<InputType, OutputType, CoreType>
{
public:
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

template<typename dftInputType>
class dftTemplate: virtual public Transform<dftInputType, ComplexDFT, ComplexDFT>
{
public:
    dftTemplate(): Transform<dftInputType, ComplexDFT, ComplexDFT>() {}

    /*
     * Brief: Set discrete Fourier transform core
     * Parameter:
     *     None
     */
    void setCore(size_t M) {
        double theta = -2.0*PI / M;
        Transform<dftInputType, ComplexDFT, ComplexDFT>::core = ComplexDFT( cos(theta), sin(theta) );
    }

    /*
     * Brief: Execute dft
     * Parameter:
     *     None
     * Return:
     *     None
     */
    void execute()
    {
        size_t M = Transform<dftInputType, ComplexDFT, ComplexDFT>::inputSequence.size();
        setCore( M );
        for(size_t u = 0; u < M; u++)
        {
            ComplexDFT sum(0.0, 0.0);
            for(size_t x = 0; x < M; x++)
            {
                sum = sum + Transform<dftInputType, ComplexDFT, ComplexDFT>::core.pow( u*x ) *
                            Transform<dftInputType, ComplexDFT, ComplexDFT>::inputSequence[x];
            }
            Transform<dftInputType, ComplexDFT, ComplexDFT>::outputSequence.push_back( sum );
        }
    }
};
typedef dftTemplate<double>   dftdouble;
typedef dftTemplate<double>   dft;
typedef dftTemplate<Complexd> dftComplex;

template<typename dftOutputType>
class idftTemplate: virtual public Transform<ComplexDFT, dftOutputType, ComplexDFT>
{
public:
    idftTemplate(): Transform<ComplexDFT, dftOutputType, ComplexDFT>() {}

    /*
     * Brief: Set inverse discrete Fourier transform core
     * Parameter:
     *     None
     */
    void setCore(size_t M) {
        double theta = 2.0*PI / M;
        Transform<ComplexDFT, dftOutputType, ComplexDFT>::core = ComplexDFT( cos(theta), sin(theta) );
    }

    /*
     * Brief: Execute idft
     * Parameter:
     *     None
     * Return:
     *     None
     */
    void execute()
    {
        size_t M = Transform<ComplexDFT, dftOutputType, ComplexDFT>::inputSequence.size();
        setCore( M );
        for(size_t u = 0; u < M; u++)
        {
            ComplexDFT sum(0, 0);
            for(size_t x = 0; x < M; x++)
            {
                sum = sum + Transform<ComplexDFT, dftOutputType, ComplexDFT>::core.pow( u*x ) *
                            Transform<ComplexDFT, dftOutputType, ComplexDFT>::inputSequence[x];
            }
            Transform<ComplexDFT, dftOutputType, ComplexDFT>::outputSequence.push_back( (dftOutputType)sum / (double)M );
        }
    }
};
typedef idftTemplate<double>   idftdouble;
typedef idftTemplate<double>   idft;
typedef idftTemplate<Complexd> idftComplex;

class fft2double: public FastTransformBase2<double, ComplexDFT, ComplexDFT>, public dftdouble
{
public:
    fft2double(): FastTransformBase2(), dftdouble() {}

    /*
     * Brief: Execute fft2
     * Parameter:
     *     None
     * Return:
     *     None
     */
    void execute();
};
typedef fft2double fft2;

class fft2Complex: public FastTransformBase2<ComplexDFT, ComplexDFT, ComplexDFT>, public dftComplex
{
public:
    fft2Complex(): FastTransformBase2(), dftComplex() {}

    /*
     * Brief: Execute fft2
     * Parameter:
     *     None
     * Return:
     *     None
     */
    void execute();
};

class ifft2Complex: public FastTransformBase2<ComplexDFT, ComplexDFT, ComplexDFT>, public idftComplex
{
public:
    ifft2Complex(): FastTransformBase2(), idftComplex() {}

    /*
     * Brief: Execute fft2
     * Parameter:
     *     None
     * Return:
     *     None
     */
    void execute();
};

class ifft2double: public FastTransformBase2<ComplexDFT, double, ComplexDFT>, public idftdouble
{
public:
    ifft2double(): FastTransformBase2(), idftdouble() {}

    /*
     * Brief: Execute fft2
     * Parameter:
     *     None
     * Return:
     *     None
     */
    void execute();
};
typedef ifft2double ifft2;

class dct: virtual public Transform<double, double, double>
{
public:
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

// class fdct: public FastTransformBase2<ComplexDFT, ComplexDFT, ComplexDFT>, public dct
// class fdct: public fft2, public dct
// {
// public:
//     /*
//      * Brief: Default constructor
//      * Parameter:
//      *     None
//      */
//     // fdct(): FastTransformBase2(), dct() {}
//     fdct(): fft2(), dct() {}

//     /*
//      * Brief: Set input sequence
//      * Parameter:
//      *     list -- reference to data sequence
//      */
//     void setData(const dft::InputList& list) {
//         dft::setData(list);
//     }

//     /*
//      * Brief: Execute fdct using xN FFT
//      * Parameter:
//      *     None
//      * Return:
//      *     None
//      */
//     void execute();

// public:
//     fft2::InputList inputSequence_xN;
// };

template<typename InputType, typename OutputType>
class Transform2D
{
public:
    typedef std::vector<InputType>   InputList;
    typedef std::vector<OutputType> OutputList;
    typedef std::vector< std::vector<InputType> >  InputMat;    // Vector of rows
    typedef std::vector< std::vector<OutputType> > OutputMat;   // Vector of rows

    Transform2D() {}
    ~Transform2D() {}

    /*
     * Brief: Set input matrix
     * Parameter:
     *     list -- reference to data sequence
     */
    void setData(const InputMat& Mat) {
        inputGraph.assign( Mat.begin(), Mat.end() );
    }

    /*
     * Brief: Display transform result
     * Parameter:
     *     None
     * Return:
     *     None
     */
    void displayResult() {
        std::cout << outputGraph << std::endl;
    }

public:
    InputMat  inputGraph;
    OutputMat outputGraph;
};

template<typename TransformType1, typename TransformType2>
class ft2D: public Transform2D<double, ComplexDFT>
{
public:
    ft2D() {}

    /*
     * Brief: Execute 2D ft
     * Parameter:
     *     None
     * Return:
     *     None
     */
    void execute()
    {
        // dft every row
        TransformType1 trans1;
        size_t rows = inputGraph.size();
        for(size_t i = 0; i < rows; i++)
        {
            trans1.inputSequence.clear();
            trans1.outputSequence.clear();
            trans1.setData(inputGraph[i]);
            trans1.execute();
            outputGraph.push_back(trans1.outputSequence);
        }

        // dft every column
        TransformType2 trans2;
        size_t cols = inputGraph[0].size();
        OutputList colTemp;
        for(size_t i = 0; i < cols; i++)
        {
            trans2.inputSequence.clear();
            trans2.outputSequence.clear();
            colTemp.clear();

            // reconstruct column vector
            for(size_t j = 0; j < rows; j++)
                colTemp.push_back( outputGraph[j][i] );

            trans2.setData(colTemp);
            trans2.execute();

            for(size_t j = 0; j < rows; j++)
                outputGraph[j][i] = trans2.outputSequence[j];
        }
    }
};
typedef ft2D<  dftdouble,  dftComplex > dft2D;
typedef ft2D< fft2double, fft2Complex > fft2D;

template<typename TransformType1, typename TransformType2>
class ift2D: public Transform2D<ComplexDFT, double>
{
public:
    ift2D() {}

    /*
     * Brief: Execute 2D ift
     * Parameter:
     *     None
     * Return:
     *     None
     */
    void execute()
    {
        InputMat outputTemp;    // vector of each column
        size_t rows = inputGraph.size();
        size_t cols = inputGraph[0].size();

        // idft every column
        TransformType1 trans1;
        InputList colTemp;
        for(size_t i = 0; i < cols; i++)
        {
            trans1.inputSequence.clear();
            trans1.outputSequence.clear();
            colTemp.clear();

            // Reconstruct column vector
            for(size_t j = 0; j < rows; j++)
                colTemp.push_back( inputGraph[j][i] );

            trans1.setData(colTemp);
            trans1.execute();
            outputTemp.push_back( trans1.outputSequence );
        }

        // idft every row
        TransformType2 trans2;
        InputList rowTemp;
        for(size_t i = 0; i < rows; i++)
        {
            trans2.inputSequence.clear();
            trans2.outputSequence.clear();
            rowTemp.clear();

            // Reconstruct row vector
            for(size_t j = 0; j < cols; j++)
                rowTemp.push_back( outputTemp[j][i] );
            trans2.setData(rowTemp);
            trans2.execute();
            outputGraph.push_back( trans2.outputSequence );
        }
    }
};
typedef ift2D<  idftComplex,  idftdouble > idft2D;
typedef ift2D< ifft2Complex, ifft2double > ifft2D;

template<typename TransformType>
class dct2DTemplate: public Transform2D<double, double>
{
public:
    dct2DTemplate() {}

    /*
     * Brief: Execute 2D dct
     * Parameter:
     *     None
     * Return:
     *     None
     */
    void execute()
    {
        // dft every row
        TransformType trans1;
        size_t rows = inputGraph.size();
        for(size_t i = 0; i < rows; i++)
        {
            trans1.inputSequence.clear();
            trans1.outputSequence.clear();
            trans1.setData(inputGraph[i]);
            trans1.execute();
            outputGraph.push_back(trans1.outputSequence);
        }

        // dft every column
        TransformType trans2;
        size_t cols = inputGraph[0].size();
        OutputList colTemp;
        for(size_t i = 0; i < cols; i++)
        {
            trans2.inputSequence.clear();
            trans2.outputSequence.clear();
            colTemp.clear();

            // reconstruct column vector
            for(size_t j = 0; j < rows; j++)
                colTemp.push_back( outputGraph[j][i] );

            trans2.setData(colTemp);
            trans2.execute();

            for(size_t j = 0; j < rows; j++)
                outputGraph[j][i] = trans2.outputSequence[j];
        }
    }
};
typedef dct2DTemplate< dct >  dct2D;
typedef dct2DTemplate< idct > idct2D;

/*
 * Brief: Overload operator <<
 * Parameter:
 *     v -- vector of double
 * Return:
 *     Reference of ostream
 */
template<typename T>
std::ostream& operator << ( std::ostream& out, const std::vector< T >& v)
{
    for(size_t i = 0; i < v.size(); i++)
    {
        out << v[i] << std::endl;
    }
    return out;
}

#endif
