# Simple-Signal-Lib

Example:
```
inline Signal<std::function<void(std::string_view)>> signal;

signal.connect([](std::string_view x) { std::cout << "Lambda: " << x << '\n'; });
signal.connect([](std::string_view x) { std::cout << "OtherLambda: " << x << '\n'; });

signal.emit("Call");
```
Output:

```
Lamda: Call
OtherLamda: Call
```

