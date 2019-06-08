#pragma once

#include "TemplateHelpers.h"
#include "ReportLine.h"

#include <chrono>
#include <sstream>
#include <string>
#include <typeinfo>
#include <functional>


namespace StateReportable::core
{
  // Base class for all states.
  // State_t should be enum or enum class with operator<< defined.
  // Duration_t should be in instantiation of std::chrono::duration.
  template <typename State_t, typename Duration_t = std::chrono::milliseconds>
  class StateBase
  {
  public:
    using ReportFn = std::function<void(ReportLine &&)>;
    using State = State_t;
    using Duration = Duration_t;

    StateBase() = delete;

    explicit StateBase(const State_t state, ReportFn reportFn) :
      m_state(state),
      m_timestamp(std::chrono::steady_clock::now()),
      m_reportFn(reportFn)
    {
      static_assert(TemplateHelpers::is_instance_of_a_given_class_template<Duration_t, std::chrono::duration>::value,
                    "Incorrect StateReportable::StateBase instantiation");
    }
    ~StateBase();

    StateBase(const StateBase &) = delete;
    StateBase(StateBase &&) = default;
    StateBase &operator=(const StateBase &) = delete;
    StateBase &operator=(StateBase &&) = default;

    StateBase<State_t, Duration_t> &operator=(const State_t state) { setState(state); return *this; };
    operator State_t() const { return m_state; };
  private:
    State_t m_state;
    std::chrono::steady_clock::time_point m_timestamp;
    ReportFn m_reportFn;

    void setState(const State_t state);
  };


  template <typename State_t, typename Duration_t>
  StateBase<State_t, Duration_t>::~StateBase()
  {
    if ( !m_reportFn )
      return;

    auto now = std::chrono::steady_clock::now();
    std::ostringstream stateBuf;
    stateBuf << m_state;
    auto line = ReportLine(
      typeid(State_t).name(),
      stateBuf.str(),
      "destruction",
      std::chrono::duration_cast<Duration_t>(now - m_timestamp).count()
    );

    m_reportFn(std::move(line));
  }

  template <typename State_t, typename Duration_t>
  void StateBase<State_t, Duration_t>::setState(const State_t state)
  {
    if ( state == m_state || !m_reportFn )
      return;

    auto now = std::chrono::steady_clock::now();

    std::ostringstream fromBuf, toBuf;
    fromBuf << m_state;
    toBuf << state;

    auto line = ReportLine(
      typeid(State_t).name(),
      fromBuf.str(),
      toBuf.str(),
      std::chrono::duration_cast<Duration_t>(now - m_timestamp).count()
    );

    m_state = state;
    m_timestamp = now;

    m_reportFn(std::move(line));
  }
} // namespace StateReportable::core
