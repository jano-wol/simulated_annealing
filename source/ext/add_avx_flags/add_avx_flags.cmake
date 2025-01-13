macro(ADD_AVX_FLAGS)
    set(AVX_FLAGS)
    set(OLD_LOG_LEVEL "${CMAKE_MESSAGE_LOG_LEVEL}")

    include(CheckCXXSourceRuns)
    set(CMAKE_REQUIRED_FLAGS)

    # Check for AVX
    if (MSVC)
        if (NOT MSVC_VERSION LESS 1600)
            set(CMAKE_REQUIRED_FLAGS "/arch:AVX")
        endif ()
    else ()
        set(CMAKE_REQUIRED_FLAGS "-mavx")
    endif ()
    set(CMAKE_MESSAGE_LOG_LEVEL "WARNING")
    check_cxx_source_runs("
        #include <immintrin.h>
        int main()
        {
          __m256 a, b, c;
          const float src[8] = { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f };
          float dst[8];
          a = _mm256_loadu_ps( src );
          b = _mm256_loadu_ps( src );
          c = _mm256_add_ps( a, b );
          _mm256_storeu_ps( dst, c );
          for( int i = 0; i < 8; i++ ){
            if( ( src[i] + src[i] ) != dst[i] ){
              return -1;
            }
          }
          return 0;
        }"
        HAVE_AVX)
    set(CMAKE_MESSAGE_LOG_LEVEL "${OLD_LOG_LEVEL}")  
    if (HAVE_AVX)
        message(STATUS "AVX support set.")
        if (MSCV)
            list(APPEND AVX_FLAGS "/arch:AVX")
        else()
            list(APPEND AVX_FLAGS "-mavx")
        endif()
    else()
        message(STATUS "AVX support not available.")        
    endif()

    # Check for AVX2
    if (MSVC)
        if (NOT MSVC_VERSION LESS 1800)
            set(CMAKE_REQUIRED_FLAGS "/arch:AVX2")
        endif ()
    else ()
        set(CMAKE_REQUIRED_FLAGS "-mavx2")
    endif ()
    set(CMAKE_MESSAGE_LOG_LEVEL "WARNING")
    check_cxx_source_runs("
        #include <immintrin.h>
        int main()
        {
          __m256i a, b, c;
          const int src[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };
          int dst[8];
          a =  _mm256_loadu_si256( (__m256i*)src );
          b =  _mm256_loadu_si256( (__m256i*)src );
          c = _mm256_add_epi32( a, b );
          _mm256_storeu_si256( (__m256i*)dst, c );
          for( int i = 0; i < 8; i++ ){
            if( ( src[i] + src[i] ) != dst[i] ){
              return -1;
            }
          }
          return 0;
        }"
        HAVE_AVX2)
    set(CMAKE_MESSAGE_LOG_LEVEL "${OLD_LOG_LEVEL}")      
    if (HAVE_AVX2)
        message(STATUS "AVX2 support set.")
        if (MSCV)
            list(APPEND AVX_FLAGS "/arch:AVX2")
        else()
            list(APPEND AVX_FLAGS "-mavx2")
        endif()
    else()
        message(STATUS "AVX2 support not available.")        
    endif()

    # Check for AVX512
    if (MSVC)
        if (NOT MSVC_VERSION LESS 1900)
            set(CMAKE_REQUIRED_FLAGS "/arch:AVX512")
        endif ()
    else ()
        set(CMAKE_REQUIRED_FLAGS "-mavx512f")
    endif ()
    set(CMAKE_MESSAGE_LOG_LEVEL "WARNING")
    check_cxx_source_runs("
        #include <immintrin.h>
        int main()
        {
          __m512 a, b, c;
          const float src[16] = { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f };
          float dst[16];
          a = _mm512_loadu_ps( src );
          b = _mm512_loadu_ps( src );
          c = _mm512_add_ps( a, b );
          _mm512_storeu_ps( dst, c );
          for( int i = 0; i < 16; i++ ){
            if( ( src[i] + src[i] ) != dst[i] ){
              return -1;
            }
          }
          return 0;
        }"
        HAVE_AVX512)
    set(CMAKE_MESSAGE_LOG_LEVEL "${OLD_LOG_LEVEL}")    
    if (HAVE_AVX512)
        message(STATUS "AVX512 support set.")
        if (MSCV)
            list(APPEND AVX_FLAGS "/arch:AVX512")
        else()
            list(APPEND AVX_FLAGS "-mavx512f")
        endif()
    else()
        message(STATUS "AVX512 support not available.")
    endif()
    if (AVX_FLAGS)
        add_compile_options(${AVX_FLAGS})
    endif()
endmacro(ADD_AVX_FLAGS)
