#pragma once

/**
 * Include any class you need specificly, never include this class!!!
 */
namespace Cache {
    namespace Covers {
        class Cover;
        class CoverFirst;
        class MetadataFirst;

        const std::vector<std::string> SUPPORTED_COVERS = {"cover.jpg", "cover.png", "front.jpg", "front.png"};
    }
}