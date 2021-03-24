#pragma once
#include <iostream>
#include <opencv2/opencv.hpp> //opencv

//---------------------------------
//	OSC
//---------------------------------
#include "OscReceivedElements.h"
#include "OscPacketListener.h"
#include "OscOutboundPacketStream.h"
#include "UdpSocket.h"
#include "IpEndpointName.h"
class OSCReceiveHandler : public osc::OscPacketListener {
private:
public:
	//************************�ǋL***************************
	cv::Size received_size;

	//�Đ����Ă��邩�@�t���O����������r�b�g�t�B�[���h
	int is_reaktor_playing;

	//�C���^�[���[�X�X�L�����̒l
	cv::Point2i interlace;

	//*******************************************************

	OSCReceiveHandler() {
		received_size = cv::Size(0, 0);
		is_reaktor_playing = 0;
		interlace = cv::Point2i(1, 1);
	}
	~OSCReceiveHandler() {}

protected:
	bool isOSCReceiveBegan = false;
	virtual void ProcessMessage(const osc::ReceivedMessage& m,
		const IpEndpointName& remoteEndpoint);
};