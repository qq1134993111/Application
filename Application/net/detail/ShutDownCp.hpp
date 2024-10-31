#pragma once

namespace net::detail
{
	template<class derived_t, class args_t>
	class ShutDownCp
	{
	public:
		ShutDownCp() {}
		~ShutDownCp() {}

	protected:
		void DoShutdown(
			const boost::asio::socket_base::shutdown_type& what = boost::asio::ip::tcp::socket::shutdown_both,
			const boost::system::error_code& ec = boost::asio::error::operation_aborted)
		{
            auto &derived = static_cast<derived_t &>(*this);

            boost::system::error_code ignored_ec;
            derived.Socket().shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
		}

	public:


	};

}