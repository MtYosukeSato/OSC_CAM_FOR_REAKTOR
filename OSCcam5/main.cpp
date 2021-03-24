// 2-10_p36_camera_open.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。

#include <iostream>           //opencv OSC
#include <Windows.h>          //opencv OSC
#include <string>             //opencv OSC
#include <stdlib.h>           //ppencv
#include <opencv2/opencv.hpp> //opencv

#include "OSCReceiveHandler.h"
#include "reaktorData.h"
#include "ysOpenCvCapture.h"
#include "ysSendOSC.h"

//実質的なmainはこちら
#include "ysThread.h" 


int main(int argc, char** argv)
{
    std::cout << "main()起動" << std::endl;
    ysThread t(argc, argv);
    t.run();
    //スレッド2本で送信受信を同時に行い、受信のデータをプロパティのポインタに渡して参照している

    return 0;
}