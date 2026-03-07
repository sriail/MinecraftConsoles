#pragma once
// Minimal PS Vita np_toolkit.h stub for Emscripten/WASM builds.
// Provides the sce::Toolkit::NP namespace types used by this codebase.

#include <stdint.h>
#include <string>
#include <vector>
#include <list>
#include "np.h"
#include "apputil.h"

namespace sce {
namespace Toolkit {
namespace NP {

// Forward declarations
struct Event;
struct MessageAttachment;
struct NpSessionInformation;
struct PresenceDetails;
struct ProductInfo;
struct ProductInfoDetailed;
struct CategoryInfo;
struct CategoryInfoSub;

// Event system
struct Event {
    int type;
    int result;
    void *data;
    uint32_t dataSize;
    Event() : type(0), result(0), data(0), dataSize(0) {}
};

// Message attachment
struct MessageAttachment {
    SceNpId  senderNpId;
    uint8_t  data[512];
    uint32_t dataSize;
};

// NP Session
struct NpSessionInformation {
    SceNpSessionId sessionId;
    uint32_t maxPlayers;
    uint32_t numPlayers;
};

// Presence
struct PresenceDetails {
    uint32_t size;
    char     statusString[64];
    uint8_t  gameData[512];
    uint32_t gameDataLen;
};

// Commerce / DLC types
struct ProductInfo {
    std::string contentId;
    std::string name;
    std::string description;
    uint32_t    price;
    bool        isPurchased;
};

struct CategoryInfoSub {
    std::string id;
    std::string name;
};

struct CategoryInfo {
    std::string id;
    std::string name;
    std::list<CategoryInfoSub> subCategories;
};

struct ProductInfoDetailed : public ProductInfo {
    std::string detailedDescription;
};

// Input parameter types
struct ProductListInputParams {
    std::string categoryId;
    uint32_t    userId;
};

struct CategoryInfoInputParams {
    std::string categoryId;
    uint32_t    userId;
};

struct DetailedProductInfoInputParams {
    std::string contentId;
    uint32_t    userId;
};

struct CheckoutInputParams {
    std::string contentId;
    uint32_t    userId;
};

struct DownloadListInputParams {
    uint32_t userId;
};

struct ProductBrowseParams {
    uint32_t userId;
};

// NP types
struct NpTitleId {
    char data[9];
};

struct CommunicationId {
    char data[9];
};

struct Parameters {
    CommunicationId commId;
};

struct ServiceType {
    uint32_t type;
};

// Friends
struct FriendInfoRequest {
    SceNpId npId;
};

struct FriendsList {
    std::vector<SceNpId> friends;
};

struct MessageData {
    SceNpId  sender;
    uint8_t  data[512];
    uint32_t dataSize;
};

// Session request types
struct CreateNpSessionRequest {
    uint32_t maxPlayers;
};

struct JoinNpSessionRequest {
    SceNpSessionId sessionId;
};

// Utilities namespace
namespace Utilities {
    template<typename T>
    class Future {
    public:
        bool isReady() const { return m_ready; }
        bool hasError() const { return m_error; }
        int  getError() const { return m_errorCode; }
        T&   get() { return m_value; }
        const T& get() const { return m_value; }
        void reset() { m_ready = false; m_error = false; }
        Future() : m_ready(false), m_error(false), m_errorCode(0) {}
    private:
        T    m_value;
        bool m_ready;
        bool m_error;
        int  m_errorCode;
    };
} // namespace Utilities

// Sub-namespaces with Interface classes
namespace Commerce {
    class Interface {
    public:
        static int getBgdlStatus(void *status, bool wait) { return 0; }
        static int getProductList(Utilities::Future<std::vector<ProductInfo>> *future,
                                   const ProductListInputParams &params,
                                   bool wait) { return 0; }
        static int getCategoryInfo(Utilities::Future<CategoryInfo> *future,
                                    const CategoryInfoInputParams &params,
                                    bool wait) { return 0; }
        static int getDetailedProductInfo(Utilities::Future<ProductInfoDetailed> *future,
                                           const DetailedProductInfoInputParams &params,
                                           bool wait) { return 0; }
        static int checkout(const CheckoutInputParams &params) { return 0; }
    };
} // namespace Commerce

namespace Sessions {
    class Interface {
    public:
        static int create(Utilities::Future<NpSessionInformation> *future,
                          const CreateNpSessionRequest &req) { return 0; }
        static int join(Utilities::Future<NpSessionInformation> *future,
                        const JoinNpSessionRequest &req) { return 0; }
        static int leave() { return 0; }
    };
} // namespace Sessions

namespace Messaging {
    class Interface {
    public:
        static int retrieveAttachment(Utilities::Future<MessageAttachment> *future,
                                       uint32_t msgId) { return 0; }
    };
} // namespace Messaging

namespace Friends {
    class Interface {
    public:
        static int getFriendsList(Utilities::Future<FriendsList> *future,
                                   const FriendInfoRequest &req) { return 0; }
    };
} // namespace Friends

namespace Presence {
    class Interface {
    public:
        static int setPresence(const PresenceDetails &details) { return 0; }
    };
} // namespace Presence

} // namespace NP
} // namespace Toolkit
} // namespace sce
