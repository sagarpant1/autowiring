//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2005-2013. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/container for documentation.
//
//////////////////////////////////////////////////////////////////////////////
#ifndef AUTOBOOST_CONTAINER_FLAT_SET_HPP
#define AUTOBOOST_CONTAINER_FLAT_SET_HPP

#ifndef AUTOBOOST_CONFIG_HPP
#  include <autoboost/config.hpp>
#endif

#if defined(AUTOBOOST_HAS_PRAGMA_ONCE)
#  pragma once
#endif

#include <autoboost/container/detail/config_begin.hpp>
#include <autoboost/container/detail/workaround.hpp>

// container
#include <autoboost/container/allocator_traits.hpp>
#include <autoboost/container/container_fwd.hpp>
#include <autoboost/container/new_allocator.hpp> //new_allocator
// container/detail
#include <autoboost/container/detail/flat_tree.hpp>
#include <autoboost/container/detail/mpl.hpp>
// move
#include <autoboost/move/traits.hpp>
#include <autoboost/move/utility_core.hpp>
// move/detail
#if defined(AUTOBOOST_NO_CXX11_VARIADIC_TEMPLATES)
#include <autoboost/move/detail/fwd_macros.hpp>
#endif
#include <autoboost/move/detail/move_helpers.hpp>
// intrusive/detail
#include <autoboost/intrusive/detail/minimal_pair_header.hpp>      //pair
#include <autoboost/intrusive/detail/minimal_less_equal_header.hpp>//less, equal
// std
#if !defined(AUTOBOOST_NO_CXX11_HDR_INITIALIZER_LIST)
#include <initializer_list>
#endif

namespace autoboost {
namespace container {

#if !defined(AUTOBOOST_CONTAINER_DOXYGEN_INVOKED)
template <class Key, class T, class Compare, class Allocator>
class flat_multimap;
#endif

//! flat_set is a Sorted Associative Container that stores objects of type Key.
//! It is also a Unique Associative Container, meaning that no two elements are the same.
//!
//! flat_set is similar to std::set but it's implemented like an ordered vector.
//! This means that inserting a new element into a flat_set invalidates
//! previous iterators and references
//!
//! Erasing an element of a flat_set invalidates iterators and references
//! pointing to elements that come after (their keys are bigger) the erased element.
//!
//! This container provides random-access iterators.
//!
//! \tparam Key is the type to be inserted in the set, which is also the key_type
//! \tparam Compare is the comparison functor used to order keys
//! \tparam Allocator is the allocator to be used to allocate memory for this container
#ifdef AUTOBOOST_CONTAINER_DOXYGEN_INVOKED
template <class Key, class Compare = std::less<Key>, class Allocator = new_allocator<Key> >
#else
template <class Key, class Compare, class Allocator>
#endif
class flat_set
   ///@cond
   : public container_detail::flat_tree<Key, container_detail::identity<Key>, Compare, Allocator>
   ///@endcond
{
   #ifndef AUTOBOOST_CONTAINER_DOXYGEN_INVOKED
   private:
   AUTOBOOST_COPYABLE_AND_MOVABLE(flat_set)
   typedef container_detail::flat_tree<Key, container_detail::identity<Key>, Compare, Allocator> base_t;

   public:
   base_t &tree()
   {  return *this;  }

   const base_t &tree() const
   {  return *this;  }

   #endif   //#ifndef AUTOBOOST_CONTAINER_DOXYGEN_INVOKED

   public:
   //////////////////////////////////////////////
   //
   //                    types
   //
   //////////////////////////////////////////////
   typedef Key                                                                         key_type;
   typedef Key                                                                         value_type;
   typedef Compare                                                                     key_compare;
   typedef Compare                                                                     value_compare;
   typedef ::autoboost::container::allocator_traits<Allocator>                             allocator_traits_type;
   typedef typename ::autoboost::container::allocator_traits<Allocator>::pointer           pointer;
   typedef typename ::autoboost::container::allocator_traits<Allocator>::const_pointer     const_pointer;
   typedef typename ::autoboost::container::allocator_traits<Allocator>::reference         reference;
   typedef typename ::autoboost::container::allocator_traits<Allocator>::const_reference   const_reference;
   typedef typename ::autoboost::container::allocator_traits<Allocator>::size_type         size_type;
   typedef typename ::autoboost::container::allocator_traits<Allocator>::difference_type   difference_type;
   typedef Allocator                                                                   allocator_type;
   typedef typename AUTOBOOST_CONTAINER_IMPDEF(base_t::stored_allocator_type)              stored_allocator_type;
   typedef typename AUTOBOOST_CONTAINER_IMPDEF(base_t::iterator)                           iterator;
   typedef typename AUTOBOOST_CONTAINER_IMPDEF(base_t::const_iterator)                     const_iterator;
   typedef typename AUTOBOOST_CONTAINER_IMPDEF(base_t::reverse_iterator)                   reverse_iterator;
   typedef typename AUTOBOOST_CONTAINER_IMPDEF(base_t::const_reverse_iterator)             const_reverse_iterator;

   public:
   //////////////////////////////////////////////
   //
   //          construct/copy/destroy
   //
   //////////////////////////////////////////////

   //! <b>Effects</b>: Default constructs an empty container.
   //!
   //! <b>Complexity</b>: Constant.
   explicit flat_set() AUTOBOOST_NOEXCEPT_IF(container_detail::is_nothrow_default_constructible<Allocator>::value &&
                                         container_detail::is_nothrow_default_constructible<Compare>::value)
      : base_t()
   {}

   //! <b>Effects</b>: Constructs an empty container using the specified
   //! comparison object and allocator.
   //!
   //! <b>Complexity</b>: Constant.
   explicit flat_set(const Compare& comp,
                     const allocator_type& a = allocator_type())
      : base_t(comp, a)
   {}

   //! <b>Effects</b>: Constructs an empty container using the specified allocator.
   //!
   //! <b>Complexity</b>: Constant.
   explicit flat_set(const allocator_type& a)
      : base_t(a)
   {}

   //! <b>Effects</b>: Constructs an empty container using the specified comparison object and
   //! allocator, and inserts elements from the range [first ,last ).
   //!
   //! <b>Complexity</b>: Linear in N if the range [first ,last ) is already sorted using
   //! comp and otherwise N logN, where N is last - first.
   template <class InputIterator>
   flat_set(InputIterator first, InputIterator last,
            const Compare& comp = Compare(),
            const allocator_type& a = allocator_type())
      : base_t(true, first, last, comp, a)
   {}

   //! <b>Effects</b>: Constructs an empty container using the specified
   //! allocator, and inserts elements from the range [first ,last ).
   //!
   //! <b>Complexity</b>: Linear in N if the range [first ,last ) is already sorted using
   //! comp and otherwise N logN, where N is last - first.
   template <class InputIterator>
   flat_set(InputIterator first, InputIterator last, const allocator_type& a)
      : base_t(true, first, last, Compare(), a)
   {}

   //! <b>Effects</b>: Constructs an empty container using the specified comparison object and
   //! allocator, and inserts elements from the ordered unique range [first ,last). This function
   //! is more efficient than the normal range creation for ordered ranges.
   //!
   //! <b>Requires</b>: [first ,last) must be ordered according to the predicate and must be
   //! unique values.
   //!
   //! <b>Complexity</b>: Linear in N.
   //!
   //! <b>Note</b>: Non-standard extension.
   template <class InputIterator>
   flat_set(ordered_unique_range_t, InputIterator first, InputIterator last,
            const Compare& comp = Compare(),
            const allocator_type& a = allocator_type())
      : base_t(ordered_unique_range, first, last, comp, a)
   {}

#if !defined(AUTOBOOST_NO_CXX11_HDR_INITIALIZER_LIST)
   //! <b>Effects</b>: Constructs an empty container using the specified comparison object and
   //! allocator, and inserts elements from the range [il.begin(), il.end()).
   //!
   //! <b>Complexity</b>: Linear in N if the range [il.begin(), il.end()) is already sorted using
   //! comp and otherwise N logN, where N is il.begin() - il.end().
   flat_set(std::initializer_list<value_type> il, const Compare& comp = Compare(),
            const allocator_type& a = allocator_type())
      : base_t(true, il.begin(), il.end(), comp, a)
   {}

   //! <b>Effects</b>: Constructs an empty container using the specified
   //! allocator, and inserts elements from the range [il.begin(), il.end()).
   //!
   //! <b>Complexity</b>: Linear in N if the range [il.begin(), il.end()) is already sorted using
   //! comp and otherwise N logN, where N is il.begin() - il.end().
   flat_set(std::initializer_list<value_type> il, const allocator_type& a)
      : base_t(true, il.begin(), il.end(), Compare(), a)
   {}

   //! <b>Effects</b>: Constructs an empty container using the specified comparison object and
   //! allocator, and inserts elements from the ordered unique range [il.begin(), il.end()). This function
   //! is more efficient than the normal range creation for ordered ranges.
   //!
   //! <b>Requires</b>: [il.begin(), il.end()) must be ordered according to the predicate and must be
   //! unique values.
   //!
   //! <b>Complexity</b>: Linear in N.
   //!
   //! <b>Note</b>: Non-standard extension.
   flat_set(ordered_unique_range_t, std::initializer_list<value_type> il,
            const Compare& comp = Compare(), const allocator_type& a = allocator_type())
      : base_t(ordered_unique_range, il.begin(), il.end(), comp, a)
   {}
#endif

   //! <b>Effects</b>: Copy constructs the container.
   //!
   //! <b>Complexity</b>: Linear in x.size().
   flat_set(const flat_set& x)
      : base_t(static_cast<const base_t&>(x))
   {}

   //! <b>Effects</b>: Move constructs thecontainer. Constructs *this using x's resources.
   //!
   //! <b>Complexity</b>: Constant.
   //!
   //! <b>Postcondition</b>: x is emptied.
   flat_set(AUTOBOOST_RV_REF(flat_set) x)
      AUTOBOOST_NOEXCEPT_IF(autoboost::container::container_detail::is_nothrow_move_constructible<Compare>::value)
      : base_t(AUTOBOOST_MOVE_BASE(base_t, x))
   {}

   //! <b>Effects</b>: Copy constructs a container using the specified allocator.
   //!
   //! <b>Complexity</b>: Linear in x.size().
   flat_set(const flat_set& x, const allocator_type &a)
      : base_t(static_cast<const base_t&>(x), a)
   {}

   //! <b>Effects</b>: Move constructs a container using the specified allocator.
   //!                 Constructs *this using x's resources.
   //!
   //! <b>Complexity</b>: Constant if a == x.get_allocator(), linear otherwise
   flat_set(AUTOBOOST_RV_REF(flat_set) x, const allocator_type &a)
      : base_t(AUTOBOOST_MOVE_BASE(base_t, x), a)
   {}

   //! <b>Effects</b>: Makes *this a copy of x.
   //!
   //! <b>Complexity</b>: Linear in x.size().
   flat_set& operator=(AUTOBOOST_COPY_ASSIGN_REF(flat_set) x)
   {  return static_cast<flat_set&>(this->base_t::operator=(static_cast<const base_t&>(x)));  }

   //! <b>Throws</b>: If allocator_traits_type::propagate_on_container_move_assignment
   //!   is false and (allocation throws or value_type's move constructor throws)
   //!
   //! <b>Complexity</b>: Constant if allocator_traits_type::
   //!   propagate_on_container_move_assignment is true or
   //!   this->get>allocator() == x.get_allocator(). Linear otherwise.
   flat_set& operator=(AUTOBOOST_RV_REF(flat_set) x)
      AUTOBOOST_NOEXCEPT_IF( (allocator_traits_type::propagate_on_container_move_assignment::value ||
                          allocator_traits_type::is_always_equal::value) &&
                           autoboost::container::container_detail::is_nothrow_move_assignable<Compare>::value)
   {  return static_cast<flat_set&>(this->base_t::operator=(AUTOBOOST_MOVE_BASE(base_t, x)));  }

#if !defined(AUTOBOOST_NO_CXX11_HDR_INITIALIZER_LIST)
   //! <b>Effects</b>: Copy all elements from il to *this.
   //!
   //! <b>Complexity</b>: Linear in il.size().
   flat_set& operator=(std::initializer_list<value_type> il)
   {
       this->clear();
       this->insert(il.begin(), il.end());
       return *this;
   }
#endif

   #ifdef AUTOBOOST_CONTAINER_DOXYGEN_INVOKED
   //! <b>Effects</b>: Returns a copy of the allocator that
   //!   was passed to the object's constructor.
   //!
   //! <b>Complexity</b>: Constant.
   allocator_type get_allocator() const AUTOBOOST_NOEXCEPT_OR_NOTHROW;

   //! <b>Effects</b>: Returns a reference to the internal allocator.
   //!
   //! <b>Throws</b>: Nothing
   //!
   //! <b>Complexity</b>: Constant.
   //!
   //! <b>Note</b>: Non-standard extension.
   stored_allocator_type &get_stored_allocator() AUTOBOOST_NOEXCEPT_OR_NOTHROW;

   //! <b>Effects</b>: Returns a reference to the internal allocator.
   //!
   //! <b>Throws</b>: Nothing
   //!
   //! <b>Complexity</b>: Constant.
   //!
   //! <b>Note</b>: Non-standard extension.
   const stored_allocator_type &get_stored_allocator() const AUTOBOOST_NOEXCEPT_OR_NOTHROW;

   //! <b>Effects</b>: Returns an iterator to the first element contained in the container.
   //!
   //! <b>Throws</b>: Nothing.
   //!
   //! <b>Complexity</b>: Constant.
   iterator begin() AUTOBOOST_NOEXCEPT_OR_NOTHROW;

   //! <b>Effects</b>: Returns a const_iterator to the first element contained in the container.
   //!
   //! <b>Throws</b>: Nothing.
   //!
   //! <b>Complexity</b>: Constant.
   const_iterator begin() const AUTOBOOST_NOEXCEPT_OR_NOTHROW;

   //! <b>Effects</b>: Returns an iterator to the end of the container.
   //!
   //! <b>Throws</b>: Nothing.
   //!
   //! <b>Complexity</b>: Constant.
   iterator end() AUTOBOOST_NOEXCEPT_OR_NOTHROW;

   //! <b>Effects</b>: Returns a const_iterator to the end of the container.
   //!
   //! <b>Throws</b>: Nothing.
   //!
   //! <b>Complexity</b>: Constant.
   const_iterator end() const AUTOBOOST_NOEXCEPT_OR_NOTHROW;

   //! <b>Effects</b>: Returns a reverse_iterator pointing to the beginning
   //! of the reversed container.
   //!
   //! <b>Throws</b>: Nothing.
   //!
   //! <b>Complexity</b>: Constant.
   reverse_iterator rbegin() AUTOBOOST_NOEXCEPT_OR_NOTHROW;

   //! <b>Effects</b>: Returns a const_reverse_iterator pointing to the beginning
   //! of the reversed container.
   //!
   //! <b>Throws</b>: Nothing.
   //!
   //! <b>Complexity</b>: Constant.
   const_reverse_iterator rbegin() const AUTOBOOST_NOEXCEPT_OR_NOTHROW;

   //! <b>Effects</b>: Returns a reverse_iterator pointing to the end
   //! of the reversed container.
   //!
   //! <b>Throws</b>: Nothing.
   //!
   //! <b>Complexity</b>: Constant.
   reverse_iterator rend() AUTOBOOST_NOEXCEPT_OR_NOTHROW;

   //! <b>Effects</b>: Returns a const_reverse_iterator pointing to the end
   //! of the reversed container.
   //!
   //! <b>Throws</b>: Nothing.
   //!
   //! <b>Complexity</b>: Constant.
   const_reverse_iterator rend() const AUTOBOOST_NOEXCEPT_OR_NOTHROW;

   //! <b>Effects</b>: Returns a const_iterator to the first element contained in the container.
   //!
   //! <b>Throws</b>: Nothing.
   //!
   //! <b>Complexity</b>: Constant.
   const_iterator cbegin() const AUTOBOOST_NOEXCEPT_OR_NOTHROW;

   //! <b>Effects</b>: Returns a const_iterator to the end of the container.
   //!
   //! <b>Throws</b>: Nothing.
   //!
   //! <b>Complexity</b>: Constant.
   const_iterator cend() const AUTOBOOST_NOEXCEPT_OR_NOTHROW;

   //! <b>Effects</b>: Returns a const_reverse_iterator pointing to the beginning
   //! of the reversed container.
   //!
   //! <b>Throws</b>: Nothing.
   //!
   //! <b>Complexity</b>: Constant.
   const_reverse_iterator crbegin() const AUTOBOOST_NOEXCEPT_OR_NOTHROW;

   //! <b>Effects</b>: Returns a const_reverse_iterator pointing to the end
   //! of the reversed container.
   //!
   //! <b>Throws</b>: Nothing.
   //!
   //! <b>Complexity</b>: Constant.
   const_reverse_iterator crend() const AUTOBOOST_NOEXCEPT_OR_NOTHROW;

   //! <b>Effects</b>: Returns true if the container contains no elements.
   //!
   //! <b>Throws</b>: Nothing.
   //!
   //! <b>Complexity</b>: Constant.
   bool empty() const AUTOBOOST_NOEXCEPT_OR_NOTHROW;

   //! <b>Effects</b>: Returns the number of the elements contained in the container.
   //!
   //! <b>Throws</b>: Nothing.
   //!
   //! <b>Complexity</b>: Constant.
   size_type size() const AUTOBOOST_NOEXCEPT_OR_NOTHROW;

   //! <b>Effects</b>: Returns the largest possible size of the container.
   //!
   //! <b>Throws</b>: Nothing.
   //!
   //! <b>Complexity</b>: Constant.
   size_type max_size() const AUTOBOOST_NOEXCEPT_OR_NOTHROW;

   //! <b>Effects</b>: Number of elements for which memory has been allocated.
   //!   capacity() is always greater than or equal to size().
   //!
   //! <b>Throws</b>: Nothing.
   //!
   //! <b>Complexity</b>: Constant.
   size_type capacity() const AUTOBOOST_NOEXCEPT_OR_NOTHROW;

   //! <b>Effects</b>: If n is less than or equal to capacity(), this call has no
   //!   effect. Otherwise, it is a request for allocation of additional memory.
   //!   If the request is successful, then capacity() is greater than or equal to
   //!   n; otherwise, capacity() is unchanged. In either case, size() is unchanged.
   //!
   //! <b>Throws</b>: If memory allocation allocation throws or Key's copy constructor throws.
   //!
   //! <b>Note</b>: If capacity() is less than "cnt", iterators and references to
   //!   to values might be invalidated.
   void reserve(size_type cnt);

   //! <b>Effects</b>: Tries to deallocate the excess of memory created
   //    with previous allocations. The size of the vector is unchanged
   //!
   //! <b>Throws</b>: If memory allocation throws, or Key's copy constructor throws.
   //!
   //! <b>Complexity</b>: Linear to size().
   void shrink_to_fit();

   #endif   //   #if defined(AUTOBOOST_CONTAINER_DOXYGEN_INVOKED)

   //////////////////////////////////////////////
   //
   //                modifiers
   //
   //////////////////////////////////////////////

   #if !defined(AUTOBOOST_NO_CXX11_VARIADIC_TEMPLATES) || defined(AUTOBOOST_CONTAINER_DOXYGEN_INVOKED)

   //! <b>Effects</b>: Inserts an object x of type Key constructed with
   //!   std::forward<Args>(args)... if and only if there is no element in the container
   //!   with key equivalent to the key of x.
   //!
   //! <b>Returns</b>: The bool component of the returned pair is true if and only
   //!   if the insertion takes place, and the iterator component of the pair
   //!   points to the element with key equivalent to the key of x.
   //!
   //! <b>Complexity</b>: Logarithmic search time plus linear insertion
   //!   to the elements with bigger keys than x.
   //!
   //! <b>Note</b>: If an element is inserted it might invalidate elements.
   template <class... Args>
   std::pair<iterator,bool> emplace(AUTOBOOST_FWD_REF(Args)... args)
   {  return this->base_t::emplace_unique(autoboost::forward<Args>(args)...); }

   //! <b>Effects</b>: Inserts an object of type Key constructed with
   //!   std::forward<Args>(args)... in the container if and only if there is
   //!   no element in the container with key equivalent to the key of x.
   //!   p is a hint pointing to where the insert should start to search.
   //!
   //! <b>Returns</b>: An iterator pointing to the element with key equivalent
   //!   to the key of x.
   //!
   //! <b>Complexity</b>: Logarithmic search time (constant if x is inserted
   //!   right before p) plus insertion linear to the elements with bigger keys than x.
   //!
   //! <b>Note</b>: If an element is inserted it might invalidate elements.
   template <class... Args>
   iterator emplace_hint(const_iterator p, AUTOBOOST_FWD_REF(Args)... args)
   {  return this->base_t::emplace_hint_unique(p, autoboost::forward<Args>(args)...); }

   #else // !defined(AUTOBOOST_NO_CXX11_VARIADIC_TEMPLATES)

   #define AUTOBOOST_CONTAINER_FLAT_SET_EMPLACE_CODE(N) \
   AUTOBOOST_MOVE_TMPL_LT##N AUTOBOOST_MOVE_CLASS##N AUTOBOOST_MOVE_GT##N \
   std::pair<iterator,bool> emplace(AUTOBOOST_MOVE_UREF##N)\
   {  return this->base_t::emplace_unique(AUTOBOOST_MOVE_FWD##N);  }\
   \
   AUTOBOOST_MOVE_TMPL_LT##N AUTOBOOST_MOVE_CLASS##N AUTOBOOST_MOVE_GT##N \
   iterator emplace_hint(const_iterator hint AUTOBOOST_MOVE_I##N AUTOBOOST_MOVE_UREF##N)\
   {  return this->base_t::emplace_hint_unique(hint AUTOBOOST_MOVE_I##N AUTOBOOST_MOVE_FWD##N); }\
   //
   AUTOBOOST_MOVE_ITERATE_0TO9(AUTOBOOST_CONTAINER_FLAT_SET_EMPLACE_CODE)
   #undef AUTOBOOST_CONTAINER_FLAT_SET_EMPLACE_CODE

   #endif   // !defined(AUTOBOOST_NO_CXX11_VARIADIC_TEMPLATES)

   #if defined(AUTOBOOST_CONTAINER_DOXYGEN_INVOKED)
   //! <b>Effects</b>: Inserts x if and only if there is no element in the container
   //!   with key equivalent to the key of x.
   //!
   //! <b>Returns</b>: The bool component of the returned pair is true if and only
   //!   if the insertion takes place, and the iterator component of the pair
   //!   points to the element with key equivalent to the key of x.
   //!
   //! <b>Complexity</b>: Logarithmic search time plus linear insertion
   //!   to the elements with bigger keys than x.
   //!
   //! <b>Note</b>: If an element is inserted it might invalidate elements.
   std::pair<iterator, bool> insert(const value_type &x);

   //! <b>Effects</b>: Inserts a new value_type move constructed from the pair if and
   //! only if there is no element in the container with key equivalent to the key of x.
   //!
   //! <b>Returns</b>: The bool component of the returned pair is true if and only
   //!   if the insertion takes place, and the iterator component of the pair
   //!   points to the element with key equivalent to the key of x.
   //!
   //! <b>Complexity</b>: Logarithmic search time plus linear insertion
   //!   to the elements with bigger keys than x.
   //!
   //! <b>Note</b>: If an element is inserted it might invalidate elements.
   std::pair<iterator, bool> insert(value_type &&x);
   #else
   private:
   typedef std::pair<iterator, bool> insert_return_pair;
   public:
   AUTOBOOST_MOVE_CONVERSION_AWARE_CATCH(insert, value_type, insert_return_pair, this->priv_insert)
   #endif

   #if defined(AUTOBOOST_CONTAINER_DOXYGEN_INVOKED)
   //! <b>Effects</b>: Inserts a copy of x in the container if and only if there is
   //!   no element in the container with key equivalent to the key of x.
   //!   p is a hint pointing to where the insert should start to search.
   //!
   //! <b>Returns</b>: An iterator pointing to the element with key equivalent
   //!   to the key of x.
   //!
   //! <b>Complexity</b>: Logarithmic search time (constant if x is inserted
   //!   right before p) plus insertion linear to the elements with bigger keys than x.
   //!
   //! <b>Note</b>: If an element is inserted it might invalidate elements.
   iterator insert(const_iterator p, const value_type &x);

   //! <b>Effects</b>: Inserts an element move constructed from x in the container.
   //!   p is a hint pointing to where the insert should start to search.
   //!
   //! <b>Returns</b>: An iterator pointing to the element with key equivalent to the key of x.
   //!
   //! <b>Complexity</b>: Logarithmic search time (constant if x is inserted
   //!   right before p) plus insertion linear to the elements with bigger keys than x.
   //!
   //! <b>Note</b>: If an element is inserted it might invalidate elements.
   iterator insert(const_iterator p, value_type &&x);
   #else
   AUTOBOOST_MOVE_CONVERSION_AWARE_CATCH_1ARG(insert, value_type, iterator, this->priv_insert, const_iterator, const_iterator)
   #endif

   //! <b>Requires</b>: first, last are not iterators into *this.
   //!
   //! <b>Effects</b>: inserts each element from the range [first,last) if and only
   //!   if there is no element with key equivalent to the key of that element.
   //!
   //! <b>Complexity</b>: At most N log(size()+N) (N is the distance from first to last)
   //!   search time plus N*size() insertion time.
   //!
   //! <b>Note</b>: If an element is inserted it might invalidate elements.
   template <class InputIterator>
   void insert(InputIterator first, InputIterator last)
      {  this->base_t::insert_unique(first, last);  }

   //! <b>Requires</b>: first, last are not iterators into *this and
   //! must be ordered according to the predicate and must be
   //! unique values.
   //!
   //! <b>Effects</b>: inserts each element from the range [first,last) .This function
   //! is more efficient than the normal range creation for ordered ranges.
   //!
   //! <b>Complexity</b>: At most N log(size()+N) (N is the distance from first to last)
   //!   search time plus N*size() insertion time.
   //!
   //! <b>Note</b>: Non-standard extension. If an element is inserted it might invalidate elements.
   template <class InputIterator>
   void insert(ordered_unique_range_t, InputIterator first, InputIterator last)
      {  this->base_t::insert_unique(ordered_unique_range, first, last);  }

#if !defined(AUTOBOOST_NO_CXX11_HDR_INITIALIZER_LIST)
   //! <b>Effects</b>: inserts each element from the range [il.begin(), il.end()) if and only
   //!   if there is no element with key equivalent to the key of that element.
   //!
   //! <b>Complexity</b>: At most N log(size()+N) (N is the distance from il.begin() to il.end())
   //!   search time plus N*size() insertion time.
   //!
   //! <b>Note</b>: If an element is inserted it might invalidate elements.
   void insert(std::initializer_list<value_type> il)
   {  this->base_t::insert_unique(il.begin(), il.end()); }

   //! <b>Requires</b>: Range [il.begin(), il.end()) must be ordered according to the predicate
   //! and must be unique values.
   //!
   //! <b>Effects</b>: inserts each element from the range [il.begin(), il.end()) .This function
   //! is more efficient than the normal range creation for ordered ranges.
   //!
   //! <b>Complexity</b>: At most N log(size()+N) (N is the distance from il.begin() to il.end())
   //!   search time plus N*size() insertion time.
   //!
   //! <b>Note</b>: Non-standard extension. If an element is inserted it might invalidate elements.
   void insert(ordered_unique_range_t, std::initializer_list<value_type> il)
   {  this->base_t::insert_unique(ordered_unique_range, il.begin(), il.end()); }
#endif

   //! @copydoc ::autoboost::container::flat_map::merge(flat_map<Key, T, C2, Allocator>&)
   template<class C2>
   AUTOBOOST_CONTAINER_FORCEINLINE void merge(flat_set<Key, C2, Allocator>& source)
   {  this->base_t::merge_unique(source.tree());   }

   //! @copydoc ::autoboost::container::flat_map::merge(flat_set<Key, C2, Allocator>&)
   template<class C2>
   AUTOBOOST_CONTAINER_FORCEINLINE void merge(AUTOBOOST_RV_REF_BEG flat_set<Key, C2, Allocator> AUTOBOOST_RV_REF_END source)
   {  return this->merge(static_cast<flat_set<Key, C2, Allocator>&>(source));   }

   //! @copydoc ::autoboost::container::flat_map::merge(flat_multimap<Key, T, C2, Allocator>&)
   template<class C2>
   AUTOBOOST_CONTAINER_FORCEINLINE void merge(flat_multiset<Key, C2, Allocator>& source)
   {  this->base_t::merge_unique(source.tree());   }

   //! @copydoc ::autoboost::container::flat_map::merge(flat_multiset<Key, C2, Allocator>&)
   template<class C2>
   AUTOBOOST_CONTAINER_FORCEINLINE void merge(AUTOBOOST_RV_REF_BEG flat_multiset<Key, C2, Allocator> AUTOBOOST_RV_REF_END source)
   {  return this->merge(static_cast<flat_multiset<Key, C2, Allocator>&>(source));   }

   #if defined(AUTOBOOST_CONTAINER_DOXYGEN_INVOKED)

   //! <b>Effects</b>: Erases the element pointed to by p.
   //!
   //! <b>Returns</b>: Returns an iterator pointing to the element immediately
   //!   following q prior to the element being erased. If no such element exists,
   //!   returns end().
   //!
   //! <b>Complexity</b>: Linear to the elements with keys bigger than p
   //!
   //! <b>Note</b>: Invalidates elements with keys
   //!   not less than the erased element.
   iterator erase(const_iterator p);

   //! <b>Effects</b>: Erases all elements in the container with key equivalent to x.
   //!
   //! <b>Returns</b>: Returns the number of erased elements.
   //!
   //! <b>Complexity</b>: Logarithmic search time plus erasure time
   //!   linear to the elements with bigger keys.
   size_type erase(const key_type& x);

   //! <b>Effects</b>: Erases all the elements in the range [first, last).
   //!
   //! <b>Returns</b>: Returns last.
   //!
   //! <b>Complexity</b>: size()*N where N is the distance from first to last.
   //!
   //! <b>Complexity</b>: Logarithmic search time plus erasure time
   //!   linear to the elements with bigger keys.
   iterator erase(const_iterator first, const_iterator last);

   //! <b>Effects</b>: Swaps the contents of *this and x.
   //!
   //! <b>Throws</b>: Nothing.
   //!
   //! <b>Complexity</b>: Constant.
   void swap(flat_set& x)
      AUTOBOOST_NOEXCEPT_IF(  allocator_traits_type::is_always_equal::value
                                 && autoboost::container::container_detail::is_nothrow_swappable<Compare>::value );

   //! <b>Effects</b>: erase(a.begin(),a.end()).
   //!
   //! <b>Postcondition</b>: size() == 0.
   //!
   //! <b>Complexity</b>: linear in size().
   void clear() AUTOBOOST_NOEXCEPT_OR_NOTHROW;

   //! <b>Effects</b>: Returns the comparison object out
   //!   of which a was constructed.
   //!
   //! <b>Complexity</b>: Constant.
   key_compare key_comp() const;

   //! <b>Effects</b>: Returns an object of value_compare constructed out
   //!   of the comparison object.
   //!
   //! <b>Complexity</b>: Constant.
   value_compare value_comp() const;

   //! <b>Returns</b>: An iterator pointing to an element with the key
   //!   equivalent to x, or end() if such an element is not found.
   //!
   //! <b>Complexity</b>: Logarithmic.
   iterator find(const key_type& x);

   //! <b>Returns</b>: A const_iterator pointing to an element with the key
   //!   equivalent to x, or end() if such an element is not found.
   //!
   //! <b>Complexity</b>: Logarithmic.
   const_iterator find(const key_type& x) const;

   //! <b>Requires</b>: size() >= n.
   //!
   //! <b>Effects</b>: Returns an iterator to the nth element
   //!   from the beginning of the container. Returns end()
   //!   if n == size().
   //!
   //! <b>Throws</b>: Nothing.
   //!
   //! <b>Complexity</b>: Constant.
   //!
   //! <b>Note</b>: Non-standard extension
   iterator nth(size_type n) AUTOBOOST_NOEXCEPT_OR_NOTHROW;

   //! <b>Requires</b>: size() >= n.
   //!
   //! <b>Effects</b>: Returns a const_iterator to the nth element
   //!   from the beginning of the container. Returns end()
   //!   if n == size().
   //!
   //! <b>Throws</b>: Nothing.
   //!
   //! <b>Complexity</b>: Constant.
   //!
   //! <b>Note</b>: Non-standard extension
   const_iterator nth(size_type n) const AUTOBOOST_NOEXCEPT_OR_NOTHROW;

   //! <b>Requires</b>: begin() <= p <= end().
   //!
   //! <b>Effects</b>: Returns the index of the element pointed by p
   //!   and size() if p == end().
   //!
   //! <b>Throws</b>: Nothing.
   //!
   //! <b>Complexity</b>: Constant.
   //!
   //! <b>Note</b>: Non-standard extension
   size_type index_of(iterator p) AUTOBOOST_NOEXCEPT_OR_NOTHROW;

   //! <b>Requires</b>: begin() <= p <= end().
   //!
   //! <b>Effects</b>: Returns the index of the element pointed by p
   //!   and size() if p == end().
   //!
   //! <b>Throws</b>: Nothing.
   //!
   //! <b>Complexity</b>: Constant.
   //!
   //! <b>Note</b>: Non-standard extension
   size_type index_of(const_iterator p) const AUTOBOOST_NOEXCEPT_OR_NOTHROW;

   #endif   //   #if defined(AUTOBOOST_CONTAINER_DOXYGEN_INVOKED)

   //! <b>Returns</b>: The number of elements with key equivalent to x.
   //!
   //! <b>Complexity</b>: log(size())+count(k)
   size_type count(const key_type& x) const
   {  return static_cast<size_type>(this->base_t::find(x) != this->base_t::cend());  }

   #if defined(AUTOBOOST_CONTAINER_DOXYGEN_INVOKED)
   //! <b>Returns</b>: An iterator pointing to the first element with key not less
   //!   than k, or a.end() if such an element is not found.
   //!
   //! <b>Complexity</b>: Logarithmic
   iterator lower_bound(const key_type& x);

   //! <b>Returns</b>: A const iterator pointing to the first element with key not
   //!   less than k, or a.end() if such an element is not found.
   //!
   //! <b>Complexity</b>: Logarithmic
   const_iterator lower_bound(const key_type& x) const;

   //! <b>Returns</b>: An iterator pointing to the first element with key not less
   //!   than x, or end() if such an element is not found.
   //!
   //! <b>Complexity</b>: Logarithmic
   iterator upper_bound(const key_type& x);

   //! <b>Returns</b>: A const iterator pointing to the first element with key not
   //!   less than x, or end() if such an element is not found.
   //!
   //! <b>Complexity</b>: Logarithmic
   const_iterator upper_bound(const key_type& x) const;

   #endif   //   #if defined(AUTOBOOST_CONTAINER_DOXYGEN_INVOKED)

   //! <b>Effects</b>: Equivalent to std::make_pair(this->lower_bound(k), this->upper_bound(k)).
   //!
   //! <b>Complexity</b>: Logarithmic
   std::pair<const_iterator, const_iterator> equal_range(const key_type& x) const
   {  return this->base_t::lower_bound_range(x);  }

   //! <b>Effects</b>: Equivalent to std::make_pair(this->lower_bound(k), this->upper_bound(k)).
   //!
   //! <b>Complexity</b>: Logarithmic
   std::pair<iterator,iterator> equal_range(const key_type& x)
   {  return this->base_t::lower_bound_range(x);  }

   #if defined(AUTOBOOST_CONTAINER_DOXYGEN_INVOKED)

   //! <b>Effects</b>: Returns true if x and y are equal
   //!
   //! <b>Complexity</b>: Linear to the number of elements in the container.
   friend bool operator==(const flat_set& x, const flat_set& y);

   //! <b>Effects</b>: Returns true if x and y are unequal
   //!
   //! <b>Complexity</b>: Linear to the number of elements in the container.
   friend bool operator!=(const flat_set& x, const flat_set& y);

   //! <b>Effects</b>: Returns true if x is less than y
   //!
   //! <b>Complexity</b>: Linear to the number of elements in the container.
   friend bool operator<(const flat_set& x, const flat_set& y);

   //! <b>Effects</b>: Returns true if x is greater than y
   //!
   //! <b>Complexity</b>: Linear to the number of elements in the container.
   friend bool operator>(const flat_set& x, const flat_set& y);

   //! <b>Effects</b>: Returns true if x is equal or less than y
   //!
   //! <b>Complexity</b>: Linear to the number of elements in the container.
   friend bool operator<=(const flat_set& x, const flat_set& y);

   //! <b>Effects</b>: Returns true if x is equal or greater than y
   //!
   //! <b>Complexity</b>: Linear to the number of elements in the container.
   friend bool operator>=(const flat_set& x, const flat_set& y);

   //! <b>Effects</b>: x.swap(y)
   //!
   //! <b>Complexity</b>: Constant.
   friend void swap(flat_set& x, flat_set& y);

   #endif   //#ifdef AUTOBOOST_CONTAINER_DOXYGEN_INVOKED

   #ifndef AUTOBOOST_CONTAINER_DOXYGEN_INVOKED
   private:
   template<class KeyType>
   std::pair<iterator, bool> priv_insert(AUTOBOOST_FWD_REF(KeyType) x)
   {  return this->base_t::insert_unique(::autoboost::forward<KeyType>(x));  }

   template<class KeyType>
   iterator priv_insert(const_iterator p, AUTOBOOST_FWD_REF(KeyType) x)
   {  return this->base_t::insert_unique(p, ::autoboost::forward<KeyType>(x)); }
   #endif   //#ifndef AUTOBOOST_CONTAINER_DOXYGEN_INVOKED
};

#ifndef AUTOBOOST_CONTAINER_DOXYGEN_INVOKED

}  //namespace container {

//!has_trivial_destructor_after_move<> == true_type
//!specialization for optimizations
template <class Key, class Compare, class Allocator>
struct has_trivial_destructor_after_move<autoboost::container::flat_set<Key, Compare, Allocator> >
{
   typedef typename ::autoboost::container::allocator_traits<Allocator>::pointer pointer;
   static const bool value = ::autoboost::has_trivial_destructor_after_move<Allocator>::value &&
                             ::autoboost::has_trivial_destructor_after_move<pointer>::value &&
                             ::autoboost::has_trivial_destructor_after_move<Compare>::value;
};

namespace container {

#endif   //#ifndef AUTOBOOST_CONTAINER_DOXYGEN_INVOKED

//! flat_multiset is a Sorted Associative Container that stores objects of type Key.
//!
//! flat_multiset can store multiple copies of the same key value.
//!
//! flat_multiset is similar to std::multiset but it's implemented like an ordered vector.
//! This means that inserting a new element into a flat_multiset invalidates
//! previous iterators and references
//!
//! Erasing an element invalidates iterators and references
//! pointing to elements that come after (their keys are bigger) the erased element.
//!
//! This container provides random-access iterators.
//!
//! \tparam Key is the type to be inserted in the multiset, which is also the key_type
//! \tparam Compare is the comparison functor used to order keys
//! \tparam Allocator is the allocator to be used to allocate memory for this container
#ifdef AUTOBOOST_CONTAINER_DOXYGEN_INVOKED
template <class Key, class Compare = std::less<Key>, class Allocator = new_allocator<Key> >
#else
template <class Key, class Compare, class Allocator>
#endif
class flat_multiset
   ///@cond
   : public container_detail::flat_tree<Key, container_detail::identity<Key>, Compare, Allocator>
   ///@endcond
{
   #ifndef AUTOBOOST_CONTAINER_DOXYGEN_INVOKED
   private:
   AUTOBOOST_COPYABLE_AND_MOVABLE(flat_multiset)
   typedef container_detail::flat_tree<Key, container_detail::identity<Key>, Compare, Allocator> base_t;

   public:
   base_t &tree()
   {  return *this;  }

   const base_t &tree() const
   {  return *this;  }
   #endif   //#ifndef AUTOBOOST_CONTAINER_DOXYGEN_INVOKED

   public:
   //////////////////////////////////////////////
   //
   //                    types
   //
   //////////////////////////////////////////////
   typedef Key                                                                         key_type;
   typedef Key                                                                         value_type;
   typedef Compare                                                                     key_compare;
   typedef Compare                                                                     value_compare;
   typedef ::autoboost::container::allocator_traits<Allocator>                             allocator_traits_type;
   typedef typename ::autoboost::container::allocator_traits<Allocator>::pointer           pointer;
   typedef typename ::autoboost::container::allocator_traits<Allocator>::const_pointer     const_pointer;
   typedef typename ::autoboost::container::allocator_traits<Allocator>::reference         reference;
   typedef typename ::autoboost::container::allocator_traits<Allocator>::const_reference   const_reference;
   typedef typename ::autoboost::container::allocator_traits<Allocator>::size_type         size_type;
   typedef typename ::autoboost::container::allocator_traits<Allocator>::difference_type   difference_type;
   typedef Allocator                                                                   allocator_type;
   typedef typename AUTOBOOST_CONTAINER_IMPDEF(base_t::stored_allocator_type)              stored_allocator_type;
   typedef typename AUTOBOOST_CONTAINER_IMPDEF(base_t::iterator)                           iterator;
   typedef typename AUTOBOOST_CONTAINER_IMPDEF(base_t::const_iterator)                     const_iterator;
   typedef typename AUTOBOOST_CONTAINER_IMPDEF(base_t::reverse_iterator)                   reverse_iterator;
   typedef typename AUTOBOOST_CONTAINER_IMPDEF(base_t::const_reverse_iterator)             const_reverse_iterator;

   //! @copydoc ::autoboost::container::flat_set::flat_set()
   explicit flat_multiset() AUTOBOOST_NOEXCEPT_IF(container_detail::is_nothrow_default_constructible<Allocator>::value &&
                                              container_detail::is_nothrow_default_constructible<Compare>::value)
      : base_t()
   {}

   //! @copydoc ::autoboost::container::flat_set::flat_set(const Compare&, const allocator_type&)
   explicit flat_multiset(const Compare& comp,
                          const allocator_type& a = allocator_type())
      : base_t(comp, a)
   {}

   //! @copydoc ::autoboost::container::flat_set::flat_set(const allocator_type&)
   explicit flat_multiset(const allocator_type& a)
      : base_t(a)
   {}

   //! @copydoc ::autoboost::container::flat_set::flat_set(InputIterator, InputIterator, const Compare& comp, const allocator_type&)
   template <class InputIterator>
   flat_multiset(InputIterator first, InputIterator last,
                 const Compare& comp        = Compare(),
                 const allocator_type& a = allocator_type())
      : base_t(false, first, last, comp, a)
   {}

   //! @copydoc ::autoboost::container::flat_set::flat_set(InputIterator, InputIterator, const allocator_type&)
   template <class InputIterator>
   flat_multiset(InputIterator first, InputIterator last, const allocator_type& a)
      : base_t(false, first, last, Compare(), a)
   {}

   //! <b>Effects</b>: Constructs an empty flat_multiset using the specified comparison object and
   //! allocator, and inserts elements from the ordered range [first ,last ). This function
   //! is more efficient than the normal range creation for ordered ranges.
   //!
   //! <b>Requires</b>: [first ,last) must be ordered according to the predicate.
   //!
   //! <b>Complexity</b>: Linear in N.
   //!
   //! <b>Note</b>: Non-standard extension.
   template <class InputIterator>
   flat_multiset(ordered_range_t, InputIterator first, InputIterator last,
                 const Compare& comp        = Compare(),
                 const allocator_type& a = allocator_type())
      : base_t(ordered_range, first, last, comp, a)
   {}

#if !defined(AUTOBOOST_NO_CXX11_HDR_INITIALIZER_LIST)
   //! @copydoc ::autoboost::container::flat_set::flat_set(std::initializer_list<value_type>, const Compare& comp, const allocator_type&)
   flat_multiset(std::initializer_list<value_type> il, const Compare& comp = Compare(),
            const allocator_type& a = allocator_type())
      : base_t(false, il.begin(), il.end(), comp, a)
   {}

   //! @copydoc ::autoboost::container::flat_set::flat_set(std::initializer_list<value_type>, const allocator_type&)
   flat_multiset(std::initializer_list<value_type> il, const allocator_type& a)
      : base_t(false, il.begin(), il.end(), Compare(), a)
   {}

   //! <b>Effects</b>: Constructs an empty container using the specified comparison object and
   //! allocator, and inserts elements from the ordered unique range [il.begin(), il.end()). This function
   //! is more efficient than the normal range creation for ordered ranges.
   //!
   //! <b>Requires</b>: [il.begin(), il.end()) must be ordered according to the predicate.
   //!
   //! <b>Complexity</b>: Linear in N.
   //!
   //! <b>Note</b>: Non-standard extension.
   flat_multiset(ordered_range_t, std::initializer_list<value_type> il,
            const Compare& comp = Compare(), const allocator_type& a = allocator_type())
      : base_t(ordered_range, il.begin(), il.end(), comp, a)
   {}
#endif

   //! @copydoc ::autoboost::container::flat_set::flat_set(const flat_set &)
   flat_multiset(const flat_multiset& x)
      : base_t(static_cast<const base_t&>(x))
   {}

   //! @copydoc ::autoboost::container::flat_set::flat_set(flat_set &&)
   flat_multiset(AUTOBOOST_RV_REF(flat_multiset) x)
      AUTOBOOST_NOEXCEPT_IF(autoboost::container::container_detail::is_nothrow_move_constructible<Compare>::value)
      : base_t(autoboost::move(static_cast<base_t&>(x)))
   {}

   //! @copydoc ::autoboost::container::flat_set::flat_set(const flat_set &, const allocator_type &)
   flat_multiset(const flat_multiset& x, const allocator_type &a)
      : base_t(static_cast<const base_t&>(x), a)
   {}

   //! @copydoc ::autoboost::container::flat_set::flat_set(flat_set &&, const allocator_type &)
   flat_multiset(AUTOBOOST_RV_REF(flat_multiset) x, const allocator_type &a)
      : base_t(AUTOBOOST_MOVE_BASE(base_t, x), a)
   {}

   //! @copydoc ::autoboost::container::flat_set::operator=(const flat_set &)
   flat_multiset& operator=(AUTOBOOST_COPY_ASSIGN_REF(flat_multiset) x)
   {  return static_cast<flat_multiset&>(this->base_t::operator=(static_cast<const base_t&>(x)));  }

   //! @copydoc ::autoboost::container::flat_set::operator=(flat_set &&)
   flat_multiset& operator=(AUTOBOOST_RV_REF(flat_multiset) x)
      AUTOBOOST_NOEXCEPT_IF( (allocator_traits_type::propagate_on_container_move_assignment::value ||
                          allocator_traits_type::is_always_equal::value) &&
                           autoboost::container::container_detail::is_nothrow_move_assignable<Compare>::value)
   {  return static_cast<flat_multiset&>(this->base_t::operator=(AUTOBOOST_MOVE_BASE(base_t, x)));  }

#if !defined(AUTOBOOST_NO_CXX11_HDR_INITIALIZER_LIST)
   //! @copydoc ::autoboost::container::flat_set::operator=(std::initializer_list<value_type>)
   flat_multiset& operator=(std::initializer_list<value_type> il)
   {
       this->clear();
       this->insert(il.begin(), il.end());
       return *this;
   }
#endif

   #if defined(AUTOBOOST_CONTAINER_DOXYGEN_INVOKED)

   //! @copydoc ::autoboost::container::flat_set::get_allocator()
   allocator_type get_allocator() const AUTOBOOST_NOEXCEPT_OR_NOTHROW;

   //! @copydoc ::autoboost::container::flat_set::get_stored_allocator()
   stored_allocator_type &get_stored_allocator() AUTOBOOST_NOEXCEPT_OR_NOTHROW;

   //! @copydoc ::autoboost::container::flat_set::get_stored_allocator() const
   const stored_allocator_type &get_stored_allocator() const AUTOBOOST_NOEXCEPT_OR_NOTHROW;

   //! @copydoc ::autoboost::container::flat_set::begin()
   iterator begin() AUTOBOOST_NOEXCEPT_OR_NOTHROW;

   //! @copydoc ::autoboost::container::flat_set::begin() const
   const_iterator begin() const;

   //! @copydoc ::autoboost::container::flat_set::cbegin() const
   const_iterator cbegin() const AUTOBOOST_NOEXCEPT_OR_NOTHROW;

   //! @copydoc ::autoboost::container::flat_set::end()
   iterator end() AUTOBOOST_NOEXCEPT_OR_NOTHROW;

   //! @copydoc ::autoboost::container::flat_set::end() const
   const_iterator end() const AUTOBOOST_NOEXCEPT_OR_NOTHROW;

   //! @copydoc ::autoboost::container::flat_set::cend() const
   const_iterator cend() const AUTOBOOST_NOEXCEPT_OR_NOTHROW;

   //! @copydoc ::autoboost::container::flat_set::rbegin()
   reverse_iterator rbegin() AUTOBOOST_NOEXCEPT_OR_NOTHROW;

   //! @copydoc ::autoboost::container::flat_set::rbegin() const
   const_reverse_iterator rbegin() const AUTOBOOST_NOEXCEPT_OR_NOTHROW;

   //! @copydoc ::autoboost::container::flat_set::crbegin() const
   const_reverse_iterator crbegin() const AUTOBOOST_NOEXCEPT_OR_NOTHROW;

   //! @copydoc ::autoboost::container::flat_set::rend()
   reverse_iterator rend() AUTOBOOST_NOEXCEPT_OR_NOTHROW;

   //! @copydoc ::autoboost::container::flat_set::rend() const
   const_reverse_iterator rend() const AUTOBOOST_NOEXCEPT_OR_NOTHROW;

   //! @copydoc ::autoboost::container::flat_set::crend() const
   const_reverse_iterator crend() const AUTOBOOST_NOEXCEPT_OR_NOTHROW;

   //! @copydoc ::autoboost::container::flat_set::empty() const
   bool empty() const AUTOBOOST_NOEXCEPT_OR_NOTHROW;

   //! @copydoc ::autoboost::container::flat_set::size() const
   size_type size() const AUTOBOOST_NOEXCEPT_OR_NOTHROW;

   //! @copydoc ::autoboost::container::flat_set::max_size() const
   size_type max_size() const AUTOBOOST_NOEXCEPT_OR_NOTHROW;

   //! @copydoc ::autoboost::container::flat_set::capacity() const
   size_type capacity() const AUTOBOOST_NOEXCEPT_OR_NOTHROW;

   //! @copydoc ::autoboost::container::flat_set::reserve(size_type)
   void reserve(size_type cnt);

   //! @copydoc ::autoboost::container::flat_set::shrink_to_fit()
   void shrink_to_fit();

   #endif   //   #if defined(AUTOBOOST_CONTAINER_DOXYGEN_INVOKED)

   //////////////////////////////////////////////
   //
   //                modifiers
   //
   //////////////////////////////////////////////

   #if !defined(AUTOBOOST_NO_CXX11_VARIADIC_TEMPLATES) || defined(AUTOBOOST_CONTAINER_DOXYGEN_INVOKED)

   //! <b>Effects</b>: Inserts an object of type Key constructed with
   //!   std::forward<Args>(args)... and returns the iterator pointing to the
   //!   newly inserted element.
   //!
   //! <b>Complexity</b>: Logarithmic search time plus linear insertion
   //!   to the elements with bigger keys than x.
   //!
   //! <b>Note</b>: If an element is inserted it might invalidate elements.
   template <class... Args>
   iterator emplace(AUTOBOOST_FWD_REF(Args)... args)
   {  return this->base_t::emplace_equal(autoboost::forward<Args>(args)...); }

   //! <b>Effects</b>: Inserts an object of type Key constructed with
   //!   std::forward<Args>(args)... in the container.
   //!   p is a hint pointing to where the insert should start to search.
   //!
   //! <b>Returns</b>: An iterator pointing to the element with key equivalent
   //!   to the key of x.
   //!
   //! <b>Complexity</b>: Logarithmic search time (constant if x is inserted
   //!   right before p) plus insertion linear to the elements with bigger keys than x.
   //!
   //! <b>Note</b>: If an element is inserted it might invalidate elements.
   template <class... Args>
   iterator emplace_hint(const_iterator p, AUTOBOOST_FWD_REF(Args)... args)
   {  return this->base_t::emplace_hint_equal(p, autoboost::forward<Args>(args)...); }

   #else // !defined(AUTOBOOST_NO_CXX11_VARIADIC_TEMPLATES)

   #define AUTOBOOST_CONTAINER_FLAT_MULTISET_EMPLACE_CODE(N) \
   AUTOBOOST_MOVE_TMPL_LT##N AUTOBOOST_MOVE_CLASS##N AUTOBOOST_MOVE_GT##N \
   iterator emplace(AUTOBOOST_MOVE_UREF##N)\
   {  return this->base_t::emplace_equal(AUTOBOOST_MOVE_FWD##N);  }\
   \
   AUTOBOOST_MOVE_TMPL_LT##N AUTOBOOST_MOVE_CLASS##N AUTOBOOST_MOVE_GT##N \
   iterator emplace_hint(const_iterator hint AUTOBOOST_MOVE_I##N AUTOBOOST_MOVE_UREF##N)\
   {  return this->base_t::emplace_hint_equal(hint AUTOBOOST_MOVE_I##N AUTOBOOST_MOVE_FWD##N); }\
   //
   AUTOBOOST_MOVE_ITERATE_0TO9(AUTOBOOST_CONTAINER_FLAT_MULTISET_EMPLACE_CODE)
   #undef AUTOBOOST_CONTAINER_FLAT_MULTISET_EMPLACE_CODE

   #endif   // !defined(AUTOBOOST_NO_CXX11_VARIADIC_TEMPLATES)

   #if defined(AUTOBOOST_CONTAINER_DOXYGEN_INVOKED)
   //! <b>Effects</b>: Inserts x and returns the iterator pointing to the
   //!   newly inserted element.
   //!
   //! <b>Complexity</b>: Logarithmic search time plus linear insertion
   //!   to the elements with bigger keys than x.
   //!
   //! <b>Note</b>: If an element is inserted it might invalidate elements.
   iterator insert(const value_type &x);

   //! <b>Effects</b>: Inserts a new value_type move constructed from x
   //!   and returns the iterator pointing to the newly inserted element.
   //!
   //! <b>Complexity</b>: Logarithmic search time plus linear insertion
   //!   to the elements with bigger keys than x.
   //!
   //! <b>Note</b>: If an element is inserted it might invalidate elements.
   iterator insert(value_type &&x);
   #else
   AUTOBOOST_MOVE_CONVERSION_AWARE_CATCH(insert, value_type, iterator, this->priv_insert)
   #endif

   #if defined(AUTOBOOST_CONTAINER_DOXYGEN_INVOKED)
   //! <b>Effects</b>: Inserts a copy of x in the container.
   //!   p is a hint pointing to where the insert should start to search.
   //!
   //! <b>Returns</b>: An iterator pointing to the element with key equivalent
   //!   to the key of x.
   //!
   //! <b>Complexity</b>: Logarithmic search time (constant if x is inserted
   //!   right before p) plus insertion linear to the elements with bigger keys than x.
   //!
   //! <b>Note</b>: If an element is inserted it might invalidate elements.
   iterator insert(const_iterator p, const value_type &x);

   //! <b>Effects</b>: Inserts a new value move constructed  from x in the container.
   //!   p is a hint pointing to where the insert should start to search.
   //!
   //! <b>Returns</b>: An iterator pointing to the element with key equivalent
   //!   to the key of x.
   //!
   //! <b>Complexity</b>: Logarithmic search time (constant if x is inserted
   //!   right before p) plus insertion linear to the elements with bigger keys than x.
   //!
   //! <b>Note</b>: If an element is inserted it might invalidate elements.
   iterator insert(const_iterator p, value_type &&x);
   #else
   AUTOBOOST_MOVE_CONVERSION_AWARE_CATCH_1ARG(insert, value_type, iterator, this->priv_insert, const_iterator, const_iterator)
   #endif

   //! <b>Requires</b>: first, last are not iterators into *this.
   //!
   //! <b>Effects</b>: inserts each element from the range [first,last) .
   //!
   //! <b>Complexity</b>: At most N log(size()+N) (N is the distance from first to last)
   //!   search time plus N*size() insertion time.
   //!
   //! <b>Note</b>: If an element is inserted it might invalidate elements.
   template <class InputIterator>
   void insert(InputIterator first, InputIterator last)
      {  this->base_t::insert_equal(first, last);  }

   //! <b>Requires</b>: first, last are not iterators into *this and
   //! must be ordered according to the predicate.
   //!
   //! <b>Effects</b>: inserts each element from the range [first,last) .This function
   //! is more efficient than the normal range creation for ordered ranges.
   //!
   //! <b>Complexity</b>: At most N log(size()+N) (N is the distance from first to last)
   //!   search time plus N*size() insertion time.
   //!
   //! <b>Note</b>: Non-standard extension. If an element is inserted it might invalidate elements.
   template <class InputIterator>
   void insert(ordered_range_t, InputIterator first, InputIterator last)
      {  this->base_t::insert_equal(ordered_range, first, last);  }

#if !defined(AUTOBOOST_NO_CXX11_HDR_INITIALIZER_LIST)
   //! <b>Effects</b>: inserts each element from the range [il.begin(), il.end()).
   //!
   //! <b>Complexity</b>: At most N log(size()+N) (N is the distance from first to last)
   //!   search time plus N*size() insertion time.
   //!
   //! <b>Note</b>: If an element is inserted it might invalidate elements.
   void insert(std::initializer_list<value_type> il)
   {  this->base_t::insert_equal(il.begin(), il.end()); }

   //! <b>Requires</b>: Range [il.begin(), il.end()) must be ordered according to the predicate.
   //!
   //! <b>Effects</b>: inserts each element from the range [il.begin(), il.end()). This function
   //! is more efficient than the normal range creation for ordered ranges.
   //!
   //! <b>Complexity</b>: At most N log(size()+N) (N is the distance from il.begin() to il.end())
   //!   search time plus N*size() insertion time.
   //!
   //! <b>Note</b>: Non-standard extension. If an element is inserted it might invalidate elements.
   void insert(ordered_range_t, std::initializer_list<value_type> il)
   {  this->base_t::insert_equal(ordered_range, il.begin(), il.end()); }
#endif

   //! @copydoc ::autoboost::container::flat_multimap::merge(flat_multimap<Key, T, C2, Allocator>&)
   template<class C2>
   AUTOBOOST_CONTAINER_FORCEINLINE void merge(flat_multiset<Key, C2, Allocator>& source)
   {  this->base_t::merge_equal(source.tree());   }

   //! @copydoc ::autoboost::container::flat_multiset::merge(flat_multiset<Key, C2, Allocator>&)
   template<class C2>
   AUTOBOOST_CONTAINER_FORCEINLINE void merge(AUTOBOOST_RV_REF_BEG flat_multiset<Key, C2, Allocator> AUTOBOOST_RV_REF_END source)
   {  return this->merge(static_cast<flat_multiset<Key, C2, Allocator>&>(source));   }

   //! @copydoc ::autoboost::container::flat_multimap::merge(flat_map<Key, T, C2, Allocator>&)
   template<class C2>
   AUTOBOOST_CONTAINER_FORCEINLINE void merge(flat_set<Key, C2, Allocator>& source)
   {  this->base_t::merge_equal(source.tree());   }

   //! @copydoc ::autoboost::container::flat_multiset::merge(flat_set<Key, C2, Allocator>&)
   template<class C2>
   AUTOBOOST_CONTAINER_FORCEINLINE void merge(AUTOBOOST_RV_REF_BEG flat_set<Key, C2, Allocator> AUTOBOOST_RV_REF_END source)
   {  return this->merge(static_cast<flat_set<Key, C2, Allocator>&>(source));   }

   #if defined(AUTOBOOST_CONTAINER_DOXYGEN_INVOKED)

   //! @copydoc ::autoboost::container::flat_set::erase(const_iterator)
   iterator erase(const_iterator p);

   //! @copydoc ::autoboost::container::flat_set::erase(const key_type&)
   size_type erase(const key_type& x);

   //! @copydoc ::autoboost::container::flat_set::erase(const_iterator,const_iterator)
   iterator erase(const_iterator first, const_iterator last);

   //! @copydoc ::autoboost::container::flat_set::swap
   void swap(flat_multiset& x)
      AUTOBOOST_NOEXCEPT_IF(  allocator_traits_type::is_always_equal::value
                                 && autoboost::container::container_detail::is_nothrow_swappable<Compare>::value );

   //! @copydoc ::autoboost::container::flat_set::clear
   void clear() AUTOBOOST_NOEXCEPT_OR_NOTHROW;

   //! @copydoc ::autoboost::container::flat_set::key_comp
   key_compare key_comp() const;

   //! @copydoc ::autoboost::container::flat_set::value_comp
   value_compare value_comp() const;

   //! @copydoc ::autoboost::container::flat_set::find(const key_type& )
   iterator find(const key_type& x);

   //! @copydoc ::autoboost::container::flat_set::find(const key_type& ) const
   const_iterator find(const key_type& x) const;

   //! @copydoc ::autoboost::container::flat_set::nth(size_type)
   iterator nth(size_type n) AUTOBOOST_NOEXCEPT_OR_NOTHROW;

   //! @copydoc ::autoboost::container::flat_set::nth(size_type) const
   const_iterator nth(size_type n) const AUTOBOOST_NOEXCEPT_OR_NOTHROW;

   //! @copydoc ::autoboost::container::flat_set::index_of(iterator)
   size_type index_of(iterator p) AUTOBOOST_NOEXCEPT_OR_NOTHROW;

   //! @copydoc ::autoboost::container::flat_set::index_of(const_iterator) const
   size_type index_of(const_iterator p) const AUTOBOOST_NOEXCEPT_OR_NOTHROW;

   //! @copydoc ::autoboost::container::flat_set::count(const key_type& ) const
   size_type count(const key_type& x) const;

   //! @copydoc ::autoboost::container::flat_set::lower_bound(const key_type& )
   iterator lower_bound(const key_type& x);

   //! @copydoc ::autoboost::container::flat_set::lower_bound(const key_type& ) const
   const_iterator lower_bound(const key_type& x) const;

   //! @copydoc ::autoboost::container::flat_set::upper_bound(const key_type& )
   iterator upper_bound(const key_type& x);

   //! @copydoc ::autoboost::container::flat_set::upper_bound(const key_type& ) const
   const_iterator upper_bound(const key_type& x) const;

   //! @copydoc ::autoboost::container::flat_set::equal_range(const key_type& ) const
   std::pair<const_iterator, const_iterator> equal_range(const key_type& x) const;

   //! @copydoc ::autoboost::container::flat_set::equal_range(const key_type& )
   std::pair<iterator,iterator> equal_range(const key_type& x);

   //! <b>Effects</b>: Returns true if x and y are equal
   //!
   //! <b>Complexity</b>: Linear to the number of elements in the container.
   friend bool operator==(const flat_multiset& x, const flat_multiset& y);

   //! <b>Effects</b>: Returns true if x and y are unequal
   //!
   //! <b>Complexity</b>: Linear to the number of elements in the container.
   friend bool operator!=(const flat_multiset& x, const flat_multiset& y);

   //! <b>Effects</b>: Returns true if x is less than y
   //!
   //! <b>Complexity</b>: Linear to the number of elements in the container.
   friend bool operator<(const flat_multiset& x, const flat_multiset& y);

   //! <b>Effects</b>: Returns true if x is greater than y
   //!
   //! <b>Complexity</b>: Linear to the number of elements in the container.
   friend bool operator>(const flat_multiset& x, const flat_multiset& y);

   //! <b>Effects</b>: Returns true if x is equal or less than y
   //!
   //! <b>Complexity</b>: Linear to the number of elements in the container.
   friend bool operator<=(const flat_multiset& x, const flat_multiset& y);

   //! <b>Effects</b>: Returns true if x is equal or greater than y
   //!
   //! <b>Complexity</b>: Linear to the number of elements in the container.
   friend bool operator>=(const flat_multiset& x, const flat_multiset& y);

   //! <b>Effects</b>: x.swap(y)
   //!
   //! <b>Complexity</b>: Constant.
   friend void swap(flat_multiset& x, flat_multiset& y);

   #endif   //#ifdef AUTOBOOST_CONTAINER_DOXYGEN_INVOKED

   #ifndef AUTOBOOST_CONTAINER_DOXYGEN_INVOKED
   private:
   template <class KeyType>
   iterator priv_insert(AUTOBOOST_FWD_REF(KeyType) x)
   {  return this->base_t::insert_equal(::autoboost::forward<KeyType>(x));  }

   template <class KeyType>
   iterator priv_insert(const_iterator p, AUTOBOOST_FWD_REF(KeyType) x)
   {  return this->base_t::insert_equal(p, ::autoboost::forward<KeyType>(x)); }
   #endif   //#ifndef AUTOBOOST_CONTAINER_DOXYGEN_INVOKED
};

#ifndef AUTOBOOST_CONTAINER_DOXYGEN_INVOKED

}  //namespace container {

//!has_trivial_destructor_after_move<> == true_type
//!specialization for optimizations
template <class Key, class Compare, class Allocator>
struct has_trivial_destructor_after_move<autoboost::container::flat_multiset<Key, Compare, Allocator> >
{
   typedef typename ::autoboost::container::allocator_traits<Allocator>::pointer pointer;
   static const bool value = ::autoboost::has_trivial_destructor_after_move<Allocator>::value &&
                             ::autoboost::has_trivial_destructor_after_move<pointer>::value &&
                             ::autoboost::has_trivial_destructor_after_move<Compare>::value;
};

namespace container {

#endif   //#ifndef AUTOBOOST_CONTAINER_DOXYGEN_INVOKED

}}

#include <autoboost/container/detail/config_end.hpp>

#endif   // AUTOBOOST_CONTAINER_FLAT_SET_HPP