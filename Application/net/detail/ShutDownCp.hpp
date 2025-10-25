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
			const net::socket_base::shutdown_type& what = net::ip::tcp::socket::shutdown_both,
			const net::error_code& ec = net::error::operation_aborted)
		{
            auto &derived = static_cast<derived_t &>(*this);

            net::error_code ignored_ec;
            derived.Socket().shutdown(net::ip::tcp::socket::shutdown_both, ignored_ec);
		}

	public:


	};

}