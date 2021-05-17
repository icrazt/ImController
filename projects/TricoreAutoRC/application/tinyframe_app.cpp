#include "tinyframe_app.h"
#include "payload_parser.h"
#include "autorc.h"
#include "stdio.h"

CSerial SerialHandle;
TinyFrame* tf_app;
TF_Msg msg;

void TF_UartInit(void)
{
    SerialHandle.Open(5, 115200);
}
void TF_UartRead(TinyFrame* tf)
{
    char* buff = new char[400];
    uint32_t len = SerialHandle.ReadData(buff, 400);
    if (len > 0)
    {
        TF_Accept(tf, (uint8_t*)buff, len);
    }
}
bool pb_full_cb(PayloadBuilder* pb, uint32_t needed)
{
    pb_rewind(pb);
    printf("payload build ok\n");
    return pb->ok=true;
}
bool pp_empty_cb(PayloadParser *pp, uint32_t needed)
{
    pp_rewind(pp);
    printf("payload parser ok\n");
    return pp->ok=true;
}

//void TF_WriteImpl(TinyFrame* tf, const uint8_t* buff, uint32_t len)
//{
//    // send to UART
//    SerialHandle.Open(5, 115200);
//}

TF_Result testListener(TinyFrame* tf, TF_Msg* msg)
{
    dumpFrameInfo(msg);
    return TF_STAY;
}
// Process Frame Type 0x01 
TF_Result AutoRC_TypeListener(TinyFrame* tf, TF_Msg* msg)
{
    // payload parser
    AutoRC_DataStruct RecvData;
    int capacity = msg->len;
    PayloadParser pp;
    pp.start = (uint8_t*)msg->data;
    pp.current=(uint8_t*)msg->data;
    pp.end= (uint8_t*)msg->data+capacity;
    pp.empty_handler=pp_empty_cb;
    pp.bigendian=1;
    pp.ok=1;

    AutoCurrentStatus.command       = pp_u32(&pp);
    AutoCurrentStatus.setspeed[0]   = pp_float(&pp);
    AutoCurrentStatus.setspeed[1]   = pp_float(&pp);
    AutoCurrentStatus.bearing       = pp_float(&pp);
    //status data
    AutoCurrentStatus.systick       = pp_float(&pp);
    AutoCurrentStatus.motorspeed[0] = pp_float(&pp);
    AutoCurrentStatus.motorspeed[1] = pp_float(&pp);
    return TF_STAY;
}

TinyFrame* TF_AppInit(void)
{
    tf_app = TF_Init(TF_MASTER);
    // TF_AddGenericListener(tf_app, testListener);
    TF_AddTypeListener(tf_app,0x01,AutoRC_TypeListener);
    printf("------ sending messages --------\n");

    TF_ClearMsg(&msg);
    msg.type = 0x22;
    msg.data = (pu8)"ImController Ready";
    msg.len = sizeof("ImController Ready")+1;
    TF_Send(tf_app, &msg);

    printf("------ waiting a message --------\n");
    return tf_app;
}


