#include <iostream>
#include <vector>
#include "transform.h"
#include "bitmap.h"
#include "imageTransform.h"
//#include "test.h"

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
    const string inputFileName = argv[1];
    if(inputFileName == "-h")
    {
        printHelp();
        return 0;
    }

    // Load file and check validation
    BitMap image( inputFileName );  // BitMap image = load(inputFileName);
    if(!image.formatCheck())
    {
        cerr << "Input file is not a bmp image." << endl;
        return -1;
    }

    // 0.0 Convert RGB to YCbCr color space
    image.RGB2YCbCr();

    // 0.1 Break image into 8*8 blocks
    LONG blockSize = 8;
    ImageBlockVector YBlocks;
    BLOCKEDIMAGESIZE YBlockedSize;
    YBlockedSize = breakBMPImage( image, YBlocks, blockSize );
    cout << "First Block" << endl;    
    
    // 1.1 DFT image blocks
    cout << "Implementing Discrete Fourier Transform..." << endl;
    ImageBlockVectorComplex Y_FreqDomain;
    fftBlocks( YBlocks, Y_FreqDomain );
    cout << "Done" << endl << endl;
    visualizeTransform( Y_FreqDomain, YBlockedSize );

    // 1.2 IDFT image blocks with Amplitude only
    cout << "Implementing Inverse Discrete Fourier Transform..." << endl;
    ImageBlockVector YBlocksReconstructed_Amp;
    ImageBlockVectorComplex Y_Amp;
    extractComponent( Y_FreqDomain, Y_Amp, 1 );// Phase, Amplitude
    ifftBlocks( Y_Amp, YBlocksReconstructed_Amp );
    cout << "Done" << endl << endl;

    cout << "Displaying reconstructed image with Amplitude only..." << endl;
    reconstructImage( YBlocksReconstructed_Amp, YBlockedSize );
    cout << "Done" << endl << endl;

    // 1.3 IDFT image blocks with Phase only
    cout << "Implementing Inverse Discrete Fourier Transform..." << endl;
    ImageBlockVector YBlocksReconstructed_Pha;
    ImageBlockVectorComplex Y_Pha;
    extractComponent( Y_FreqDomain, Y_Pha, 2 );// Phase, Amplitude
    ifftBlocks( Y_Pha, YBlocksReconstructed_Pha );
    cout << "Done" << endl << endl;

    cout << "Displaying reconstructed image with Phase only..." << endl;
    reconstructImage( YBlocksReconstructed_Pha, YBlockedSize );
    cout << "Done" << endl << endl;

    // 2.1 DCT image blocks
    cout << "Implementing Discrete Cosine Transform..." << endl;
    ImageBlockVectordouble Y_CosDomain;
    dctBlocks( YBlocks, Y_CosDomain );
    cout << "Done" << endl << endl;

    // 2.2 IDCT image blocks
    for(int i = 0; i < 40; i++) {
    cout << "Implementing Inverse Discrete Cosine Transform..." << endl;
    ImageBlockVector YBlocksReconstructed;
    idctBlocks( Y_CosDomain, YBlocksReconstructed, int(blockSize*blockSize * (0.02*i)+0.5) );
    cout << 0.02*i <<", " << int(blockSize*blockSize * (0.02*i)+0.5) << endl;
    cout << "Done" << endl << endl;

    cout << "Displaying reconstructed image..." << endl;
    reconstructImage( YBlocksReconstructed, YBlockedSize );
    cout << "Done" << endl << endl;
    }

    return 0;
}


