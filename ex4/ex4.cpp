#include <iostream>
#include <cmath>
#include <opencv2/opencv.hpp>
#define PI 3.1415926

using namespace std;
using namespace cv;

cv::Point findObject(cv::Mat srcImg, cv::Mat& roi, std::vector<Point> C)
{
    int left = srcImg.cols;
    int right = 0;
    int top = srcImg.rows;
    int bottom = 0;
    for(int i = 0; i < C.size(); i++)
    {
        if( C[i].x < left )
            left = C[i].x;
        if( C[i].x > right )
            right = C[i].x;
        if( C[i].y < top )
            top = C[i].y;
        if( C[i].y > bottom )
            bottom = C[i].y;
    }

    int centerX = (right + left)/2;
    int centerY = (bottom + top)/2;
    int diameter = (right-left) > (bottom-top)? (right-left): (bottom-top);
    rectangle( srcImg, Rect(centerX-diameter/2-20, centerY-diameter/2-20, diameter+40, diameter+40), Scalar(0, 255, 0), 2 );
    namedWindow("ROI", WINDOW_NORMAL);
    imshow("ROI",srcImg);
    waitKey();

    Mat ROI = srcImg(Rect(left-5, top-5, right-left+10, bottom-top+10));
    roi = ROI.clone();

    return Point( (right - left)/2, (bottom - top)/2 );
}

int maxContourAreaIndex(std::vector<std::vector<Point>> C)
{
    int index = 0;
    double maxArea = 0.0;
    
    for(int i = 0; i < C.size(); i++)
    {
        if( contourArea(C[i]) > maxArea )
        {
            maxArea = contourArea(C[i]);
            index = i;
        }
    }
    
    return index;
}

void findEdgePointOnLine( const std::vector<int>& pixelsOnLine, std::vector<int>& edgePointPos )
{
    int len  = pixelsOnLine.size();
    int half = len / 2;

    for(int i = 0; i < half; i++)
    {
        if(pixelsOnLine[i] != 0)
            edgePointPos.push_back( half - i );
    }
    for(int i = half; i < len; i++)
    {
        if(pixelsOnLine[i] != 0)
            edgePointPos.push_back( i - half );
    }
}

void scanOnLine( cv::Mat& img, std::vector<int>& pixelsOnLine, float theta )
{
    pixelsOnLine.clear();
    
    int height = img.rows;
    int width  = img.cols;
    int radius = img.rows / 2;
    int centerX = width/2, centerY = height/2;

    int x0 = centerX - radius * cos(theta);
    int y0 = centerY - radius * sin(theta);
    int x1 = centerX + radius * cos(theta);
    int y1 = centerY + radius * sin(theta);
    float resX = float(x1 - x0) / float(radius * 2);
    float resY = float(y1 - y0) / float(radius * 2);
    
    for(int i = 0; i < radius * 2; i++)
    {
        pixelsOnLine.push_back( img.ptr<uchar>(int(y0+i*resY))[int(x0+i*resX)] );
    }
}

void findRadiusInDirections( cv::Mat& img, std::vector<int>& allEdgePointPos )
{
    int div = 24;
    std::vector<int> pixelsOnLine, edgePointPos;
    for(int i = 0; i < div; i++)
    {
        scanOnLine(img, pixelsOnLine, 2 * PI / div * i);
        findEdgePointOnLine( pixelsOnLine, allEdgePointPos );
    }
}

int main()
{
    Mat src,dst;
    src = imread("../data/1 (1).jpg");
    if(src.empty())
    {
        printf("can not load image \n");
        return -1;
    }

    Mat grayImg, thresImg;
    cvtColor(src,grayImg,COLOR_BGR2GRAY);
    threshold(grayImg, thresImg, 100, 255, THRESH_OTSU);
    namedWindow("threshold", WINDOW_NORMAL);
    imshow("threshold", thresImg);
    waitKey();

    std::vector<std::vector<Point>> contours_thres;
    std::vector<Vec4i> hierarchy_thres;
    Mat outerContour = Mat::zeros(src.size(), CV_8UC1);
    findContours(thresImg, contours_thres, hierarchy_thres, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
    drawContours(outerContour, contours_thres, maxContourAreaIndex(contours_thres), Scalar(255), 2, 8, hierarchy_thres, 0, Point(0,0));

    namedWindow("Outer contour", WINDOW_NORMAL);
    imshow("Outer contour",outerContour);
    waitKey();

    Mat singleCell, singleCellgray, grayImgCannyed;
    Point center;
    center = findObject(src, singleCell, contours_thres[maxContourAreaIndex(contours_thres)]);
    cvtColor(singleCell,singleCellgray,COLOR_BGR2GRAY);
    Canny(singleCellgray, grayImgCannyed, 50, 80);
    namedWindow("Canny Image", WINDOW_NORMAL);
    imshow("Canny Image",grayImgCannyed);
    waitKey();

    std::vector<int> allEdgePointPos;
    findRadiusInDirections( grayImgCannyed, allEdgePointPos );
    Mat EdgePointPos( Size(1, allEdgePointPos.size()), CV_32FC1 );
    
    for(int i = 0; i < allEdgePointPos.size(); i++)
        EdgePointPos.ptr<float>(0)[i] = allEdgePointPos[i];
    // cout << EdgePointPos << endl;
    
    const TermCriteria term_criteria = TermCriteria(TermCriteria::EPS + TermCriteria::COUNT, 10, 1.0);
	Mat labels_, radius;
	double value = kmeans(EdgePointPos, 5, labels_, term_criteria, 10, KMEANS_RANDOM_CENTERS, radius);
    cv::sort(radius,radius,CV_SORT_ASCENDING + CV_SORT_EVERY_COLUMN); 
    cout << "radius = " << radius << endl;

    cv::circle ( singleCell, cv::Point2d ( singleCell.rows/2, singleCell.cols/2 ), 8, cv::Scalar ( 0,0,255 ), 2 );
    for(int i = 0; i < radius.rows; i++)
    {
        cv::circle ( singleCell, cv::Point2d ( singleCell.rows/2+radius.ptr<float>(0)[i], singleCell.cols/2 ), 8, cv::Scalar ( 0,255,0 ), 2 );
    }
    // namedWindow("Radius", WINDOW_NORMAL);
    // imshow("Radius",singleCell);
    // waitKey();

    Mat thickness( Size(1, 4), CV_32FC1 );
    for(int i = 0; i < 4; i++)
    {
        thickness.ptr<float>(0)[i] = radius.ptr<float>(0)[i+1] - radius.ptr<float>(0)[i];
    }
    cout << "thickness = " << thickness << endl;
    

    return 0;
}
