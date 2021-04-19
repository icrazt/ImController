#include "tinyframe_app.h"
#include "serial.h"
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

void TF_WriteImpl(TinyFrame* tf, const uint8_t* buff, uint32_t len)
{
    // send to UART
    SerialHandle.Open(5, 115200);
}

TF_Result testListener(TinyFrame* tf, TF_Msg* msg)
{
    dumpFrameInfo(msg);
    return TF_STAY;
}

TinyFrame* TF_AppInit(void)
{
    tf_app = TF_Init(TF_MASTER);
    TF_AddGenericListener(tf_app, testListener);
    printf("------ sending messages --------\n");

    TF_ClearMsg(&msg);
    msg.type = 0x22;
    msg.data = (pu8)"ImController Ready";
    msg.len = sizeof("ImController Ready")+1;
    TF_Send(tf_app, &msg);

    printf("------ waiting a message --------\n");
    return tf_app;
}


