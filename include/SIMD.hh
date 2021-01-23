#ifndef SIMD_HH
#    define SIMD_HH
#    ifdef _MSC_VER
         // msvc
         // hack: vscode defines _MSC_VER but use gcc/clang style of defining simd support (MMX, SSE, SSE2 macro)
#        if (_M_IX86_FP != 0 || defined __SSE__)
#            define __MMX__ 1
#            define __SSE__ 1
#            include <mmintrin.h>
#            include <xmmintrin.h>
#        endif
#        if (_M_IX86_FP == 2 || defined __SSE2__)
#            define __SSE2__ 1
#            include <emmintrin.h>
#        endif
#    else
         // gcc/clang
#        ifdef __MMX__
#            include <mmintrin.h>
#        endif
#        ifdef __SSE__
#            include <xmmintrin.h>
#        endif
#        ifdef __SSE2__
#            include <emmintrin.h>
#        endif
#    endif
#    ifdef __AVX__
#        include <immintrin.h>
#    endif
#    define _SIMD_LEVEL __MMX__ + __SSE__ + __SSE2__ + __AVX__
#    include <array>

namespace SIMD
{   
    class Float4
    {
    public:
    private:
#    if _SIMD_LEVEL > 2
        __m128 mValues;
#    else
        std::array<float, 4> mValues;
#    endif
    }
}
#endif