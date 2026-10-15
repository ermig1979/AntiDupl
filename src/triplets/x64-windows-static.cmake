# Replaces vcpkg's built-in x64-windows-static (src/vcpkg.json lists this
# folder in "overlay-triplets").
#
# Builds the libraries with toolset v143, the one the projects use. On a
# machine with Visual Studio 2026, vcpkg would otherwise pick v145: then the
# v143 link fails (unresolved __std_min_element_1u and similar), and if the
# project is built with v145 too, libde265 1.0.16 is miscompiled and
# AntiDupl.dll fails to load (de265_init() crashes, error 1114).
set(VCPKG_TARGET_ARCHITECTURE x64)
set(VCPKG_CRT_LINKAGE static)
set(VCPKG_LIBRARY_LINKAGE static)
set(VCPKG_PROVIDED_FORTRAN ON)
set(VCPKG_PLATFORM_TOOLSET v143)
