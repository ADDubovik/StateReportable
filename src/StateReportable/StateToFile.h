#pragma once

#include "StateBase.h"


namespace StateReportable
{
  template <typename State_t, typename Duration_t = std::chrono::milliseconds>
  class StateToFile : public State<State_t, Duration_t>
  {
  };
} // namespace StateReportable
