#include "sketch/count_eq.h"

template<typename T>
int do_main() {
    std::vector<T> rhs(100), lhs(100);
    std::iota(rhs.begin(), rhs.end(), 0u);
    std::iota(lhs.begin(), lhs.end(), 0u);
    lhs[17] = 1;
    auto nm = sketch::eq::count_eq(lhs.data(), rhs.data(), 100);
    std::fprintf(stderr, "nmatched: %zu\n", nm);
    if(sizeof(T) == 1) {
        std::memset(&rhs[0], 0, 100);
        std::memset(&lhs[0], 0, 100);
        auto nm = sketch::eq::count_eq_nibbles((const uint8_t *)lhs.data(), (const uint8_t *)rhs.data(), 200);
        std::fprintf(stderr, "nm: %zu\n", nm);
        assert(nm == 200);
    }
    double tt = 0., t2 = 0.;
    const size_t n = 1000000;
    wy::WyRand<uint64_t> rng(13);
    rhs.resize(n);
    lhs.resize(n);
    for(auto &i: rhs) i = rng();
    for(auto &i: lhs) i = rng();
    size_t nm2 = 0, nmb = 0;
    constexpr size_t NPER = sizeof(T) / sizeof(char);
    for(size_t i = 0; i < 1; ++i) {
        auto t = std::chrono::high_resolution_clock::now();
        nm2 += sketch::eq::count_eq_nibbles((const uint8_t *)lhs.data(), (const uint8_t *)rhs.data(), rhs.size() * NPER * 2);
        auto e = std::chrono::high_resolution_clock::now();
        tt += std::chrono::duration<double, std::milli>(e - t).count();
        t = std::chrono::high_resolution_clock::now();
        nmb += sketch::eq::count_eq_bytes((const uint8_t *)lhs.data(), (const uint8_t *)rhs.data(), rhs.size() * NPER);
        e = std::chrono::high_resolution_clock::now();
        t2 += std::chrono::duration<double, std::milli>(e - t).count();
    }
    std::fprintf(stderr, "%gms per %zu nibbles, %zu (%%%g) match\n", tt, rhs.size() * NPER * 2, nm2, double(nm2) / n / NPER / 2 * 100.);
    std::fprintf(stderr, "%gms per %zu bytes, %zu (%%%g) match\n", t2, rhs.size() * NPER, nmb, double(nmb) / n / NPER * 100.);
    return nm != 99;
}

int main() {
    return do_main<uint16_t>() || do_main<uint32_t>() || do_main<uint64_t>() || do_main<uint8_t>();
}
