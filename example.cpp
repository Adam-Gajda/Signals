#include "Signal.hpp"
#include "SignalGuard.hpp"
#include <functional>
#include <iostream>
#include <new>
#include <string_view>

inline Signal<std::function<void(std::string_view)>> signal;

class Example
{
public:
    Example()
    {
        signalGuard = signal.connect([this](std::string_view x) { std::cout << "Method: " << x << '\n'; });
    }
    SignalGuard signalGuard;
};

struct Functor
{
    void operator()(std::string_view x)
    {
        std::cout << "Functor: " << x << '\n';
    }
};

void function(std::string_view x)
{
    std::cout << "Function: " << x << '\n';
}

int main()
{
    std::function<void(std::string_view)> functionPtr = [](std::string_view x) {
        std::cout << "FunctionPtr: " << x << '\n';
    };
    Example* example = new Example();

    SignalGuard functorGuard = signal.connect(Functor{});
    auto disconnectLambda = signal.connect([](std::string_view x) { std::cout << "Lambda: " << x << '\n'; });
    auto disconnectFunction = signal.connect(function);
    auto disconnectFunctionPtr = signal.connect(functionPtr);
    signal.emit("First Call");

    std::cout << "\nDisconnects\n\n";
    functorGuard.disconnect();
    disconnectLambda();
    disconnectFunction();

    // disconnectFunctor(); throws bad_function_call
    signal.emit("Second Call");
    std::cout << '\n';
    delete example;// Method disconnected
    signal.emit("Third Call");
}
