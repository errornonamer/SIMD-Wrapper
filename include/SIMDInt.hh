#ifndef SIMDINT_HH
#define SIMDINT_HH
#include "SIMD.hh"

#include <cstdint>

namespace SIMD
{
    class BaseInt128
    {
    public:
        explicit BaseInt128(int8_t *pArray)
        {
#if _SIMD_LEVEL > 2
            mValues = _mm_castps_si128(_mm_load_ps(reinterpret_cast<float *>(pArray)));
#else
            std::memcpy(mValues.data(), pArray, sizeof(int8_t) * 16);
#endif
        }

    protected:
#if _SIMD_LEVEL > 2
        __m128i mValues;
#else
        std::array<int8_t, 16> mValues{};
#endif
    };

    class BaseInt256
    {
    public:
        explicit BaseInt256(int8_t *pArray)
        {
#if _SIMD_LEVEL > 3
            mValues = _mm256_castps_si256(_mm256_load_ps(reinterpret_cast<float *>(pArray)));
#else
#    if _SIMD_LEVEL > 2
            mValues1 = _mm_castps_si128(_mm_load_ps(reinterpret_cast<float *>(pArray)));
            mValues2 = _mm_castps_si128(_mm_load_ps(reinterpret_cast<float *>(pArray + 16)));
#    else
            std::memcpy(mValues.data(), pArray, sizeof(int8_t) * 32);
#    endif
#endif
        }

    protected:
#if _SIMD_LEVEL > 3
        __m256i mValues;
#else
#    if _SIMD_LEVEL > 2
        __m128i mValues1;
        __m128i mValues2;
#    else
        std::array<int8_t, 32> mValues{};
#    endif
#endif
    };
} // namespace SIMD
#endif
