#include <bits/stdc++.h>
#include <vector>
#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

// #define CP_OPEN 0
#define CP_OPEN "/media/alex/Data/baseRelate/pic_data/frame%04d.jpg"


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

int slide_windows_num = 32;
int sld_win_cols;
int sld_win_rows;
std::vector<cv::Rect> sld_wins;
int sld_step = 10;
double win_rate_thre = 0.6;

void Slide(cv::Mat& binary_image) {
    // get integral image
    cv::Mat integral_image;
    cv::integral(binary_image, integral_image);
    integral_image /= 255;

    // init slide wins
}


int main() {
    cv::VideoCapture cp(CP_OPEN);
    cv::Mat frame;

    cv::namedWindow("params");

    // A channnel thre
    int a_min = 0;
    int a_max = 116;
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
        
        cv::Mat open_kernal = cv::getStructuringElement(MORPH_RECT, cv::Size(open_kernal_size*2+1, open_kernal_size*2+1));
        cv::morphologyEx(thre_result, thre_result, MORPH_OPEN, open_kernal);



        //////////////////////////////////abandon Hough version////////////////////////////////
        // cv::Mat mor_gradiant;
        // cv::Mat open_kernal = cv::getStructuringElement(MORPH_RECT, cv::Size(open_kernal_size*2+1, open_kernal_size*2+1));
        // cv::Mat mor_kernal = cv::getStructuringElement(MORPH_RECT, cv::Size(morgrad_kernal_size*2+1, morgrad_kernal_size*2+1));
        // cv::morphologyEx(thre_result, thre_result, MORPH_OPEN, open_kernal);
        // cv::morphologyEx(thre_result, mor_gradiant, MORPH_GRADIENT, mor_kernal);


        // std::vector<cv::Vec4i> lines;
        // cv::HoughLinesP(mor_gradiant, lines, 1., CV_PI/180, 100);

        // cv::Mat train_data(lines.size(), 1, CV_32FC1);
        // cv::Mat labels, center;
        // // cout<<lines.size()<<endl;
        // for (size_t i = 0; i < lines.size(); i++) {
        //     Vec4i plines=lines[i];
        //     // line(frame,Point(plines[0],plines[1]),Point(plines[2],plines[3]),cv::Scalar(0, 0, 255),3);
        //     double distance = sqrt(1.0*(plines[0]-plines[2])*(plines[0]-plines[2])
        //                             + (plines[1]-plines[3])*(plines[1]-plines[3]));
        //     double delta_y = 1.0*plines[1] - plines[3];
        //     // cout<<delta_y/distance<<endl;
        //     train_data.at<float>(i, 0) = 1.0*(plines[3]-plines[1]) / (plines[2]-plines[0]);
        // }
        // // cout<<endl;
        // int k = 2;
        // int attemp = 10;
        // cv::TermCriteria term_criteria = cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 100, 0.01);
        // // cout<<train_data.rows<<endl;
        // if (train_data.rows > 2) {
        //     cv::kmeans(train_data, k, labels, term_criteria, attemp, cv::KMEANS_PP_CENTERS, center);
        //     // cout<<"here"<<endl;
        // }
        // for (int i = 0; i < labels.rows; i++) {
        //     Vec4f plines=lines[i];
        //     if (labels.at<float>(i, 0) == 0) {
        //         line(frame,Point(plines[0],plines[1]),Point(plines[2],plines[3]),cv::Scalar(0, 0, 255),3);
        //     }
        //     else {
        //         line(frame,Point(plines[0],plines[1]),Point(plines[2],plines[3]),cv::Scalar(0, 255, 0),3);
        //     }
        // }

        cv::imshow("living", frame);
        cv::imshow("thre_result", thre_result);
        // cv::imshow("mor_gradiant", mor_gradiant);
        char key = cv::waitKey(1);
        if (key == 'q') {
            cout<<"??????"<<endl;
            break;
        }
    }


    return 0;
}