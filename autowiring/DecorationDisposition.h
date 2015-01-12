// Copyright (C) 2012-2014 Leap Motion, Inc. All rights reserved.
#pragma once
#include "AutowiringConfig.h"
#include "AnySharedPointer.h"
#include <cassert>
#include <vector>
#include TYPE_INDEX_HEADER

struct SatCounter;

struct DecorationKey {
  DecorationKey(const DecorationKey& rhs) :
    ti(rhs.ti),
    tshift(rhs.tshift)
  {}
  
  explicit DecorationKey(const std::type_info& ti, int tshift = 0) :
    ti(ti),
    tshift(tshift)
  {}
  
  // The type index
  const std::type_info& ti;
  
  // Zero refers to a decoration created on this packet, a positive number [tshift] indicates
  // a decoration attached [tshift] packets ago.
  int tshift;
  
  bool operator==(const DecorationKey& rhs) const {
    return ti == rhs.ti && tshift == rhs.tshift;
  }
};

namespace std {
  template<>
  struct hash<DecorationKey> {
    size_t operator()(const DecorationKey& key) const {
#if AUTOWIRING_USE_LIBCXX
      return key.ti.hash_code() + key.tshift;
#else
      return std::type_index(key.ti).hash_code() + key.tshift;
#endif
    }
  };
}

/// <remarks>
/// A disposition holder, used to maintain initialization state on the key
/// </remarks>
struct DecorationDisposition
{
#if AUTOWIRING_USE_LIBCXX
  DecorationDisposition(DecorationDisposition&&) = delete;
  void operator=(DecorationDisposition&&) = delete;
#else
  // The methods below are needed for c++98 builds
  DecorationDisposition(DecorationDisposition&& source) :
    m_decoration(source.m_decoration),
    m_pImmediate(source.m_pImmediate),
    m_publisher(source.m_publisher),
    isCheckedOut(source.isCheckedOut),
    satisfied(source.satisfied)
  {}
  DecorationDisposition& operator=(DecorationDisposition&& source) {
    m_decoration = std::move(source.m_decoration);
    m_pImmediate = source.m_pImmediate;
    source.m_pImmediate = nullptr;
    m_publisher = source.m_publisher;
    isCheckedOut = source.isCheckedOut;
    satisfied = source.satisfied;
    return *this;
  }
#endif //AUTOWIRING_USE_LIBCXX

  DecorationDisposition(void) :
    m_type(nullptr),
    m_pImmediate(nullptr),
    m_publisher(nullptr),
    isCheckedOut(false),
    satisfied(false)
  {}

  DecorationDisposition(const DecorationDisposition& source) :
    m_type(source.m_type),
    m_pImmediate(source.m_pImmediate),
    m_publisher(source.m_publisher),
    m_subscribers(source.m_subscribers),
    isCheckedOut(source.isCheckedOut),
    satisfied(source.satisfied)
  {}

  DecorationDisposition& operator=(const DecorationDisposition& source) {
    m_type = source.m_type;
    m_pImmediate = source.m_pImmediate;
    m_publisher = source.m_publisher;
    m_subscribers = source.m_subscribers;
    isCheckedOut = source.isCheckedOut;
    satisfied = source.satisfied;
    return *this;
  }
private:
  // Destructured key for this decoration. Use accessor functions to access
  // This is needed because DecorationKey doesn't have a default constructor
  const std::type_info* m_type;
  int m_tshift;
  
public:
  // The decoration proper--potentially, this decoration might be from a prior execution of this
  // packet.  In the case of immediate decorations, this value will be invalid.
  AnySharedPointer m_decoration;

  // A pointer to the immediate decorations, if one is specified, or else nullptr
  const void* m_pImmediate;

  // Provider for this decoration, where it can be statically inferred.  Note that a provider for
  // this decoration may exist even if this value is null, in the event that dynamic decoration is
  // taking place.
  SatCounter* m_publisher;

  // Satisfaction counters
  std::vector<SatCounter*> m_subscribers;

  // Indicates that the internally held object is currently checked out,
  // but might not be satisfied, since the data is being prepared.
  bool isCheckedOut;

  // Flag indicating that this entry is satisfied
  // This implies that the entry has been previously checked out.
  // NOTE: In order to make a type unsatisfiable set (and persist)
  // isCheckedOut == true && satisfied == false
  bool satisfied;
  
  // Set the key that identifies this decoration
  void SetKey(const DecorationKey& key) {
    m_type = &key.ti;
    m_tshift = key.tshift;
  }
  
  // Get the key that identifies this decoration
  DecorationKey GetKey(void) const {
    assert(m_type);
    return DecorationKey(*m_type, m_tshift);
  }
  
  operator bool() {
    return !!m_type;
  }

  void Reset(void) {
    // IMPORTANT: Do not reset type_info
    m_decoration->reset();
    m_pImmediate = nullptr;
    isCheckedOut = false;
    satisfied = false;
  }
};
