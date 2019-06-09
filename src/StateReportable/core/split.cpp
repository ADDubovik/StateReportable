#include "split.h"
#include "VectorHelpers.h"

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
        VectorHelpers::doubleCapacityIfNeeded(result);
        result.emplace_back(std::move(buf));
      }
    }

    return result;
  }
} // namespace StateReportable::core
