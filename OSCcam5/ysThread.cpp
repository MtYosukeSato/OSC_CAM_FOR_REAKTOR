#pragma once
#include "ysThread.h"

#include <Windows.h> //Sleep()
#include "ysOpenCvCapture.h"
#include "ysSendOSC.h"

const cv::Size DEFAULT_SIZE = cv::Size(64,48);
const int DEFAULT_FPS = 30;

//スレッド　カメラからReaktor
void ysThread::threadToReaktor()
{
    //カメラのオブジェクト
    ysOpenCvCapture ys_cam(argc, argv, DEFAULT_SIZE, DEFAULT_FPS);
    //OSCのオブジェクト
    ysSendOSC ys_osc("/osc_camera", DEFAULT_SIZE);      //コンストラクタに渡したものがOSCのアドレスになる
    
    //初期サイズのOSC送信
    ys_osc.ysSendFirstSize(ys_cam);
    //初期サイズ用に1回だけループ外で送信
    ys_osc.ysSetData2Array(ys_cam);
    ys_osc.ysSendToOSC();
    ys_osc.arr_ni.clear();

    bool is_not_empty = true;

    while (is_not_empty)
    {
        //ReaktorからのOSC　幅と高さの変更チェック
        if ((*Pt_from_Reaktor).received_size.width != 0 && (*Pt_from_Reaktor).received_size.height != 0 &&
            ys_cam.size_for_set != (*Pt_from_Reaktor).received_size)
        {
            ys_cam.size_for_set = (*Pt_from_Reaktor).received_size;//どちらもcv::Sizeクラス
            ys_osc.default_size = (*Pt_from_Reaktor).received_size;
        }

        //ReaktorからのOSC
        if (ys_osc.inter_mod != (*Pt_from_Reaktor).interlace)
        {
            ys_osc.inter_mod = (*Pt_from_Reaktor).interlace;
        }

        //カメラのオブジェクトからframeを取り出して配列にセット
        is_not_empty = ys_osc.ysSetData2Array(ys_cam);
        //ここでfalseするとasyncエラーで止まるのでifの意味がない

        if (is_not_empty)
        {
            //Reaktorの再生ボタンがおされているか
            if ((*Pt_from_Reaktor).is_reaktor_playing == 1)//できればsetDataも無効にしたいが、Asyncエラーが出る
                //配列からOSC送信
                ys_osc.ysSendToOSC();

            //送信用配列を空にする
            ys_osc.arr_ni.clear();

            Sleep(5);
        }
    }
}


//スレッド　Reaktorからカメラ
void ysThread::threadFromReaktor()
{
    OSCReceiveHandler listener;
    Pt_from_Reaktor = &listener;

    UdpListeningReceiveSocket s(
        IpEndpointName(IpEndpointName::ANY_ADDRESS, 10000),//Reaktor側の設定に合わせる
        &listener);
    //ANY_ADDRESSだと問題がありそうなので、ゆくゆくはセーフなアドレスのやりたとりを調べる

    s.RunUntilSigInt(); //これが呼び出されるとループしているもよう
}

//スレッドの実行
void ysThread::run()
{
    std::thread th_a(&ysThread::threadToReaktor, this);  //Reaktorへの画像転送
    std::thread th_b(&ysThread::threadFromReaktor, this);//Reaktorからの命令受信
    //メソッドに引数を渡すには.... (&ysThread::メソッド, this, 引数);
    //変数共用がある場合は、std::mutexが必要

    th_a.join(); //joinしないとabort()エラーがでる
    th_b.join();
}