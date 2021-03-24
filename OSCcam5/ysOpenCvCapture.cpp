#pragma once
#include "ysOpenCvCapture.h"

//���V�[�u�Ɏg���A�h���X���C���X�^���X�쐬���ɋL�q�ł���悤�ɂ���
//�R���X�g���N�^
ysOpenCvCapture::ysOpenCvCapture(
    int argc,
    char** argv,
    cv::Size default_size,
    unsigned int fps)
    : cap_fps(fps)
{
    //�N�����̈��������������A�A
    if (argc == 1) cap.open(0);       //**�V�X�e���̃J�����f�o�C�X����Ȃ�0���w�肷��΂��ꂪ�J��
    else           cap.open(argv[1]); //���������������t�@�C�����ƍl���ĊJ��

    if (!cap.isOpened())
    {
        std::cerr << "Couldn't open capture." << std::endl;
        exit(1); //�v���O�������I���i�L�[����͂���ƁA�A�ɂ������j***************
    }

    //�T�C�Y�̕ۑ�
    size_for_set = default_size;

    //fps�̐ݒ�
    cap.set(cv::CAP_PROP_FPS, fps);
    //cap.set�ŃT�C�Y��ݒ肷��ƁA�؂蔲���摜�̂悤�ɂȂ�̂�cap >> frame�����T�C�Y���Ă���
}