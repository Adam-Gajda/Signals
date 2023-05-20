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
    SignalGuard lamdaGuard = signal.connect([](std::string_view x) { std::cout << "Lambda: " << x << '\n'; });
    SignalGuard funtionGuard = signal.connect(function);
    {
        SignalGuard funtionPtrGuard = signal.connect(functionPtr);
        funtionPtrGuard.detach();
    }
    signal.emit("First Call");

    std::cout << "\nDisconnects\n\n";
    functorGuard.disconnect();
    lamdaGuard.disconnect();
    funtionGuard.disconnect();

    signal.emit("Second Call");
    std::cout << '\n';
    delete example;// Method disconnected
    signal.emit("Third Call");
}
