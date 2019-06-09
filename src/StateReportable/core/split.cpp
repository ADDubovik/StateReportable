#include "split.h"

#include <cstring>
#include <algorithm>


namespace StateReportable::core
{
  std::vector<std::string> split(const std::string &str, char separator)
  {
    std::vector<std::string> result;

    auto iter = str.cbegin(), endIter = str.cend();
    while ( iter != endIter )
    {
      std::string buf;
      while ( iter != endIter )
        if ( *iter != separator )
          buf += *(iter++);
        else
        {
          ++iter;
          break;
        }

      if ( !buf.empty() )
      {
        // Optimization
        if ( result.size() == result.capacity() )
          result.reserve(std::max<size_t>(1u, result.size() * 2));

        result.emplace_back(std::move(buf));
      }
    }

    return result;
  }
} // namespace StateReportable::core
