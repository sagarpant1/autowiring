/*=============================================================================
    Copyright (c) 2007 Tobias Schwinger

    Use modification and distribution are subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
==============================================================================*/

#ifndef AUTOBOOST_FUNCTIONAL_FACTORY_HPP_INCLUDED
#   ifndef AUTOBOOST_PP_IS_ITERATING

#     include <autoboost/preprocessor/iteration/iterate.hpp>
#     include <autoboost/preprocessor/repetition/enum_params.hpp>
#     include <autoboost/preprocessor/repetition/enum_binary_params.hpp>

#     include <new>
#     include <autoboost/pointee.hpp>
#     include <autoboost/get_pointer.hpp>
#     include <autoboost/non_type.hpp>
#     include <autoboost/type_traits/remove_cv.hpp>

#     if defined(AUTOBOOST_FUNCTIONAL_FACTORY_SUPPORT_NONE_T)
#       include <autoboost/none_t.hpp>
#     endif

#     ifndef AUTOBOOST_FUNCTIONAL_FACTORY_MAX_ARITY
#       define AUTOBOOST_FUNCTIONAL_FACTORY_MAX_ARITY 10
#     elif AUTOBOOST_FUNCTIONAL_FACTORY_MAX_ARITY < 3
#       undef  AUTOBOOST_FUNCTIONAL_FACTORY_MAX_ARITY
#       define AUTOBOOST_FUNCTIONAL_FACTORY_MAX_ARITY 3
#     endif

namespace autoboost
{
    enum factory_alloc_propagation
    {
        factory_alloc_for_pointee_and_deleter,
        factory_passes_alloc_to_smart_pointer
    };

#if defined(AUTOBOOST_FUNCTIONAL_FACTORY_SUPPORT_NONE_T)
    template< typename Pointer, class Allocator = autoboost::none_t,
        factory_alloc_propagation AP = factory_alloc_for_pointee_and_deleter >
    class factory;
#else
    template< typename Pointer, class Allocator = void,
        factory_alloc_propagation AP = factory_alloc_for_pointee_and_deleter >
    class factory;
#endif

    //----- ---- --- -- - -  -   -

    template< typename Pointer, factory_alloc_propagation AP >
    class factory<Pointer, void, AP>
    {
      public:
        typedef typename autoboost::remove_cv<Pointer>::type result_type;
        typedef typename autoboost::pointee<result_type>::type value_type;

        factory()
        { }

#     define AUTOBOOST_PP_FILENAME_1 <autoboost/functional/factory.hpp>
#     define AUTOBOOST_PP_ITERATION_LIMITS (0,AUTOBOOST_FUNCTIONAL_FACTORY_MAX_ARITY)
#     include AUTOBOOST_PP_ITERATE()
    };

#if defined(AUTOBOOST_FUNCTIONAL_FACTORY_SUPPORT_NONE_T)
    template< typename Pointer, factory_alloc_propagation AP >
    class factory<Pointer, autoboost::none_t, AP>
        : public factory<Pointer, void, AP>
    {};
#endif

    template< class Pointer, class Allocator, factory_alloc_propagation AP >
    class factory
        : private Allocator::template rebind< typename autoboost::pointee<
            typename autoboost::remove_cv<Pointer>::type >::type >::other
    {
      public:
        typedef typename autoboost::remove_cv<Pointer>::type result_type;
        typedef typename autoboost::pointee<result_type>::type value_type;

        typedef typename Allocator::template rebind<value_type>::other
            allocator_type;

        explicit factory(allocator_type const & a = allocator_type())
          : allocator_type(a)
        { }

      private:

        struct deleter
            : allocator_type
        {
            inline deleter(allocator_type const& that)
              : allocator_type(that)
            { }

            allocator_type& get_allocator() const
            {
                return *const_cast<allocator_type*>(
                    static_cast<allocator_type const*>(this));
            }

            void operator()(value_type* ptr) const
            {
                if (!! ptr) ptr->~value_type();
                const_cast<allocator_type*>(static_cast<allocator_type const*>(
                    this))->deallocate(ptr,1);
            }
        };

        inline allocator_type& get_allocator() const
        {
            return *const_cast<allocator_type*>(
                static_cast<allocator_type const*>(this));
        }

        inline result_type make_pointer(value_type* ptr, autoboost::non_type<
            factory_alloc_propagation,factory_passes_alloc_to_smart_pointer>)
        const
        {
            return result_type(ptr,deleter(this->get_allocator()));
        }
        inline result_type make_pointer(value_type* ptr, autoboost::non_type<
            factory_alloc_propagation,factory_alloc_for_pointee_and_deleter>)
        const
        {
            return result_type(ptr,deleter(this->get_allocator()),
                this->get_allocator());
        }

      public:

#     define AUTOBOOST_TMP_MACRO
#     define AUTOBOOST_PP_FILENAME_1 <autoboost/functional/factory.hpp>
#     define AUTOBOOST_PP_ITERATION_LIMITS (0,AUTOBOOST_FUNCTIONAL_FACTORY_MAX_ARITY)
#     include AUTOBOOST_PP_ITERATE()
#     undef AUTOBOOST_TMP_MACRO
    };

    template< typename Pointer, class Allocator, factory_alloc_propagation AP >
    class factory<Pointer&, Allocator, AP>;
    // forbidden, would create a dangling reference
}

#     define AUTOBOOST_FUNCTIONAL_FACTORY_HPP_INCLUDED
#   else // defined(AUTOBOOST_PP_IS_ITERATING)
#     define N AUTOBOOST_PP_ITERATION()
#     if !defined(AUTOBOOST_TMP_MACRO)
#       if N > 0
    template< AUTOBOOST_PP_ENUM_PARAMS(N, typename T) >
#       endif
    inline result_type operator()(AUTOBOOST_PP_ENUM_BINARY_PARAMS(N,T,& a)) const
    {
        return result_type( new value_type(AUTOBOOST_PP_ENUM_PARAMS(N,a)) );
    }
#     else // defined(AUTOBOOST_TMP_MACRO)
#       if N > 0
    template< AUTOBOOST_PP_ENUM_PARAMS(N, typename T) >
#       endif
    inline result_type operator()(AUTOBOOST_PP_ENUM_BINARY_PARAMS(N,T,& a)) const
    {
        value_type* memory = this->get_allocator().allocate(1);
        try
        {
            return make_pointer(
                new(memory) value_type(AUTOBOOST_PP_ENUM_PARAMS(N,a)),
                autoboost::non_type<factory_alloc_propagation,AP>() );
        }
        catch (...) { this->get_allocator().deallocate(memory,1); throw; }
    }
#     endif
#     undef N
#   endif // defined(AUTOBOOST_PP_IS_ITERATING)

#endif // include guard
