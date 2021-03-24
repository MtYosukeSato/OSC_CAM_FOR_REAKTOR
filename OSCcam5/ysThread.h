#pragma once
#include <thread>
#include "OSCReceiveHandler.h"

//�X���b�h�N���X�B�����I��main()
class ysThread
{
private:
    //int tmp_width;//�X���b�h���Ƃɂ���肷����̃|�C���^�Ō���Ȃ���΍l������
    //�X���b�h���ʂɂȂ��Ă���Ƃ��납��A�{�̂ւ̃v���p�e�B�Q�ƁE�A�h���X�Q�Ƃ͂�����������

    int argc;    //main�̈����f�[�^�p
    char** argv; //main�̈����f�[�^�p
    OSCReceiveHandler* Pt_from_Reaktor; //Receive�����f�[�^���Q�Ƃ��邽�߂̃|�C���^ threadFromReaktor�Ŏg�p

    void threadToReaktor();   //Reaktor����̃f�[�^��M
    void threadFromReaktor(); //Reaktor�ւ̃f�[�^���M

public:
    void run();

    ysThread(int c, char** v) : argc(c), argv(v) { Pt_from_Reaktor = nullptr; }; //�R���X�g���N�^
    ~ysThread() {};                                 //�f�X�g���N�^

};
