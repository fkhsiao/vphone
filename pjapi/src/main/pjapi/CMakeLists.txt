cmake_minimum_required(VERSION 3.4.1)

# Name your project here
project(PJAPI)

# Our source code directories
set (pj_dir ${CMAKE_CURRENT_SOURCE_DIR}/../pjproject)
set (pj_build_dir ${CMAKE_CURRENT_SOURCE_DIR}/../pjproject)

# Resulting pjproject static libraries will go here
set (generated_libs_dir ${CMAKE_CURRENT_SOURCE_DIR}/../../../build/generated/pjproject)
set (pjinstalled_libs_dir ${generated_libs_dir}/usr/local/lib)
set (pjinstalled_incs_dir ${generated_libs_dir}/usr/local/include)

set (NDK_TOOLCHAIN_VERSION 4.9)
set (APP_PLATFORM android-26)

# we create pjproject generated directories
file(MAKE_DIRECTORY ${generated_libs_dir})
file(MAKE_DIRECTORY ${generated_libs_dir}/usr)
file(MAKE_DIRECTORY ${generated_libs_dir}/usr/local)
file(MAKE_DIRECTORY ${generated_libs_dir}/usr/local/lib)

# Resulting pjApi.so will go here
set (final_output_dir ${CMAKE_CURRENT_SOURCE_DIR}/../../../../app/src/main/jniLibs/armeabi)
# we create the final output dir for pjapi.so
file(MAKE_DIRECTORY ${final_output_dir})

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set (lib_type 'debug')
else ()
    set (lib_type 'release')
endif ()

# Find external libs to link
find_library(log_lib log)
find_library(android_lib android)


# Define the pjapi library files to be compiled
add_library (pjapi SHARED   pjApi.c
             )

# Set include directories for pjapi library
# we use the include files directly from the source -- we could use the generated ones,
# but if we did so we would also have to include the include files in the BYPRODUCTS lists
target_include_directories ( pjapi PUBLIC
                 /include
                 ${pj_dir}/pjsip/include
                 ${pj_dir}/pjlib-util/include
                 ${pj_dir}/pjmedia/include
                 ${pj_dir}/pjlib/include
                 ${pj_dir}/pjnath/include
                 ${pj_dir}/third_party/g7221/common
                 ${pj_dir}/third_party/resample/include
                 ${pj_dir}/third_party/gsm/inc
                 ${pj_dir}/third_party/ilbc
                 ${pj_dir}/third_party/speex/include
                 ${pj_dir}/third_party/srtp/include
             )

add_custom_target ( pjproject
           COMMAND ${CMAKE_COMMAND} -E env "ANDROID_NDK_ROOT=${ANDROID_NDK}"
               "TARGET_ABI=${ANDROID_ABI}"
               "APP_PLATFORM=${APP_PLATFORM}" ./configure-android --use-ndk-cflags
           COMMAND make dep
           COMMAND make
           WORKING_DIRECTORY ${pj_dir}
                   )

add_custom_target( pjinstall
                    COMMAND ${CMAKE_COMMAND} -E env "DESTDIR=${generated_libs_dir}" make install > ${generated_libs_dir}/install_log.txt
                    WORKING_DIRECTORY ${pj_dir}
                    BYPRODUCTS ${pjinstalled_libs_dir}/libpj-arm-unknown-linux-androideabi.a
                               ${pjinstalled_libs_dir}/libpjsua-arm-unknown-linux-androideabi.a
                               ${pjinstalled_libs_dir}/libpjsip-ua-arm-unknown-linux-androideabi.a
                               ${pjinstalled_libs_dir}/libpjsip-arm-unknown-linux-androideabi.a
                               ${pjinstalled_libs_dir}/libpjsip-simple-arm-unknown-linux-androideabi.a
                               ${pjinstalled_libs_dir}/libpjlib-util-arm-unknown-linux-androideabi.a
                               ${pjinstalled_libs_dir}/libpjnath-arm-unknown-linux-androideabi.a
                               ${pjinstalled_libs_dir}/libpjmedia-arm-unknown-linux-androideabi.a
                               ${pjinstalled_libs_dir}/libpjmedia-codec-arm-unknown-linux-androideabi.a
                               ${pjinstalled_libs_dir}/libpjmedia-audiodev-arm-unknown-linux-androideabi.a
                               ${pjinstalled_libs_dir}/libg7221codec-arm-unknown-linux-androideabi.a
                               ${pjinstalled_libs_dir}/libresample-arm-unknown-linux-androideabi.a
                               ${pjinstalled_libs_dir}/libsrtp-arm-unknown-linux-androideabi.a
                               ${pjinstalled_libs_dir}/libwebrtc-arm-unknown-linux-androideabi.a
                               ${pjinstalled_libs_dir}/libpjsua2-arm-unknown-linux-androideabi.a
                               ${pjinstalled_libs_dir}/libpjmedia-videodev-arm-unknown-linux-androideabi.a
                               ${pjinstalled_libs_dir}/libyuv-arm-unknown-linux-androideabi.a
                               ${pjinstalled_libs_dir}/libgsmcodec-arm-unknown-linux-androideabi.a
                               ${pjinstalled_libs_dir}/libilbccodec-arm-unknown-linux-androideabi.a
                               ${pjinstalled_libs_dir}/libspeex-arm-unknown-linux-androideabi.a
                    )

add_dependencies (pjinstall pjproject)
add_dependencies (pjapi pjinstall)


# PJ SIP LIBRARIES

add_library(pjLib STATIC IMPORTED)
set_property(TARGET pjLib PROPERTY IMPORTED_LOCATION ${pjinstalled_libs_dir}/libpj-arm-unknown-linux-androideabi.a)

add_library(pjSua STATIC IMPORTED)
set_property(TARGET pjSua PROPERTY IMPORTED_LOCATION ${pjinstalled_libs_dir}/libpjsua-arm-unknown-linux-androideabi.a)

add_library(pjSip-Ua STATIC IMPORTED)
set_property(TARGET pjSip-Ua PROPERTY IMPORTED_LOCATION ${pjinstalled_libs_dir}/libpjsip-ua-arm-unknown-linux-androideabi.a)

add_library(pjSip STATIC IMPORTED)
set_property(TARGET pjSip PROPERTY IMPORTED_LOCATION ${pjinstalled_libs_dir}/libpjsip-arm-unknown-linux-androideabi.a)

add_library(pjSimple STATIC IMPORTED)
set_property(TARGET pjSimple PROPERTY IMPORTED_LOCATION ${pjinstalled_libs_dir}/libpjsip-simple-arm-unknown-linux-androideabi.a)

add_library(pjUtil STATIC IMPORTED)
set_property(TARGET pjUtil PROPERTY IMPORTED_LOCATION ${pjinstalled_libs_dir}/libpjlib-util-arm-unknown-linux-androideabi.a)

add_library(pjNath STATIC IMPORTED)
set_property(TARGET pjNath PROPERTY IMPORTED_LOCATION ${pjinstalled_libs_dir}/libpjnath-arm-unknown-linux-androideabi.a)

add_library(pjMedia STATIC IMPORTED)
set_property(TARGET pjMedia PROPERTY IMPORTED_LOCATION ${pjinstalled_libs_dir}/libpjmedia-arm-unknown-linux-androideabi.a)

add_library(pjMediaCodec STATIC IMPORTED)
set_property(TARGET pjMediaCodec PROPERTY IMPORTED_LOCATION ${pjinstalled_libs_dir}/libpjmedia-codec-arm-unknown-linux-androideabi.a)

add_library(pjMediaAudioDev STATIC IMPORTED)
set_property(TARGET pjMediaAudioDev PROPERTY IMPORTED_LOCATION ${pjinstalled_libs_dir}/libpjmedia-audiodev-arm-unknown-linux-androideabi.a)

add_library(pjThird7221 STATIC IMPORTED)
set_property(TARGET pjThird7221 PROPERTY IMPORTED_LOCATION ${pjinstalled_libs_dir}/libg7221codec-arm-unknown-linux-androideabi.a)

add_library(pjThirdResample STATIC IMPORTED)
set_property(TARGET pjThirdResample PROPERTY IMPORTED_LOCATION ${pjinstalled_libs_dir}/libresample-arm-unknown-linux-androideabi.a)

add_library(pjThirdSrtp STATIC IMPORTED)
set_property(TARGET pjThirdSrtp PROPERTY IMPORTED_LOCATION ${pjinstalled_libs_dir}/libsrtp-arm-unknown-linux-androideabi.a)

add_library(pjWebRtc STATIC IMPORTED)
set_property(TARGET pjWebRtc PROPERTY IMPORTED_LOCATION ${pjinstalled_libs_dir}/libwebrtc-arm-unknown-linux-androideabi.a)
add_library(pjSua2 STATIC IMPORTED)
set_property(TARGET pjSua2 PROPERTY IMPORTED_LOCATION ${pjinstalled_libs_dir}/libpjsua2-arm-unknown-linux-androideabi.a)

add_library(pjMediaVideo STATIC IMPORTED)
set_property(TARGET pjMediaVideo PROPERTY IMPORTED_LOCATION ${pjinstalled_libs_dir}/libpjmedia-videodev-arm-unknown-linux-androideabi.a)

add_library(pjYuv STATIC IMPORTED)
set_property(TARGET pjYuv PROPERTY IMPORTED_LOCATION ${pjinstalled_libs_dir}/libyuv-arm-unknown-linux-androideabi.a)

add_library(pjGsm STATIC IMPORTED)
set_property(TARGET pjGsm PROPERTY IMPORTED_LOCATION ${pjinstalled_libs_dir}/libgsmcodec-arm-unknown-linux-androideabi.a)

add_library(pjIlbc STATIC IMPORTED)
set_property(TARGET pjIlbc PROPERTY IMPORTED_LOCATION ${pjinstalled_libs_dir}/libilbccodec-arm-unknown-linux-androideabi.a)

add_library(pjSpeex STATIC IMPORTED)
set_property(TARGET pjSpeex PROPERTY IMPORTED_LOCATION ${pjinstalled_libs_dir}/libspeex-arm-unknown-linux-androideabi.a)


# LINK ALL OF OUR LIBRARIES TOGETHER

# Add in our libraries external to this project
# Since the library manager used here only searches a listed library in one pass
# the library ordering here is critical.  Note that some libraries are searched
# twice by including them in the list more that once.
# OpenSSL is another case.  ssl and crypto must appear before and after the
# SECOND instance of pjLib.
target_link_libraries(pjapi pjSua pjSip pjSip-Ua pjSimple
                pjLib pjNath pjUtil pjMediaCodec pjMedia pjThirdSrtp
                pjMediaAudioDev pjThirdResample pjWebRtc pjIlbc pjGsm
		        pjLib pjUtil log OpenSLES android
                )

add_custom_command (TARGET pjapi POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/../../../build/intermediates/cmake/${lib_type}/obj/armeabi-v7a/libpjapi.so
${final_output_dir}/libpjapi.so)


# Move our created library to the output directory
#install (TARGETS pjapi DESTINATION ${final_output_dir})


