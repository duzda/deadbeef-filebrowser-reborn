#include "coverfactory.hpp"

#include "coverfirst.hpp"
#include "metadatafirst.hpp"

using namespace Cache::Covers;

Cover* CoverFactory::getCoverAlgorithm(int index) {
    Cover* cover;

    if (index == 0) {
        cover = new CoverFirst();
    } else if (index == 1) {
        cover = new MetadataFirst();
    } else {
        cover = new CoverFirst();
    }

    return cover;
}