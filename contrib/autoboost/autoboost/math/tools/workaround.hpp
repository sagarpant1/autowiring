//  Copyright (c) 2006-7 John Maddock
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef AUTOBOOST_MATH_TOOLS_WORHAROUND_HPP
#define AUTOBOOST_MATH_TOOLS_WORHAROUND_HPP

#ifdef _MSC_VER
#pragma once
#endif

#include <autoboost/math/tools/config.hpp>

namespace autoboost{ namespace math{ namespace tools{
//
// We call this short forwarding function so that we can work around a bug
// on Darwin that causes std::fmod to return a NaN.  The test case is:
// std::fmod(1185.0L, 1.5L);
//
template <class T>
inline T fmod_workaround(T a, T b) AUTOBOOST_MATH_NOEXCEPT(T)
{
   AUTOBOOST_MATH_STD_USING
   return fmod(a, b);
}
#if (defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__)) && ((LDBL_MANT_DIG == 106) || (__LDBL_MANT_DIG__ == 106))
template <>
inline long double fmod_workaround(long double a, long double b) AUTOBOOST_NOEXCEPT
{
   return ::fmodl(a, b);
}
#endif

}}} // namespaces

#endif // AUTOBOOST_MATH_TOOLS_WORHAROUND_HPP
