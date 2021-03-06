#ifndef SIMDFLOAT_HH
#define SIMDFLOAT_HH
#include "SIMD.hh"

#   include <cstdint>

namespace SIMD
{
    class BaseFloat128
    {
    public:
        BaseFloat128() = default;

        explicit BaseFloat128(int8_t *pBytes)
        {
#if SIMD_LEVEL > 1
            mValues = _mm_load_ps(reinterpret_cast<float *>(pBytes));
#else
            std::memcpy(mValues.data(), pBytes, sizeof(int8_t) * 16);
#endif
        }

    protected:
#if SIMD_LEVEL > 1
        __m128 mValues;
#else
        std::array<float, 4> mValues{};
#endif
    };

    class BaseFloat256
    {
    public:
        BaseFloat256() = default;

        explicit BaseFloat256(int8_t *pBytes)
        {
if SIMD_LEVEL > 3
            mValues = _mm128_load_ps(reinterpret_cast<float *>(pBytes));
#else
#    if SIMD_LEVEL > 1
            mValues1 = _mm_load_ps(reinterpret_cast<float *>(pBytes));
            mValues2 = _mm_load_ps(reinterpret_cast<float *>(pBytes) + 4);
#    else
            std::memcpy(mValues.data(), pBytes, sizeof(int8_t) * 32);
#    endif
#endif
        }
    protected:
#if SIMD_LEVEL > 3
        __m256 mValues;
#else
#    if SIMD_LEVEL > 1
        __m128 mValues1;
        __m128 mValues2;
#    else
        std::array<float, 8> mValues{};
#    endif
    };

    class Float4 : public BaseFloat128
    {
    public:
        explicit Float4(std::array<float, 4> pArray) : BaseFloat128(reinterpret_cast<int8_t *>(pArray.data())){}

#if SIMD_LEVEL > 1
        explicit Float4(__m128 pValue) : BaseFloat128()
        {
            mValues = pValue;
        }
#endif

#if SIMD_USE_EXCEPTION != 0
        [[nodiscard]] bool HasZero() const
        {
            for (auto i = 0; i < 4; i++)
            {
                if (mValues[i] == 0.f)
                {
                    return true;
                }
            }
            return false;
        }
#endif

        float operator[](int pIndex) const
        {
#if SIMD_USE_EXCEPTION != 0
            if (pIndex > 3 || pIndex < 0)
            {
                throw std::exception("SIMD::Float4::operator[](int): out of bound access");
            }
#endif
            return mValues[pIndex];
        }

        const float &operator[](int pIndex)
        {
#if SIMD_USE_EXCEPTION != 0
            if (pIndex > 3 || pIndex < 0)
            {
                throw std::exception("SIMD::Float4::operator[](int): out of bound access");
            }
#endif
            return mValues[pIndex]; //NOLINT
        }

        Float4 &operator+=(const Float4 pOther)
        {
#if SIMD_LEVEL > 1
            mValues = _mm_add_ps(mValues, pOther.mValues);
#else
            for (auto i = 0; i < 4; i++)
            {
                mValues[i] += pOther.mValues[i];
            }
#endif
            return *this;
        }

        Float4 &operator+=(const float pScala)
        {
            return *this += Float4({pScala, pScala, pScala, pScala});
        }

        Float4 operator+(const Float4 pOther)
        {
#if SIMD_LEVEL > 1
            auto val = _mm_add_ps(mValues, pOther.mValues);
            return Float4(val);
#else
            auto val = mValues;
            for (auto i = 0; i < val; i++)
            {
                val[i] += pOther.mValues[i];
            }
            return Float4(val);
#endif
        }

        Float4 operator+(const float pScala)
        {
            return *this + Float4({pScala, pScala, pScala, pScala});
        }

        Float4 &operator-=(const Float4 pOther)
        {
#if SIMD_LEVEL > 1
            mValues = _mm_sub_ps(mValues, pOther.mValues);
#else
            for (auto i = 0; i < 4; i++)
            {
                mValues[i] -= pOther.mValues[i];
            }
#endif
            return *this;
        }

        Float4 &operator-=(const float pScala)
        {
            return *this -= Float4({pScala, pScala, pScala, pScala});
        }

        Float4 operator-(const Float4 pOther)
        {
#if SIMD_LEVEL > 1
            auto val = _mm_sub_ps(mValues, pOther.mValues);
            return Float4(val);
#else
            auto val = mValues;
            for (auto i = 0; i < val; i++)
            {
                val[i] -= pOther.mValues[i];
            }
            return Float4(val);
#endif
        }

        Float4 operator-(const float pScala)
        {
            return *this - Float4({pScala, pScala, pScala, pScala});
        }

        Float4 &operator*=(const Float4 pOther)
        {
#if SIMD_LEVEL > 1
            mValues = _mm_mul_ps(mValues, pOther.mValues);
#else
            for (auto i = 0; i < 4; i++)
            {
                mValues[i] *= pOther.mValues[i];
            }
#endif
            return *this;
        }

        Float4 &operator*=(const float pScala)
        {
            return *this *= Float4({pScala, pScala, pScala, pScala});
        }

        Float4 operator*(const Float4 pOther)
        {
#if SIMD_LEVEL > 1
            auto val = _mm_mul_ps(mValues, pOther.mValues);
            return Float4(val);
#else
            auto val = mValues;
            for (auto i = 0; i < val; i++)
            {
                val[i] *= pOther.mValues[i];
            }
            return Float4(val);
#endif
        }

        Float4 operator*(const float pScala)
        {
            return *this * Float4({pScala, pScala, pScala, pScala});
        }

        Float4 &operator/=(const Float4 pOther)
        {
#if SIMD_USE_EXCEPTION != 0
            if (pOther.HasZero())
            {
                throw std::exception("SIMD::Float4::operator/=(Float4): divided by zero");
            }
#endif
#if SIMD_LEVEL > 1
            mValues = _mm_div_ps(mValues, pOther.mValues);
#else
            for (auto i = 0; i < 4; i++)
            {
                mValues[i] /= pOther.mValues[i];
            }
#endif
            return *this;
        }

        Float4 &operator/=(const float pScala)
        {
            return *this /= Float4({pScala, pScala, pScala, pScala});
        }

        Float4 operator/(const Float4 pOther)
        {
#if SIMD_USE_EXCEPTION != 0
            if (pOther.HasZero())
            {
                throw std::exception("SIMD::Float4::operator/(Float4): divided by zero");
            }
#endif
#if SIMD_LEVEL > 1
            auto val = _mm_div_ps(mValues, pOther.mValues);
            return Float4(val);
#else
            auto val = mValues;
            for (auto i = 0; i < val; i++)
            {
                val[i] /= pOther.mValues[i];
            }
            return Float4(val);
#endif
        }

        Float4 operator/(const float pScala)
        {
            return *this / Float4({pScala, pScala, pScala, pScala});
        }
    };

    class Float8 : public BaseFloat256
    {
    public:
        explicit Float8(std::array<float, 8> pArray) : BaseFloat256(reinterpret_cast<int8_t *>(pArray.data()))
        {
        }

        float operator[](int pIndex) const
        {
#if SIMD_USE_EXCEPTION != 0
            if (pIndex > 7 || pIndex < 0)
            {
                throw std::exception("SIMD::Float8::operator[](int): out of bound access");
            }
#endif

#if SIMD_LEVEL > 1 && SIMD_LEVEL < 4
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
#if SIMD_USE_EXCEPTION != 0
            if (pIndex > 7 || pIndex < 0)
            {
                throw std::exception("SIMD::Float8::operator[](int): out of bound access");
            }
#endif

#if SIMD_LEVEL > 1 && SIMD_LEVEL < 4
            if (pIndex > 3)
            {
                return mValues2[pIndex - 4]; //NOLINT
            }
            return mValues1[pIndex]; //NOLINT
#else
            return mValues[pIndex]; //NOLINT
#endif
        }
    };
} // namespace SIMD
#endif
