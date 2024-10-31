
#pragma once


#ifndef __NET_LISTENER_HPP__
#define __NET_LISTENER_HPP__


#include <memory>
#include <functional>
#include <array>
#include <tuple>
#include <type_traits>
#include "boost/assert.hpp"

#include "TypeTraits.hpp"

namespace net::detail
{
	enum class EventType : std::int8_t
	{
		recv,
		send,
		connect,
		connect_failed,
		disconnect,
		accept,
		handshake,
		upgrade,
		init,
		start,
		stop,
		max
	};

	template<typename = void>
	inline constexpr std::string_view ToString(EventType v)
	{
		using namespace std::string_view_literals;
		switch (v)
		{
		case EventType::recv       : return "recv";
		case EventType::send       : return "send";
		case EventType::connect    : return "connect";
        case EventType::connect_failed    :return "connect_failed";
		case EventType::disconnect : return "disconnect";
		case EventType::accept     : return "accept";
		case EventType::handshake  : return "handshake";
		case EventType::upgrade    : return "upgrade";
		case EventType::init       : return "init";
		case EventType::start      : return "start";
		case EventType::stop       : return "stop";
		case EventType::max		: return "max";
		default				        : return "none";
		}
		return "none";
	}

	class ObserverBase
	{
	public:
		virtual ~ObserverBase() noexcept {}
	};

	template<class... Args>
	class Observer_t : public ObserverBase
	{
	public:
		using func_type = std::function<void(Args...)>;
		using args_type = std::tuple<Args...>;

		explicit Observer_t(const func_type &  fn) : fn_(fn) {}
		explicit Observer_t(      func_type && fn) : fn_(std::move(fn)) {}
		explicit Observer_t(const Observer_t<Args...> &  other) : fn_(other.fn_) {}
		explicit Observer_t(      Observer_t<Args...> && other) : fn_(std::move(other.fn_)) {}

		template<class F, class ...C>
		explicit Observer_t(F&& f, C&&... c)
		{
			this->Bind(std::forward<F>(f), std::forward<C>(c)...);
		}

		template<class F, class ...C>
		inline void Bind(F&& f, C&&... c)
		{
			if constexpr (sizeof...(C) == std::size_t(0))
			{
				this->fn_ = func_type(std::forward<F>(f));
			}
			else
			{
				if constexpr (std::is_member_function_pointer_v<detail::remove_cvref_t<F>>)
				{
					if constexpr (sizeof...(C) == std::size_t(1))
					{
						this->BindMemFn(std::forward<F>(f), std::forward<C>(c)...);
					}
					else
					{
						this->BindMemFnFront(std::forward<F>(f), std::forward<C>(c)...);
					}
				}
				else
				{
					this->BindFnFront(std::forward<F>(f), std::forward<C>(c)...);
				}
			}
		}

		template<class F, class C>
		inline void BindMemFn(F&& f, C&& c)
		{
			if constexpr /**/ (std::is_pointer_v<detail::remove_cvref_t<C>>)
			{
				this->fn_ = [fn = std::forward<F>(f), s = std::forward<C>(c)](Args&&... args) mutable
				{
					(s->*fn)(std::forward<Args>(args)...);
				};
			}
			else if constexpr (std::is_reference_v<std::remove_cv_t<C>>)
			{
				this->fn_ = [fn = std::forward<F>(f), s = std::forward<C>(c)](Args&&... args) mutable
				{
					(s.*fn)(std::forward<Args>(args)...);
				};
			}
			else
			{
				static_assert(detail::always_false_v<F>,
					"the class object parameters of C&& c must be pointer or reference");
			}
		}

		template<class F, class C, class... Ts>
		inline void BindMemFnFront(F&& f, C&& c, Ts&&... ts)
		{
			if constexpr /**/ (std::is_pointer_v<detail::remove_cvref_t<C>>)
			{
				this->fn_ = [fn = std::forward<F>(f), s = std::forward<C>(c), tp = std::tuple(std::forward<Ts>(ts)...)]
				(Args&&... args) mutable
				{
					invoke_memfn_front(fn, s, std::make_index_sequence<sizeof...(Ts)>{}, tp, std::forward<Args>(args)...);
				};
			}
			else if constexpr (std::is_reference_v<std::remove_cv_t<C>>)
			{
				this->fn_ = [fn = std::forward<F>(f), s = std::forward<C>(c), tp = std::tuple(std::forward<Ts>(ts)...)]
				(Args&&... args) mutable
				{
					invoke_memfn_front(fn, std::addressof(s), std::make_index_sequence<sizeof...(Ts)>{}, tp, std::forward<Args>(args)...);
				};
			}
			else
			{
				static_assert(detail::always_false_v<F>,
					"the class object parameters of C&& c must be pointer or reference");
			}
		}

		template<typename F, typename C, std::size_t... I, typename... Ts>
		inline static void InvokeMemFnFront(F& f, C* c, std::index_sequence<I...>, std::tuple<Ts...>& tp, Args&&... args)
		{
			(c->*f)(std::get<I>(tp)..., std::forward<Args>(args)...);
		}

		template<class F, class... Ts>
		inline void BindFnFront(F&& f, Ts&&... ts)
		{
			this->fn_ = [fn = std::forward<F>(f), tp = std::tuple(std::forward<Ts>(ts)...)]
			(Args&&... args) mutable
			{
				invoke_fn_front(fn, std::make_index_sequence<sizeof...(Ts)>{}, tp, std::forward<Args>(args)...);
			};
		}

		template<typename F, std::size_t... I, typename... Ts>
		inline static void InvokeFnFront(F& f, std::index_sequence<I...>, std::tuple<Ts...>& tp, Args&&... args)
		{
			f(std::get<I>(tp)..., std::forward<Args>(args)...);
		}

		inline void operator()(Args&&... args)
		{
			if (this->fn_)
				this->fn_(std::forward<Args>(args)...);
		}

		inline func_type Move() noexcept { return std::move(this->fn_); }

	protected:
		func_type fn_;
	};

	class Listener_t
	{
	public:
		Listener_t() {}
		~Listener_t() = default;

		template<class T>
		inline void Bind(EventType e, T&& observer)
		{
			this->observers_[detail::to_underlying(e)] =
				std::unique_ptr<ObserverBase>(new T(std::forward<T>(observer)));
		}

		template<class... Args>
		inline void Notify(EventType e, Args&&... args)
		{
			using observer_type = Observer_t<Args...>;

			observer_type* observer_ptr = static_cast<observer_type*>(
				this->observers_[detail::to_underlying(e)].get());
			if (observer_ptr)
			{
				// You can define ASIO_NO_EXCEPTIONS in the /asio2/config.hpp to disable the
				// exception. so when the exception occurs, you can check the stack trace.
			#if !defined(ASIO_NO_EXCEPTIONS) && !defined(BOOST_ASIO_NO_EXCEPTIONS)
				try
				{
			#endif
					(*observer_ptr)(std::forward<Args>(args)...);
			#if !defined(ASIO_NO_EXCEPTIONS) && !defined(BOOST_ASIO_NO_EXCEPTIONS)
				}
				catch (boost::system::system_error const& ex)
				{
					std::ignore = ex;

					std::string msg = "An exception occured in the user callback function 'bind_";
					msg += detail::ToString(e);
					msg += "' : ";
					msg += ex.what();

					std::cout << msg << "\n";


					BOOST_ASSERT(false);
				}
				catch (std::exception const& ex)
				{
		
					std::string msg = "An exception occured in the user callback function 'bind_";
					msg += detail::ToString(e);
					msg += "' : ";
					msg += ex.what();
                    std::cout << msg << "\n";


					BOOST_ASSERT(false);
				}
				catch (...)
				{
	
					// just for see the exception information.
					std::string msg = "An exception occured in the user callback function 'bind_";
					msg += detail::ToString(e);
					msg += "'";
                    std::cout << msg << "\n";


					BOOST_ASSERT(false);
				}
			#endif
			}
		}

		inline std::unique_ptr<ObserverBase>& Find(EventType e) noexcept
		{
			return this->observers_[detail::to_underlying(e)];
		}

		inline std::unique_ptr<ObserverBase> const& Find(EventType e) const noexcept
		{
			return this->observers_[detail::to_underlying(e)];
		}

		inline void Clear() noexcept
		{
			for (std::unique_ptr<ObserverBase>& p : this->observers_)
			{
				p.reset();
			}
		}

	protected:
		std::array<std::unique_ptr<ObserverBase>, detail::to_underlying(EventType::max)> observers_;
	};
}

#endif // !__NET_LISTENER_HPP__
