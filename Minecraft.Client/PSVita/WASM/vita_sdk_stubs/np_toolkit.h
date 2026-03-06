#pragma once
#include <np.h>
#include <stdint.h>

namespace sce { namespace Toolkit { namespace NP {

struct Event {
    int type;
    int result;
    void *data;
};

class MessageAttachment {
public:
    const void *getData() const { return nullptr; }
    size_t getSize() const { return 0; }
};

namespace Utilities {

template<typename T>
class Future {
public:
    bool isReady() const { return false; }
    T get() { return T(); }
};

} // namespace Utilities

}}} // namespace sce::Toolkit::NP
