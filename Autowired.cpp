#include "stdafx.h"
#include "Autowired.h"
#include "CoreContext.h"
#include "GlobalCoreContext.h"

DestroyTracker::DestroyTracker(void):
    m_context(GetCurrentContext())
{
  m_tracker = cpp11::shared_ptr<DestroyTracker>(this, NullOp<DestroyTracker*>);
}
    
cpp11::shared_ptr<CoreContext> NewContextThunk(void) {
  return CoreContext::NewContext();
}

cpp11::shared_ptr<CoreContext> NewContextThunk(cpp11::shared_ptr<CoreContext>& pParent) {
  return CoreContext::NewContext(pParent);
}

// Autowired CoreContext:
void AutowiredCreator<CoreContext, false>::Create() {
  // Same as before--we don't allow creation on a non-null entity
  if(*this)
    return;

  // Typically we must do an m_context->Add, but that's not needed here.
  // All we have to do is to create a context.  Contexts are required to
  // be members of themselves, meaning that the context ctor will handle
  // the addition for us.
  *this = NewContextThunk();
  m_context = *this;
}

/// <summary>
/// Creates a dependent context based on the currently bound context
/// </summary>
void AutowiredCreator<CoreContext, false>::Push() {
  *this =
    *this ?
    NewContextThunk(*this) :
    GetGlobalContext();
  m_context = *this;
}

/// <summary>
/// Moves this context pointer up one notch to its parent context
/// </summary>
/// <remarks>
/// This may cause the current pointer to become null, if the current
/// context is the global context.
/// </remarks>
void AutowiredCreator<CoreContext, false>::Pop(void) {
  *this = cpp11::static_pointer_cast<CoreContext, Autowirer>(get()->GetParentContext());
  if(*this) {
    m_context = *this;
    get()->SetCurrent();
  } else {
    m_context.reset();
    CoreContext::EvictCurrent();
  }
}

cpp11::shared_ptr<CoreContext> GetGlobalContextAsCoreContext(void) {
  return cpp11::static_pointer_cast<CoreContext, GlobalCoreContext>(GetGlobalContext());
}

Autowired<CoreContext>::Autowired(bool forceNew) {
  if(forceNew)
    Create();
  else
    *this = GetCurrentContext();
  DestroyTracker::m_context = *this;
}

void AddGlobalObjects(InstantiatorLink* pLink) {
  GlobalCoreContext::AddGlobalObjects(pLink);
}