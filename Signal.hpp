#pragma once

#include <algorithm>
#include <deque>
#include <functional>

using Disconnector = std::function<void()>;

template<typename Callable>
class Signal
{
public:
    template<typename... Args>
    void emit(Args&&... args) const
    {
        for (const auto& callable : callableWrappers)
        {
            callable(args...);
        }
    }

    [[maybe_unused]] Disconnector connect(Callable&& callable)
    {
        CallableWrapper callableWrapper;
        callableWrapper.callable = std::move(callable);
        callableWrapper.id = idCounter;
        callableWrappers.push_back(std::move(callableWrapper));

        const std::size_t disconnectId = idCounter++;
        return [this, disconnectId]() {
            const auto it = std::find_if(callableWrappers.cbegin(),
              callableWrappers.cend(),
              [disconnectId](const auto& callableWrapper) { return callableWrapper.id == disconnectId; });
            if (it == callableWrappers.cend())
            {
                throw std::bad_function_call{};
            }
            callableWrappers.erase(it);
        };
    }

    [[maybe_unused]] Disconnector connect(const Callable& callable)
    {
        CallableWrapper callableWrapper;
        callableWrapper.callable = callable;
        callableWrapper.id = idCounter;
        callableWrappers.push_back(std::move(callableWrapper));

        const std::size_t disconnectId = idCounter++;
        return [this, disconnectId]() {
            const auto it = std::find_if(callableWrappers.cbegin(),
              callableWrappers.cend(),
              [disconnectId](const auto& callableWrapper) { return callableWrapper.id == disconnectId; });
            if (it == callableWrappers.cend())
            {
                throw std::bad_function_call{};
            }
            callableWrappers.erase(it);
        };
    }

private:
    struct CallableWrapper
    {
        Callable callable;
        std::size_t id;

        template<typename... Args>
        void operator()(Args&&... args) const
        {
            callable(std::forward<Args>(args)...);
        }
    };
    std::size_t idCounter = 0;
    std::deque<CallableWrapper> callableWrappers;
};
