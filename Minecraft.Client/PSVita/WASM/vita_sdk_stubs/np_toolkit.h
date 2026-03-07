#pragma once
#include <np.h>
#include <stdint.h>
#include <vector>
#include <list>
#include <string>

/* ---- SCE Toolkit NP constants ---- */
#define SCE_TOOLKIT_NP_SUCCESS 0
#define SCE_TOOLKIT_NP_OFFLINE (-1)

#define SCE_TOOLKIT_NP_COMMERCE_CATEGORY_ID_LEN 56
#define SCE_TOOLKIT_NP_COMMERCE_PRODUCT_ID_LEN 48
#define SCE_TOOLKIT_NP_COMMERCE_CATEGORY_NAME_LEN 256
#define SCE_TOOLKIT_NP_COMMERCE_CATEGORY_DESCRIPTION_LEN 512
#define SCE_TOOLKIT_NP_COMMERCE_URL_LEN 256
#define SCE_TOOLKIT_NP_COMMERCE_PRODUCT_NAME_LEN 256
#define SCE_TOOLKIT_NP_COMMERCE_PRODUCT_SHORT_DESCRIPTION_LEN 512
#define SCE_TOOLKIT_NP_COMMERCE_SP_NAME_LEN 256
#define SCE_TOOLKIT_NP_COMMERCE_CURRENCY_CODE_LEN 4
#define SCE_TOOLKIT_NP_COMMERCE_CURRENCY_SYMBOL_LEN 4
#define SCE_TOOLKIT_NP_COMMERCE_THOUSAND_SEPARATOR_LEN 4
#define SCE_TOOLKIT_NP_COMMERCE_DECIMAL_LETTER_LEN 4
#define SCE_TOOLKIT_NP_COMMERCE_SKU_ID_LEN 56
#define SCE_TOOLKIT_NP_COMMERCE_PRODUCT_LONG_DESCRIPTION_LEN 4096
#define SCE_TOOLKIT_NP_COMMERCE_PRODUCT_LEGAL_DESCRIPTION_LEN 4096
#define SCE_TOOLKIT_NP_COMMERCE_RATING_SYSTEM_ID_LEN 16
#define SCE_TOOLKIT_NP_COMMERCE_ENTITLEMENT_ID_LEN 48
#define SCE_TOOLKIT_NP_COMMERCE_NOT_PURCHASED 0

#define SCE_TOOLKIT_NP_SKU_PRICE_LEN (SCE_TOOLKIT_NP_COMMERCE_CURRENCY_CODE_LEN \
    + SCE_TOOLKIT_NP_COMMERCE_CURRENCY_SYMBOL_LEN \
    + SCE_TOOLKIT_NP_COMMERCE_THOUSAND_SEPARATOR_LEN \
    + SCE_TOOLKIT_NP_COMMERCE_DECIMAL_LETTER_LEN)

#define SCE_TOOLKIT_NP_CREATE_SESSION_TYPE_PUBLIC 0
#define SCE_TOOLKIT_NP_FRIENDS_LIST_ALL 0
#define SCE_TOOLKIT_NP_MESSAGE_TYPE_CUSTOM_DATA 0
#define SCE_TOOLKIT_NP_PRESENCE_DATA 0
#define SCE_TOOLKIT_NP_PRESENCE_STATUS 0

namespace sce { namespace Toolkit { namespace NP {

/* ---- Event ---- */
struct Event {
    int type;
    int result;
    void *data;
};

/* ---- Init / Parameters ---- */
struct CommunicationId {
    char data[16];
};

struct NpTitleId {
    void setTitleSecret(const SceNpTitleId &titleId, const SceNpTitleSecret &secret) {}
};

struct Parameters {
    int dummy;
};

inline int init(const Parameters &) { return 0; }
inline int registerClientId(const void *) { return 0; }
inline int registerNpCommsId(const void *) { return 0; }

/* ---- Message ---- */
class MessageAttachment {
public:
    const void *getData() const { return nullptr; }
    size_t getSize() const { return 0; }
};

struct MessageData {
    int type;
    void *data;
    size_t size;
};

inline int sendMessage(const void *, bool = false) { return 0; }
inline int displayReceivedMessages(bool = false) { return 0; }
inline int retrieveMessageAttachment(void *, bool = false) { return 0; }

/* ---- Presence ---- */
struct PresenceDetails {
    char data[256];
};

inline int setPresence(const void *, bool = false) { return 0; }

/* ---- Friends ---- */
struct FriendsList {
    int count;
};

inline int getFriendslist(void *, int, bool = false) { return 0; }

/* ---- Session ---- */
struct NpSessionInformation {
    SceNpSessionId sessionId;
    int maxPlayers;
};

struct CreateNpSessionRequest {
    int type;
    int maxPlayers;
    void *data;
};

struct JoinNpSessionRequest {
    SceNpSessionId sessionId;
};

inline int createSession(const void *, bool = false) { return 0; }

/* ---- Matching ---- */
namespace matching {
    inline int create(const void *) { return 0; }
} // namespace matching

/* ---- Commerce ---- */
struct ProductInfo {
    char productId[SCE_TOOLKIT_NP_COMMERCE_PRODUCT_ID_LEN];
    char productName[SCE_TOOLKIT_NP_COMMERCE_PRODUCT_NAME_LEN];
    char price[SCE_TOOLKIT_NP_SKU_PRICE_LEN];
    int32_t purchasabilityFlag;
    int32_t skuCount;
};

struct ProductInfoDetailed {
    char productId[SCE_TOOLKIT_NP_COMMERCE_PRODUCT_ID_LEN];
    char productName[SCE_TOOLKIT_NP_COMMERCE_PRODUCT_NAME_LEN];
    char price[SCE_TOOLKIT_NP_SKU_PRICE_LEN];
    char longDescription[SCE_TOOLKIT_NP_COMMERCE_PRODUCT_LONG_DESCRIPTION_LEN];
    int32_t purchasabilityFlag;
};

struct CategoryInfo {
    char categoryId[SCE_TOOLKIT_NP_COMMERCE_CATEGORY_ID_LEN];
    char categoryName[SCE_TOOLKIT_NP_COMMERCE_CATEGORY_NAME_LEN];
    uint32_t countOfSubCategory;
    uint32_t countOfProduct;
    std::list<struct CategoryInfoSub> subCategories;
};

struct CategoryInfoSub {
    char categoryId[SCE_TOOLKIT_NP_COMMERCE_CATEGORY_ID_LEN];
    char categoryName[SCE_TOOLKIT_NP_COMMERCE_CATEGORY_NAME_LEN];
};

struct ProductListInputParams {
    int userId;
    const char *categoryId;
};

struct CategoryInfoInputParams {
    int userId;
    const char *categoryId;
};

struct DetailedProductInfoInputParams {
    int userId;
    const char *categoryId;
    const char *productId;
};

struct CheckoutInputParams {
    const char *skuIdList[16];
    uint32_t skuIdCount;
};

struct DownloadListInputParams {
    int type;
};

struct ProductBrowseParams {
    const char *productId;
};

namespace Commerce {
namespace Interface {
    inline int getProductList(void *, const ProductListInputParams &, bool = false) { return 0; }
    inline int getCategoryInfo(void *, const CategoryInfoInputParams &, bool = false) { return 0; }
    inline int getDetailedProductInfo(void *, const DetailedProductInfoInputParams &, bool = false) { return 0; }
    inline int checkout(const CheckoutInputParams &, bool = false) { return 0; }
    inline int displayDownloadList(const DownloadListInputParams &, bool = false) { return 0; }
    inline int productBrowse(const ProductBrowseParams &, bool = false) { return 0; }
    inline int getBgdlStatus(void *, bool = false) { return 0; }
    inline int installContent(bool = false) { return 0; }
} // namespace Interface
} // namespace Commerce

/* ---- Utilities ---- */
namespace Utilities {

template<typename T>
class Future {
public:
    bool isReady() const { return false; }
    T get() { return T(); }
};

} // namespace Utilities

}}} // namespace sce::Toolkit::NP
