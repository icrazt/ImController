#include <mutex>
#include <assert.h>
#include "tinyframe_app.h"
#include "payload_builder.h"
#include "autorc.h"

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
			if(ConnectionType == UsingSerial)
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
		// TODO: split ctrl and status part
		void transmit() {
			//Choose transmit method in TF_WriteImpl, decided by ConnectionType
			assert(tf_handle != NULL);
			// Build TF message
			int capacity = sizeof(AutoRC_DataStruct);
			uint8_t* buf = (uint8_t*)malloc(capacity);
			PayloadBuilder pb;
			pb.start = buf;
			pb.current = buf;
			pb.end = (buf)+(capacity);
			pb.bigendian = 1;
			pb.full_handler = pb_full_cb;
			pb.ok = 1;
			//Control data transmit
			pb_u32(&pb, 0); 
			pb_float(&pb, XBOX_Ctl[0]);
			pb_float(&pb, XBOX_Ctl[1]);
			pb_float(&pb, 0);
			//Status data,no use ,delete in the future
			pb_float(&pb, 1.0);
			pb_float(&pb, 1.0);
			pb_float(&pb, 1.0);
			assert(pb.ok);
			
			TF_Msg demo_msg;
			demo_msg.type = 0x01;
			demo_msg.data = buf;
			demo_msg.len  = capacity+1;
			TF_Send(tf_handle, &demo_msg);
			
			free(buf);
		}
};

extern IMC_DataClass imcdata;
