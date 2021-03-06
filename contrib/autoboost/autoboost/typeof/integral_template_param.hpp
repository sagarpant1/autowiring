// Copyright (C) 2005 Arkadiy Vertleyb
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef AUTOBOOST_TYPEOF_INTEGRAL_TEMPLATE_PARAM_HPP_INCLUDED
#define AUTOBOOST_TYPEOF_INTEGRAL_TEMPLATE_PARAM_HPP_INCLUDED

#define AUTOBOOST_TYPEOF_unsigned (unsigned)
#define AUTOBOOST_TYPEOF_signed (signed)

#define char_AUTOBOOST_TYPEOF (char)
#define short_AUTOBOOST_TYPEOF (short)
#define int_AUTOBOOST_TYPEOF (int)
#define long_AUTOBOOST_TYPEOF (long)

#define AUTOBOOST_TYPEOF_char_AUTOBOOST_TYPEOF (char)
#define AUTOBOOST_TYPEOF_short_AUTOBOOST_TYPEOF (short)
#define AUTOBOOST_TYPEOF_int_AUTOBOOST_TYPEOF (int)
#define AUTOBOOST_TYPEOF_long_AUTOBOOST_TYPEOF (long)
#define AUTOBOOST_TYPEOF_bool_AUTOBOOST_TYPEOF (bool)
#define AUTOBOOST_TYPEOF_unsigned_AUTOBOOST_TYPEOF (unsigned)
#define AUTOBOOST_TYPEOF_size_t_AUTOBOOST_TYPEOF (size_t)

#define AUTOBOOST_TYPEOF_MAKE_OBJ_char          AUTOBOOST_TYPEOF_INTEGRAL_PARAM(char)
#define AUTOBOOST_TYPEOF_MAKE_OBJ_short         AUTOBOOST_TYPEOF_INTEGRAL_PARAM(short)
#define AUTOBOOST_TYPEOF_MAKE_OBJ_int           AUTOBOOST_TYPEOF_INTEGRAL_PARAM(int)
#define AUTOBOOST_TYPEOF_MAKE_OBJ_long          AUTOBOOST_TYPEOF_INTEGRAL_PARAM(long)
#define AUTOBOOST_TYPEOF_MAKE_OBJ_bool          AUTOBOOST_TYPEOF_INTEGRAL_PARAM(bool)
#define AUTOBOOST_TYPEOF_MAKE_OBJ_unsigned      AUTOBOOST_TYPEOF_INTEGRAL_PARAM(unsigned)
#define AUTOBOOST_TYPEOF_MAKE_OBJ_size_t        AUTOBOOST_TYPEOF_INTEGRAL_PARAM(size_t)
#define AUTOBOOST_TYPEOF_MAKE_OBJ_unsignedchar  AUTOBOOST_TYPEOF_INTEGRAL_PARAM(unsigned char)
#define AUTOBOOST_TYPEOF_MAKE_OBJ_unsignedshort AUTOBOOST_TYPEOF_INTEGRAL_PARAM(unsigned short)
#define AUTOBOOST_TYPEOF_MAKE_OBJ_unsignedint   AUTOBOOST_TYPEOF_INTEGRAL_PARAM(unsigned int)
#define AUTOBOOST_TYPEOF_MAKE_OBJ_unsignedlong  AUTOBOOST_TYPEOF_INTEGRAL_PARAM(unsigned long)
#define AUTOBOOST_TYPEOF_MAKE_OBJ_signedchar    AUTOBOOST_TYPEOF_INTEGRAL_PARAM(signed char)
#define AUTOBOOST_TYPEOF_MAKE_OBJ_signedshort   AUTOBOOST_TYPEOF_INTEGRAL_PARAM(signed short)
#define AUTOBOOST_TYPEOF_MAKE_OBJ_signedint     AUTOBOOST_TYPEOF_INTEGRAL_PARAM(signed int)
#define AUTOBOOST_TYPEOF_MAKE_OBJ_signedlong    AUTOBOOST_TYPEOF_INTEGRAL_PARAM(signed long)
#define AUTOBOOST_TYPEOF_MAKE_OBJ_integral(x)   AUTOBOOST_TYPEOF_INTEGRAL_PARAM(x)

#define AUTOBOOST_TYPEOF_INTEGRAL(X) integral(X) AUTOBOOST_TYPEOF_EAT
#define AUTOBOOST_TYPEOF_EAT_AUTOBOOST_TYPEOF
#define AUTOBOOST_TYPEOF_integral(X) (integral(X))

#define AUTOBOOST_TYPEOF_INTEGRAL_PARAM(Type)\
    (INTEGRAL_PARAM)\
    (Type)

#define AUTOBOOST_TYPEOF_INTEGRAL_PARAM_GETTYPE(Param)\
    AUTOBOOST_PP_SEQ_ELEM(1, Param)

#define AUTOBOOST_TYPEOF_INTEGRAL_PARAM_EXPANDTYPE(Param)\
    AUTOBOOST_TYPEOF_INTEGRAL_PARAM_GETTYPE(Param)

// INTEGRAL_PARAM "virtual functions" implementation

#define AUTOBOOST_TYPEOF_INTEGRAL_PARAM_ENCODE(This, n)\
    typedef typename autoboost::type_of::encode_integral<\
        AUTOBOOST_PP_CAT(V, n),\
        AUTOBOOST_TYPEOF_INTEGRAL_PARAM_GETTYPE(This),\
        AUTOBOOST_PP_CAT(P, n)\
    >::type AUTOBOOST_PP_CAT(V, AUTOBOOST_PP_INC(n));

#define AUTOBOOST_TYPEOF_INTEGRAL_PARAM_DECODE(This, n)\
    typedef autoboost::type_of::decode_integral<AUTOBOOST_TYPEOF_INTEGRAL_PARAM_GETTYPE(This), AUTOBOOST_PP_CAT(iter, n)> AUTOBOOST_PP_CAT(d, n);\
    static const AUTOBOOST_TYPEOF_INTEGRAL_PARAM_GETTYPE(This) AUTOBOOST_PP_CAT(P, n) = AUTOBOOST_PP_CAT(d, n)::value;\
    typedef typename AUTOBOOST_PP_CAT(d, n)::iter AUTOBOOST_PP_CAT(iter, AUTOBOOST_PP_INC(n));

#define AUTOBOOST_TYPEOF_INTEGRAL_PARAM_PLACEHOLDER(Param)\
    (AUTOBOOST_TYPEOF_INTEGRAL_PARAM_GETTYPE(Param))0

#define AUTOBOOST_TYPEOF_INTEGRAL_PARAM_DECLARATION_TYPE(Param)\
    AUTOBOOST_TYPEOF_INTEGRAL_PARAM_GETTYPE(Param)

#define AUTOBOOST_TYPEOF_INTEGRAL_PARAM_PLACEHOLDER_TYPES(Param, n)\
    AUTOBOOST_PP_CAT(T,n)

#define AUTOBOOST_TYPEOF_INTEGRAL_PARAM_ISTEMPLATE 0

#endif//AUTOBOOST_TYPEOF_INTEGRAL_TEMPLATE_PARAM_HPP_INCLUDED
