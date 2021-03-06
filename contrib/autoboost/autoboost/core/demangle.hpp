#ifndef AUTOBOOST_CORE_DEMANGLE_HPP_INCLUDED
#define AUTOBOOST_CORE_DEMANGLE_HPP_INCLUDED

// core::demangle
//
// Copyright 2014 Peter Dimov
// Copyright 2014 Andrey Semashev
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt

#include <autoboost/config.hpp>
#include <string>

#if defined(AUTOBOOST_HAS_PRAGMA_ONCE)
# pragma once
#endif

#if defined( __clang__ ) && defined( __has_include )
# if __has_include(<cxxabi.h>)
#  define AUTOBOOST_CORE_HAS_CXXABI_H
# endif
#elif defined( __GLIBCXX__ ) || defined( __GLIBCPP__ )
# define AUTOBOOST_CORE_HAS_CXXABI_H
#endif

#if defined( AUTOBOOST_CORE_HAS_CXXABI_H )
# include <cxxabi.h>
# include <cstdlib>
# include <cstddef>
#endif

namespace autoboost
{

namespace core
{

inline char const * demangle_alloc( char const * name ) AUTOBOOST_NOEXCEPT;
inline void demangle_free( char const * name ) AUTOBOOST_NOEXCEPT;

class scoped_demangled_name
{
private:
    char const * m_p;

public:
    explicit scoped_demangled_name( char const * name ) AUTOBOOST_NOEXCEPT :
        m_p( demangle_alloc( name ) )
    {
    }

    ~scoped_demangled_name() AUTOBOOST_NOEXCEPT
    {
        demangle_free( m_p );
    }

    char const * get() const AUTOBOOST_NOEXCEPT
    {
        return m_p;
    }

    AUTOBOOST_DELETED_FUNCTION(scoped_demangled_name( scoped_demangled_name const& ))
    AUTOBOOST_DELETED_FUNCTION(scoped_demangled_name& operator= ( scoped_demangled_name const& ))
};


#if defined( AUTOBOOST_CORE_HAS_CXXABI_H )

inline char const * demangle_alloc( char const * name ) AUTOBOOST_NOEXCEPT
{
    int status = 0;
    std::size_t size = 0;
    return abi::__cxa_demangle( name, NULL, &size, &status );
}

inline void demangle_free( char const * name ) AUTOBOOST_NOEXCEPT
{
    std::free( const_cast< char* >( name ) );
}

inline std::string demangle( char const * name )
{
    scoped_demangled_name demangled_name( name );
    char const * const p = demangled_name.get();
    if( p )
    {
        return p;
    }
    else
    {
        return name;
    }
}

#else

inline char const * demangle_alloc( char const * name ) AUTOBOOST_NOEXCEPT
{
    return name;
}

inline void demangle_free( char const * ) AUTOBOOST_NOEXCEPT
{
}

inline std::string demangle( char const * name )
{
    return name;
}

#endif

} // namespace core

} // namespace autoboost

#undef AUTOBOOST_CORE_HAS_CXXABI_H

#endif // #ifndef AUTOBOOST_CORE_DEMANGLE_HPP_INCLUDED
