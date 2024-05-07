#pragma once
#include <iostream>
#include <sstream>
#include <mutex>

using std::cout;
using std::endl;

//using LOGGER class instead of std::cout during this project for efficiency and readability.
class LOGGER
{
public:
static std::mutex logMtx;
static void INFO(std::string message);
};
