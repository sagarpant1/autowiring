/*
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * Copyright (c) 2011 Helge Bahmann
 * Copyright (c) 2013-2014 Andrey Semashev
 */
/*!
 * \file   lockpool.cpp
 *
 * This file contains implementation of the lockpool used to emulate atomic ops.
 */

#include <cstddef>
#include <autoboost/config.hpp>
#include <autoboost/assert.hpp>
#include <autoboost/memory_order.hpp>
#include <autoboost/atomic/capabilities.hpp>

#if AUTOBOOST_ATOMIC_FLAG_LOCK_FREE == 2
#include <autoboost/atomic/detail/operations_lockfree.hpp>
#elif !defined(AUTOBOOST_HAS_PTHREADS)
#error Boost.Atomic: Unsupported target platform, POSIX threads are required when native atomic operations are not available
#else
#include <pthread.h>
#define AUTOBOOST_ATOMIC_USE_PTHREAD
#endif

#include <autoboost/atomic/detail/lockpool.hpp>
#include <autoboost/atomic/detail/pause.hpp>

namespace autoboost {
namespace atomics {
namespace detail {

namespace {

// This seems to be the maximum across all modern CPUs
// NOTE: This constant is made as a macro because some compilers (gcc 4.4 for one) don't allow enums or namespace scope constants in alignment attributes
#define AUTOBOOST_ATOMIC_CACHE_LINE_SIZE 64

template< unsigned int N >
struct padding
{
    char data[N];
};
template< >
struct padding< 0 >
{
};

struct AUTOBOOST_ALIGNMENT(AUTOBOOST_ATOMIC_CACHE_LINE_SIZE) padded_lock
{
#if defined(AUTOBOOST_ATOMIC_USE_PTHREAD)
    typedef pthread_mutex_t lock_type;
#else
    typedef atomics::detail::operations< 1u, false > operations;
    typedef operations::storage_type lock_type;
#endif

    lock_type lock;
    // The additional padding is needed to avoid false sharing between locks
    enum { padding_size = (sizeof(lock_type) <= AUTOBOOST_ATOMIC_CACHE_LINE_SIZE ?
        (AUTOBOOST_ATOMIC_CACHE_LINE_SIZE - sizeof(lock_type)) :
        (AUTOBOOST_ATOMIC_CACHE_LINE_SIZE - sizeof(lock_type) % AUTOBOOST_ATOMIC_CACHE_LINE_SIZE)) };
    padding< padding_size > pad;
};

static padded_lock g_lock_pool[41]
#if defined(AUTOBOOST_ATOMIC_USE_PTHREAD)
=
{
    PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER,
    PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER,
    PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER,
    PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER,
    PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER,
    PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER,
    PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER,
    PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER,
    PTHREAD_MUTEX_INITIALIZER
}
#endif
;

} // namespace


#if !defined(AUTOBOOST_ATOMIC_USE_PTHREAD)

// NOTE: This function must NOT be inline. Otherwise MSVC 9 will sometimes generate broken code for modulus operation which result in crashes.
AUTOBOOST_ATOMIC_DECL lockpool::scoped_lock::scoped_lock(const volatile void* addr) AUTOBOOST_NOEXCEPT :
    m_lock(&g_lock_pool[reinterpret_cast< std::size_t >(addr) % (sizeof(g_lock_pool) / sizeof(*g_lock_pool))].lock)
{
    while (padded_lock::operations::test_and_set(*static_cast< padded_lock::lock_type* >(m_lock), memory_order_acquire))
    {
        atomics::detail::pause();
    }
}

AUTOBOOST_ATOMIC_DECL lockpool::scoped_lock::~scoped_lock() AUTOBOOST_NOEXCEPT
{
    padded_lock::operations::clear(*static_cast< padded_lock::lock_type* >(m_lock), memory_order_release);
}

AUTOBOOST_ATOMIC_DECL void signal_fence() AUTOBOOST_NOEXCEPT;

#else // !defined(AUTOBOOST_ATOMIC_USE_PTHREAD)

AUTOBOOST_ATOMIC_DECL lockpool::scoped_lock::scoped_lock(const volatile void* addr) AUTOBOOST_NOEXCEPT :
    m_lock(&g_lock_pool[reinterpret_cast< std::size_t >(addr) % (sizeof(g_lock_pool) / sizeof(*g_lock_pool))].lock)
{
    AUTOBOOST_VERIFY(pthread_mutex_lock(static_cast< pthread_mutex_t* >(m_lock)) == 0);
}

AUTOBOOST_ATOMIC_DECL lockpool::scoped_lock::~scoped_lock() AUTOBOOST_NOEXCEPT
{
    AUTOBOOST_VERIFY(pthread_mutex_unlock(static_cast< pthread_mutex_t* >(m_lock)) == 0);
}

#endif // !defined(AUTOBOOST_ATOMIC_USE_PTHREAD)

AUTOBOOST_ATOMIC_DECL void lockpool::thread_fence() AUTOBOOST_NOEXCEPT
{
#if AUTOBOOST_ATOMIC_THREAD_FENCE > 0
    atomics::detail::thread_fence(memory_order_seq_cst);
#else
    // Emulate full fence by locking/unlocking a mutex
    scoped_lock lock(0);
#endif
}

AUTOBOOST_ATOMIC_DECL void lockpool::signal_fence() AUTOBOOST_NOEXCEPT
{
    // This function is intentionally non-inline, even if empty. This forces the compiler to treat its call as a compiler barrier.
#if AUTOBOOST_ATOMIC_SIGNAL_FENCE > 0
    atomics::detail::signal_fence(memory_order_seq_cst);
#endif
}

} // namespace detail
} // namespace atomics
} // namespace autoboost
