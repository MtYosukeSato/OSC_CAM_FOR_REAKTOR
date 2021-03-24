#pragma once
#include "ysOpenCvCapture.h"

//レシーブに使うアドレスをインスタンス作成時に記述できるようにする
//コンストラクタ
ysOpenCvCapture::ysOpenCvCapture(
    int argc,
    char** argv,
    cv::Size default_size,
    unsigned int fps)
    : cap_fps(fps)
{
    //起動時の引数が一つだったら、、
    if (argc == 1) cap.open(0);       //**システムのカメラデバイスが一つなら0を指定すればそれが開く
    else           cap.open(argv[1]); //引数が二つだったらファイル名と考えて開く

    if (!cap.isOpened())
    {
        std::cerr << "Couldn't open capture." << std::endl;
        exit(1); //プログラムを終了（キーを入力すると、、にしたい）***************
    }

    //サイズの保存
    size_for_set = default_size;

    //fpsの設定
    cap.set(cv::CAP_PROP_FPS, fps);
    //cap.setでサイズを設定すると、切り抜き画像のようになるのでcap >> frameをリサイズしている
}