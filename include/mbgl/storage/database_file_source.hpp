#pragma once

#include <mbgl/actor/actor_ref.hpp>
#include <mbgl/storage/file_source.hpp>
#include <mbgl/storage/offline.hpp>
#include <mbgl/util/expected.hpp>
#include <mbgl/util/optional.hpp>

namespace mbgl {

class ResourceOptions;
using PathChangeCallback = std::function<void()>;

// TODO: Split DatabaseFileSource into Ambient cache and Database interfaces.
class DatabaseFileSource : public FileSource {
public:
    explicit DatabaseFileSource(const ResourceOptions& options);
    ~DatabaseFileSource() override;

    // FileSource overrides
    std::unique_ptr<AsyncRequest> request(const Resource&, Callback) override;
    void forward(const Resource&, const Response&) override;
    bool canRequest(const Resource&) const override;

    // Methods common to ambient cache and offline database
    virtual void setResourceCachePath(const std::string&, optional<ActorRef<PathChangeCallback>>);
    virtual void resetDatabase(std::function<void(std::exception_ptr)>);

    // Ambient cache
    virtual void put(const Resource&, const Response&);
    virtual void invalidateAmbientCache(std::function<void(std::exception_ptr)>);
    virtual void clearAmbientCache(std::function<void(std::exception_ptr)>);
    virtual void setMaximumAmbientCacheSize(uint64_t size, std::function<void(std::exception_ptr)> callback);

    // Offline
    virtual void listOfflineRegions(std::function<void(expected<OfflineRegions, std::exception_ptr>)>);
    virtual void createOfflineRegion(const OfflineRegionDefinition& definition,
                                     const OfflineRegionMetadata& metadata,
                                     std::function<void(expected<OfflineRegion, std::exception_ptr>)>);
    virtual void updateOfflineMetadata(const int64_t regionID,
                                       const OfflineRegionMetadata& metadata,
                                       std::function<void(expected<OfflineRegionMetadata, std::exception_ptr>)>);
    virtual void setOfflineRegionObserver(OfflineRegion&, std::unique_ptr<OfflineRegionObserver>);
    virtual void setOfflineRegionDownloadState(OfflineRegion&, OfflineRegionDownloadState);
    virtual void getOfflineRegionStatus(OfflineRegion&,
                                        std::function<void(expected<OfflineRegionStatus, std::exception_ptr>)>) const;
    virtual void mergeOfflineRegions(const std::string& sideDatabasePath,
                                     std::function<void(expected<OfflineRegions, std::exception_ptr>)>);
    virtual void deleteOfflineRegion(OfflineRegion, std::function<void(std::exception_ptr)>);
    virtual void invalidateOfflineRegion(OfflineRegion&, std::function<void(std::exception_ptr)>);
    virtual void setOfflineMapboxTileCountLimit(uint64_t) const;

private:
    class Impl;
    const std::unique_ptr<Impl> impl;
};

} // namespace mbgl
