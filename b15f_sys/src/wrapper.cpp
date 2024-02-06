#include "wrapper.hpp"
#include  <exception>

B15F* tryGetInstance(ConnectionError& error) {
    B15F* instance = nullptr;
    try {
        instance = &B15F::getInstance();
    } catch(const std::exception& except) {
        error = ConnectionError::Err;
    }
    error = ConnectionError::None;
    return instance;
}