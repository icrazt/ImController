#include <mutex>
#include <assert.h>
#include "tinyframe_app.h"

enum IMC_ConnectionType
{
	UsingSerial = 0,
	UsingWebsocket = 1        
};

class IMC_DataClass {
	private:
		std::mutex data_mutex;
		
	public:
		IMC_ConnectionType ConnectionType = UsingWebsocket;
		TinyFrame* tf_handle     = NULL;
		CSerial*   serial_handle = NULL;
		// TODO:temporary data ,up down left right 
		float      XBOX_Ctl[4]   ={0};   
		double     time_stamp = 0;

		int receive(std::string str) {
			if (ConnectionType == UsingSerial)
			{
				assert(serial_handle != NULL);
				return 0;
			}
			if (ConnectionType == UsingWebsocket)
			{
				assert(tf_handle != NULL);
				TF_Accept(tf_handle, (uint8_t*)str.c_str(), str.size());
				return 0;
			}
		}
		// Transmit data build by TinyFrame
		// TODO: input parament : TF_FrameType ,data buff, len
		void transmit() {
			if (ConnectionType == UsingSerial)
			{
				assert(serial_handle != NULL);
			}
			if (ConnectionType == UsingWebsocket)
			{
				assert(tf_handle != NULL);
				TF_Msg demo_msg;
				demo_msg.type = 0x01;
				std::string str = "Hello world!";
				demo_msg.data = (const uint8_t*)str.c_str();
				demo_msg.len = str.size()+1;
				TF_Send(tf_handle, &demo_msg);
			}
			
		}
};

extern IMC_DataClass imcdata;
