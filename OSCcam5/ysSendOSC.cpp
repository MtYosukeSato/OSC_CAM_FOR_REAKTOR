#pragma once
#include "ysSendOSC.h"

#include "OscReceivedElements.h"     //OSC
#include "OscPacketListener.h"       //OSC
#include "OscOutboundPacketStream.h" //OSC
#include "UdpSocket.h"               //OSC
#include "IpEndpointName.h"          //OSC

#include <Windows.h>

//#define TO_PC_EXE //PC��exe��OSC���܂Ƃߎ�M����ۂ�argment�������킹��̂ɕK�v


void ysSendOSC::ysPrintSendNotice(reaktorData rdata)
{
    std::cout << " r:" << (int)rdata.bgr[2] << " g:" << (int)rdata.bgr[1] << " b:" << (int)rdata.bgr[0];
    std::cout << " idx:" << rdata.reaktor_idx << std::endl;
}

//�C���^�[���[�X�X�L�����p�̒l�Z�b�g�֐�
void ysSendOSC::ysAddInterLaceNumber(void)
{
    inter_num.y = ++inter_num.y % inter_mod.y;
    inter_num.x = ++inter_num.x % inter_mod.x;
    //mod_num_inter�͓������l�ɂȂ�Ȃ��悤�ɂ��邱��(����؂�Ȃ��悤�ɂ���)
    //�f�����[�h�ƃf�[�^���[�h�ŁA�f�����[�h�̎������C���^�[���[�X�ɂ��邩
}


//OSC�f�[�^��z��ɃZ�b�g�@�i�ŏI�I�ɂ�cv::�z��̂܂܂ŏ����ł����������j�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@
bool ysSendOSC::ysSetData2Array(ysOpenCvCapture ysOpenCv)//cv::VideoCapture caputure_data)
{
    cv::Mat frame;            //cv::VideoCapture c����̓ǂݎ�����t���[����ۊǂ���
    cv::Vec3b* ptr;           //�t���[���̃|�C���^��ۊǂ���
    reaktorData tmp_data;     //��r�Ȃǂ���悤�̕ۊǔz��
    int px_y = 0, px_x = 0;   //���ݓǂݎ���Ă��铮��̃s�N�Z���ꏊ
    
    //���M�p�z��ɓ��ꂽindex�T�C�Y������
    index_saved = 0; 

    //�L���v�`���[�f�[�^����t���[�������o��**************************
    //���ӁF������640x480�ɂȂ��Ă��遨cap�C���X�^���X���ĂԂƂ��ɐݒ肷�ׂ�
    ysOpenCv.cap >> frame;

    //�t���[������Ȃ�I��
    if (frame.empty()) return false;


    //��]�T�C�Y�Ƀ��T�C�Y
    //***********************
    //���T�C�Y�͖��ʂȂ̂� cap.set()��get()��͍�����K�v������B�J�����̎d�l�O���Ƃ��܂�������Ȃ��炵���B
    //************************
    cv::resize(frame, frame, default_size);


    //�t���[���̃s�N�Z����S����
    for (px_y = 0; px_y < frame.rows; px_y++)
    {
        //�C���^�[���[�X���M�p�X�L�b�v�E�c����
        if (inter_mod.y != 1 && px_y % inter_mod.y != inter_num.y) continue;

        //�|�C���^�̎擾
        ptr = frame.ptr<cv::Vec3b>(px_y);

        for (px_x = 0; px_x < frame.cols; px_x++)
        {
            //�C���^�[���[�X���M�p�X�L�b�v�E������
            if (inter_mod.x != 1 && px_x % inter_mod.x != inter_num.x) continue;

            //�Y�t�f�[�^���m�F
            tmp_data.bgr = ptr[px_x]; //�|�C���^��bgr�̒l bgr[0]��B�Abgr[1]���f�Abgr[2]���q
            tmp_data.reaktor_idx = (px_x + px_y * default_size.width);
            
            //���sdiff
            //�z��ɓ��ꂽ�C���f�b�N�X�̍ő�l
            index_saved = tmp_data.reaktor_idx;

            arr_ni.push_back(tmp_data);
        }
    }
    //�C���^�[���[�X����O���[�v�����炷
    ysAddInterLaceNumber();

    std::cout << "�f�[�^�i�[ �T�C�Y��" << arr_ni.size() << std::endl;
    return true;//�ۗ��@������true��if�͈Ӗ����Ȃ��ifalse�ɂ���ƁAAsync�G���[�Ŏ~�܂�j
}


//reaktorData�N���X�̔z����Z���h���郁�\�b�h
void ysSendOSC::ysSendToOSC(void)
{
    if (arr_ni.size() == 0) return; //�T�C�Y���Ȃ��Ƃ��͏I��

    UdpTransmitSocket transmitSocket(IpEndpointName(ADDRESS, PORT)); //UDP�p�̃\�P�b�g�N���X
    bool flag_overflow = true; //�o�b�t�@�t���[�t���O
    int loop_count = 0;
    int r, g, b;

    reaktorData* Pt; //vector�z��𑖍����邽�߂̃|�C���^
    Pt = &arr_ni[0]; //�N���X�����oarr_ni�̐擪�A�h���X  arr_ni�Ƃ����ł��悳����

    //�ő�C���f�b�N�X�̕ۊ�
    index_send = 0;

    do {
        if (flag_overflow) flag_overflow = false;

        char buffer[OUTPUT_BUFFER_SIZE] = { 0 }; //static���͂�����
        osc::OutboundPacketStream pstream(buffer, OUTPUT_BUFFER_SIZE);//�����static��t����ƃI�[�o�[�t���[

        //���M�J�n����
        pstream << osc::BeginBundleImmediate;
        pstream << osc::BeginMessage(osc_address);

        while (true)
        {
            if (pstream.Size() > OUTPUT_BUFFER_SIZE - sizeof(*Pt) - sizeof(int))//int�̓��[�v�J�E���^�̕��H�H�H
            {                                                                   //sizeof(*Pt)�͊��҂����l�ł͂Ȃ�����
                flag_overflow = true;
                break;
            }
            else //�I�[�o�[�t���[���Ȃ��Ȃ�f�[�^�̃o���h��
            {
                //�z��ɕۑ������C���f�b�N�X�Ƒ��M�����C���f�b�N�X�������Ȃ�I��
                if (index_send == index_saved) break;

                //�F�̒l�𒊏o
                r = (int)(*Pt).bgr[2];
                g = (int)(*Pt).bgr[1];
                b = (int)(*Pt).bgr[0];

                //24bit�̐��l�ɂ܂Ƃ߂đ���iReaktor�Ńf�R�[�h�j
                pstream << (r << 16 | g << 8 | b); 

                //���M�����C���f�b�N�X�̍ő�l��ۊ�
                index_send = (int)(*Pt).reaktor_idx;
                //idx�̃p�b�N
                pstream << (int)(*Pt).reaktor_idx;

                //�|�C���^��i�߂�i�z���i�߂�j
                ++Pt;

                #ifdef TO_PC_EXE
                    //���[�v�J�E���^�iReaktor�ł͕s�v)
                    ++loop_count;
                #endif
            }
        }
        
        #ifdef TO_PC_EXE
            //���[�v�J�E���g�̑��M Reaktor�s�v
            ysSendLoopCount(loop_count);
            loop_count = 0;
            //std::cout << "�f�[�^���M �J�E���g��" << loop_count << std::endl;
        #endif
        //////////

        //���M���b�Z�[�W�̃o���h������
        pstream << osc::EndMessage;
        pstream << osc::EndBundle;
        //�f�[�^�̑��M
        transmitSocket.Send(pstream.Data(), pstream.Size());

        //Reaktor�̕��ׂ����炷���߂̃X���[�v�i�s�N�Z�������������Ȃ�����s�v�j
        //Sleep(1);


        //�����̔��������́A�������M������Ɣ����Ȃ��Ȃ�
        //�z��ɓ��ꂽ�C���f�b�N�X�ƁA���M�����C���f�b�N�X�Ŕ�r���ău���C�N�����ق����ǂ�

    } while (Pt <= &arr_ni[arr_ni.size() - 1] && index_send != index_saved); //�|�C���^��ReaktorData�z��͈͓̔��Ȃ�J��Ԃ�

    //std::cout << "1�t���[���`��I��" << std::endl;
}


//�����T�C�Y��Reaktor�ɑ��M����
void ysSendOSC::ysSendFirstSize(ysOpenCvCapture ysOpenCv)
{
    UdpTransmitSocket sock(IpEndpointName(ADDRESS, PORT)); //UDP�p�̃\�P�b�g�N���X
    char buffer[OUTPUT_BUFFER_SIZE] = { 0 };
    osc::OutboundPacketStream pstream(buffer, OUTPUT_BUFFER_SIZE);

    //���M�J�n����
    pstream << osc::BeginBundleImmediate;
    pstream << osc::BeginMessage("/first_size");
    pstream << (int)ysOpenCv.size_for_set.width;
    pstream << (int)ysOpenCv.size_for_set.height;
    pstream << osc::EndMessage;
    pstream << osc::EndBundle;
    //�f�[�^�̑��M
    sock.Send(pstream.Data(), pstream.Size());
    std::cout << "�����T�C�Y���M" << ysOpenCv.size_for_set << std::endl;
}
