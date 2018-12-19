#include <opencv2/core.hpp>

/****************** 
 * Brief: (数字图像处理-->实验三) Image filter header file
 * Author:
 *     Ding Qingpeng (丁庆鹏, Student No. 18B903053)
 ******************/

/* 
 * Brief: Compute convolution
 * Parameter:
 *     mat1 -- take mat1 as f(t)
 *     mat2 -- take mat2 as g(t)
 * Return:
 *     Convolution result
 */
unsigned char conv(cv::Mat& mat1, cv::Mat& mat2);

enum{
    Guassian = 1,
    Impulse  = 2
};

/* 
 * Brief: Add noise to image of given type
 * Parameter:
 *     image -- source image
 *     type -- noise type
 * Return:
 *     None
 */
void addNoise( cv::Mat& image, int type );

/* 
 * Brief: Add impulse (pepper) noise to image
 * Parameter:
 *     image -- destination image
 *     percentage -- percetage of pixels affected by noise
 *     visualize -- flag to determine visualization
 * Return:
 *     None
 */
void addImpulseNoise( cv::Mat& image, double percentage = 0.1, bool visualize = true );

/* 
 * Brief: Add Guassian noise to image
 * Parameter:
 *     image -- destination image
 *     mean -- mean of Guassian distribution
 *     stdev -- standard deviation of Guassian distribution
 *     visualize -- flag to determine visualization
 * Return:
 *     None
 */
void addGuassianNoise( cv::Mat& image, double mean = 5, double stdev = 10, bool visualize = true );

/* 
 * Brief: Implement median filter
 * Parameter:
 *     in -- source image
 *     out -- filtered image
 * Return:
 *     None
 */
void medianFilter( cv::Mat& in, cv::Mat& out );

/* 
 * Brief: Implement mean filter
 * Parameter:
 *     in -- source image
 *     out -- filtered image
 * Return:
 *     None
 */
void meanFilter( cv::Mat& in, cv::Mat& out, int kernelSize = 3 );

