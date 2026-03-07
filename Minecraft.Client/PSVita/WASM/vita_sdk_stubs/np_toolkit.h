#pragma once
/*
 * Minimal PS Vita NP Toolkit SDK stubs for Emscripten/WASM builds.
 * Provides the sce::Toolkit::NP namespace types used across the PSVita
 * network and commerce headers.
 */

#include <stdint.h>
#include <string>
#include <vector>

namespace sce {
namespace Toolkit {
namespace NP {

/* -------------------------------------------------------------------------
 * Generic event notification
 * ---------------------------------------------------------------------- */
struct Event {
    int type;
    int result;
    void *data;
    Event() : type(0), result(0), data(0) {}
};

/* -------------------------------------------------------------------------
 * Message / invite attachment (used by GetInviteDataAndProcess)
 * ---------------------------------------------------------------------- */
struct MessageAttachment {
    uint32_t id;
    char     subject[64];
    char     body[512];
    uint8_t  data[1024];
    uint32_t dataSize;
    MessageAttachment() : id(0), dataSize(0) { subject[0] = 0; body[0] = 0; }
};

/* -------------------------------------------------------------------------
 * Rich presence details
 * ---------------------------------------------------------------------- */
struct PresenceDetails {
    char status[256];
    char gameData[512];
    PresenceDetails() { status[0] = 0; gameData[0] = 0; }
};

/* -------------------------------------------------------------------------
 * Commerce / store types
 * ---------------------------------------------------------------------- */
struct CategoryInfo {
    char id[64];
    char name[128];
    CategoryInfo() { id[0] = 0; name[0] = 0; }
};

struct ProductInfo {
    char skuId[64];
    char name[128];
    char description[512];
    int  price;
    bool purchased;
    ProductInfo() : price(0), purchased(false) { skuId[0] = 0; name[0] = 0; description[0] = 0; }
};

struct ProductInfoDetailed : public ProductInfo {
    char additionalInfo[256];
    ProductInfoDetailed() { additionalInfo[0] = 0; }
};

/* -------------------------------------------------------------------------
 * Friends list
 * ---------------------------------------------------------------------- */
struct FriendEntry {
    char onlineId[16];
    bool online;
    FriendEntry() : online(false) { onlineId[0] = 0; }
};

struct FriendsList {
    std::vector<FriendEntry> friends;
};

/* -------------------------------------------------------------------------
 * Session information (NP sessions)
 * ---------------------------------------------------------------------- */
struct NpSessionInformation {
    uint64_t sessionId;
    NpSessionInformation() : sessionId(0) {}
};

/* -------------------------------------------------------------------------
 * Utilities namespace — provides a minimal Future<T> template
 * ---------------------------------------------------------------------- */
namespace Utilities {

template <typename T>
class Future {
public:
    Future() : m_ready(false) {}

    bool isReady() const { return m_ready; }
    const T &get() const { return m_value; }
    T &get() { return m_value; }

    void set(const T &value) { m_value = value; m_ready = true; }
    void reset() { m_ready = false; }

private:
    T    m_value;
    bool m_ready;
};

} /* namespace Utilities */

} /* namespace NP */
} /* namespace Toolkit */
} /* namespace sce */
