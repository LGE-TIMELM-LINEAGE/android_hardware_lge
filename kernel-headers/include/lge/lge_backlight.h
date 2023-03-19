#pragma once

#include <sys/ioctl.h>
// File descriptor is an attribute file which takes ASCII char as input. Tested with echo command so newline character is included.
const char  LGE_FP_LHBM_OFF = '0';
const char  LGE_FP_LHBM_ON = '1';
const char  LGE_FP_LHBM_READY = '2';
const char  LGE_FP_LHBM_EXIT = '3';
