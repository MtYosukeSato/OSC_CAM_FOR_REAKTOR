#pragma once
#include <opencv2/opencv.hpp>

//openCVのキャプチャを開いたりするクラス
class ysOpenCvCapture {
public:
    cv::VideoCapture cap; //opencv
    cv::Size size_for_set;//希望しているサイズ　ysThreadで更新される
    unsigned int cap_fps;

    ysOpenCvCapture(int argc, char** argv, cv::Size default_size, unsigned int fps);
};
