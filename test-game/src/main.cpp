#include "pch.hpp"
#include "ion.hpp"

int main([[maybe_unused]] int argc, [[maybe_unused]] const char **argv)
{
    ion::Logger::scoped("D:\\src\\c++\\ion\\logs", [](ion::Logger& logger) 
    {
        logger.info("Test", 1, 2, 3);
    });
    
    return 0;
}