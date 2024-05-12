#include "callremote.h"

void FuncCaller::call(std::string functionName, std::vector<std::string> args)
{
    try {
        functions[functionName](args);
    }
    catch (const std::exception& e) {
        spdlog::error(e.what());
    }
}

FuncCaller::FuncCaller()
{
}

void FuncCaller::test(std::vector<std::string> args)
{
    for (auto start = args.begin(); start != args.end(); start++) {
        spdlog::info(*start);
    }

    spdlog::info("you have entered to test function");
}
