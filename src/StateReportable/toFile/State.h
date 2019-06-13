#pragma once

#include "StateReportable/core/StateBase.h"
#include "StateReportable/core/Dispatcher.h"
#include "Reporter.h"


namespace StateReportable::toFile
{
  template <typename State_t, typename Duration_t = std::chrono::milliseconds>
  class State : public core::StateBase<State_t, Duration_t>
  {
  public:
    using Dispatcher = core::Dispatcher<Reporter>;
    using Base = core::StateBase<State_t, Duration_t>;

    explicit State(const State_t state)
        : Base(
          state,
          [weak = Dispatcher::instanceWeak()](core::ReportLine &&line)
          {
            if ( auto strong = weak.lock() )
              strong->send(std::move(line));
          }
        )
    {}
    ~State() = default;

    State(const State &) = delete;
    State(State &&) = default;
    State &operator=(const State &) = delete;
    State &operator=(State &&) = default;

    State<State_t, Duration_t> &operator=(const State_t state)
    {
      static_cast<Base &>(*this) = state;
      return *this;
    };
  };
} // namespace StateReportable::toFile
