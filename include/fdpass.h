#ifndef FDPASS_H
#define FDPASS_H

#include "bot.h"

enum fdpass_control_types {
FDPASS_CONTROL_TYPE_REGISTER = 1,
FDPASS_CONTROL_TYPE_PUT,
FDPASS_CONTROL_TYPE_GET,
FDPASS_CONTROL_TYPE_NOP,
};

enum fdpass_control_subtypes { 
FDPASS_CONTROL_SUBTYPE_TCP = 1,
FDPASS_CONTROL_SUBTYPE_UDP,
FDPASS_CONTROL_SUBTYPE_RAW,
FDPASS_CONTROL_SUBTYPE_NOP,
};

typedef struct fdpass_control {
unsigned char type;
unsigned char subtype;

} fdpass_control_t;


#endif
