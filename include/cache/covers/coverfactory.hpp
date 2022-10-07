#include "cache.hpp"

class Cache::Covers::CoverFactory {
public:
    /**
     * Returns Cover object based on index that can create cover images.
     * 
     * @param index See implementation, should match the order of config_dialog in plugin.cpp.
     */
    static Cache::Covers::Cover* getCoverAlgorithm(int index);
private:
    CoverFactory();
    ~CoverFactory();
};