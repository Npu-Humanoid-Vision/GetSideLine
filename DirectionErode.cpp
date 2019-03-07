#include <bits/stdc++.h>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

#define CP_OPEN 1
// #define CP_OPEN "/media/alex/Data/baseRelate/pic_data/frame%04d.jpg"

#define WAIT_KEY 1

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


int main(int argc, char const *argv[]) {
    cv::VideoCapture cp(CP_OPEN);
    cv::Mat frame;
    cv::namedWindow("params");

    // Channel thre relate
    int thre_min = 168;
    int thre_max = 255;
    cv::createTrackbar("thre_min", "params", &thre_min, 255);
    cv::createTrackbar("thre_max", "params", &thre_max, 255);

    // kernal relate
    int vertical_kernal_size = 2;
    int horizontal_kernal_size = 2;
    int gaus_kernal_size = 1;
    cv::createTrackbar("vert_ker_size", "params", &vertical_kernal_size, 233);
    cv::createTrackbar("hori_ker_size", "params", &horizontal_kernal_size, 233);
    cv::createTrackbar("gaus_ker_size", "params", &gaus_kernal_size, 66);

    while (1) {
        cp >> frame;
        if (frame.empty()) {
            cout<<"waiting for camera rebooting"<<endl;
            cp.open(CP_OPEN);
            continue;
        }
        cv::GaussianBlur(frame, frame, cv::Size(2*gaus_kernal_size+1, 2*gaus_kernal_size+1), 0);

        cv::Mat used_channel = GetUsedChannel(frame, L);
        cv::Mat thre_image = used_channel > thre_min & used_channel < thre_max;

        cv::Mat hori_kernal = cv::getStructuringElement(MORPH_RECT, Size(horizontal_kernal_size, 1));
        cv::Mat veri_kernal = cv::getStructuringElement(MORPH_RECT, Size(1, vertical_kernal_size));

        cv::Mat veri_eroded;
        cv::Mat hori_eroded;
        cv::erode(thre_image, hori_eroded, hori_kernal);
        cv::erode(thre_image, veri_eroded, veri_kernal);
        // thre_image = thre_image - veri_eroded;

        cv::Mat thre_minus_veri_eroded = thre_image-veri_eroded;

        cv::Mat thre_minus_veri_eroded_hori_eroded;
        cv::erode(thre_minus_veri_eroded, thre_minus_veri_eroded_hori_eroded, hori_kernal);

        // cv::Mat t_for_contours = thre_minus_veri_eroded.clone();
        // cv::findContours(t_for_contours, )

        cv::imshow("frame", frame);
        cv::imshow("thre", thre_image);
        cv::imshow("veri_eroded", veri_eroded);
        cv::imshow("hori_eroded", hori_eroded);
        cv::imshow("thre_minus_veri", thre_minus_veri_eroded);
        cv::imshow("thre_minus_veri_hori-erode", thre_minus_veri_eroded_hori_eroded);
        char key = cv::waitKey(WAIT_KEY);
        if (key == 'q') {
            break;
        }
    }
    return 0;
}
