//
// ssl/detail/handshake_op.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef AUTOBOOST_ASIO_SSL_DETAIL_HANDSHAKE_OP_HPP
#define AUTOBOOST_ASIO_SSL_DETAIL_HANDSHAKE_OP_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <autoboost/asio/detail/config.hpp>

#if !defined(AUTOBOOST_ASIO_ENABLE_OLD_SSL)
# include <autoboost/asio/ssl/detail/engine.hpp>
#endif // !defined(AUTOBOOST_ASIO_ENABLE_OLD_SSL)

#include <autoboost/asio/detail/push_options.hpp>

namespace autoboost {
namespace asio {
namespace ssl {
namespace detail {

#if !defined(AUTOBOOST_ASIO_ENABLE_OLD_SSL)

class handshake_op
{
public:
  handshake_op(stream_base::handshake_type type)
    : type_(type)
  {
  }

  engine::want operator()(engine& eng,
      autoboost::system::error_code& ec,
      std::size_t& bytes_transferred) const
  {
    bytes_transferred = 0;
    return eng.handshake(type_, ec);
  }

  template <typename Handler>
  void call_handler(Handler& handler,
      const autoboost::system::error_code& ec,
      const std::size_t&) const
  {
    handler(ec);
  }

private:
  stream_base::handshake_type type_;
};

#endif // !defined(AUTOBOOST_ASIO_ENABLE_OLD_SSL)

} // namespace detail
} // namespace ssl
} // namespace asio
} // namespace autoboost

#include <autoboost/asio/detail/pop_options.hpp>

#endif // AUTOBOOST_ASIO_SSL_DETAIL_HANDSHAKE_OP_HPP
