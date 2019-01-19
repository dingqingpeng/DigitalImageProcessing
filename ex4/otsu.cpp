#include <opencv2/core/core.cpp>

int otsu( cv::Mat img )
{
    int height = img.rows;
    int width  = img.cols;

    int histogram[256];
    float grayScaleProb[256];

    // Initialize arrays
    for( int i = 0; i < 256; i++ )
    {
        histogram[i]   = 0;
        grayScaleProb[i] = 0.0;
    }

    // Count number of each pixel
    for( int i = 0; i < width; i++ )
    {
        for( int j = 0; j < height; j++ )
        {
            histogram[ img.ptr<uchar>(i)[j] ]++;
        }
    }
    // Compute probability
    for( int i = 0; i < 256; i++ )
        grayScaleProb[i] = (float)histogram[i] / (float)( height * width );
    
    // OTSU algorithm
    // ICV = PA * (MA - M)^2 + PB * (MB - M)^2
    // t = arg max(ICV)
    // where:
    // PA, PB -- probability of part A and B
    // MA, MB -- gray scale mean of part A and B
    // M -- gray scale mean of image
    float ICVMax = 0.0;
    float ICV;
    int threshold = 0;
    // Traverse every gray scale, compute ICV and find the maximum
    for( int i = 0; i < 256; i++ )
    {
        float PA = 0.0;
        float PB = 0.0;
        float EA = 0.0;
        float EB = 0.0;
        
        // Part A / Background
        for( int j = 0; j <= i; j++ )
        {
            PA += grayScaleProb[j];
            EA += j * grayScaleProb[j];
        }
        // Part B / Foreground
        for( int j = i; j < 256; j++ )
        {
            PB += grayScaleProb[j];
            EB += j * grayScaleProb[j];
        }

        float MA = EA / PA;
        float MB = EB / PB;
        float M  = EA + EB;
        float ICV = PA * (MA - M) * (MA - M) + PB * (MB - M) * (MB - M);
        // Find max
        if( ICV > ICVMax )
        {
            ICVMax = ICV;
            threshold = i;
        }
    }
    
    return threshold;
}
