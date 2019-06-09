#pragma once

#include <vector>
#include <algorithm>


namespace VectorHelpers
{
  template<typename T>
  void doubleCapacityIfNeeded(std::vector<T> &vec)
  {
    if ( vec.size() == vec.capacity() )
      vec.reserve(std::max<size_t>(1u, vec.size() * 2));
  };
} // namespace VectorHelpers
