#pragma once
#include <string>
#include "ysOpenCvCapture.h"
#include "reaktorData.h" //#include <opencv2/opencv.hpp>


//#define ADDRESS "127.0.0.1" //���[�J���z�X�g�i�ۗ��F���[�h�������Ő؂�ւ�����悤�ɂ��邱�Ɓj
#define ADDRESS "192.168.10.200"
#define PORT 8000
#define OUTPUT_BUFFER_SIZE 3096 


//OSC���M����f�[�^�N���X
class ysSendOSC {
public:
    char osc_address[255];           //OSC�̃A�h���X
    cv::Size default_size;           //�f���̕��ƍ��� ysThread�ōX�V�����
    std::vector<reaktorData> arr_ni; //OSC���M����f�[�^���i�[����̔z��
    
    //�σC���^�[���[�X�X�L����
    cv::Point2i inter_mod; //���O���[�v�ɕ����邩�imod�p�j
    cv::Point2i inter_num; //���݂̔z���mod�O���[�v

    bool ysSetData2Array(ysOpenCvCapture ysOpenCv);//�f�[�^��arr_ni�ɃZ�b�g
    void ysSendToOSC(void);                        //arr_ni��OSC���M�������

    //�R���X�g���N�^
    ysSendOSC(const char* name, cv::Size size)
    {
        strcpy(osc_address, name); //OSC�A�h���X�̖��O�i�t�H���_���݂����Ȃ�j
        default_size = size;

        //�C���^�[���[�X�̏����l
        inter_num = cv::Point2i(0,0);//x,y
        inter_mod = cv::Point2i(1,1);//x,y 5,2
    };

    //�����T�C�Y�̑��M
    void ysSendFirstSize(ysOpenCvCapture ysOpenCv);

private:
    int index_saved; //�z��ɓ��ꂽ�C���f�b�N�X�i�ő�j
    int index_send;  //���M�����C���f�b�N�X�i�ő�j

    //�C���^�[���[�X�p�̒l�̃Z�b�g
    void ysAddInterLaceNumber(void);

    //�R���\�[���ւ̏o�́@�f�o�b�O�p
    void ysPrintSendNotice(reaktorData rdata);
};