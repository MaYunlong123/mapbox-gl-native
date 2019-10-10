#include <mbgl/storage/resource_options.hpp>

#include "file_source.hpp"

namespace mbgl {

std::atomic_size_t requestCount{0};
std::atomic_size_t transferredSize{0};
std::atomic_bool active{false};

ProxyFileSource::ProxyFileSource(const std::string& cachePath,
                                 const std::string& assetPath,
                                 bool supportCacheOnlyRequests_)
    : DefaultFileSource(cachePath, assetPath, supportCacheOnlyRequests_) {}

ProxyFileSource::ProxyFileSource(const std::string& cachePath,
                                 std::unique_ptr<FileSource>&& assetFileSource_,
                                 bool supportCacheOnlyRequests_)
    : DefaultFileSource(cachePath, std::move(assetFileSource_), supportCacheOnlyRequests_) {}

ProxyFileSource::~ProxyFileSource() = default;

std::unique_ptr<AsyncRequest> ProxyFileSource::request(const Resource& resource, Callback callback) {
    std::size_t size = 0;
    auto result = DefaultFileSource::request(resource, [=, &size](Response response) {
        size = response.data != nullptr ? response.data->size() : 0;
        callback(response);
    });
    if (active) {
        requestCount++;
        transferredSize += size;
    }
    return result;
}

std::shared_ptr<FileSource> FileSource::createPlatformFileSource(const ResourceOptions& options) {
    auto fileSource = std::make_shared<ProxyFileSource>(
        options.cachePath(), options.assetPath(), options.supportsCacheOnlyRequests());
    fileSource->setAccessToken(options.accessToken());
    fileSource->setAPIBaseURL(options.baseURL());
    return fileSource;
}

// static
void ProxyFileSource::setTrackingActive(bool active_) {
    active = active_;
}

// static
bool ProxyFileSource::isTrackingActive() {
    return active;
}

// static
void ProxyFileSource::resetMetrics() {
    requestCount = 0;
    transferredSize = 0;
}

// static
size_t ProxyFileSource::getRequestCount() {
    return requestCount;
}

// static
size_t ProxyFileSource::getTransferredSize() {
    return transferredSize;
}

} // namespace mbgl
