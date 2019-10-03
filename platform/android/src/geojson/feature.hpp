#pragma once

#include <mbgl/util/feature.hpp>

#include <jni/jni.hpp>

namespace mbgl {
namespace android {
namespace geojson {

class Feature {
public:
    static constexpr auto Name() { return "com/mapbox/geojson/Feature"; };

    static mbgl::FeatureGeoJSON convert(jni::JNIEnv&, const jni::Object<Feature>&);
    static jni::Local<jni::Array<jni::Object<Feature>>> convert(jni::JNIEnv&, const std::vector<mbgl::Feature>&);
    static jni::Local<jni::Array<jni::Object<Feature>>> convert(jni::JNIEnv&, const std::vector<mbgl::FeatureGeoJSON>&);

    static void registerNative(jni::JNIEnv&);
};

} // namespace geojson
} // namespace android
} // namespace mbgl