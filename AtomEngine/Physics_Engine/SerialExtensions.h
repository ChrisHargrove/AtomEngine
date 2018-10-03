#pragma once

#include <CEREAL/cereal.hpp>
#include <CEREAL/archives/xml.hpp>
#include <GLM/glm.hpp>
#include <GLM/gtc/quaternion.hpp>

namespace glm {

    template<class Archive>
    void serialize(Archive& archive, glm::vec2& v) {
        archive(cereal::make_nvp("X", v.x), cereal::make_nvp("Y", v.y));
    }

    template<class Archive>
    void serialize(Archive& archive, glm::vec3& v) {
        archive(cereal::make_nvp("X", v.x), cereal::make_nvp("Y", v.y), cereal::make_nvp("Z", v.z));
    }

    template<class Archive>
    void serialize(Archive& archive, glm::vec4& v) {
        archive(cereal::make_nvp("X", v.x), cereal::make_nvp("Y", v.y), cereal::make_nvp("Z", v.z), cereal::make_nvp("W", v.w));
    }

    template<class Archive>
    void serialize(Archive& archive, glm::quat& q){
        archive(cereal::make_nvp("X", q.x), cereal::make_nvp("Y", q.y), cereal::make_nvp("Z", q.z), cereal::make_nvp("W", q.w));
    }

}