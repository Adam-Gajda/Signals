#pragma once

#include <algorithm>
#include <deque>
#include <functional>

using Disconnetor = std::function<void()>;

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

    [[maybe_unused]] Disconnetor connect(Callable&& callable)
    {
        CallableWraper callableWrapper;
        callableWrapper.callable = std::move(callable);
        callableWrapper.id = idCounter;
        callableWrappers.push_back(std::move(callableWrapper));

        const std::size_t disconnectId = idCounter++;
        return [this, disconnectId]() {
            const auto it = std::find_if(callableWrappers.cbegin(),
              callableWrappers.cend(),
              [disconnectId](const auto& callableWrapper) { return callableWrapper.id == disconnectId; });
            if (it == callableWrappers.end())
            {
                throw std::bad_function_call{};
            }
            callableWrappers.erase(it);
        };
    }

    [[maybe_unused]] Disconnetor connect(const Callable& callable)
    {
        CallableWraper callableWrapper;
        callableWrapper.callable = callable;
        callableWrapper.id = idCounter;
        callableWrappers.push_back(std::move(callableWrapper));

        const std::size_t disconnectId = idCounter++;
        return [this, disconnectId]() {
            const auto it = std::find_if(callableWrappers.cbegin(),
              callableWrappers.cend(),
              [disconnectId](const auto& callableWrapper) { return callableWrapper.id == disconnectId; });
            if (it == callableWrappers.end())
            {
                throw std::bad_function_call{};
            }
            callableWrappers.erase(it);
        };
    }

private:
    struct CallableWraper
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
    std::deque<CallableWraper> callableWrappers;
};
