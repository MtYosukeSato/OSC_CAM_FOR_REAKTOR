#pragma once
#include "ysThread.h"

#include <Windows.h> //Sleep()
#include "ysOpenCvCapture.h"
#include "ysSendOSC.h"

const cv::Size DEFAULT_SIZE = cv::Size(64,48);
const int DEFAULT_FPS = 30;

//�X���b�h�@�J��������Reaktor
void ysThread::threadToReaktor()
{
    //�J�����̃I�u�W�F�N�g
    ysOpenCvCapture ys_cam(argc, argv, DEFAULT_SIZE, DEFAULT_FPS);
    //OSC�̃I�u�W�F�N�g
    ysSendOSC ys_osc("/osc_camera", DEFAULT_SIZE);      //�R���X�g���N�^�ɓn�������̂�OSC�̃A�h���X�ɂȂ�
    
    //�����T�C�Y��OSC���M
    ys_osc.ysSendFirstSize(ys_cam);
    //�����T�C�Y�p��1�񂾂����[�v�O�ő��M
    ys_osc.ysSetData2Array(ys_cam);
    ys_osc.ysSendToOSC();
    ys_osc.arr_ni.clear();

    bool is_not_empty = true;

    while (is_not_empty)
    {
        //Reaktor�����OSC�@���ƍ����̕ύX�`�F�b�N
        if ((*Pt_from_Reaktor).received_size.width != 0 && (*Pt_from_Reaktor).received_size.height != 0 &&
            ys_cam.size_for_set != (*Pt_from_Reaktor).received_size)
        {
            ys_cam.size_for_set = (*Pt_from_Reaktor).received_size;//�ǂ����cv::Size�N���X
            ys_osc.default_size = (*Pt_from_Reaktor).received_size;
        }

        //Reaktor�����OSC
        if (ys_osc.inter_mod != (*Pt_from_Reaktor).interlace)
        {
            ys_osc.inter_mod = (*Pt_from_Reaktor).interlace;
        }

        //�J�����̃I�u�W�F�N�g����frame�����o���Ĕz��ɃZ�b�g
        is_not_empty = ys_osc.ysSetData2Array(ys_cam);
        //������false�����async�G���[�Ŏ~�܂�̂�if�̈Ӗ����Ȃ�

        if (is_not_empty)
        {
            //Reaktor�̍Đ��{�^����������Ă��邩
            if ((*Pt_from_Reaktor).is_reaktor_playing == 1)//�ł����setData�������ɂ��������AAsync�G���[���o��
                //�z�񂩂�OSC���M
                ys_osc.ysSendToOSC();

            //���M�p�z�����ɂ���
            ys_osc.arr_ni.clear();

            Sleep(5);
        }
    }
}


//�X���b�h�@Reaktor����J����
void ysThread::threadFromReaktor()
{
    OSCReceiveHandler listener;
    Pt_from_Reaktor = &listener;

    UdpListeningReceiveSocket s(
        IpEndpointName(IpEndpointName::ANY_ADDRESS, 10000),//Reaktor���̐ݒ�ɍ��킹��
        &listener);
    //ANY_ADDRESS���Ɩ�肪���肻���Ȃ̂ŁA�䂭�䂭�̓Z�[�t�ȃA�h���X�̂�肽�Ƃ�𒲂ׂ�

    s.RunUntilSigInt(); //���ꂪ�Ăяo�����ƃ��[�v���Ă�����悤
}

//�X���b�h�̎��s
void ysThread::run()
{
    std::thread th_a(&ysThread::threadToReaktor, this);  //Reaktor�ւ̉摜�]��
    std::thread th_b(&ysThread::threadFromReaktor, this);//Reaktor����̖��ߎ�M
    //���\�b�h�Ɉ�����n���ɂ�.... (&ysThread::���\�b�h, this, ����);
    //�ϐ����p������ꍇ�́Astd::mutex���K�v

    th_a.join(); //join���Ȃ���abort()�G���[���ł�
    th_b.join();
}