#pragma once
#include <string>
#include "ysOpenCvCapture.h"
#include "reaktorData.h" //#include <opencv2/opencv.hpp>


//#define ADDRESS "127.0.0.1" //ローカルホスト（保留：モードを引数で切り替えられるようにすること）
#define ADDRESS "192.168.10.200"
#define PORT 8000
#define OUTPUT_BUFFER_SIZE 3096 


//OSC送信するデータクラス
class ysSendOSC {
public:
    char osc_address[255];           //OSCのアドレス
    cv::Size default_size;           //映像の幅と高さ ysThreadで更新される
    std::vector<reaktorData> arr_ni; //OSC送信するデータを格納するの配列
    
    //可変インターレーススキャン
    cv::Point2i inter_mod; //何グループに分けるか（mod用）
    cv::Point2i inter_num; //現在の配列内modグループ

    bool ysSetData2Array(ysOpenCvCapture ysOpenCv);//データをarr_niにセット
    void ysSendToOSC(void);                        //arr_niをOSC送信するもの

    //コンストラクタ
    ysSendOSC(const char* name, cv::Size size)
    {
        strcpy(osc_address, name); //OSCアドレスの名前（フォルダ名みたいなやつ）
        default_size = size;

        //インターレースの初期値
        inter_num = cv::Point2i(0,0);//x,y
        inter_mod = cv::Point2i(1,1);//x,y 5,2
    };

    //初期サイズの送信
    void ysSendFirstSize(ysOpenCvCapture ysOpenCv);

private:
    int index_saved; //配列に入れたインデックス（最大）
    int index_send;  //送信したインデックス（最大）

    //インターレース用の値のセット
    void ysAddInterLaceNumber(void);

    //コンソールへの出力　デバッグ用
    void ysPrintSendNotice(reaktorData rdata);
};