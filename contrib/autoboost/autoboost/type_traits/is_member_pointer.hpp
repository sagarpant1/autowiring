
//  (C) Copyright Dave Abrahams, Steve Cleary, Beman Dawes,
//      Howard Hinnant and John Maddock 2000.
//  (C) Copyright Mat Marcus, Jesse Jones and Adobe Systems Inc 2001

//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.

//    Fixed is_pointer, is_reference, is_const, is_volatile, is_same,
//    is_member_pointer based on the Simulated Partial Specialization work
//    of Mat Marcus and Jesse Jones. See  http://opensource.adobe.com or
//    http://groups.yahoo.com/group/autoboost/message/5441
//    Some workarounds in here use ideas suggested from "Generic<Programming>:
//    Mappings between Types and Values"
//    by Andrei Alexandrescu (see http://www.cuj.com/experts/1810/alexandr.html).


#ifndef AUTOBOOST_TT_IS_MEMBER_POINTER_HPP_INCLUDED
#define AUTOBOOST_TT_IS_MEMBER_POINTER_HPP_INCLUDED

#include <autoboost/type_traits/config.hpp>
#include <autoboost/detail/workaround.hpp>

#if !AUTOBOOST_WORKAROUND(__BORLANDC__, < 0x600)
#   include <autoboost/type_traits/is_member_function_pointer.hpp>
#else
#   include <autoboost/type_traits/is_reference.hpp>
#   include <autoboost/type_traits/is_array.hpp>
#   include <autoboost/type_traits/detail/is_mem_fun_pointer_tester.hpp>
#   include <autoboost/type_traits/detail/yes_no_type.hpp>
#   include <autoboost/type_traits/detail/false_result.hpp>
#   include <autoboost/type_traits/detail/ice_or.hpp>
#endif

// should be the last #include
#include <autoboost/type_traits/detail/bool_trait_def.hpp>

namespace autoboost {

#if defined( __CODEGEARC__ )
AUTOBOOST_TT_AUX_BOOL_TRAIT_DEF1(is_member_pointer,T,__is_member_pointer(T))
#elif AUTOBOOST_WORKAROUND(__BORLANDC__, < 0x600)
AUTOBOOST_TT_AUX_BOOL_TRAIT_DEF1(is_member_pointer,T,false)
AUTOBOOST_TT_AUX_BOOL_TRAIT_PARTIAL_SPEC1_2(typename T,typename U,is_member_pointer,U T::*,true)

#else
AUTOBOOST_TT_AUX_BOOL_TRAIT_DEF1(is_member_pointer,T,::autoboost::is_member_function_pointer<T>::value)
AUTOBOOST_TT_AUX_BOOL_TRAIT_PARTIAL_SPEC1_2(typename T,typename U,is_member_pointer,U T::*,true)

#if !AUTOBOOST_WORKAROUND(__MWERKS__,<=0x3003) && !AUTOBOOST_WORKAROUND(__IBMCPP__, <=600)
AUTOBOOST_TT_AUX_BOOL_TRAIT_PARTIAL_SPEC1_2(typename T,typename U,is_member_pointer,U T::*const,true)
AUTOBOOST_TT_AUX_BOOL_TRAIT_PARTIAL_SPEC1_2(typename T,typename U,is_member_pointer,U T::*volatile,true)
AUTOBOOST_TT_AUX_BOOL_TRAIT_PARTIAL_SPEC1_2(typename T,typename U,is_member_pointer,U T::*const volatile,true)
#endif

#endif

} // namespace autoboost

#include <autoboost/type_traits/detail/bool_trait_undef.hpp>

#endif // AUTOBOOST_TT_IS_MEMBER_POINTER_HPP_INCLUDED