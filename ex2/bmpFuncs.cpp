#include <iostream>
#include <vector>
#include <fstream>
#include <cstring>
#include <math.h>
#include <cstdio>
#include "bitmap.h"
#define pi 3.1415926

/********Only used for displaying BMP file**********/
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
using namespace cv;
/********Only used for displaying BMP file**********/

/****************** 
 * Brief: Functions operating bitmap image
 * Author:
 *     Ding Qingpeng (丁庆鹏, Student No. 18B903053)
 ******************/

using namespace std;

void printHelp()
{
    cout << "Help: bitread FileName [ColorSpace]" << endl;
    cout << "FileName:   name of image file" << endl;
    cout << "ColorSpace: -RGB" << endl;
    cout << "            -YIQ" << endl;
    cout << "            -HSI" << endl;
    cout << "            -YCbCr" << endl;
    cout << "            -XYZ" << endl;
}

std::ostream& operator << ( std::ostream& out, IMAGEDATA data)
{
    out << "Red:   " << (int)data.Red << endl;
    out << "Green: " << (int)data.Green << endl;
    out << "Blue:  " << (int)data.Blue << endl;
    return out;
}

BitMap::BitMap(const std::string& fileName)
{
    // Open file and check if file is valid
    ifstream infile( fileName, ios::in | ios::binary );
    if(!infile)
    {
        cerr << "Failed to open file: " << fileName << endl;
    }

    // Read header data in binary format
    infile.read((char*)(fHeader = new(BITMAPFILEHEADER)), sizeof(BITMAPFILEHEADER));
    infile.read((char*)(iHeader = new(BITMAPINFOHEADER)), sizeof(BITMAPINFOHEADER));

    // Read palette data
    size_t height = iHeader->biHeight;
    size_t width = iHeader->biWidth;
    size_t blockSize = sizeof(IMAGEDATA) * height * width;
    int paletteSize = fHeader->bfOffBits - sizeof(BITMAPFILEHEADER) - sizeof(BITMAPINFOHEADER);
    usePalette = (paletteSize == 0)? false: true;

    // Read image data
    imageData = (IMAGEDATA*)malloc(blockSize);

    // If usePalette == true, reassign RGB values to imageData
    if( usePalette )
    {
        infile.read((char*)(rgbQuad = (RGBQUAD*)malloc(paletteSize)), paletteSize);
        BYTE* ptemp = (BYTE*)malloc(blockSize);
        infile.read((char*)ptemp, blockSize);
        for(size_t i = 0; i < height; i++)
            for(size_t j = 0; j < width; j++)
            {
                int index = i * width + j;
                RGBQUAD temp = rgbQuad[ ptemp[index] ];
                // cout << "-->" << i*width + j << endl;
                imageData[index].Red   = temp.rgbRed;
                imageData[index].Green = temp.rgbGreen;
                imageData[index].Blue  = temp.rgbBlue;
            }
        free(ptemp);
    }
    // If usePalette == false, read image data directly
    else
    {
        rgbQuad = NULL;
        infile.read((char*)imageData, blockSize);
    }

    // Default color space type
    colorSpace = RGB;

    infile.close();
}

BitMap::BitMap(const BitMap& image)
{
    int paletteSize = image.fHeader->bfOffBits - sizeof(BITMAPFILEHEADER) - sizeof(BITMAPINFOHEADER);
    this->usePalette = image.usePalette;
    size_t pixelCounts = (image.iHeader->biWidth) * (image.iHeader->biHeight);
    size_t blockSize = sizeof(IMAGEDATA) * pixelCounts;

    // Allocate memory for new image
    this->fHeader = new(BITMAPFILEHEADER);
    this->iHeader = new(BITMAPINFOHEADER);
    
    // this->rgbQuad = new(RGBQUAD);
    this->imageData = (IMAGEDATA*)malloc(blockSize);

    // Copy memory
    memcpy(this->fHeader, image.fHeader, sizeof(BITMAPFILEHEADER));
    memcpy(this->iHeader, image.iHeader, sizeof(BITMAPINFOHEADER));
    if( usePalette )
    {
        this->rgbQuad = (RGBQUAD*)malloc(paletteSize);
        memcpy(this->rgbQuad, image.rgbQuad, paletteSize);
    }
    else
        this->rgbQuad = NULL;
    memcpy(this->imageData, image.imageData, blockSize);

    // Default color space type
    colorSpace = RGB;
}

bool BitMap::formatCheck()
{    
    return (fHeader->bfType == 0x4d42)? true: false;
}

void BitMap::showFileHeader()
{
    cout << "-- File Header --" << endl;
    cout << "File type:       " << hex << "0x" << fHeader->bfType << dec << endl;
    cout << "File size:       " << fHeader->bfSize << " Bytes" << endl;
    cout << "Reserved word 1: " << fHeader->bfReserved1 << endl;
    cout << "Reserved word 2: " << fHeader->bfReserved2 << endl;
    cout << "Offset bytes:    " << fHeader->bfOffBits << endl << endl;
}

void BitMap::showInfoHeader()
{
    cout << "-- Info Header --" << endl;
    cout << "Size of info header:      " << iHeader->biSize << " Bytes"<< endl;
    cout << "Image width:              " << iHeader->biWidth << endl;
    cout << "Image height:             " << iHeader->biHeight << endl;
    cout << "Plane counts:             " << iHeader->biPlanes << endl;
    cout << "Bit counts of each pixel: " << iHeader->biBitCounts << endl;
    cout << "Compression type:         " << iHeader->biCompression << endl;
    cout << "Size of compressed image: " << iHeader->biSizeImage << " Bytes" << endl;
    cout << "Oriental resolution:      " << iHeader->biXpelsPerMeter << " p/m" << endl;
    cout << "Vertical resolution:      " << iHeader->biYpelsPerMeter << " p/m" << endl;
    cout << "Colors used in bitmap:    " << iHeader->biClrUsed << endl;
    cout << "Important colors:         " << iHeader->biClrImportant << endl << endl;
}

IMAGEDATA BitMap::at(size_t row, size_t col)
{
    size_t rowInBmp = iHeader->biHeight -1 - row;
    if(rowInBmp < 0 || rowInBmp >= iHeader->biHeight || col < 0 || col >= iHeader->biWidth )
        throw "-- Out of range";
    
    if( usePalette )
        return *(IMAGEDATA*)&(rgbQuad[ ((BYTE*)imageData + rowInBmp*iHeader->biWidth)[col] ]);
    else
        return imageData[ rowInBmp*iHeader->biWidth + col ];
}

void BitMap::RGB2YIQ()
{
    size_t pixelCounts = (iHeader->biWidth) * (iHeader->biHeight);
    for(size_t i = 0; i < pixelCounts; ++i)
    {
        // Compute Y, I and Q value
        BYTE Y = BYTE(0.299 * imageData[i].Red + 0.587 * imageData[i].Green + 0.114 * imageData[i].Blue);
        BYTE I = BYTE(0.596 * imageData[i].Red - 0.275 * imageData[i].Green - 0.321 * imageData[i].Blue);
        BYTE Q = BYTE(0.212 * imageData[i].Red - 0.523 * imageData[i].Green - 0.311 * imageData[i].Blue);

        // Assign YIQ to RBG
        imageData[i].Red   = Y;
        imageData[i].Green = I;
        imageData[i].Blue  = Q;
    }
    colorSpace = YIQ;
}

void BitMap::RGB2HSI()
{
    size_t pixelCounts = (iHeader->biWidth) * (iHeader->biHeight);
    for(size_t i = 0; i < pixelCounts; ++i)
    {
        // Compute r, g and b value
        int t = imageData[i].Red + imageData[i].Green + imageData[i].Blue;
        double r = (double)imageData[i].Red   / (double)t;
        double g = (double)imageData[i].Green / (double)t;
        double b = (double)imageData[i].Blue  / (double)t;

        // Compute h, s and i value
        double h = 0;
        if (b <= g)
            h = acos( 0.5*(r-g+r-b) / sqrt( (r-g)*(r-g)+(r-b)*(g-b) ) );
        else
            h = 2*pi - acos( 0.5*(r-g+r-b) / sqrt( (r-g)*(r-g)+(r-b)*(g-b) ) );
        double s = 1 - 3 * min( min(r, g), b );
        double ii = t / (3.0 * 255);

        // Compute H, S and I value
        BYTE H = BYTE(h * 180 / pi);
        BYTE S = BYTE(s * 100);
        BYTE I = BYTE(ii * 255);

        // Assign HSI to RBG
        imageData[i].Red   = H;
        imageData[i].Green = S;
        imageData[i].Blue  = I;
    }
    colorSpace = HSI;
}

void BitMap::RGB2YCbCr()
{
    size_t pixelCounts = (iHeader->biWidth) * (iHeader->biHeight);
    for(size_t i = 0; i < pixelCounts; ++i)
    {
        // Compute Y, Cb and Cr value
        BYTE Y  = BYTE( 0.299 * imageData[i].Red + 0.587 * imageData[i].Green + 0.114 * imageData[i].Blue);
        BYTE Cb = BYTE(-0.169 * imageData[i].Red - 0.331 * imageData[i].Green + 0.5   * imageData[i].Blue);
        BYTE Cr = BYTE( 0.5   * imageData[i].Red - 0.419 * imageData[i].Green - 0.081 * imageData[i].Blue);

        // Assign YCbCr to RBG
        imageData[i].Red   = Y;
        imageData[i].Green = Cb;
        imageData[i].Blue  = Cr;
    }
    colorSpace = YCbCr;
}

void BitMap::RGB2XYZ()
{
    size_t pixelCounts = (iHeader->biWidth) * (iHeader->biHeight);
    double M[3][3] = { 0.436052025, 0.385081593, 0.143087414,
                       0.222491598, 0.716886060, 0.060621486,
                       0.013929122, 0.097097002, 0.714185470 };
    for(size_t i = 0; i < pixelCounts; ++i)
    {
        // Compute R, G and B value
        double R = gammaX( imageData[i].Red   / 255.0 );
        double G = gammaX( imageData[i].Green / 255.0 );
        double B = gammaX( imageData[i].Blue  / 255.0 );

        // Compute X, Y and Z value
        BYTE X = BYTE(100 * (M[0][0] * R + M[0][1] * G + M[0][2] * B ));
        BYTE Y = BYTE(100 * (M[1][0] * R + M[1][1] * G + M[1][2] * B ));
        BYTE Z = BYTE(100 * (M[2][0] * R + M[2][1] * G + M[2][2] * B ));

        // Assign YCbCr to RBG
        imageData[i].Red   = X;
        imageData[i].Green = Y;
        imageData[i].Blue  = Z;
    }
    colorSpace = XYZ;
}

int BitMap::getcSpace()
{
    vector<string> vName = { "RGB", "YIQ", "HSI", "YCbCr", "XYZ" };
    cout << "Color space: " << vName[colorSpace-1] << endl;
    return colorSpace;
}

double BitMap::gammaX(double x)
{
    return (x > 0.04045)? pow( (x+0.055)/1.055, 2.4 ): (x / 12.92);
}

void write(const std::string newFileName, BitMap& image, int flag)
{
    // Open file and check if file is valid
    ofstream outfile( newFileName, ios::out | ios::binary );
    if(!outfile)
    {
        cerr << "Failed to open file: " << newFileName << endl;
    }

    // Allocate memory for new image
    size_t pixelCounts = (image.iHeader->biWidth) * (image.iHeader->biHeight);
    size_t blockSize = sizeof(IMAGEDATA) * pixelCounts;

    //! [Edit pixel values]
    if(flag)
        for(size_t i = 0; i < pixelCounts; ++i)
        {
            image.imageData[i].Red   = 0;
            image.imageData[i].Green /= 2;
            image.imageData[i].Blue  /= 2;
        }
    //! [Edit pixel values]

    // Write data to file
    outfile.write((char*)image.fHeader, sizeof(BITMAPFILEHEADER));
    outfile.write((char*)image.iHeader, sizeof(BITMAPINFOHEADER));
    outfile.write((char*)image.imageData, blockSize);
    outfile.close();
}

BitMap load(const std::string fileName)
{
    BitMap image ( fileName );
    return image;
}

void displayColorSpace(const BitMap& image, int cSpace)
{
    BitMap temp = image;
    switch (cSpace)
    {
        case 1:
            break;
        case 2:
            temp.RGB2YIQ();
            break;
        case 3:
            temp.RGB2HSI();
            break;
        case 4:
            temp.RGB2YCbCr();
            break;
        case 5:
            temp.RGB2XYZ();
            break;
        default:
            break;
    }
    retrieveComponent(temp, cSpace);
}

void retrieveComponent(const BitMap& image, int cSpace)
{
    vector<BitMap> vTemp = {image, image, image};
    vector< vector<string> > vName = {
                            { "R", "G", "B" },
                            { "Y", "I", "Q" },
                            { "H", "S", "I" },
                            { "Y", "Cb", "Cr" },
                            { "X", "Y", "Z" } };
    vector<string> name = vName[cSpace-1];
    
    // Split channels
    size_t pixelCounts = (image.iHeader->biWidth) * (image.iHeader->biHeight);
    for(size_t i = 0; i < pixelCounts; ++i)
    {
        vTemp[0].imageData[i].Red   = image.imageData[i].Red;
        vTemp[0].imageData[i].Green = 0;
        vTemp[0].imageData[i].Blue  = 0;

        vTemp[1].imageData[i].Red   = 0;
        vTemp[1].imageData[i].Green = image.imageData[i].Green;
        vTemp[1].imageData[i].Blue  = 0;

        vTemp[2].imageData[i].Red   = 0;
        vTemp[2].imageData[i].Green = 0;
        vTemp[2].imageData[i].Blue  = image.imageData[i].Blue;
    }
    
    // Display channels
    for(int i = 0; i < name.size(); i++)
    {
        namedWindow(name[i]);
        moveWindow(name[i], i * image.iHeader->biWidth, 50);
        bmpshow(name[i], vTemp[i], i+1);
    }
    waitKey(0);
}

void bmpshow(const std::string& windowName, const BitMap& image, int channel)
{
    // Compute image size parameters
    size_t width  = image.iHeader->biWidth;
    size_t height = image.iHeader->biHeight;
    size_t pixelCounts = (image.iHeader->biWidth) * (image.iHeader->biHeight);
    size_t blockSize = sizeof(IMAGEDATA) * pixelCounts;

    if (channel == 0)
    {
        // Create (OpenCV) Mat type data
        Mat bmp(height, width, CV_8UC3);

        // Resort image data, flip along x(width direction) axis
        // And copy image data to Mat
        for(size_t i = 0; i < height; i++)
        memcpy(bmp.ptr<IMAGEDATA>(i), image.imageData + (height-1-i)*width, sizeof(IMAGEDATA) * width);
        imshow(windowName, bmp);
    }
    else if(channel == 1 || channel == 2 || channel == 3)
    {
        // Create single channel Mat
        Mat bmp(height, width, CV_8UC1);

        // Resort image data, flip along x(width direction) axis
        // And copy single channel data to Mat
        for(size_t i = 0; i < height; i++)
            for(size_t j = 0; j < width; j++)
            {
                IMAGEDATA temp = (image.imageData + (height-1-i)*width)[j];
                bmp.ptr<BYTE>(i)[j] = ((BYTE*)&temp)[3-channel];
            }
        imshow(windowName, bmp);
    }
    else    cout << "Wrong channel" << endl;
}
