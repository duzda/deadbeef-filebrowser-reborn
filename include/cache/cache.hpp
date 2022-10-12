#pragma once

#include <vector>
#include <string>

/**
 * Include any class you need specificly, never include this class!!!
 */
namespace Cache {
    namespace Covers {
        class CoverFactory;
        class Cover;
        class CoverFirst;
        class MetadataFirst;
        class MetadataOnly;

        const std::vector<std::string> SUPPORTED_COVERS = {"cover.jpg", "cover.png", "front.jpg", "front.png"};
    }
}