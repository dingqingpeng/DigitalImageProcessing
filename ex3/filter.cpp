#include <opencv2/core.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include "filter.h"

/****************** 
 * Brief: (数字图像处理-->实验三) Image filter Implementation
 * Author:
 *     Ding Qingpeng (丁庆鹏, Student No. 18B903053)
 ******************/

void addNoise( cv::Mat& image, int type )
{
    
    switch (type)
    {
        case 1:
            
            break;
        case 2:
            addImpulseNoise( image, 0.02 );
            break;
        default:
            break;
    }
}

void addImpulseNoise( cv::Mat& image, double percentage, bool visualize )
{
    std::random_device rd;
    std::mt19937 gen(rd());
    int rows = image.rows;
    int cols = image.cols;
    
    for(int i = 0; i < (int)rows * cols * percentage; i++)
    {
        int row = gen() % rows;
        int col = gen() % cols;
        image.ptr<unsigned char>(row)[col] = gen() % 2 * 255;
    }

    if( visualize )
    {
        cv::imshow( "Impulse Noise", image );
        cv::waitKey(0);
    }
}

void addGuassianNoise( cv::Mat& image, double mean, double stdev, bool visualize )
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<> d(mean, stdev);
    int rows = image.rows;
    int cols = image.cols;
    int temp;

    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < cols; j++)
        {
            temp = image.ptr<unsigned char>(i)[j] + d(gen);
            temp = temp > 255? 255: temp;
            temp = temp <   0?   0: temp;
            image.ptr<unsigned char>(i)[j] = temp;
        }
    }

    if( visualize )
    {
        cv::imshow( "Guassian Noise", image );
        cv::waitKey(0);
    }
}

void meanFilter( cv::Mat& in, cv::Mat& out, int kernelSize )
{
    ;
}
