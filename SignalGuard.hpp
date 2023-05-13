#pragma once
#include <functional>

using Disconnector = std::function<void()>;

class SignalGuard final
{
public:
    SignalGuard() = default;
    SignalGuard(Disconnector&& callableDisconnect);
    ~SignalGuard();

    SignalGuard(const SignalGuard&) = delete;
    SignalGuard& operator=(const SignalGuard&) = delete;

    SignalGuard(SignalGuard&& other) noexcept;
    SignalGuard& operator=(SignalGuard&& other) noexcept;

    void disconnect();

private:
    Disconnector callableDisconnect = nullptr;
};
