#ifndef _NEXTION_H

#define _NEXTION_H



#include "main.h"



// return data first byte

#define NEX_TOUCH_EVENT       0x65

#define NEX_CURR_PAGE_ID      0x66

#define NEX_TOUCH_COORDS      0x67

#define NEX_TOUCH_EVENT_SLEEP 0x68

#define NEX_STRING_VAR_DATA   0x70

#define NEX_NUMERIC_VAR_DATA  0x71

#define NEX_AUTO_SLEEP        0x86

#define NEX_AUTO_WAKEUP       0x87

#define NEX_START_SUCCESS     0x88

#define NEX_SD_DETECT         0x89

#define NEX_VAR_NAME_INVALID  0x1A

#define NEX_UNKNOWN           0xFF

#define NEX_RETURN_SIGN_NULL  0





extern char NexGetPageID(void);

extern void NexCommand(char *command);

extern void NexGotoPage(int page_id);

extern void NexSendTxt(char *object, char *text);

extern void NexSetVisibility(char *object, char vis);

#endif
