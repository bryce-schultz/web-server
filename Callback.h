#pragma once
#include <functional>

template
<typename T1, typename T2>
using Callback = std::function<void(T1, T2)>;
