#pragma once
#include <vector>             //配列
#include <opencv2/opencv.hpp> //opencv

/********************************************
reaktorData.h

OSC送信する前にカメラデータを入れる配列
・一時保管することで前ピクセルとの比較をしたりする

*********************************************/

//データクラス vector配列で利用
class reaktorData {
public:
    cv::Vec3b    bgr;        //rgb
    unsigned int reaktor_idx;//multidisplayのindex 必要な最大値：総ピクセル数

    reaktorData();
    reaktorData(cv::Vec3b b, unsigned int id);
};