#include "coverfactory.hpp"

#include "coverfirst.hpp"
#include "metadatafirst.hpp"
#include "metadataonly.hpp"

using namespace Cache::Covers;

Cover* CoverFactory::getCoverAlgorithm(int index) {
    Cover* cover;

    if (index == 0) {
        cover = new CoverFirst();
    } else if (index == 1) {
        cover = new MetadataFirst();
    } else if (index == 2) {
        cover = new MetadataOnly();
    } else {
        cover = new CoverFirst();
    }

    return cover;
}