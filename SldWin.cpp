#include <bits/stdc++.h>
#include <vector>
#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

int main(int argc, char const *argv[]) {
    cv::VideoCapture cp(0);
    cv::Mat frame;
    
    int slide_windows_num = 32;
    int sld_win_cols;
    int sld_win_rows;
    std::vector<cv::Rect> sld_wins;
    int sld_step = 10;
    double win_rate_thre = 0.6;


    // cv::namedWindow("living");
    // cv::namedWindow("final_thre");
    // cv::namedWindow("sliding...");

    // H thre
    int H_min = 0, H_max = 255;
    cv::namedWindow("final_thre");
    cv::namedWindow("Hp", CV_WINDOW_NORMAL);
    cv::createTrackbar("H_min", "Hp", &H_min, 255);
    cv::createTrackbar("H_max", "Hp", &H_max, 255);

    // L thre
    int L_min = 0, L_max = 255;
    cv::createTrackbar("L_min", "Hp", &L_min, 255);
    cv::createTrackbar("L_max", "Hp", &L_max, 255);

    // S thre
    int S_min = 0, S_max = 255;
    cv::createTrackbar("S_min", "Hp", &S_min, 255);
    cv::createTrackbar("S_max", "Hp", &S_max, 255);

    // erode relate
    int erode_times = 1;
    int erode_kernal_size = 1;

    // dilate relate
    int dilate_times = 1;
    int dilate_kernal_size = 1;

    cv::createTrackbar("e_times", "Hp", &erode_times, 10);
    cv::createTrackbar("e_kn_size", "Hp", &erode_kernal_size, 9);

    cv::createTrackbar("d_times", "Hp", &dilate_times, 10);
    cv::createTrackbar("d_kn_size", "Hp", &dilate_kernal_size, 9);

    int begin_flag = 0;

    // init params
    while (1) {
        cp >> frame;
        if (frame.empty()) {
            continue;
        }
        sld_win_cols = frame.cols/slide_windows_num;
        sld_win_rows = 10;
        for (auto i = 0; i < slide_windows_num; i++) {
            sld_wins.push_back(cv::Rect(i*sld_win_cols, 0, sld_win_cols, sld_win_rows));
        }
        break;
    }
    while (1) {
        cp >> frame;
        cv::GaussianBlur(frame, frame, cv::Size(7, 7), 0);
        if (frame.empty()) {
            cerr << __LINE__ <<endl;
            return -1;
        }   
        for (auto i = 0; i < slide_windows_num; i++) {
            sld_wins[i] = cv::Rect(i*sld_win_cols, 0, sld_win_cols, sld_win_rows);
        }

        // H, L, S 通道
        cv::Mat t_HLS;
        cv::Mat t_HLS_Channels[3];
        cv::cvtColor(frame, t_HLS, CV_BGR2HLS_FULL);
        cv::split(t_HLS, t_HLS_Channels);

        cv::Mat final_thre = t_HLS_Channels[0] >= H_min & t_HLS_Channels[0] <= H_max;
        final_thre = final_thre & (t_HLS_Channels[1] >= L_min & t_HLS_Channels[1] <= L_max);
        final_thre = final_thre & (t_HLS_Channels[2] >= S_min & t_HLS_Channels[2] <= S_max);

        cv::erode(final_thre, final_thre, cv::Mat(2*erode_kernal_size+1, 2*erode_kernal_size+1, CV_8UC1), cv::Point(-1, -1), erode_times);
        cv::dilate(final_thre, final_thre, cv::Mat(2*dilate_kernal_size+1, 2*dilate_kernal_size+1, CV_8UC1), cv::Point(-1, -1), dilate_times);


        cv::imshow("final_thre", final_thre);

        
        for (auto i = sld_wins.begin(); i != sld_wins.end() && begin_flag; i++) {
            // srand((int) clock());
            // Sleep(1);
            // i->y += rand() % 20;
            bool sld_valid = true;

            while (sld_valid) {
                int t_counter = 0;
                cv::Mat t_roi = final_thre(*i);
                // cv::imshow("233", t_roi);
                // cv::waitKey();

                for (auto iter = t_roi.begin<uchar>(); iter != t_roi.end<uchar>(); iter++) {
                    if (*iter == 255) {
                        t_counter++;
                    }
                }
                // cout<<t_counter<<endl;
                if (t_counter*1.0/i->area() > win_rate_thre) {
                    sld_valid = false;
                }

                i->y += sld_step;
                if (i->y + sld_win_rows > frame.rows) {
                    i->y = frame.rows - sld_win_rows;
                    sld_valid = false;
                }
            }

            cv::rectangle(frame, *i, cv::Scalar(255, 0, 0));
        }
        cv::imshow("233", frame);
        char key = cv::waitKey(20);
        if (key == 'q') {
            break;
        }
        else if (key == 'b') {
            begin_flag = ~begin_flag;
        }
    }
    return 0;
}
