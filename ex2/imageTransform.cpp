#include <iostream>
#include <vector>
#include <chrono>
#include "transform.h"
#include "bitmap.h"
#include "imageTransform.h"

#define PI 3.1415926

/********Only used for displaying image**********/
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
// using namespace cv;
/********Only used for displaying image**********/

/******************
 * Brief: (数字图像处理-->实验二) Image transform functions
 * Author:
 *     Ding Qingpeng (丁庆鹏, Student No. 18B903053)
 ******************/

BLOCKEDIMAGESIZE breakBMPImage( BitMap image, ImageBlockVector& blockData, int blockSize, int channel )
{
    LONG rows = image.iHeader->biHeight / blockSize;
    LONG cols = image.iHeader->biWidth  / blockSize;

    ImageBlock oneBlock;
    PixelVector oneVector;
    for(LONG i = 0; i < rows; i++)          // Break each row
    {
        for(LONG j = 0; j < cols; j++)      // Break each column
        {
            oneBlock.clear();
            for(LONG k = 0; k < blockSize; k++)      // Row in block
            {
                oneVector.clear();
                for(LONG l = 0; l < blockSize; l++)  // Column in block
                {
                    IMAGEDATA pixel = image.at( i*blockSize+k, j*blockSize+l );
                    BYTE pixelChannel;
                    switch (channel)
                    {
                    case 1:
                        pixelChannel = pixel.Red;
                        break;
                    case 2:
                        pixelChannel = pixel.Green;
                        break;
                    case 3:
                        pixelChannel = pixel.Blue;
                        break;
                    default:
                        break;
                    }
                    oneVector.push_back( pixelChannel );
                }
                oneBlock.push_back( oneVector );
            }
            blockData.push_back( oneBlock );
        }
    }

    BLOCKEDIMAGESIZE blockedImageSize = { rows, cols };
    return blockedImageSize;
}

template<typename TransformType>
void ftSingleBlock( const ImageBlock& originalData, ImageBlockComplex& transformedData, bool shift )
{
    LONG rows = originalData.size();

    // Type conversion
    std::vector< std::vector<double> > originalDatadouble(rows);
    for(LONG i = 0; i < rows; i++)
        originalDatadouble[i].assign( originalData[i].begin(), originalData[i].end() );

    // Shift
    if( shift )
        for(LONG i = 0; i < rows; i++) {
            for(LONG j = 0; j < rows; j++) {
                originalDatadouble[i][j] *= pow(-1, i+j);
            }
        }
    
    // Implement ft
    TransformType trans;
    trans.setData( originalDatadouble );
    trans.execute();
    transformedData.assign( trans.outputGraph.begin(), trans.outputGraph.end() );
}

void dftBlocks( const ImageBlockVector& originalData, ImageBlockVectorComplex& transformedData, bool printTimeConsumption )
{
    std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
    ImageBlockComplex oneBlockComplex;
    for(int i = 0; i < originalData.size(); i++)
    {
        ftSingleBlock<dft2D>( originalData[i], oneBlockComplex );
        transformedData.push_back( oneBlockComplex );
    }
    std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
    std::chrono::duration<double> time_used = std::chrono::duration_cast<std::chrono::duration<double>>( t2-t1 );

    if( printTimeConsumption )
        std::cout << "-- Time consumption: " << time_used.count()*1000 << " ms" << std::endl;
}

void fftBlocks( const ImageBlockVector& originalData, ImageBlockVectorComplex& transformedData, bool printTimeConsumption )
{
    std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
    ImageBlockComplex oneBlockComplex;
    for(int i = 0; i < originalData.size(); i++)
    {
        ftSingleBlock<fft2D>( originalData[i], oneBlockComplex );
        transformedData.push_back( oneBlockComplex );
        if ( i == 0 )
            findMaxAmpInBlock(oneBlockComplex);
    }

    
    // for(size_t i = 0; i < transformedData.size(); i++)
    // {
    //     std::cout << transformedData[i][0][0] << std::endl;
    // }
    

    std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
    std::chrono::duration<double> time_used = std::chrono::duration_cast<std::chrono::duration<double>>( t2-t1 );

    if( printTimeConsumption )
        std::cout << "-- Time consumption: " << time_used.count()*1000 << " ms" << std::endl;
}

template<typename TransformType>
void iftSingleBlock( const ImageBlockComplex& transformedData, ImageBlock& originalData, bool shift )
{
    LONG rows = transformedData.size();

    // Implement ift
    TransformType trans;
    trans.setData( transformedData );
    trans.execute();
    // originalData.assign( trans.outputGraph.begin(), trans.outputGraph.end() );

    // Shift back
    if( shift )
        for(LONG i = 0; i < rows; i++) {
            for(LONG j = 0; j < rows; j++) {
                trans.outputGraph[i][j] *= pow(-1, i+j);
            }
        }

    double max = findMaxInBlock( trans.outputGraph );
    double min = findMinInBlock( trans.outputGraph );

    // Type conversion
    std::vector< BYTE > originalDataBYTE(rows);
    for(LONG i = 0; i < rows; i++){
        // originalDataBYTE.clear();
        for(LONG j = 0; j < rows; j++) {
            double current = trans.outputGraph[i][j];
            current = ( current - min ) / ( max - min ) * 255.0;
            originalDataBYTE[j] = (BYTE)( current + 0.5 );///5500.0*255.0
            // std::cout << (int)originalDataBYTE[j] << std::endl;
            // std::cout << current << std::endl;
        }
        originalData.push_back( originalDataBYTE );
    }
}

void idftBlocks( const ImageBlockVectorComplex& transformedData, ImageBlockVector& originalData, bool printTimeConsumption )
{
    std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
    ImageBlockBYTE oneBlockBYTE;
    for(int i = 0; i < transformedData.size(); i++)
    {
        oneBlockBYTE.clear();
        iftSingleBlock<idft2D>( transformedData[i], oneBlockBYTE );
        originalData.push_back( oneBlockBYTE ); 
    }
    std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
    std::chrono::duration<double> time_used = std::chrono::duration_cast<std::chrono::duration<double>>( t2-t1 );

    if( printTimeConsumption )
        std::cout << "-- Time consumption: " << time_used.count()*1000 << " ms" << std::endl;
}

void ifftBlocks( const ImageBlockVectorComplex& transformedData, ImageBlockVector& originalData, bool printTimeConsumption )
{
    std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
    ImageBlockBYTE oneBlockBYTE;
    for(int i = 0; i < transformedData.size(); i++)
    // for(int i = 0; i < 1; i++)
    {
        oneBlockBYTE.clear();
        iftSingleBlock<ifft2D>( transformedData[i], oneBlockBYTE );
        originalData.push_back( oneBlockBYTE );        
    }

    std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
    std::chrono::duration<double> time_used = std::chrono::duration_cast<std::chrono::duration<double>>( t2-t1 );

    if( printTimeConsumption )
        std::cout << "-- Time consumption: " << time_used.count()*1000 << " ms" << std::endl;
}

void visualizeTransform( ImageBlockVectorComplex& transformedData, BLOCKEDIMAGESIZE imageSize )
{   
    int blockSize = transformedData[0].size();
    LONG rows = blockSize * imageSize.rows;
    LONG cols = blockSize * imageSize.cols;
    double AMPfactor;
    double PHAfactor;
    
    // Create single channel Mat
    cv::Mat amplitudeImage( rows, cols, CV_8UC1 );
    cv::Mat     phaseImage( rows, cols, CV_8UC1 );

    // Construct image
    for(LONG i = 0; i < rows; i++)
    {
        // std::cout << "--> i = " << i << std::endl;
        for(LONG j = 0; j < cols; j++)
        {
            int rowofBlock = i / blockSize;
            int colofBlock = j / blockSize;
            int rowinBlock = i % blockSize;
            int colinBlock = j % blockSize;
            int index1 = rowofBlock * imageSize.cols + colofBlock;
            int index2 = rowinBlock;
            int index3 = colinBlock;
            // std::cout << rowofBlock << std::endl;
            // std::cout << index1 << ", " << index2 << ", " << index3 << std::endl;
            ComplexDFT cPoint = transformedData[ index1 ][ index2 ][ index3 ];
            // std::cout << cPoint.abs() << std::endl;

            // Compute normalize factor
            AMPfactor = 255.0 / log( transformedData[ index1 ][ blockSize/2 ][ blockSize/2 ].abs() ); //std::cout << cPoint.abs() << std::endl;
            PHAfactor = 255.0 / PI;

            amplitudeImage.ptr<BYTE>(i)[j] = (BYTE)( log( cPoint.abs() ) * AMPfactor + 0.5 );//log( cPoint.abs() )
                phaseImage.ptr<BYTE>(i)[j] = (BYTE)( cPoint.pha() * PHAfactor + 0.5 );
            // std::cout << (int)amplitudeImage.ptr<BYTE>(i)[j] << std::endl;
                phaseImage.ptr<BYTE>(i)[j] = (BYTE)cPoint.pha();
            // std::cout << log( cPoint.abs() ) << ", "<< (int)amplitudeImage.ptr<BYTE>(i)[j] << ", " << std::endl;
            // std::cout << cPoint.pha()                << ", "<< (int)phaseImage.ptr<BYTE>(i)[j] << ", " << std::endl;
        }
        // std::cout << std::endl;
    }
    
    cv::imshow("Amplitude", amplitudeImage);
    cv::waitKey(0);
    cv::imshow("Phase", phaseImage);
    cv::waitKey(0);
}

void extractComponent( ImageBlockVectorComplex& transformedData, ImageBlockVectorComplex& extraction, int component )
{
    int blockSize = transformedData[0].size();
    std::vector<Complexd> oneRowExtraction;
    ImageBlockComplex oneBlockExtraction;
    for(LONG i = 0; i < transformedData.size(); i++)
    {
        oneBlockExtraction.clear();
        for(LONG j = 0; j < blockSize; j++)
        {
            oneRowExtraction.clear();
            for(LONG k = 0; k < blockSize; k++)
            {
                double norm  = transformedData[i][j][k].abs();
                double phase = transformedData[i][j][k].pha();
                switch (component)
                {
                    case 1:
                        oneRowExtraction.push_back( ComplexDFT( norm, 0.0 ) );
                        break;
                    case 2:
                        oneRowExtraction.push_back( ComplexDFT( 255 * cos(phase), 255 * sin(phase) ) );
                        break;
                    default:
                        oneRowExtraction.push_back( transformedData[i][j][k] );
                        break;
                }
            }
            oneBlockExtraction.push_back( oneRowExtraction );
        }
        extraction.push_back( oneBlockExtraction );
    }
}

void reconstructImage( const ImageBlockVectorBYTE& reconstructedBlocks, BLOCKEDIMAGESIZE imageSize )
{
    int blockSize = reconstructedBlocks[0].size();
    LONG rows = blockSize * imageSize.rows;
    LONG cols = blockSize * imageSize.cols;

    // Create single channel Mat
    cv::Mat image( rows, cols, CV_8UC1 );

    // Reonstruct image
    for(LONG i = 0; i < rows; i++)
    {
        for(LONG j = 0; j < cols; j++)
        {
            int rowofBlock = i / blockSize;
            int colofBlock = j / blockSize;
            int rowinBlock = i % blockSize;
            int colinBlock = j % blockSize;
            int index1 = rowofBlock * imageSize.cols + colofBlock;
            int index2 = rowinBlock;
            int index3 = colinBlock;
            image.ptr<BYTE>(i)[j] = reconstructedBlocks[ index1 ][ index2 ][ index3 ];
        }
    }
    cv::imshow("Reconstructed image", image);
    cv::waitKey(0);
}

void findMaxAmpInBlock( ImageBlockComplex originalData )
{
    double max = 0.0;
    int posx = 0, posy = 0;
    
    for(size_t i = 0; i < originalData.size(); i++)
    {
        
        for(size_t j = 0; j < originalData.size(); j++)
        {
            
            if ( max < originalData[i][j].abs() ) {
                max = originalData[i][j].abs();
                posx = i;
                posy = j;
            }
            
        }
        
    }
    std::cout << "pos = (" << posx << ", " << posy << std::endl;
}

double findMaxInBlock( std::vector< std::vector<double> > originalData )
{
    double max = 0.0;
    int posx = 0, posy = 0;
    
    for(size_t i = 0; i < originalData.size(); i++)
    {
        
        for(size_t j = 0; j < originalData.size(); j++)
        {
            
            if ( max < originalData[i][j] ) {
                max = originalData[i][j];
                posx = i;
                posy = j;
            }
            
        }
        
    }
    std::cout << "pos = (" << posx << ", " << posy << ")" << std::endl;
    std::cout << "max = " << max << std::endl;
    return max;
}

double findMinInBlock( std::vector< std::vector<double> > originalData )
{
    double min = 10000.0;
    int posx = 0, posy = 0;
    
    for(size_t i = 0; i < originalData.size(); i++)
    {
        
        for(size_t j = 0; j < originalData.size(); j++)
        {
            
            if ( min > originalData[i][j] ) {
                min = originalData[i][j];
                posx = i;
                posy = j;
            }
            
        }
        
    }
    std::cout << "pos = (" << posx << ", " << posy << ")" << std::endl;
    std::cout << "min = " << min << std::endl;
    return min;
}
