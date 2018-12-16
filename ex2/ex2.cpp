#include <iostream>
#include <vector>
#include "transform.h"
#include "bitmap.h"
#include "imageTransform.h"
//#include "test.h"

#define DEBUG
#include <chrono>
#include <typeinfo>

using namespace std;

vector<Complexd> idata;
vector<Complexd> idatai;
vector<double> tempidata;
vector<vector<Complexd>> iidata;
vector<vector<double>> tempdata;
vector<vector<double>> tempdatadct;

template<typename TRANS, typename inputDataType, typename outputDataType>
void Trans( std::vector<inputDataType> inputdata, std::vector<outputDataType>& outputdata )
{
    TRANS trans;
    std::cout << "-- " << typeid(trans).name() << " --" << std::endl;
    std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();

    trans.setData(inputdata);
    trans.execute();

    std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
    std::chrono::duration<double> time_used = std::chrono::duration_cast<std::chrono::duration<double>>( t2-t1 );
    std::cout << "Time consumption: " << time_used.count()*1000 << " ms" << std::endl;

#ifdef DEBUG
    trans.displayResult();
#endif

outputdata.clear();
    // for(size_t i = 0; i < trans.outputSequence.size(); i++)
    // {
    //     outputdata.push_back(trans.outputSequence[i]);
    // }

    for(size_t i = 0; i < trans.outputGraph.size(); i++)
    {
            outputdata.push_back(trans.outputGraph[i]);
    }
}

/******************
 * Brief: (数字图像处理-->实验二) Fast Fourier transform and discrete cosine transform
 * Parameters:
 *     argv[1] -- data file name
 * Author:
 *     Ding Qingpeng (丁庆鹏, Student No. 18B903053)
 ******************/
int main(int argc, char const *argv[])
{
    const string inputFileName = argv[1];
    if(inputFileName == "-h")
    {
        printHelp();
        return 0;
    }

    vector<double> datad  = { 15, 32, 9, 222, 118, 151, 5, 7, 56, 233, 56, 121, 235, 89, 98, 111 };
    vector<double> datad1 = { 9, 222, 118, 151, 5, 7, 56, 233, 56, 121, 235, 89, 98, 111, 15, 32 };
    vector<double> datad2 = { 118, 151, 5, 7, 56, 233, 56, 121, 235, 89, 98, 111, 15, 32, 9, 222 };
    
    // for(size_t i = 0; i < 25; i++)
    // {
    //     datad.push_back( sin(i) );
    // }
    
    // vector<double> datad = { 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0 };
    vector<Complexd> dataComplex;
    for(size_t i = 0; i < datad.size(); i++)
        dataComplex.push_back( Complexd(datad[i]) );

    // Trans<fft2Complex, Complexd, Complexd>(dataComplex, idata);
    // Trans<ifft2double, Complexd, double>(idata, tempidata);
    // Trans<dftComplex, Complexd, Complexd>(dataComplex, idata);
    
    // for(size_t i = 0; i < idata.size(); i++)
    // {
    //     cout << idata[i].abs() << endl;
    // }
    
//    Trans<idftdouble, Complexd>(idata);
    // // Trans<fdct>(data);
    // Trans<dct, double>(datad);
    // // Trans<idct, double>(idata);

    vector< vector<double> > data2D;

    // for(size_t i = 0; i < 3; i++)
    // {
    //     data2D.push_back(datad);
    // }
    data2D.push_back( datad );
    data2D.push_back( datad1 );
    data2D.push_back( datad2 );

    // dft2D trans_dft2D;
    // trans_dft2D.setData(data2D);
    // cout << "-- dft-- " << endl;
    // Trans<dft2D, vector<double>, vector<Complexd>>(data2D, iidata);
    // cout << "-- inverse dft-- " << endl;
    // Trans<idft2D, vector<Complexd>, vector<double>>( iidata, tempdata );

    // fft2D trans_fft2D;
    // trans_fft2D.setData(data2D);
    // cout << "-- fft-- " << endl;
    // Trans<fft2D, vector<double>, vector<Complexd>>(data2D, iidata);
    // cout << "-- inverse fft-- " << endl;
    // Trans<ifft2D, vector<Complexd>, vector<double>>( iidata, tempdata );

    // dct2D trans_dct2D;
    // trans_dct2D.setData(data2D);
    // cout << "-- dct-- " << endl;
    // Trans<dct2D, vector<double>, vector<double>>(data2D, tempdata);
    // cout << "-- inverse dct-- " << endl;
    // Trans<idct2D, vector<double>, vector<double>>( tempdata, tempdatadct );

    // cout << "---------------------------------------------" << endl;
    // cout << "---------------------------------------------" << endl;


    // Load file and check validation
    BitMap image( inputFileName );  // BitMap image = load(inputFileName);
    if(!image.formatCheck())
    {
        cerr << "Input file is not a bmp image." << endl;
        return -1;
    }

    // Convert RGB to YCbCr color space
    image.RGB2YCbCr();

    // Break image into 8*8 blocks
    ImageBlockVector YBlocks;
    BLOCKEDIMAGESIZE YBlockedSize;
    YBlockedSize = breakBMPImage( image, YBlocks, 512 );
    cout << "First Block" << endl;    
    

    // DFT image blocks
    cout << "Implementing Discrete Fourier Transform..." << endl;
    ImageBlockVectorComplex Y_FreqDomain;
    fftBlocks( YBlocks, Y_FreqDomain );
    cout << "Done" << endl << endl;
    visualizeTransform( Y_FreqDomain, YBlockedSize );

    // IDFT image blocks
    cout << "Implementing Inverse Discrete Fourier Transform..." << endl;
    ImageBlockVector YBlocksReconstructed;
    ImageBlockVectorComplex Y_Extracted;
    extractComponent( Y_FreqDomain, Y_Extracted, 2 );// Phase, Amplitude
    ifftBlocks( Y_Extracted, YBlocksReconstructed );
    cout << "Done" << endl << endl;

    cout << "Displaying reconstructed image..." << endl;
    reconstructImage( YBlocksReconstructed, YBlockedSize );
    cout << "Done" << endl << endl;

    return 0;
}


