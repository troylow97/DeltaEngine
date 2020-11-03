#pragma once
#include <cstdint>
#include <functional>

namespace DeltaEngine
{

using byte = uint8_t;
using size = size_t;

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

constexpr u8 u8_max = UINT8_MAX;
constexpr u16 u16_max = UINT16_MAX;
constexpr u32 u32_max = UINT32_MAX;
constexpr u64 u64_max = UINT64_MAX;

using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

constexpr i8 i8_min = INT8_MIN;
constexpr i8 i8_max = INT8_MAX;
constexpr i16 i16_min = INT16_MIN;
constexpr i16 i16_max = INT16_MAX;
constexpr i32 i32_min = INT32_MIN;
constexpr i32 i32_max = INT32_MAX;
constexpr i64 i64_min = INT64_MIN;
constexpr i64 i64_max = INT64_MAX;

using ptr = void *;
using uptr = uintptr_t;
using iptr = intptr_t;
using ptrdiff = ptrdiff_t;

using f32 = float;
using f64 = double;

template <typename... Args>
using Action = std::function<void( Args... )>;

template <typename Result, typename... Args>
using Func = std::function<Result( Args... )>;

}

#define DE_BIND_EVENT_FN(fn) [&](auto&&... args) -> decltype(auto) { return fn(std::forward<decltype(args)>(args)...); }