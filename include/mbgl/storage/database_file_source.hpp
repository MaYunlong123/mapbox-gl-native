#pragma once

#include <mbgl/actor/actor_ref.hpp>
#include <mbgl/storage/file_source.hpp>
#include <mbgl/storage/offline.hpp>
#include <mbgl/util/expected.hpp>
#include <mbgl/util/optional.hpp>

namespace mbgl {

namespace util {
template <typename T>
class Thread;
} // namespace util

class ResourceOptions;
using PathChangeCallback = std::function<void()>;

class DatabaseFileSource : public FileSource {
public:
    explicit DatabaseFileSource(const ResourceOptions& options);
    ~DatabaseFileSource() override;

    // FileSource overrides
    std::unique_ptr<AsyncRequest> request(const Resource&, Callback) override;
    void forward(const Resource&, const Response&) override;
    bool canRequest(const Resource&) const override;

    // Methods common to ambient cache and offline database
    void setResourceCachePath(const std::string&, optional<ActorRef<PathChangeCallback>>);
    void resetDatabase(std::function<void(std::exception_ptr)>);

    // Ambient cache
    void put(const Resource&, const Response&);
    void invalidateAmbientCache(std::function<void(std::exception_ptr)>);
    void clearAmbientCache(std::function<void(std::exception_ptr)>);
    void setMaximumAmbientCacheSize(uint64_t size, std::function<void(std::exception_ptr)> callback);

    // Offline
    void listOfflineRegions(std::function<void(expected<OfflineRegions, std::exception_ptr>)>);
    void createOfflineRegion(const OfflineRegionDefinition& definition,
                             const OfflineRegionMetadata& metadata,
                             std::function<void(expected<OfflineRegion, std::exception_ptr>)>);
    void updateOfflineMetadata(const int64_t regionID,
                               const OfflineRegionMetadata& metadata,
                               std::function<void(expected<OfflineRegionMetadata, std::exception_ptr>)>);
    void setOfflineRegionObserver(OfflineRegion&, std::unique_ptr<OfflineRegionObserver>);
    void setOfflineRegionDownloadState(OfflineRegion&, OfflineRegionDownloadState);
    void getOfflineRegionStatus(OfflineRegion&,
                                std::function<void(expected<OfflineRegionStatus, std::exception_ptr>)>) const;
    void mergeOfflineRegions(const std::string& sideDatabasePath,
                             std::function<void(expected<OfflineRegions, std::exception_ptr>)>);
    void deleteOfflineRegion(OfflineRegion, std::function<void(std::exception_ptr)>);
    void invalidateOfflineRegion(OfflineRegion&, std::function<void(std::exception_ptr)>);
    void setOfflineMapboxTileCountLimit(uint64_t) const;

private:
    class Impl;
    const std::unique_ptr<util::Thread<Impl>> impl;
};

} // namespace mbgl
