#ifndef OPTIONAL_H
#define OPTIONAL_H

#include <memory>

namespace ecs
{

// Type alias for documenting that the pointer may be null.
template <typename T>
using optionalPtr = T*;

// Type alias for documenting that the unique_ptr may be null.
/*template <typename T>
using optionalUniquePtr = std::unique_ptr<T>*;*/

} // namespace ecs

#endif // OPTIONAL_H
