
#pragma once

#ifndef __NET_OBJECT_HPP__
#define __NET_OBJECT_HPP__

#include <memory>

namespace net
{
	class Object
	{
	public:
	};
}

namespace net::detail
{
	/**
	 * the lowest based class used fo CRTP
	 * see : CRTP and multilevel inheritance 
	 * https://stackoverflow.com/questions/18174441/crtp-and-multilevel-inheritance
	 */
	template<class derived_t, bool enable_shared_from_this = true>
	class Object_t : public net::Object, public std::enable_shared_from_this<derived_t>
	{
	protected:
		/**
		 * @brief constructor
		 */
		Object_t() = default;

		/**
		 * @brief destructor
		 */
		~Object_t() = default;

	protected:
		/**
		 * @brief obtain derived class object through CRTP mechanism
		 */
		inline const derived_t & Derived() const noexcept
		{
			return static_cast<const derived_t &>(*this);
		}

		/**
		 * @brief obtain derived class object through CRTP mechanism
		 */
		inline derived_t & Derived() noexcept
		{
			return static_cast<derived_t &>(*this);
		}

		/**
		 * @brief if the "derived_t" is created like a "shared_ptr", it will return
		 *             a not empty shared_ptr<derived_t>, othwise it will return a empty
		 *             shared_ptr<derived_t>.
		 */
		inline std::shared_ptr<derived_t> SelfPtr() noexcept
		{
			// if the "derived_t" (maybe server,client,session...) is created like a
			// "shared_ptr", then here will return a not empty shared_ptr, otherwise
			// here will return a empty shared_ptr.
			// e.g : when the "derived_t" is udp_cast, and user has called post_condition_event,
			// and hold the "event_ptr" into another thread, and when the udp_cast is 
			// soppted and destroyed, and user called the "event_ptr->notify()" 
			// in the "another thread", if the udp_cast is created like a "shared_ptr",
			// then the event_ptr's member variable "derive_ptr_" will hold the shared_ptr
			// of udp_cast, this has no problem. but if the udp_cast is created as a 
			// local variable not a "shared_ptr", then the event_ptr's member variable
			// "derive_ptr_" will hold a empty shared_ptr, then it will crash when 
			// user called the "event_ptr->notify()" in the "another thread", beacuse
			// at this time, the "event_timer_io_" maybe destroyed already.
			return this->Derived().weak_from_this().lock();
		}
	};

	template<class derived_t>
	class Object_t<derived_t, false> : public net::Object
	{
	protected:
		/**
		 * @brief constructor
		 */
		Object_t() = default;

		/**
		 * @brief destructor
		 */
		~Object_t() = default;

	protected:
		/**
		 * @brief obtain derived class object through CRTP mechanism
		 */
		inline const derived_t & Derived() const noexcept
		{
			return static_cast<const derived_t &>(*this);
		}

		/**
		 * @brief obtain derived class object through CRTP mechanism
		 */
		inline derived_t & Derived() noexcept
		{
			return static_cast<derived_t &>(*this);
		}

		/**
		 * @brief always return a empty shared_ptr<derived_t>.
		 */
		inline std::shared_ptr<derived_t> SelfPtr() noexcept
		{
			return std::shared_ptr<derived_t>{};
		}
	};
}

#endif // !__NET_OBJECT_HPP__
