# Signals

Example:
```
inline Signal<std::function<void(std::string_view)>> signal;

SignalGuard lambdaGuard = signal.connect([](std::string_view x) { std::cout << "Lambda: " << x << '\n'; });
SignalGuard otherLambdaGuard = signal.connect([](std::string_view x) { std::cout << "OtherLambda: " << x << '\n'; });

signal.emit("Call");
```
Output:

```
Lamda: Call
OtherLamda: Call
```

