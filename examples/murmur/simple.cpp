#include <cstdint>
#include <cstddef>
#include <iostream>

uint32_t murmur_32_scramble(uint32_t k) {
    k *= 0xcc9e2d51;
    k = (k << 15) | (k >> 17);
    k *= 0x1b873593;
    return k;
}

uint32_t murmur3_32(const uint8_t* key, size_t len, uint32_t seed) {
    uint32_t h = seed;
    uint32_t k;

    size_t nblocks = len / 4;
    for (size_t i = 0; i < nblocks; i++) {
        k = (uint32_t)key[0]
          | ((uint32_t)key[1] << 8)
          | ((uint32_t)key[2] << 16)
          | ((uint32_t)key[3] << 24);
        key += 4;

        k = murmur_32_scramble(k);
        h ^= k;
        h = (h << 13) | (h >> 19);
        h = h * 5 + 0xe6546b64;
    }

    // tail
    k = 0;
    switch (len & 3) {
        case 3: k ^= key[2] << 16;
        case 2: k ^= key[1] << 8;
        case 1: k ^= key[0];
                k = murmur_32_scramble(k);
                h ^= k;
    }

    // finalize
    h ^= len;
    h ^= h >> 16;
    h *= 0x85ebca6b;
    h ^= h >> 13;
    h *= 0xc2b2ae35;
    h ^= h >> 16;

    return h;
}

int main() {
    const char* str = "123456";
    size_t len = 6;
    uint32_t seed = 0xE2;
    uint32_t hash = murmur3_32((const uint8_t*)str, len, seed);
    std::cout << std::hex << hash << std::endl;  // Erwartet: c99d5e62
}
