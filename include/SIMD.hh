#ifndef SIMD_HH
#define SIMD_HH
#ifdef _MSC_VER
// msvc
// hack: vscode defines _MSC_VER but use gcc/clang style of defining simd support (MMX, SSE, SSE2 macro)
#    if (_M_IX86_FP != 0 || defined __SSE__)
#        define __MMX__ 1
#        define __SSE__ 1
#        include <mmintrin.h>
#        include <xmmintrin.h>
#    endif
#    if (_M_IX86_FP == 2 || defined __SSE2__)
#        define __SSE2__ 1
#        include <emmintrin.h>
#    endif
#else
// gcc/clang
#    ifdef __MMX__
#        include <mmintrin.h>
#    endif
#    ifdef __SSE__
#        include <xmmintrin.h>
#    endif
#    ifdef __SSE2__
#        include <emmintrin.h>
#    endif
#endif
#ifdef __AVX__
#    include <immintrin.h>
#endif
#define _SIMD_LEVEL __MMX__ + __SSE__ + __SSE2__ + __AVX__
#include <array>

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
    }
} // namespace SIMD
#endif