# OSC CAM FOR REAKTOR
OpenCV��Web�J�����f�����擾���A�����NativeInstruments Reaktor6�ɑ��M���ĉf���\��������v���O����

# Description
�J�������̖͂������y�p�\�t�g�E�F�A�ł���NativeInstruments Reaktor�ɂ�OSC(OpenSoundControl)�̑���M���W���[����
�_�f�[�^����\��������Multidisplay���W���[�������邱�Ƃ���A�J�����̃f�[�^��K�؂Ȍ`�ɂ��đ��M������΁A
�J�������͂Ƃ��ė��p�ł���̂ł͂Ȃ����Ǝv���܂����B����Ŏ����ɍ���Ă݂����̂ł��B

#Demo
![�X�N���[���V���b�g](https://raw.github.com/wiki/MtYosukeSato/OSC_CAM_FOR_REAKTOR/img/capture.png)

#Usage
1.NativeInstruments Reaktor�̊Y���A���T���u�����J���܂��B
2.�t�@�C����OSC Setting��OSC Active�`�F�b�N�{�b�N�X���I���ɂ��܂��B
3.Reaktor��Receive�^�u�ŁAReceive on Port��C++��ysSendOSC.h�ň�v���Ă��邩���m�F���܂��B
4.C++��ysSendOSC.h�Őݒ肵�Ă���IP�A�h���X��Reaktor�����삵�Ă���PC�ƈ�v���Ă��邩���m�F���܂��B
�i����PC�̏ꍇ��127.0.0.1���ǂ������m�F���܂��B�j
4.Reaktor��Send�^�u�Ń|�[�g��ݒ肵�AC++��ysThread.cpp�ƈ�v���邩���m�F���܂�
5.Reaktor��Send�Őݒ肵�Ă���IP�A�h���X���A�J�������ڑ����ꂽPC�ƈ�v���邩���m�F���܂��B
�i����PC�̏ꍇ��127.0.0.1���ǂ������m�F���܂��B�j

6.OSC_CAM_FOR_REAKTOR.exe���N���b�N�����Web�J�����̉f�����M���͂��߂܂��B
7.Reaktor�̃t�@�C����OSC Setting��Receive�^�u�ɂ���Incoming OSC Monitor�Ƀf�[�^���\������邱�Ƃ��m�F���܂��B
8.Reaktor�̃A���T���u���ɖ߂�ASpece�L�[�܂��͍Đ��{�^���̃N���b�N�����ē��삳���܂��B

Reaktor���̂b�o�t�g�p�����n�u�d�q�ƂȂ��M�T�C�Y���������ύX�������Ƃ��́A�v�Ƃg�̃c�}�~�𒲐߂��܂��B
�܂��A�����̒x�����̂�\������ꍇ�́ASend Interlace�𒲐߂���ƃC���^�[���[�X�ő���M����悤�ɂȂ蕉�ׂ��y������܂��B
�iOSC��M�̏�������R����ق�Reaktor��CPU���ׂ������܂��B�j

#Requirement
����m�F���͂ǂ����Windows10�ł��B

#Author
���� �m��

#License
�m�F���ł��B���̂Ƃ��돤�p���p�����ł��Ȃ��Z�p�f���ł��B
�i�O�����C�u�����Ƃ��āAOpenCV��oscpack�𗘗p���Ă��܂��B�j