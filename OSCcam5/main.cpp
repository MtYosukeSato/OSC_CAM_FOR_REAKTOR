// 2-10_p36_camera_open.cpp : ���̃t�@�C���ɂ� 'main' �֐����܂܂�Ă��܂��B�v���O�������s�̊J�n�ƏI���������ōs���܂��B

#include <iostream>           //opencv OSC
#include <Windows.h>          //opencv OSC
#include <string>             //opencv OSC
#include <stdlib.h>           //ppencv
#include <opencv2/opencv.hpp> //opencv

#include "OSCReceiveHandler.h"
#include "reaktorData.h"
#include "ysOpenCvCapture.h"
#include "ysSendOSC.h"

//�����I��main�͂�����
#include "ysThread.h" 


int main(int argc, char** argv)
{
    std::cout << "main()�N��" << std::endl;
    ysThread t(argc, argv);
    t.run();
    //�X���b�h2�{�ő��M��M�𓯎��ɍs���A��M�̃f�[�^���v���p�e�B�̃|�C���^�ɓn���ĎQ�Ƃ��Ă���

    return 0;
}