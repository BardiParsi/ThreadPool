#include "LOGGER.h"


std::mutex LOGGER::logMtx;

void LOGGER::INFO(std::string message) {
    std::lock_guard<std::mutex> coutLock(logMtx);
    std::stringstream ss;
    ss << message;
    cout << ss.str() << endl;
}
