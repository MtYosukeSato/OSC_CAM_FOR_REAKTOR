#pragma once
#include <thread>
#include "OSCReceiveHandler.h"

//スレッドクラス。実質的なmain()
class ysThread
{
private:
    //int tmp_width;//スレッドごとにやり取りするものポインタで見れなければ考慮する
    //スレッドが別になっているところから、本体へのプロパティ参照・アドレス参照はおかしいかも

    int argc;    //mainの引数データ用
    char** argv; //mainの引数データ用
    OSCReceiveHandler* Pt_from_Reaktor; //Receiveしたデータを参照するためのポインタ threadFromReaktorで使用

    void threadToReaktor();   //Reaktorからのデータ受信
    void threadFromReaktor(); //Reaktorへのデータ送信

public:
    void run();

    ysThread(int c, char** v) : argc(c), argv(v) { Pt_from_Reaktor = nullptr; }; //コンストラクタ
    ~ysThread() {};                                 //デストラクタ

};
