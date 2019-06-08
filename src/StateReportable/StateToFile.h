#pragma once

#include "core/StateBase.h"
#include "core/Dispatcher.h"
#include "ReporterToFile.h"


namespace StateReportable
{
  template <typename State_t, typename Duration_t = std::chrono::milliseconds>
  class StateToFile : public core::StateBase<State_t, Duration_t>
  {
  public:
    using Dispatcher = core::Dispatcher<ReporterToFile>;
    using Base = core::StateBase<State_t, Duration_t>;

    explicit StateToFile(const State_t state)
        : State<State_t, Duration_t>(
          state,
          [weak = Dispatcher::instance_weak()](core::ReportLine &&line)
          {
            if ( auto strong = weak.lock() )
              strong->send(std::move(line));
          }
        )
    {}
    ~StateToFile() = default;

    StateToFile(const StateToFile &) = delete;
    StateToFile(StateToFile &&) = default;
    StateToFile &operator=(const StateToFile &) = delete;
    StateToFile &operator=(StateToFile &&) = default;

    StateToFile<State_t, Duration_t> &operator=(const State_t state)
    {
      static_cast<Base &>(*this) = state;
      return *this;
    };
  };
} // namespace StateReportable
