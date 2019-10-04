#pragma once

#include <mbgl/actor/actor_ref.hpp>
#include <mbgl/storage/file_source.hpp>
#include <mbgl/util/optional.hpp>

namespace mbgl {

class ResourceTransform;

class OnlineFileSource : public FileSource {
public:
    OnlineFileSource();
    ~OnlineFileSource() override;

    std::unique_ptr<AsyncRequest> request(const Resource&, Callback) override;
    bool canRequest(const Resource&) const override;
    void pause() override;
    void resume() override;

    void setProperty(const std::string&, const mapbox::base::Value&) override;
    mapbox::base::Value getProperty(const std::string&) const override;

    void setResourceTransform(optional<ActorRef<ResourceTransform>>);

    // For testing only.
    void setOnlineStatus(bool);

private:
    class Impl;
    const std::unique_ptr<Impl> impl;
};

} // namespace mbgl
