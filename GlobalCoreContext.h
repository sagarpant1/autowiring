#ifndef _GLOBAL_CORE_CONTEXT
#define _GLOBAL_CORE_CONTEXT
#include "InstantiatorLink.h"
#include "CoreContext.h"
#include <boost/thread/mutex.hpp>
 
// A special class designed to make it easier to detect when our context is the global context
class GlobalCoreContext:
  public CoreContext
{
private:
  /// <summary>
  /// The global context ctor
  /// </summary>
  /// <param name="instantiator">A function which will fill the global context with global members</param>
  /// <remarks>
  /// In any single program, only one global context exists.  Once the global context
  /// is initialized, it cannot be destroyed or re-initialized.  Members added to the
  /// global context cannot be removed until the whole program terminates.  Users who
  /// wish to add members to the global context should do so only after consideration
  /// of necessity.
  /// </remarks>
  GlobalCoreContext(void);

public:
  ~GlobalCoreContext(void);

  /// <summary>
  /// A declarative way to describe the global context.
  /// </summary>
  /// <param name="W">
  /// A structure containing AutoRequired members which are all supposed to be in the global scope
  /// </param>
  /// <remarks>
  /// This method may be called as many times as desired.
  /// </remarks>
  template<class W>
  static void AddGlobalObjects(void) {
    static InstantiatorLink current = {
      nullptr,
      &InstantiateW<W>
    };
    AddGlobalObjects(&current);
  }

  static void AddGlobalObjects(InstantiatorLink* pLink) {
    pLink->pFlink = s_instantiator;
    s_instantiator = pLink;
  }

  /// <summary>
  /// Obtains the global core context, or initializes it if necessary
  /// </summary>
  static cpp11::shared_ptr<GlobalCoreContext> Get(void);
  
  /// <summary>
  /// Releases the global context, allowing reinitialization
  /// </summary>
  /// <remarks>
  /// In addition to releasing the global core context, all initializer operations
  /// are dumped.  Note that releasing the context is not synonymous with destroying
  /// the global context--if there are shared_ptr references to the global context,
  /// those references will keep the global context resident.  Here are some places
  /// which may maintain shared_ptr references to the global contexts:
  ///
  /// * Held references via SetCurrent
  /// * Child contexts of the global context
  ///
  /// This method should only be used with great caution.  Note that, if the calling
  /// thread holds a reference to the current global context via SetCurrent, it is
  /// the caller's responsibility to manually release that reference via EvictCurrent.
  /// </remarks>
  static void Release(void) {
    // Release local:
    boost::lock_guard<boost::mutex> lk(s_initLock);
    s_instantiator = nullptr;
    s_globalContext.reset();
  }

private:
  // Singly linked list of instantiators
  static InstantiatorLink* s_instantiator;

  // Global context shared pointer and lock:
  static boost::mutex s_initLock;
  static cpp11::shared_ptr<GlobalCoreContext> s_globalContext;

  /// <summary>
  /// Initializes, then destroys, a single instance of class W in local scope
  /// </summary>
  template<class W>
  static void InstantiateW() {
    W w;
  }
};

/// <summary>
/// Provides a declarative way to set the global context
/// </summary>
template<class W>
struct GlobalContextDesignation {
  GlobalContextDesignation(void) {
    GlobalCoreContext::AddGlobalObjects<W>();
  }
};

/// <summary>
/// Obtains the global context, provided at global scope to allow forward declaration
/// </summary>
cpp11::shared_ptr<GlobalCoreContext> GetGlobalContext(void);

#endif