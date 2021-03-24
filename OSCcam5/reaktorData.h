#pragma once
#include <vector>             //�z��
#include <opencv2/opencv.hpp> //opencv

/********************************************
reaktorData.h

OSC���M����O�ɃJ�����f�[�^������z��
�E�ꎞ�ۊǂ��邱�ƂőO�s�N�Z���Ƃ̔�r�������肷��

*********************************************/

//�f�[�^�N���X vector�z��ŗ��p
class reaktorData {
public:
    cv::Vec3b    bgr;        //rgb
    unsigned int reaktor_idx;//multidisplay��index �K�v�ȍő�l�F���s�N�Z����

    reaktorData();
    reaktorData(cv::Vec3b b, unsigned int id);
};