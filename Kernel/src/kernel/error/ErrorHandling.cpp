#include "error/ErrorHandling.h"

namespace error
{
    static const char* msgBuffer = nullptr;

    void set_error_message(const char* msg)
    {
        msgBuffer = msg;
    }
    const char* get_error_message()
    {
        return msgBuffer;
    }
} 