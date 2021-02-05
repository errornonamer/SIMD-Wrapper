#ifndef SIMDFLOAT_HH
#    define SIMDFLOAT_HH
#    include "SIMD.hh"

#    if SIMD_USE_EXCEPTION != 0
#        include <exception>
#    endif

namespace SIMD
{
    class Float4
    {
    public:
        explicit Float4(std::array<float, 4> pArray)
        {
#    if SIMD_LEVEL > 1
            mValues = _mm_load_ps(pArray.data());
#    else
            mValues = pArray;
#    endif
        }

        float operator[](int pIndex) const
        {
#    if SIMD_USE_EXCEPTION != 0
            if (pIndex > 3 || pIndex < 0)
            {
                throw std::exception("SIMD::Float4::operator[]: out of bound access");
            }
#    endif
            return mValues[pIndex];
        }

        const float &operator[](int pIndex)
        {
#    if SIMD_USE_EXCEPTION != 0
            if (pIndex > 3 || pIndex < 0)
            {
                throw std::exception("SIMD::Float4::operator[]: out of bound access");
            }
#    endif
            return mValues[pIndex]; //NOLINT
        }

    private:
#        if SIMD_LEVEL > 1
        __m128 mValues;
#        else
        std::array<float, 4> mValues{};
#        endif
    };

    class Float8
    {
    public:
        explicit Float8(std::array<float, 8> pArray)
        {
#        if SIMD_LEVEL > 3
            mValues = _mm256_load_ps(pArray.data());
#        else
#            if SIMD_LEVEL > 1
            mValues1 = _mm_load_ps(pArray.data());
            mValues2 = _mm_load_ps(pArray.data() + 4);
#            else
            mValues = pArray;
#            endif
#        endif
        }

        float operator[](int pIndex) const
        {
#    if SIMD_USE_EXCEPTION != 0
            if (pIndex > 7 || pIndex < 0)
            {
                throw std::exception("SIMD::Float8::operator[]: out of bound access");
            }
#    endif

#        if SIMD_LEVEL > 1 && SIMD_LEVEL < 4
            if (pIndex > 3)
            {
                return mValues2[pIndex - 4];
            }
            return mValues1[pIndex];
#        else
            return mValues[pIndex];
#        endif
        }

        const float &operator[](int pIndex)
        {
#    if SIMD_USE_EXCEPTION != 0
            if (pIndex > 7 || pIndex < 0)
            {
                throw std::exception("SIMD::Float8::operator[]: out of bound access");
            }
#    endif
            
#        if SIMD_LEVEL > 1 && SIMD_LEVEL < 4
            if (pIndex > 3)
            {
                return mValues2[pIndex - 4]; //NOLINT
            }
            return mValues1[pIndex]; //NOLINT
#        else
            return mValues[pIndex]; //NOLINT
#        endif
        }

    private:
#        if SIMD_LEVEL > 3
        __m256 mValues;
#        else
#            if SIMD_LEVEL > 1
        __m128 mValues1;
        __m128 mValues2;
#            else
        std::array<float, 8> mValues{};
#            endif
#        endif
    };
} // namespace SIMD
#    endif
