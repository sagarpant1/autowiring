// Copyright (C) 2012-2018 Leap Motion, Inc. All rights reserved.
#pragma once
#include "index_tuple.h"
#include <memory>
#include <tuple>

namespace autowiring {
  // Callable wrapper type, always invoked in a synchronized context
  struct callable_base {
    virtual ~callable_base(void) {}
    virtual void operator()() {}
    callable_base* m_pFlink = nullptr;
  };

  template<typename Fn, typename... Args>
  struct callable :
    callable_base
  {
    callable(Fn&& fn, Args&&... args) :
      fn(std::move(fn)),
      args(std::forward<Args>(args)...)
    {}

    Fn fn;
    std::tuple<typename std::decay<Args>::type...> args;

    template<int... N>
    void call(index_tuple<N...>) {
      fn(
        std::move(std::get<N>(args))...
      );
    }

    void operator()() override {
      call(typename make_index_tuple<sizeof...(Args)>::type{});
    }
  };

  template<typename Fn>
  struct callable<Fn> :
    callable_base
  {
    callable(Fn&& fn) : fn(std::move(fn)) {}
    Fn fn;
    void operator()() override { fn(); }
  };
}
