#include <bits/stdc++.h>
#include <vector>
#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

#define CP_OPEN 0

cv::Mat GetUsedChannel(cv::Mat& src_img, int flag) {
    cv::Mat t;
    cv::Mat t_cs[3];
    switch (flag) {
        case 0:
        case 1:
        case 2:
            cv::cvtColor(src_img, t, CV_BGR2HSV_FULL);
            cv::split(t, t_cs);
            return t_cs[flag];
        case 3:
        case 4:
        case 5:
            cv::cvtColor(src_img, t, CV_BGR2Lab);
            cv::split(t, t_cs);
            return t_cs[flag-3];
    }
}


int main() {
    cv::VideoCapture cp(CP_OPEN);
    cv::Mat frame;

    // A channnel thre
    int a_min;
    int a_max;
    cv::namedWindow("a_thre");
    cv::createTrackbar("a_min", "a_thre", &a_min, 255);
    cv::createTrackbar("a_max", "a_thre", &a_max, 255);
     
    
    while (1) {
        cp >> frame;
        if (frame.empty()) {
            cout<<"waiting for rebooting..."<<endl;
            cp.open(CP_OPEN);
        }

    }


    return 0;
}