#include "cache.hpp"

class Cache::Covers::CoverFactory {
public:
    static Cache::Covers::Cover* getCoverAlgorithm(int index);
private:
    CoverFactory();
    ~CoverFactory();
};