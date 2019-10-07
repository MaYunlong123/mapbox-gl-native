#include "node_map.hpp"

#include <mbgl/storage/file_source_manager.hpp>
#include <mbgl/storage/resource_options.hpp>

namespace mbgl {

class NodeFileSourceManagerImpl final : public FileSourceManager {
public:
    NodeFileSourceManagerImpl() {
        registerFileSourceFactory(FileSourceType::ResourceLoader, [](const ResourceOptions& options) {
            return std::make_unique<node_mbgl::NodeFileSource>(
                reinterpret_cast<node_mbgl::NodeMap*>(options.platformContext()));
        });
    }
};

FileSourceManager* FileSourceManager::get() noexcept {
    static NodeFileSourceManagerImpl instance;
    return &instance;
}

} // namespace mbgl
