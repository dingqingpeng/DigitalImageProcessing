#ifndef BITMAP_H_
#define BITMAP_H_

#include <iostream>

/****************** 
 * Brief: Bitmap file structures and class
 * Author:
 *     Ding Qingpeng (丁庆鹏, Student No. 18B903053)
 ******************/

#pragma pack(push)
#pragma pack(1)     // Note: Aligns structure members on min(1-byte, element size) boundary,
                    //       or say, turn off memory alignment.
                    //       If not, sizeof(tagBITMAPFILEHEADER) would be 16 instead of 14.
typedef unsigned char  BYTE;
typedef unsigned short WORD;
typedef unsigned int   DWORD;
typedef unsigned int   LONG;    // Note: biXpelsPerMeter & biYpelsPerMeter actually each occupies 4 bytes,
                                //       so "unsigned int" should be used instead of "long".

/* 
 * Brief: bitmap file header struct, 14 bytes in total
 */
typedef struct tagBITMAPFILEHEADER
{
    WORD  bfType;       // File type
    DWORD bfSize;       // File size
    WORD  bfReserved1;  // Reserved word 1
    WORD  bfReserved2;  // Reserved word 2
    DWORD bfOffBits;    // Offset bits from file header to image data
                        // Why "bfOffBits" instead of "bfOffBytes"?
}BITMAPFILEHEADER;

/* 
 * Brief: info header of this bitmap file, 40 bytes in total
 */
typedef struct tagBITMAPINFOHEADER
{
    DWORD biSize;       // Size of info header
    LONG  biWidth;      // Image width
    LONG  biHeight;     // Image height
    WORD  biPlanes;
    WORD  biBitCounts;  // Bit counts of each pixel
    DWORD biCompression;// Compression type
    DWORD biSizeImage;  // Size of compressed image
    LONG  biXpelsPerMeter;  // Pixels per meter orientally, resolution
    LONG  biYpelsPerMeter;  // Pixels per meter vertically, resolution
    DWORD biClrUsed;        // Colors used in bitmap
    DWORD biClrImportant;   // Important colors in bitmap
}BITMAPINFOHEADER;

/* 
 * Brief: Palette
 */
typedef struct tagRGBQUAD
{
    BYTE rgbBlue;       // Blue component
    BYTE rgbGreen;      // Green component
    BYTE rgbRed;        // Red component
    BYTE rgbReserved;   // Reserved
}RGBQUAD;

/* 
 * Brief: R, G and B components
 */
typedef struct tagIMAGEDATA
{
    BYTE Blue;       // Blue component
    BYTE Green;      // Green component
    BYTE Red;        // Red component
}IMAGEDATA;
#pragma pack(pop)

enum ColorSpace {
        RGB   = 1,
        YIQ   = 2,
        HSI   = 3,
        YCbCr = 4,
        XYZ   = 5 };

class BitMap
{
    typedef BITMAPFILEHEADER* Ptr_fHeader;
    typedef BITMAPINFOHEADER* Ptr_iHeader;
    typedef RGBQUAD*          Ptr_rgbQuad;
    typedef IMAGEDATA*        Ptr_imageData;
public:
    /* 
     * Brief: Constructor
     * Parameter:
     *     None
     */
    BitMap(): fHeader(NULL), iHeader(NULL), rgbQuad(NULL), imageData(NULL), colorSpace(RGB), usePalette(false) {}

    /* 
     * Brief: Overload constructor
     * Parameter:
     *     fileName -- image file name including file directory
     */
    BitMap(const std::string& fileName);

    /* 
     * Brief: Copy constructor
     * Parameter:
     *     fileName -- image file name including file directory
     */
    BitMap(const BitMap& image);

    /* 
     * Brief: Deconstructor, release memories
     * Parameter:
     *     None
     */
    ~BitMap() { delete fHeader; delete iHeader; free(rgbQuad); free(imageData); }

    /* 
     * Brief: Check if input file is a valid bitmap image
     *        Specifically, the first WORD of file header should be 0x4d42, or "BM" according to ASCII
     * Parameter:
     *     None
     * Return:
     *     flag -- true if image is valid bitmap image
     */
    bool formatCheck();
    
    /* 
     * Brief: Display formatted file header
     * Parameter:
     *     None
     * Return:
     *     void
     */
    void showFileHeader();

    /* 
     * Brief: Display formatted file header
     * Parameter:
     *     None
     * Return:
     *     void
     */
    void showInfoHeader();

    /* 
     * Brief: Query rgb data at a certain point
     * Parameter:
     *     row -- vertical coordinate
     *     col -- oriental coordinate
     * Return:
     *     IMAGEDATA structure
     */
    IMAGEDATA at(size_t row, size_t col);

    /* 
     * Brief: Convert RGB color space to YIQ color space
     * Parameter:
     *     None
     * Return:
     *     None
     */
    void RGB2YIQ();

    /* 
     * Brief: Convert RGB color space to HSI color space
     * Parameter:
     *     None
     * Return:
     *     None
     */
    void RGB2HSI();

    /* 
     * Brief: Convert RGB color space to YCbCr color space
     * Parameter:
     *     None
     * Return:
     *     None
     */
    void RGB2YCbCr();

    /* 
     * Brief: Convert RGB color space to XYZ color space
     * Parameter:
     *     None
     * Return:
     *     None
     */
    void RGB2XYZ();

    /* 
     * Brief: Print color space information
     * Parameter:
     *     None
     * Return:
     *     color space enum
     */
    int getcSpace();

public:
    Ptr_fHeader   fHeader;
    Ptr_iHeader   iHeader;
    Ptr_rgbQuad   rgbQuad;
    Ptr_imageData imageData;
    bool usePalette;

private:
    /* 
     * Brief: Gamma function used for Convertion from RGB to XYZ color space
     * Parameter:
     *     x -- input
     * Return:
     *     output
     */
    double gammaX(double x);
private:
    int colorSpace;
};

/* 
 * Brief: Print help information
 * Parameter:
 *     None
 */
void printHelp();

/* 
 * Brief: load image
 * Parameter:
 *     fileName -- image file name including file directory
 */
BitMap load(const std::string fileName);

/* 
 * Brief: Overload operator <<
 * Parameter:
 *     out -- reference of out stream
 *     data -- IMAGEDATA structure
 */
std::ostream& operator << ( std::ostream& out, IMAGEDATA data);

/* 
 * Brief: Write data to a new image file.
 *        In order to enhance the difference between old and new images,
 *        make red component equals to 0 while writing to new image.
 * Parameter:
 *     newFileName -- new file name
 *     image -- reference of image
 * Return:
 *     None
 */
void write(const std::string newFileName, BitMap& image, int flag = 1);

/* 
 * Brief: Display RGB component
 * Parameter:
 *     image -- source image
 *     cSpace -- color space enum variable
 * Return:
 *     None
 */
void displayColorSpace(const BitMap& image, int cSpace = RGB);

/* 
 * Brief: Retrieve component of color space and display
 * Parameter:
 *     image -- source image
 *     cSpace -- color space enum variable
 * Return:
 *     None
 */
void retrieveComponent(const BitMap& image, int cSpace = RGB);

/* 
 * Brief: Show bmp image with OpenCV
 * Parameter:
 *     windowName -- name of display window
 *     image -- image to be displayed
 * Return:
 *     None
 */
void bmpshow(const std::string& windowName, const BitMap& image, int channel = 0);

#endif
