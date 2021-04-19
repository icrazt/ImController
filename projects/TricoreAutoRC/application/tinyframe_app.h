extern "C" {
#include "TinyFrame.h"
#include "utilities/utils.h"
#include "utilities/payload_builder.h"
#include "utilities/payload_parser.h"
}

#include "serial.h"

// crazt implement:

void TF_UartInit(void);
void TF_UartRead(TinyFrame* tf);
TinyFrame* TF_AppInit(void);

