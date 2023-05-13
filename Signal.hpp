#pragma once
#include <algorithm>
#include <cstddef>
#include <deque>
#include <functional>
#include <utility>

using Disconnector = std::function<void()>;

template<typename Callable>
class Signal final
{
public:
    Signal() = default;
    ~Signal() = default;
    Signal(const Signal&) = delete;
    Signal& operator=(const Signal&) = delete;
    Signal(Signal&&) = delete;
    Signal& operator=(Signal&&) = delete;

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
        callableWrappers.emplace_back(std::move(callable), idCounter);

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
        callableWrappers.emplace_back(callable, idCounter);

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
        CallableWrapper(Callable&& callable, std::size_t id) : callable(std::move(callable)), id(id)
        {
        }
        CallableWrapper(const Callable& callable, std::size_t id) : callable(callable), id(id)
        {
        }
        template<typename... Args>
        void operator()(Args&&... args) const
        {
            callable(std::forward<Args>(args)...);
        }

        Callable callable;
        std::size_t id;
    };
    std::size_t idCounter = 0;
    std::deque<CallableWrapper> callableWrappers;
};
