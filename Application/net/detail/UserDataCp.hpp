
#pragma once

#ifndef __NET_USER_DATA_COMPONENT_HPP__
#define __NET_USER_DATA_COMPONENT_HPP__

#include <any>

namespace net::detail
{
	template<class derived_t, class args_t = void>
	class UserDataCp
	{
	public:

		UserDataCp() = default;


		~UserDataCp() = default;

		UserDataCp(UserDataCp&&) noexcept = default;
		UserDataCp(UserDataCp const&) = default;
		UserDataCp& operator=(UserDataCp&&) noexcept = default;
		UserDataCp& operator=(UserDataCp const&) = default;

	public:

		
		template<class DataT>
		inline derived_t & SetUserData(DataT && data)
		{
			this->user_data_ = std::forward<DataT>(data);
			return (static_cast<derived_t &>(*this));
		}

		template<class DataT>
		inline DataT GetUserData() noexcept
		{
			if constexpr (std::is_reference_v<DataT>)
			{
				typename std::add_pointer_t<std::remove_reference_t<DataT>> r =
					std::any_cast<std::remove_reference_t<DataT>>(std::addressof(this->user_data_));
				if (r)
				{
					return (*r);
				}
				else
				{
					static typename std::remove_reference_t<DataT> st{};
					return st;
				}
			}
			else if constexpr (std::is_pointer_v<DataT>)
			{
				// user_data_ is pointer, and DataT is pointer too.
				if (this->user_data_.type() == typeid(DataT))
					return std::any_cast<DataT>(this->user_data_);

				// user_data_ is not pointer, but DataT is pointer.
				return std::any_cast<std::remove_pointer_t<DataT>>(std::addressof(this->user_data_));
			}
			else
			{
			#if !defined(ASIO_NO_EXCEPTIONS) && !defined(BOOST_ASIO_NO_EXCEPTIONS)
				try
				{
			#endif
					return std::any_cast<DataT>(this->user_data_);
			#if !defined(ASIO_NO_EXCEPTIONS) && !defined(BOOST_ASIO_NO_EXCEPTIONS)
				}
				catch (const std::bad_any_cast&)
				{
					if (this->user_data_.has_value())
					{
						BOOST_ASSERT(false);
					}
				}
				return DataT{};
			#endif
			}
		}


		inline derived_t& ClearUserData() noexcept
		{
			this->user_data_.reset();
			return (static_cast<derived_t&>(*this));
		}


		inline std::any& UserDataAny() noexcept { return this->user_data_; }


		inline std::any const& UserDataAny() const noexcept { return this->user_data_; }

	protected:
		/// user data
		std::any user_data_;
	};
}

#endif // !__NET_USER_DATA_COMPONENT_HPP__
