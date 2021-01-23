#ifndef SIMDFLOAT_HH
#define SIMDFLOAT_HH
#include "SIMD.hh"

namespace SIMD
{
    class Float4
    {
    public:
        explicit Float4(float pArray[]) //NOLINT(cppcoreguidelines-avoid-c-arrays, hicpp-avoid-c-arrays, modernize-avoid-c-arrays)
        {
#if _SIMD_LEVEL > 1
            mValues = _mm_load_ps(pArray);
#else
            mValues = pArray;
#endif
        }

        explicit Float4(std::array<float, 4> pArray)
        {
#if _SIMD_LEVEL > 1
            mValues = _mm_load_ps(pArray.data());
#else
            mValues = pArray;
#endif
        }

        float operator[](int pIndex) const
        {
            return mValues[pIndex];
        }

        const float &operator[](int pIndex)
        {
            return mValues[pIndex]; //NOLINT
        }

    private:
#if _SIMD_LEVEL > 1
        __m128 mValues;
#else
        std::array<float, 4> mValues{};
#endif
    };

    class Float8
    {
    public:
        explicit Float8(float pArray[]) //NOLINT(cppcoreguidelines-avoid-c-arrays, hicpp-avoid-c-arrays, modernize-avoid-c-arrays)
        {
#if _SIMD_LEVEL > 3
            mValues = _mm256_load_ps(pArray);
#else
#    if _SIMD_LEVEL > 1
            mValues1 = _mm_load_ps(pArray);
            mValues2 = _mm_load_ps(pArray + 4);
#    else
            mValues = pArray;
#    endif
#endif
        }

        explicit Float8(std::array<float, 8> pArray)
        {
#if _SIMD_LEVEL > 3
            mValues = _mm256_load_ps(pArray.data());
#else
#    if _SIMD_LEVEL > 1
            mValues1 = _mm_load_ps(pArray.data());
            mValues2 = _mm_load_ps(pArray.data() + 4);
#    else
            mValues = pArray;
#    endif
#endif
        }

        float operator[](int pIndex) const
        {
#if _SIMD_LEVEL > 1 && _SIMD_LEVEL < 4
            if (pIndex > 3)
            {
                return mValues2[pIndex - 4];
            }
            return mValues1[pIndex];
#else
            return mValues[pIndex];
#endif
        }

        const float &operator[](int pIndex)
        {
#if _SIMD_LEVEL > 1 && _SIMD_LEVEL < 4
            if (pIndex > 3)
            {
                return mValues2[pIndex - 4]; //NOLINT
            }
            return mValues1[pIndex]; //NOLINT
#else
            return mValues[pIndex]; //NOLINT
#endif
        }

    private:
#if _SIMD_LEVEL > 3
        __m256 mValues;
#else
#    if _SIMD_LEVEL > 1
        __m128 mValues1;
        __m128 mValues2;
#    else
        std::array<float, 8> mValues{};
#    endif
#endif
    };
} // namespace SIMD
#endif
