//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2014-2014.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/container for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef AUTOBOOST_CONTAINER_DETAIL_ITERATOR_HPP
#define AUTOBOOST_CONTAINER_DETAIL_ITERATOR_HPP

#ifndef AUTOBOOST_CONFIG_HPP
#  include <autoboost/config.hpp>
#endif

#if defined(AUTOBOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include <autoboost/intrusive/detail/iterator.hpp>

namespace autoboost {
namespace container {

using ::autoboost::intrusive::iterator_traits;
using ::autoboost::intrusive::iterator_distance;
using ::autoboost::intrusive::iterator_advance;
using ::autoboost::intrusive::iterator;
using ::autoboost::intrusive::iterator_enable_if_tag;
using ::autoboost::intrusive::iterator_disable_if_tag;
using ::autoboost::intrusive::iterator_arrow_result;

}  //namespace container {
}  //namespace autoboost {

#endif   //#ifndef AUTOBOOST_CONTAINER_DETAIL_ITERATORS_HPP