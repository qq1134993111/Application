
#pragma once

#ifndef __NET_SOCKET_COMPONENT_HPP__
#define __NET_SOCKET_COMPONENT_HPP__


#include <string>

#include <boost/asio.hpp>

namespace net::detail
{
	template<class derived_t, class args_t>
	class SocketCp
	{
	public:
		using socket_type = std::remove_cv_t<std::remove_reference_t<typename args_t::socket_t>>;

		explicit SocketCp(boost::asio::io_context& ioc) : socket_(std::make_shared<socket_type>(ioc))
		{
		}

		explicit SocketCp(std::shared_ptr<typename args_t::socket_t> ptr) : socket_(std::move(ptr))
		{
		}

		~SocketCp()
		{
		}

	public:

		inline socket_type& Socket() noexcept
		{
			return *(this->socket_);
		}


		inline const socket_type& Socket() const noexcept
		{
			return *(this->socket_);
		}

		inline socket_type& Stream() noexcept
		{
			return *(this->socket_);
		}

		inline const socket_type& Stream() const noexcept
		{
			return *(this->socket_);
		}


		inline std::string LocalAddress() const noexcept
		{
			return this->GetLocalAddress();
		}


		inline std::string GetLocalAddress() const noexcept
		{
			try
			{
				return this->socket_->lowest_layer().local_endpoint().address().to_string();
			}
			catch (const boost::system::system_error& e)
			{
			}
			return std::string();
		}

		inline unsigned short LocalPort() const noexcept
		{
			return this->GetLocalPort();
		}


		inline unsigned short GetLocalPort() const noexcept
		{
			boost::system::error_code ec;
			return this->socket_->lowest_layer().local_endpoint(ec).port();
		}

		inline std::string RemoteAddress() const noexcept
		{
			return this->GetRemoteAddress();
		}

		inline std::string GetRemoteAddress() const noexcept
		{
			try
			{
				return this->socket_->lowest_layer().remote_endpoint().address().to_string();
			}
			catch (const boost::system::system_error& e)
			{

			}

			try
			{
				boost::asio::ip::address addr = this->remote_endpoint_.address();

				if (!addr.is_unspecified())
				{
					return addr.to_string();
				}
			}
			catch (const boost::system::system_error&)
			{
			}


			return std::string();
		}

		inline unsigned short RemotePort() const noexcept
		{
			return this->GetRemotePort();
		}

		inline unsigned short GetRemotePort() const noexcept
		{
			try
			{
				return this->socket_->lowest_layer().remote_endpoint().port();
			}
			catch (const boost::system::system_error& e)
			{

			}

			try
			{
				return this->remote_endpoint_.port();
			}
			catch (const  boost::system::system_error&)
			{
			}


			return 0;
		}

	public:

		inline derived_t& SetSndBufSize(int val) noexcept
		{
			boost::system::error_code ec;
			this->socket_->lowest_layer().set_option(boost::asio::socket_base::send_buffer_size(val), ec);
			return (static_cast<derived_t&>(*this));
		}


		inline int GetSndBufSize() const noexcept
		{
			boost::system::error_code ec;
			boost::asio::socket_base::send_buffer_size option{};
			this->socket_->lowest_layer().get_option(option, ec);
			return option.value();
		}


		inline derived_t& SetRcvBufSize(int val) noexcept
		{
			boost::system::error_code ec;
			this->socket_->lowest_layer().set_option(boost::asio::socket_base::receive_buffer_size(val), ec);
			return (static_cast<derived_t&>(*this));
		}


		inline int GetRcvBufSize() const noexcept
		{
			boost::system::error_code ec;
			boost::asio::socket_base::receive_buffer_size option{};
			this->socket_->lowest_layer().get_option(option, ec);
			return option.value();
		}


		inline derived_t& KeepAlive(bool val) noexcept
		{
			return this->SetKeepAlive(val);
		}


		inline derived_t& SetKeepAlive(bool val) noexcept
		{
			boost::system::error_code ec;
			this->socket_->lowest_layer().set_option(boost::asio::socket_base::keep_alive(val), ec);
			return (static_cast<derived_t&>(*this));
		}


		inline bool IsKeepAlive() const noexcept
		{
			boost::system::error_code ec;
			boost::asio::socket_base::keep_alive option{};
			this->socket_->lowest_layer().get_option(option, ec);
			return option.value();
		}


		inline derived_t& ReuseAddress(bool val) noexcept
		{
			return this->SetReuseAddress(val);
		}


		inline derived_t& SetReuseAddress(bool val) noexcept
		{
			boost::system::error_code ec;
			this->socket_->lowest_layer().set_option(boost::asio::socket_base::reuse_address(val), ec);
			return (static_cast<derived_t&>(*this));
		}

		inline bool IsReuseAddress() const noexcept
		{
			boost::system::error_code ec;
			boost::asio::socket_base::reuse_address option{};
			this->socket_->lowest_layer().get_option(option, ec);
			return option.value();
		}


		inline derived_t& NoDelay(bool val) noexcept
		{
			return this->SetNoDelay(val);
		}


		inline derived_t& SetNoDelay(bool val) noexcept
		{
			boost::system::error_code ec;
			this->socket_->lowest_layer().set_option(boost::asio::ip::tcp::no_delay(val), ec);
			return (static_cast<derived_t&>(*this));
		}

		inline bool IsNoDelay() const noexcept
		{
			boost::system::error_code ec;
			boost::asio::ip::tcp::no_delay option{};
			this->socket_->lowest_layer().get_option(option, ec);
			return option.value();
		}

		inline derived_t& SetLinger(bool enable, int timeout) noexcept
		{
			boost::system::error_code ec;
			this->socket_->lowest_layer().set_option(boost::asio::socket_base::linger(enable, timeout), ec);
			return (static_cast<derived_t&>(*this));
		}


		inline boost::asio::socket_base::linger GetLinger() const noexcept
		{
			boost::system::error_code ec;
			boost::asio::socket_base::linger option{};
			this->socket_->lowest_layer().get_option(option, ec);
			return option;
		}

	protected:

		std::shared_ptr<typename args_t::socket_t> socket_;
		typename socket_type::endpoint_type remote_endpoint_{};
	};
}

#endif // !__NET_SOCKET_COMPONENT_HPP__
