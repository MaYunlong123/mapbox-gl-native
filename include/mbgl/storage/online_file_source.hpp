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

    // TODO: Move set/get APIs under generic prop methods:
    // - void  FileSource::setProperty(key, value)
    // - value FileSource::getProperty(key)
    void setAPIBaseURL(const std::string&);
    std::string getAPIBaseURL() const;

    void setAccessToken(const std::string&);
    std::string getAccessToken() const;

    void setMaximumConcurrentRequests(uint32_t);
    uint32_t getMaximumConcurrentRequests() const;

    void setResourceTransform(optional<ActorRef<ResourceTransform>>);

    // For testing only.
    void setOnlineStatus(bool);

private:
    class Impl;
    const std::unique_ptr<Impl> impl;
};

} // namespace mbgl
