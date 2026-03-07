#pragma once
/* np_toolkit.h — PS Vita NP Toolkit C++ API stubs for Emscripten/WASM builds */

#include <stdint.h>
#include <string.h>
#include <string>
#include <vector>
#include "np.h"
#include "apputil.h"

/* ─────────────────────────────────────────────────────────────
 * sce::Toolkit::NP — Minimal stub namespace matching the
 * real NP Toolkit C++ API surface used by the game code.
 * ───────────────────────────────────────────────────────────── */
namespace sce {
namespace Toolkit {
namespace NP {

/* ── Forward declarations ───────────────────────────────────── */
struct Event;
class  Parameters;

/* ── Utilities ──────────────────────────────────────────────── */
namespace Utilities {

template<typename T>
class Future {
public:
    Future() : m_ready(false) {}

    bool isReady() const { return m_ready; }
    bool hasValue() const { return m_ready; }

    T getValue() const { return m_value; }
    T &getValueRef() { return m_value; }
    const T &getValueRef() const { return m_value; }

    void setValue(const T &val) { m_value = val; m_ready = true; }
    void reset() { m_ready = false; }

private:
    T    m_value;
    bool m_ready;
};

} /* namespace Utilities */

/* ── Communication ID ───────────────────────────────────────── */
struct CommunicationId {
    char data[SCE_NP_COMMUNICATION_ID_SIZE];
    CommunicationId() { memset(data, 0, sizeof(data)); }
    explicit CommunicationId(const char *s)
    { memset(data, 0, sizeof(data)); if (s) strncpy(data, s, sizeof(data)-1); }
};

/* ── Presence ────────────────────────────────────────────────── */
struct PresenceDetails {
    int     presenceType;
    uint8_t data[SCE_NP_BASIC_IN_GAME_PRESENCE_DATA_SIZE_MAX];
    uint32_t dataSize;

    PresenceDetails() : presenceType(SCE_NP_BASIC_IN_GAME_PRESENCE_TYPE_DEFAULT), dataSize(0)
    { memset(data, 0, sizeof(data)); }
};

/* ── Message attachment ──────────────────────────────────────── */
struct MessageAttachment {
    SceNpId senderNpId;
    uint8_t data[512];
    uint32_t dataSize;
    MessageAttachment() : dataSize(0)
    { memset(&senderNpId, 0, sizeof(senderNpId)); memset(data, 0, sizeof(data)); }
};

/* ── Message data ─────────────────────────────────────────────── */
struct MessageData {
    char subject[256];
    char body[512];
    MessageData() { memset(subject,0,sizeof(subject)); memset(body,0,sizeof(body)); }
};

/* ── Commerce product info ───────────────────────────────────── */
#define SCE_NP_COMMERCE2_PRODUCT_ID_LEN               37
#define SCE_NP_COMMERCE2_PRODUCT_NAME_LEN             129
#define SCE_NP_COMMERCE2_PRODUCT_SHORT_DESCRIPTION_LEN 257
#define SCE_NP_COMMERCE2_PRODUCT_LONG_DESCRIPTION_LEN  1025
#define SCE_NP_COMMERCE2_PRODUCT_LEGAL_DESCRIPTION_LEN 1025
#define SCE_NP_COMMERCE2_SKU_ID_LEN                   37
#define SCE_NP_COMMERCE2_SP_NAME_LEN                  129
#define SCE_NP_COMMERCE2_URL_LEN                      513
#define SCE_NP_COMMERCE2_CATEGORY_ID_LEN              37
#define SCE_NP_COMMERCE2_RATING_SYSTEM_ID_LEN         37
#define SCE_NP_COMMERCE2_RECV_BUF_SIZE                (64*1024)
#define SCE_NP_COMMERCE2_VERSION                      2
#define SCE_NP_COMMERCE2_ICON_DISP_RIGHT              0

#define SCE_NP_COMMERCE2_EVENT_DO_DL_LIST_SUCCESS         0x0001
#define SCE_NP_COMMERCE2_EVENT_DO_CHECKOUT_SUCCESS        0x0002
#define SCE_NP_COMMERCE2_EVENT_DO_CHECKOUT_BACK           0x0003
#define SCE_NP_COMMERCE2_EVENT_DO_PRODUCT_CODE_STARTED    0x0010
#define SCE_NP_COMMERCE2_EVENT_DO_PRODUCT_CODE_SUCCESS    0x0011
#define SCE_NP_COMMERCE2_EVENT_DO_PRODUCT_CODE_FINISHED   0x0012
#define SCE_NP_COMMERCE2_EVENT_DO_PRODUCT_CODE_BACK       0x0013
#define SCE_NP_COMMERCE2_EVENT_DO_PROD_BROWSE_OPENED      0x0020
#define SCE_NP_COMMERCE2_SERVER_ERROR_SESSION_EXPIRED     0x80550602

struct ProductInfo {
    char productId[SCE_NP_COMMERCE2_PRODUCT_ID_LEN];
    char name[SCE_NP_COMMERCE2_PRODUCT_NAME_LEN];
    char shortDescription[SCE_NP_COMMERCE2_PRODUCT_SHORT_DESCRIPTION_LEN];
    char skuId[SCE_NP_COMMERCE2_SKU_ID_LEN];
    bool isPurchased;
    ProductInfo() : isPurchased(false)
    {
        memset(productId,0,sizeof(productId));
        memset(name,0,sizeof(name));
        memset(shortDescription,0,sizeof(shortDescription));
        memset(skuId,0,sizeof(skuId));
    }
};

struct ProductInfoDetailed : public ProductInfo {
    char longDescription[SCE_NP_COMMERCE2_PRODUCT_LONG_DESCRIPTION_LEN];
    char legalDescription[SCE_NP_COMMERCE2_PRODUCT_LEGAL_DESCRIPTION_LEN];
    ProductInfoDetailed()
    {
        memset(longDescription,0,sizeof(longDescription));
        memset(legalDescription,0,sizeof(legalDescription));
    }
};

struct CategoryInfoSub {
    char categoryId[SCE_NP_COMMERCE2_CATEGORY_ID_LEN];
    char name[SCE_NP_COMMERCE2_PRODUCT_NAME_LEN];
    CategoryInfoSub() { memset(categoryId,0,sizeof(categoryId)); memset(name,0,sizeof(name)); }
};

struct CategoryInfo {
    char categoryId[SCE_NP_COMMERCE2_CATEGORY_ID_LEN];
    std::vector<CategoryInfoSub> subCategories;
    std::vector<ProductInfo>     products;
    CategoryInfo() { memset(categoryId,0,sizeof(categoryId)); }
};

/* ── Event ───────────────────────────────────────────────────── */
struct Event {
    enum UserEvent {
        /* Core / service */
        serviceError = 0,
        serviceOnline,
        serviceSignedIn,
        serviceSignedOut,

        /* Presence */
        presenceSet,
        presenceSetFailed,

        /* Profile */
        profileError,
        profileUpdated,

        /* Messaging */
        messageSent,
        messageError,
        messageRetrieved,
        messageInGameDataReceived,
        messageInGameDataRetrievalDone,
        messageDialogTerminated,

        /* Sessions */
        sessionCreated,
        sessionJoined,
        sessionLeft,
        sessionError,

        /* Commerce (UserEvent sub-events) */
        commerceSessionCreated,
        commerceSessionAborted,
        commerceGotProductList,
        commerceGotDetailedProductInfo,
        commerceGotCategoryInfo,
        commerceGotEntitlementList,
        commerceNoEntitlements,
        commerceProductBrowseStarted,
        commerceProductBrowseSuccess,
        commerceProductBrowseAborted,
        commerceProductBrowseFinished,
        commerceCheckoutStarted,
        commerceCheckoutSuccess,
        commerceCheckoutFinished,
        commerceCheckoutBack = commerceCheckoutFinished,
        commerceDownloadListStarted,
        commerceDownloadListFinished,
        commerceInstallStarted,
        commerceInstallFinished,
        commerceError,

        /* Game custom data */
        gameCustomDataDialogOpened,
        gameCustomDataDialogClosed,
        gameCustomDataDialogError,

        userEventCount
    };

    UserEvent event;
    int       returnCode;

    /* Associated data (union-style via pointers, nullable) */
    const PresenceDetails    *pPresenceDetails;
    const MessageAttachment  *pMessageAttachment;
    const ProductInfo        *pProductInfo;
    const CategoryInfo       *pCategoryInfo;
    const std::vector<ProductInfo> *pProductList;
    const std::vector<SceNpEntitlement> *pEntitlements;

    Event()
        : event(serviceError), returnCode(0),
          pPresenceDetails(0), pMessageAttachment(0),
          pProductInfo(0), pCategoryInfo(0),
          pProductList(0), pEntitlements(0) {}
};

typedef void (*EventCallback)(const Event &ev);

/* ── Service types ──────────────────────────────────────────── */
namespace ServiceType {
    enum Type {
        core       = 0x01,
        presence   = 0x02,
        profile    = 0x04,
        messaging  = 0x08,
        trophy     = 0x10,
        commerce   = 0x20,
        matching   = 0x40
    };
}

/* ── Parameters ─────────────────────────────────────────────── */
class Parameters {
public:
    EventCallback        callback;
    unsigned int         services;
    SceNpCommunicationId commId;
    int                  userId;

    Parameters() : callback(0), services(0), userId(0)
    { memset(&commId, 0, sizeof(commId)); }
};

/* ── Core interface ──────────────────────────────────────────── */
namespace Interface {
    static inline int init(const Parameters *params)
    { (void)params; return SCE_OK; }

    static inline int term(void) { return SCE_OK; }

    static inline int registerNpCommsId(const SceNpCommunicationId *commId,
        const SceNpCommunicationPassphrase *passphrase)
    { (void)commId; (void)passphrase; return SCE_OK; }

    static inline int processEvent(void) { return SCE_OK; }
} /* namespace Interface */

/* ── Presence interface ─────────────────────────────────────── */
namespace Presence {
namespace Interface {
    static inline int setPresence(const PresenceDetails *pd,
        Utilities::Future<int> *future)
    { (void)pd; (void)future; return SCE_OK; }
} /* namespace Interface */
} /* namespace Presence */

/* ── Messaging interface ────────────────────────────────────── */
struct InGameParams {
    char subject[256];
    char body[512];
    const void *data;
    uint32_t dataSize;
    InGameParams() : data(0), dataSize(0)
    { memset(subject,0,sizeof(subject)); memset(body,0,sizeof(body)); }
};

namespace Messaging {
namespace Interface {
    static inline int sendMessage(const SceNpId *npId, const InGameParams *params,
        Utilities::Future<int> *future)
    { (void)npId; (void)params; (void)future; return SCE_OK; }

    static inline int displayReceivedMessages(Utilities::Future<MessageAttachment> *future)
    { (void)future; return SCE_OK; }

    static inline int retrieveMessageAttachment(const SceNpBasicAttachmentDataId id,
        Utilities::Future<MessageAttachment> *future)
    { (void)id; (void)future; return SCE_OK; }
} /* namespace Interface */
} /* namespace Messaging */

/* ── Commerce input params ──────────────────────────────────── */
struct ProductListInputParams {
    char categoryId[SCE_NP_COMMERCE2_CATEGORY_ID_LEN];
    uint32_t startIdx;
    uint32_t count;
    ProductListInputParams() : startIdx(0), count(20)
    { memset(categoryId,0,sizeof(categoryId)); }
};

struct CategoryInfoInputParams {
    char categoryId[SCE_NP_COMMERCE2_CATEGORY_ID_LEN];
    CategoryInfoInputParams() { memset(categoryId,0,sizeof(categoryId)); }
};

struct DetailedProductInfoInputParams {
    char productId[SCE_NP_COMMERCE2_PRODUCT_ID_LEN];
    DetailedProductInfoInputParams() { memset(productId,0,sizeof(productId)); }
};

struct CheckoutInputParams {
    char skuId[SCE_NP_COMMERCE2_SKU_ID_LEN];
    CheckoutInputParams() { memset(skuId,0,sizeof(skuId)); }
};

struct DownloadListInputParams {
    int dummy;
    DownloadListInputParams() : dummy(0) {}
};

struct ProductBrowseParams {
    char productId[SCE_NP_COMMERCE2_PRODUCT_ID_LEN];
    ProductBrowseParams() { memset(productId,0,sizeof(productId)); }
};

/* ── Commerce interface ─────────────────────────────────────── */
namespace Commerce {
namespace Interface {
    static inline int createSession(
        Utilities::Future<SceNpCommerce2SessionInfo> *future)
    { (void)future; return SCE_OK; }

    static inline int getProductList(const ProductListInputParams *params,
        Utilities::Future<std::vector<ProductInfo> > *future)
    { (void)params; (void)future; return SCE_OK; }

    static inline int getCategoryInfo(const CategoryInfoInputParams *params,
        Utilities::Future<CategoryInfo> *future)
    { (void)params; (void)future; return SCE_OK; }

    static inline int getDetailedProductInfo(const DetailedProductInfoInputParams *params,
        Utilities::Future<ProductInfoDetailed> *future)
    { (void)params; (void)future; return SCE_OK; }

    static inline int productBrowse(const ProductBrowseParams *params,
        Utilities::Future<int> *future)
    { (void)params; (void)future; return SCE_OK; }

    static inline int checkout(const CheckoutInputParams *params,
        Utilities::Future<int> *future)
    { (void)params; (void)future; return SCE_OK; }

    static inline int displayDownloadList(const DownloadListInputParams *params,
        Utilities::Future<int> *future)
    { (void)params; (void)future; return SCE_OK; }

    static inline int getBgdlStatus(
        Utilities::Future<SceAppUtilBgdlStatus> *future, bool update)
    { (void)future; (void)update; return SCE_OK; }

    static inline int installContent(const char *contentId,
        Utilities::Future<int> *future)
    { (void)contentId; (void)future; return SCE_OK; }
} /* namespace Interface */
} /* namespace Commerce */

} /* namespace NP */
} /* namespace Toolkit */
} /* namespace sce */
