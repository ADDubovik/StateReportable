#pragma once

#include "core/Dispatcher.h"

// Special-purpose file - should be included to .cpp (not to .h!)
// to avoid potential template + singleton problems.
// Also, user should instantiate Dispatcher::instanceWeak() with typename needed.

namespace StateReportable::core
{
  template<typename Destination_t>
  std::weak_ptr<Dispatcher<Destination_t>> Dispatcher<Destination_t>::instanceWeak()
  {
    // "new" needed and no std::make_shared because of private c-tor
    static auto inst = std::shared_ptr<My_t>(new My_t());
    return inst;
  }
} // namespace StateReportable::core
