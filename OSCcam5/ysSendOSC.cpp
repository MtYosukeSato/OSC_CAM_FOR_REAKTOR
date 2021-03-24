#pragma once
#include "ysSendOSC.h"

#include "OscReceivedElements.h"     //OSC
#include "OscPacketListener.h"       //OSC
#include "OscOutboundPacketStream.h" //OSC
#include "UdpSocket.h"               //OSC
#include "IpEndpointName.h"          //OSC

#include <Windows.h>

//#define TO_PC_EXE //PCのexeでOSCをまとめ受信する際にargment数を合わせるのに必要


void ysSendOSC::ysPrintSendNotice(reaktorData rdata)
{
    std::cout << " r:" << (int)rdata.bgr[2] << " g:" << (int)rdata.bgr[1] << " b:" << (int)rdata.bgr[0];
    std::cout << " idx:" << rdata.reaktor_idx << std::endl;
}

//インターレーススキャン用の値セット関数
void ysSendOSC::ysAddInterLaceNumber(void)
{
    inter_num.y = ++inter_num.y % inter_mod.y;
    inter_num.x = ++inter_num.x % inter_mod.x;
    //mod_num_interは同じ数値にならないようにすること(割り切れないようにする)
    //映像モードとデータモードで、映像モードの時だけインターレースにするか
}


//OSCデータを配列にセット　（最終的にはcv::配列のままで処理できそうかも）　　　　　　　　　　　　　　　　　　
bool ysSendOSC::ysSetData2Array(ysOpenCvCapture ysOpenCv)//cv::VideoCapture caputure_data)
{
    cv::Mat frame;            //cv::VideoCapture cからの読み取ったフレームを保管する
    cv::Vec3b* ptr;           //フレームのポインタを保管する
    reaktorData tmp_data;     //比較などするようの保管配列
    int px_y = 0, px_x = 0;   //現在読み取っている動画のピクセル場所
    
    //送信用配列に入れたindexサイズ初期化
    index_saved = 0; 

    //キャプチャーデータからフレームを取り出し**************************
    //注意：ここで640x480になっている→capインスタンスを呼ぶときに設定すべき
    ysOpenCv.cap >> frame;

    //フレームが空なら終了
    if (frame.empty()) return false;


    //希望サイズにリサイズ
    //***********************
    //リサイズは無駄なので cap.set()とget()を模索する必要がある。カメラの仕様外だとうまくあたらないらしい。
    //************************
    cv::resize(frame, frame, default_size);


    //フレームのピクセルを全走査
    for (px_y = 0; px_y < frame.rows; px_y++)
    {
        //インターレース送信用スキップ・縦方向
        if (inter_mod.y != 1 && px_y % inter_mod.y != inter_num.y) continue;

        //ポインタの取得
        ptr = frame.ptr<cv::Vec3b>(px_y);

        for (px_x = 0; px_x < frame.cols; px_x++)
        {
            //インターレース送信用スキップ・横方向
            if (inter_mod.x != 1 && px_x % inter_mod.x != inter_num.x) continue;

            //添付データを確認
            tmp_data.bgr = ptr[px_x]; //ポインタがbgrの値 bgr[0]がB、bgr[1]がＧ、bgr[2]がＲ
            tmp_data.reaktor_idx = (px_x + px_y * default_size.width);
            
            //失敗diff
            //配列に入れたインデックスの最大値
            index_saved = tmp_data.reaktor_idx;

            arr_ni.push_back(tmp_data);
        }
    }
    //インターレースするグループをずらす
    ysAddInterLaceNumber();

    std::cout << "データ格納 サイズは" << arr_ni.size() << std::endl;
    return true;//保留　ここのtrueとifは意味がない（falseにすると、Asyncエラーで止まる）
}


//reaktorDataクラスの配列をセンドするメソッド
void ysSendOSC::ysSendToOSC(void)
{
    if (arr_ni.size() == 0) return; //サイズがないときは終了

    UdpTransmitSocket transmitSocket(IpEndpointName(ADDRESS, PORT)); //UDP用のソケットクラス
    bool flag_overflow = true; //バッファフローフラグ
    int loop_count = 0;
    int r, g, b;

    reaktorData* Pt; //vector配列を走査するためのポインタ
    Pt = &arr_ni[0]; //クラスメンバarr_niの先頭アドレス  arr_niとだけでもよさそう

    //最大インデックスの保管
    index_send = 0;

    do {
        if (flag_overflow) flag_overflow = false;

        char buffer[OUTPUT_BUFFER_SIZE] = { 0 }; //staticをはずした
        osc::OutboundPacketStream pstream(buffer, OUTPUT_BUFFER_SIZE);//これにstaticを付けるとオーバーフロー

        //送信開始処理
        pstream << osc::BeginBundleImmediate;
        pstream << osc::BeginMessage(osc_address);

        while (true)
        {
            if (pstream.Size() > OUTPUT_BUFFER_SIZE - sizeof(*Pt) - sizeof(int))//intはループカウンタの分？？？
            {                                                                   //sizeof(*Pt)は期待した値ではないかも
                flag_overflow = true;
                break;
            }
            else //オーバーフローしないならデータのバンドル
            {
                //配列に保存したインデックスと送信したインデックスが同じなら終了
                if (index_send == index_saved) break;

                //色の値を抽出
                r = (int)(*Pt).bgr[2];
                g = (int)(*Pt).bgr[1];
                b = (int)(*Pt).bgr[0];

                //24bitの数値にまとめて送る（Reaktorでデコード）
                pstream << (r << 16 | g << 8 | b); 

                //送信したインデックスの最大値を保管
                index_send = (int)(*Pt).reaktor_idx;
                //idxのパック
                pstream << (int)(*Pt).reaktor_idx;

                //ポインタを進める（配列を進める）
                ++Pt;

                #ifdef TO_PC_EXE
                    //ループカウンタ（Reaktorでは不要)
                    ++loop_count;
                #endif
            }
        }
        
        #ifdef TO_PC_EXE
            //ループカウントの送信 Reaktor不要
            ysSendLoopCount(loop_count);
            loop_count = 0;
            //std::cout << "データ送信 カウントは" << loop_count << std::endl;
        #endif
        //////////

        //送信メッセージのバンドル処理
        pstream << osc::EndMessage;
        pstream << osc::EndBundle;
        //データの送信
        transmitSocket.Send(pstream.Data(), pstream.Size());

        //Reaktorの負荷を減らすためのスリープ（ピクセル数が小さくなったら不要）
        //Sleep(1);


        //ここの抜け条件は、差分送信をすると抜けなくなる
        //配列に入れたインデックスと、送信したインデックスで比較してブレイクしたほうが良い

    } while (Pt <= &arr_ni[arr_ni.size() - 1] && index_send != index_saved); //ポインタがReaktorData配列の範囲内なら繰り返し

    //std::cout << "1フレーム描画終了" << std::endl;
}


//初期サイズをReaktorに送信する
void ysSendOSC::ysSendFirstSize(ysOpenCvCapture ysOpenCv)
{
    UdpTransmitSocket sock(IpEndpointName(ADDRESS, PORT)); //UDP用のソケットクラス
    char buffer[OUTPUT_BUFFER_SIZE] = { 0 };
    osc::OutboundPacketStream pstream(buffer, OUTPUT_BUFFER_SIZE);

    //送信開始処理
    pstream << osc::BeginBundleImmediate;
    pstream << osc::BeginMessage("/first_size");
    pstream << (int)ysOpenCv.size_for_set.width;
    pstream << (int)ysOpenCv.size_for_set.height;
    pstream << osc::EndMessage;
    pstream << osc::EndBundle;
    //データの送信
    sock.Send(pstream.Data(), pstream.Size());
    std::cout << "初期サイズ送信" << ysOpenCv.size_for_set << std::endl;
}
