#pragma once

#include <type_traits>

#if defined(__cpp_lib_to_underlying) && __cpp_lib_to_underlying

using std::to_underlying;

#else

template <class T>
[[nodiscard]] constexpr std::underlying_type_t<T> to_underlying(T Value) noexcept {
	return static_cast<std::underlying_type_t<T>>(Value);
}

#endif
