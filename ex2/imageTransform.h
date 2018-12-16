#ifndef IMAGETRANSFORM_H_INCLUDED
#define IMAGETRANSFORM_H_INCLUDED

#include <vector>
#include "bitmap.h"
#include "imageTransform.h"

typedef std::vector<BYTE>                    PixelVector;
typedef std::vector< std::vector<BYTE> >     ImageBlockBYTE;
typedef std::vector< std::vector<Complexd> > ImageBlockComplex;
typedef std::vector< std::vector< std::vector<BYTE> > >     ImageBlockVectorBYTE;
typedef std::vector< std::vector< std::vector<Complexd> > > ImageBlockVectorComplex;
typedef ImageBlockBYTE       ImageBlock;
typedef ImageBlockVectorBYTE ImageBlockVector;

typedef struct _BLOCKEDIMAGESIZE
{
    LONG rows;
    LONG cols;
}BLOCKEDIMAGESIZE;

template<typename T>
struct _TRANSFORMEDIMAGE
{
    T imageData;
    BLOCKEDIMAGESIZE blockedImageSize;
};
typedef _TRANSFORMEDIMAGE<ImageBlockVectorComplex> transImageComplex;
typedef _TRANSFORMEDIMAGE<ImageBlockVectorBYTE>    transImagedouble;

enum{
    Y  = 1,
    Cb = 2,
    Cr = 3
};

/*
 * Brief: Break bmp image into dada blocks
 * Parameter:
 *     image -- source image
 *     blockData -- reference of data block
 *     blockSize -- size of block
 *     channel -- desired channel
 * Return:
 *     None
 */
BLOCKEDIMAGESIZE breakBMPImage( BitMap image, ImageBlockVector& blockData, int blockSize = 8, int channel = Y );

/*
 * Brief: DFT a single image block
 * Parameter:
 *     originalData -- original image data
 *     transformedData -- transformed data
 *     shift -- flag to determine whether to shift the image
 * Return:
 *     Blocked image size
 */
template<typename TransformType>
void ftSingleBlock( const ImageBlock& originalData, ImageBlockComplex& transformedData, bool shift = true );

/*
 * Brief: DFT image blocks
 * Parameter:
 *     originalData -- original image block vector
 *     transformedData -- transformed block vector
 *     printTimeConsumption -- flag to determine whether to print time consumption
 * Return:
 *     None
 */
void dftBlocks( const ImageBlockVector& originalData, ImageBlockVectorComplex& transformedData, bool printTimeConsumption = true );
void fftBlocks( const ImageBlockVector& originalData, ImageBlockVectorComplex& transformedData, bool printTimeConsumption = true );

/*
 * Brief: IFT a single image block
 * Parameter:
 *     transformedData -- transformed data
 *     originalData -- original image data
 *     shift -- flag to determine whether to shift the image
 * Return:
 *     None
 */
template<typename TransformType>
void iftSingleBlock( const ImageBlockComplex& transformedData, ImageBlock& originalData, bool shift = true );

/*
 * Brief: IFT image blocks
 * Parameter:
 *     transformedData -- transformed block vector
 *     originalData -- original image block vector
 *     printTimeConsumption -- flag to determine whether to print time consumption
 * Return:
 *     None
 */
void idftBlocks( const ImageBlockVectorComplex& transformedData, ImageBlockVector& originalData, bool printTimeConsumption = true );
void ifftBlocks( const ImageBlockVectorComplex& transformedData, ImageBlockVector& originalData, bool printTimeConsumption = true );

/*
 * Brief: Visualize transformed image
 * Parameter:
 *     transformedData -- transformed block vector
 * Return:
 *     None
 */
void visualizeTransform( ImageBlockVectorComplex& transformedData, BLOCKEDIMAGESIZE imageSize );

enum{
    Amplitude = 1,
    Phase     = 2
};
/*
 * Brief: Reconstruct image with information from inverse transformation
 * Parameter:
 *     transformedData -- transformed block vector
 *     component -- Component to extract
 * Return:
 *     None
 */
void extractComponent( ImageBlockVectorComplex& transformedData, ImageBlockVectorComplex& extraction, int component );

/*
 * Brief: Reconstruct image with information from inverse transformation
 * Parameter:
 *     reconstructedBlocks -- transformed block vector
 *     imageSize -- Blocked image size
 * Return:
 *     None
 */
void reconstructImage( const ImageBlockVectorBYTE& reconstructedBlocks, BLOCKEDIMAGESIZE imageSize );

void findMaxAmpInBlock( ImageBlockComplex originalData );
double findMaxInBlock( std::vector< std::vector<double> > originalData );
double findMinInBlock( std::vector< std::vector<double> > originalData );

#endif // IMAGETRANSFORM_H_INCLUDED
