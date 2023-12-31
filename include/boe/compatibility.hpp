#pragma once

#include <type_traits>
#include <format>

#if defined(__cpp_lib_to_underlying) && __cpp_lib_to_underlying

using std::to_underlying;

#else

template <class T>
[[nodiscard]] constexpr std::underlying_type_t<T> to_underlying(T Value) noexcept {
	return static_cast<std::underlying_type_t<T>>(Value);
}

#endif

template< std::output_iterator<const char&> OutputIt, class... Types>
constexpr void format_to_buf(OutputIt Out, const std::format_string<Types...> Fmt, Types&&... Args) {
	const auto out = std::vformat_to(std::move(Out), Fmt.get(), std::make_format_args(Args...));
	*out = '\0';
}
