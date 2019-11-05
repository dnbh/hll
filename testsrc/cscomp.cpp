#include "cs_compress.h"


int main() {
    sketch::KWiseHasherSet<4> hf(1337);
    std::vector<double> init(1000, 1.);
    blaze::DynamicVector<double> init_1(1000, 1.);
    for(size_t i = 0; i < init.size(); ++i) init[i] = std::pow(i, 2);
    auto step1 = cs_compress(init, 100, hf);
    auto step1_1= cs_compress(init_1, 100, hf);
    auto step1_2 = cs_compress(init, 13, hf);
    auto step2 = cs_decompress(step1, hf, 1000);
    auto step2_1 = cs_decompress(step1_1, hf, 1000);
    // top_indices_from_compressed(const C &in, size_t newdim, size_t olddim, const KWiseHasherSet<4> &hf, unsigned k) 
    auto topind = top_indices_from_compressed(step1, 1000, 100, hf, 20);
    std::fprintf(stderr, "topind sizes: %zu, %zu\n", topind.first.size(), topind.second.size());
    for(const auto i: topind.first) {
        std::fprintf(stderr, "wooo %lf\n", double(i));
    }
    auto topind2 = top_indices_from_compressed(step1_2, 1000, 13, hf, 20);
    std::fprintf(stderr, "topind2 sizes: %zu, %zu\n", topind2.first.size(), topind2.second.size());
    for(const auto i: topind2.first)
        std::fprintf(stderr, "wooo %lf\n", double(i));
}