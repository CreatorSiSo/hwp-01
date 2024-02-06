#include "b15f/b15f.h"
#include <cstdint>

enum class ConnectionError: uint8_t {
    None = 0,
    Err = 1,
};

B15F* tryGetInstance(ConnectionError& error);