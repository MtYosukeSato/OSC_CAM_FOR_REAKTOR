# OSC CAM FOR REAKTOR
OpenCVでWebカメラ映像を取得し、それをNativeInstruments Reaktor6に送信して映像表示させるプログラム

# Description
カメラ入力の無い音楽用ソフトウェアであるNativeInstruments ReaktorにはOSC(OpenSoundControl)の送受信モジュールと
点データ等を表示させるMultidisplayモジュールがあることから、カメラのデータを適切な形にして送信をすれば、
カメラ入力として利用できるのではないかと思いました。それで試しに作ってみたものです。

#Demo
![スクリーンショット](https://raw.github.com/wiki/MtYosukeSato/OSC_CAM_FOR_REAKTOR/img/capture.png)

#Usage
1.NativeInstruments Reaktorの該当アンサンブルを開きます。
2.ファイル→OSC SettingでOSC Activeチェックボックスをオンにします。
3.ReaktorのReceiveタブで、Receive on PortがC++のysSendOSC.hで一致しているかを確認します。
4.C++のysSendOSC.hで設定しているIPアドレスがReaktorが動作しているPCと一致しているかを確認します。
（同じPCの場合は127.0.0.1かどうかを確認します。）
4.ReaktorのSendタブでポートを設定し、C++のysThread.cppと一致するかを確認します
5.ReaktorのSendで設定しているIPアドレスが、カメラが接続されたPCと一致するかを確認します。
（同じPCの場合は127.0.0.1かどうかを確認します。）

6.OSC_CAM_FOR_REAKTOR.exeをクリックするとWebカメラの映像送信をはじめます。
7.Reaktorのファイル→OSC SettingのReceiveタブにあるIncoming OSC Monitorにデータが表示されることを確認します。
8.Reaktorのアンサンブルに戻り、Speceキーまたは再生ボタンのクリックをして動作させます。

Reaktor側のＣＰＵ使用率がＯＶＥＲとなり受信サイズを小さく変更したいときは、ＷとＨのツマミを調節します。
また、動きの遅いものを表示する場合は、Send Interlaceを調節するとインターレースで送受信するようになり負荷が軽減されます。
（OSC受信の処理が沢山あるほどReaktorのCPU負荷が増します。）

#Requirement
動作確認環境はどちらもWindows10です。

#Author
佐藤 洋介

#License
確認中です。今のところ商用利用等ができない技術デモです。
（外部ライブラリとして、OpenCVとoscpackを利用しています。）