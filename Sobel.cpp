#include <bits/stdc++.h>
#include <vector>
#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

#ifdef __linux__
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#endif

#ifdef __WIN32
#include <io.h>
#include <windows.h>
#endif

void GetImgNames(string root_path, std::vector<std::string>& names) {
#ifdef __linux__
    struct dirent* filename;
    DIR* dir;
    dir = opendir(root_path.c_str());  
    if(NULL == dir) {  
        return;  
    }  

    int iName=0;
    while((filename = readdir(dir)) != NULL) {  
        if( strcmp( filename->d_name , "." ) == 0 ||
            strcmp( filename->d_name , "..") == 0)
            continue;

        string t_s(filename->d_name);
        names.push_back(t_s);
    }
#endif

#ifdef __WIN32
    intptr_t hFile = 0;
    struct _finddata_t fileinfo;
    string p;

    hFile = _findfirst(p.assign(root_path).append("/*.jpg").c_str(), &fileinfo);

    if (hFile != -1) {
        do {
            if (strcmp(fileinfo.name, ".") == 0 || strcmp(fileinfo.name, "..") == 0) {
                continue;
            }
            names.push_back(fileinfo.name); 
        } while (_findnext(hFile, &fileinfo) == 0);
    }
#endif
}

int main() {
    // string root_path = "../../BackUpSource/Ball/Train/Raw";
    string root_path = "D:/baseRelate/pic_data";
    std::vector<string> image_names;
    GetImgNames(root_path, image_names);

    for (auto i = image_names.begin(); i != image_names.end(); i++) {
        cv::Mat frame = cv::imread(root_path+"/"+*i);
        cout<<*i<<endl;

        if (frame.empty()) {
            continue;
        }
        cv::Mat t;
        cv::Mat t_cs[3];
        
        cv::cvtColor(frame, t, CV_BGR2HSV);
        cv::split(t, t_cs);

        // cv::split(frame, t_cs);

        cv::Mat dst;
        cv::Sobel(t_cs[2], dst, CV_8U, 0, 1);
        cv::convertScaleAbs(dst, dst);


        // cv::threshold(dst, dst, 0, 255, CV_THRESH_OTSU);

        cv::imshow("211", dst);
        cv::imshow("233", frame);
        char key = cv::waitKey(0);
        if (key == 'q') {
            break;
        }
        else if (key == 'n') {
            continue;
        }
        else if (key == 'p') {
            i--;
            i--;
        }
    }
    return 0;
}