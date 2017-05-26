
//          Copyright Oliver Kowalke 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef AUTOBOOST_CONTEXT_DETAIL_APPLY_H
#define AUTOBOOST_CONTEXT_DETAIL_APPLY_H

#include <functional>
#include <tuple>
#include <type_traits>
#include <utility>

#include <autoboost/config.hpp>

#include <autoboost/context/detail/config.hpp>
#if defined(AUTOBOOST_NO_CXX17_STD_INVOKE)
#include <autoboost/context/detail/invoke.hpp>
#endif
#include <autoboost/context/detail/index_sequence.hpp>

#ifdef AUTOBOOST_HAS_ABI_HEADERS
# include AUTOBOOST_ABI_PREFIX
#endif

#if defined(AUTOBOOST_MSVC)
# pragma warning(push)
# pragma warning(disable: 4100)
#endif

namespace autoboost {
namespace context {
namespace detail {

template< typename Fn, typename Tpl, std::size_t ... I >
auto
apply_impl( Fn && fn, Tpl && tpl, index_sequence< I ... >)
#if defined(AUTOBOOST_NO_CXX17_STD_INVOKE)
    -> decltype( invoke( std::forward< Fn >( fn), std::get< I >( std::forward< Tpl >( tpl) ) ... ) )
#else
    -> decltype( std::invoke( std::forward< Fn >( fn), std::get< I >( std::forward< Tpl >( tpl) ) ... ) )
#endif
{
#if defined(AUTOBOOST_NO_CXX17_STD_INVOKE)
    return invoke( std::forward< Fn >( fn), std::get< I >( std::forward< Tpl >( tpl) ) ... );
#else
    return std::invoke( std::forward< Fn >( fn), std::get< I >( std::forward< Tpl >( tpl) ) ... );
#endif
}

template< typename Fn, typename Tpl >
auto
apply( Fn && fn, Tpl && tpl)
    -> decltype( apply_impl( std::forward< Fn >( fn),
                 std::forward< Tpl >( tpl),
                 make_index_sequence< std::tuple_size< typename std::decay< Tpl >::type >::value >{}) )
{
    return apply_impl( std::forward< Fn >( fn),
                       std::forward< Tpl >( tpl),
                       make_index_sequence< std::tuple_size< typename std::decay< Tpl >::type >::value >{});
}

}}}

#if defined(AUTOBOOST_MSVC)
# pragma warning(pop)
#endif

#ifdef AUTOBOOST_HAS_ABI_HEADERS
#include AUTOBOOST_ABI_SUFFIX
#endif

#endif // AUTOBOOST_CONTEXT_DETAIL_APPLY_H