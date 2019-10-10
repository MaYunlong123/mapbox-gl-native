#pragma once

#include <mbgl/storage/default_file_source.hpp>

namespace mbgl {

class ProxyFileSource : public DefaultFileSource {
public:
    ProxyFileSource(const std::string& cachePath, const std::string& assetPath, bool supportCacheOnlyRequests = true);
    ProxyFileSource(const std::string& cachePath,
                    std::unique_ptr<FileSource>&& assetFileSource,
                    bool supportCacheOnlyRequests = true);
    ~ProxyFileSource() override;

    std::unique_ptr<AsyncRequest> request(const Resource&, Callback) override;

    /**
     * @brief Starts/stops metrics tracking.
     */
    static void setTrackingActive(bool);
    /**
     * @brief Returns metrics tracking status.
     */
    static bool isTrackingActive();
    /**
     * @brief Resets the metrics.
     */
    static void resetMetrics();
    /**
     * @brief Returns the total amount of requests.
     *
     * @return size_t
     */
    static size_t getRequestCount();

    /**
     * @brief Returns the size of transferred data (in bytes).
     *
     * @return size_t
     */
    static size_t getTransferredSize();

private:
};

} // namespace mbgl
