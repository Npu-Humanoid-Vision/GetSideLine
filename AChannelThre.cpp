#include <bits/stdc++.h>
#include <vector>
#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

#define CP_OPEN 0

enum {H, S, V, L, A, B};

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

    cv::namedWindow("params");

    // A channnel thre
    int a_min;
    int a_max;
    cv::createTrackbar("a_min", "params", &a_min, 255);
    cv::createTrackbar("a_max", "params", &a_max, 255);
    
    // Gaussian bulr
    int gaus_kernal_size = 1;
    cv::createTrackbar("gaus_kernal_size", "params", &gaus_kernal_size, 7);

    int open_kernal_size = 1;
    int morgrad_kernal_size = 1;
    cv::createTrackbar("open_kernal_size", "params", &open_kernal_size, 7);
    cv::createTrackbar("morgrad_kernal_size", "params", &morgrad_kernal_size, 7);

    
    while (1) {
        cp >> frame;
        if (frame.empty()) {
            cout<<"waiting for rebooting..."<<endl;
            cp.open(CP_OPEN);
            continue;
        }

        cv::GaussianBlur(frame, frame, cv::Size(gaus_kernal_size*2+1, gaus_kernal_size*2+1), 0);
        // cv::fastNlMeansDenoising(frame, frame);
        cv::Mat used_channel = GetUsedChannel(frame, A);

        cv::Mat thre_result = used_channel>=a_min & used_channel<=a_max;
        
        cv::Mat mor_gradiant;
        cv::Mat open_kernal = cv::getStructuringElement(MORPH_RECT, cv::Size(open_kernal_size*2+1, open_kernal_size*2+1));
        cv::Mat mor_kernal = cv::getStructuringElement(MORPH_RECT, cv::Size(morgrad_kernal_size*2+1, morgrad_kernal_size*2+1));
        cv::morphologyEx(thre_result, thre_result, MORPH_OPEN, open_kernal);
        cv::morphologyEx(thre_result, mor_gradiant, MORPH_GRADIENT, mor_kernal);

        std::vector<cv::Vec2f> lines;
        cv::HoughLines(mor_gradiant, lines, 200., 0.1, 20);
        for (int i = 0; i<lines.size(); i++) {
            double rho = lines[i][0];
            double theta = lines[i][1];
            int x = rho/cos(theta);
            int y = rho/sin(theta);
            cout<<x<<' '<<0<<endl
                <<0<<' '<<y<<endl;
            cv::line(frame, cv::Point(0, y), cv::POint(x, 0), cv::Scalar(0, 0, 255));
        }

        cv::imshow("living", frame);
        cv::imshow("thre_result", thre_result);
        cv::imshow("mor_gradiant", mor_gradiant);
        char key = cv::waitKey(1);
        if (key == 'q') {
            break;
        }
    }


    return 0;
}