#pragma once
#include "OSCReceiveHandler.h"

void OSCReceiveHandler::ProcessMessage(const osc::ReceivedMessage& m,
	const IpEndpointName& remoteEndpoint)
{
	(void)remoteEndpoint;
	//cv::Size�ւ̎�M
	try {
		if (strcmp(m.AddressPattern(), "/pic_size") == 0) //���M���Ŏw�肵���A�h���X
		{
			osc::ReceivedMessage::const_iterator pic_arg = m.ArgumentsBegin();
			int w = (int)(pic_arg++)->AsFloat();
			int h = (int)(pic_arg++)->AsFloat();
			received_size = cv::Size(w, h);
			std::cout << "�T�C�Y��M " << received_size << std::endl;

			if (pic_arg != m.ArgumentsEnd())
			{
				throw osc::ExcessArgumentException();
			}
		}

	}
	catch (osc::Exception& e) {
		std::cout << "OSC Receive: Error while parsing message: "
			<< m.AddressPattern() << ": " << e.what() << "\n";
	}


	//Reaktor���Đ������ǂ���
	try {
		if (strcmp(m.AddressPattern(), "/R_is_playing") == 0) //���M���Ŏw�肵���A�h���X
		{
			osc::ReceivedMessage::const_iterator is_playing_arg = m.ArgumentsBegin();
			is_reaktor_playing = (int)(is_playing_arg++)->AsFloat();
			std::cout << "�Đ�����M " << is_reaktor_playing << std::endl;

			if (is_playing_arg != m.ArgumentsEnd())
			{
				throw osc::ExcessArgumentException();
			}
		}

	}
	catch (osc::Exception& e) {
		std::cout << "OSC Receive: Error while parsing message: "
			<< m.AddressPattern() << ": " << e.what() << "\n";
	}


	//�C���^�[���[�X�X�L�����̒l
	try {
		if (strcmp(m.AddressPattern(), "/interlace") == 0) //���M���Ŏw�肵���A�h���X
		{
			osc::ReceivedMessage::const_iterator inter_arg = m.ArgumentsBegin();
			int x = (int)(inter_arg++)->AsFloat();
			int y = (int)(inter_arg++)->AsFloat();
			interlace = cv::Point(x, y);
			std::cout << "�C���^�[���[�X��XY��M " << interlace << std::endl;

			if (inter_arg != m.ArgumentsEnd())
			{
				throw osc::ExcessArgumentException();
			}
		}

	}
	catch (osc::Exception& e) {
		std::cout << "OSC Receive: Error while parsing message: "
			<< m.AddressPattern() << ": " << e.what() << "\n";
	}
}