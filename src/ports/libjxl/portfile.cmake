vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO libjxl/libjxl
    REF "v${VERSION}"
    SHA512 0cfd81d9d3b783f96bd04f244d73ffbc12186c89993d46064a2751bef0a446a5e86be05add5c10f60d4482d39333d9bf3f9a866d6eb84f8fa0d4e8b5828fd74c
    HEAD_REF main
    PATCHES
        fix-dependencies.patch
        avoid-exe-linker-flags.patch # https://github.com/libjxl/libjxl/pull/4229
        msvc-remove-libm.patch
        disambiguate-pow-calls.patch
)

vcpkg_check_features(OUT_FEATURE_OPTIONS FEATURE_OPTIONS
    FEATURES
        tools JPEGXL_ENABLE_TOOLS
    INVERTED_FEATURES
        tools CMAKE_DISABLE_FIND_PACKAGE_GIF
        tools CMAKE_DISABLE_FIND_PACKAGE_JPEG
        tools CMAKE_DISABLE_FIND_PACKAGE_PNG
        tools CMAKE_DISABLE_FIND_PACKAGE_ZLIB
)

if(VCPKG_TARGET_IS_UWP)
    string(APPEND VCPKG_C_FLAGS " /wd4146")
    string(APPEND VCPKG_CXX_FLAGS " /wd4146")
endif()

vcpkg_cmake_configure(
    SOURCE_PATH "${SOURCE_PATH}"
    OPTIONS
        "-DJPEGXL_VERSION=${JPEGXL_VERSION}"
        -DJPEGXL_FORCE_SYSTEM_HWY=ON
        -DJPEGXL_FORCE_SYSTEM_BROTLI=ON
        -DJPEGXL_FORCE_SYSTEM_HWY=ON
        -DJPEGXL_FORCE_SYSTEM_LCMS2=ON
        ${FEATURE_OPTIONS}
        -DJPEGXL_ENABLE_BENCHMARK=OFF
        -DJPEGXL_ENABLE_DOXYGEN=OFF
        -DJPEGXL_ENABLE_EXAMPLES=OFF
        -DJPEGXL_ENABLE_FUZZERS=OFF
        -DJPEGXL_ENABLE_JNI=OFF
        -DJPEGXL_ENABLE_MANPAGES=OFF
        -DJPEGXL_ENABLE_OPENEXR=OFF
        -DJPEGXL_ENABLE_PLUGINS=OFF
        -DJPEGXL_ENABLE_SJPEG=OFF
        -DJPEGXL_ENABLE_SKCMS=OFF
        -DJPEGXL_ENABLE_TCMALLOC=OFF
        -DBUILD_TESTING=OFF
        -DCMAKE_FIND_PACKAGE_TARGETS_GLOBAL=ON
        -DJPEGXL_BUNDLE_LIBPNG=OFF
    MAYBE_UNUSED_VARIABLES
        CMAKE_DISABLE_FIND_PACKAGE_GIF
        CMAKE_DISABLE_FIND_PACKAGE_JPEG
        CMAKE_DISABLE_FIND_PACKAGE_PNG
        CMAKE_DISABLE_FIND_PACKAGE_ZLIB
)

vcpkg_cmake_install()
vcpkg_copy_pdbs()
vcpkg_fixup_pkgconfig()

if(JPEGXL_ENABLE_TOOLS)
    vcpkg_copy_tools(TOOL_NAMES cjxl djxl jxlinfo AUTO_CLEAN)
endif()

if(VCPKG_LIBRARY_LINKAGE STREQUAL "static")
    vcpkg_replace_string("${CURRENT_PACKAGES_DIR}/include/jxl/jxl_export.h" "ifdef JXL_STATIC_DEFINE" "if 1")

    # AntiDupl's addition to vcpkg's port: Butteraugli, which libjxl compiles
    # into jxl.lib but keeps out of its public API. Its header and the
    # internal headers it includes go to include/lib/jxl/..., the paths they
    # include each other by. Only the static library can be linked this way:
    # a DLL doesn't export these functions. They are libjxl's internals, so a
    # newer libjxl may need AntiDupl's caller (adButteraugli.cpp) changed.
    foreach(header IN ITEMS
            lib/jxl/butteraugli/butteraugli.h
            lib/jxl/base/common.h
            lib/jxl/base/compiler_specific.h
            lib/jxl/base/sanitizer_definitions.h
            lib/jxl/base/status.h
            lib/jxl/image.h
            lib/jxl/memory_manager_internal.h)
        get_filename_component(directory "${header}" DIRECTORY)
        file(INSTALL "${SOURCE_PATH}/${header}" DESTINATION "${CURRENT_PACKAGES_DIR}/include/${directory}")
    endforeach()
endif()

file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include")

vcpkg_install_copyright(FILE_LIST "${SOURCE_PATH}/LICENSE")
