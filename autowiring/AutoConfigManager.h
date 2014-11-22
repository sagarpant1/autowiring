// Copyright (C) 2012-2014 Leap Motion, Inc. All rights reserved.
#pragma once
#include "ConfigRegistry.h"
#include "autowiring_error.h"
#include <string>
#include STL_UNORDERED_MAP
#include MEMORY_HEADER

struct AnySharedPointer;

class AutoConfigManager:
  public ContextMember
{
public:
  AutoConfigManager();
  virtual ~AutoConfigManager();
  
private:
  std::mutex m_lock;
  std::unordered_map<std::string, AnySharedPointer> m_attributes;

public:
  /// <summary>
  /// Get a reference to where the config value is stored
  /// </summary>
  /// <remarks>
  /// This method will throw an exception if the specified name cannot be found as a configurable value
  /// in the application, or if the specified value type does not match the type expected by this field
  /// </remarks>
  AnySharedPointer& Get(const std::string& name);
  
  /// <summary>
  /// Assigns the specified value to an AnySharedPointer slot
  /// </summary>
  /// <remarks>
  /// This method will throw an exception if the specified name cannot be found as a configurable value
  /// in the application, or if the specified value type does not match the type expected by this field
  /// </remarks>
  template<class T>
  void Set(const std::string& name, const T& value) {
    
    for(const auto& ctxt : ContextEnumerator(GetContext())) {
      AutowiredFast<AutoConfigManager> mgmt(ctxt);
      if(mgmt) {
        std::lock_guard<std::mutex> lk(mgmt->m_lock);
        mgmt->m_attributes[name] = AnySharedPointer(std::make_shared<T>(value));
      }
    }
  }

  void Set(const std::string& name, const char* value);

  /// <summary>
  /// Coerces the string representation of the specified field to the correct value type
  /// </summary>
  /// <remarks>
  /// This method will throw an exception if there is no string converter available on this type
  /// </remarks>
  void SetParsed(const std::string& name, const std::string& value);
};
