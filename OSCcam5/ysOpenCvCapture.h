#pragma once
#include <opencv2/opencv.hpp>

//openCV�̃L���v�`�����J�����肷��N���X
class ysOpenCvCapture {
public:
    cv::VideoCapture cap; //opencv
    cv::Size size_for_set;//��]���Ă���T�C�Y�@ysThread�ōX�V�����
    unsigned int cap_fps;

    ysOpenCvCapture(int argc, char** argv, cv::Size default_size, unsigned int fps);
};
