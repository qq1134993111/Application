#pragma once
#include<cstdint>
#include<string_view>

namespace net::detail
{
enum class State_t : std::int8_t
{
    connecting,
    connected,
    disconnect,
    stopping,
    stopped
};

	template<typename = void>
	inline constexpr std::string_view ToString(State_t v)
	{
		using namespace std::string_view_literals;
		switch (v)
		{

        case State_t::connecting:
            return "connecting";
        case State_t::connected:
            return "connected";
        case State_t::disconnect:
            return "disconnect";
        case State_t::stopping:
            return "stopping";
        case State_t::stopped:
            return "stopped";
		default: return "none";
		}
		return "none";
	}
}