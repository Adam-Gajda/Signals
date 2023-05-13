#include "SignalGuard.hpp"
#include <iostream>
#include <utility>

SignalGuard::SignalGuard(Disconnector&& callableDisconnect) : callableDisconnect{ std::move(callableDisconnect) }
{
}

SignalGuard::SignalGuard(SignalGuard&& other) noexcept
  : callableDisconnect{ std::exchange(other.callableDisconnect, nullptr) }
{
}

SignalGuard& SignalGuard::operator=(SignalGuard&& other) noexcept
{
    callableDisconnect = std::exchange(other.callableDisconnect, nullptr);
    return *this;
}

SignalGuard::~SignalGuard()
{
    disconnect();
}

void SignalGuard::disconnect()
{
    if (callableDisconnect)
    {
        callableDisconnect();
        callableDisconnect = nullptr;
    }
}
