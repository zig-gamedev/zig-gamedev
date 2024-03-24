set(TARGET_NAME nfd)

set(PUBLIC_HEADER_FILES
  include/nfd.h
  include/nfd.hpp)

set(SOURCE_FILES ${PUBLIC_HEADER_FILES})

if(nfd_PLATFORM STREQUAL PLATFORM_WIN32)
  list(APPEND SOURCE_FILES nfd_win.cpp)
endif()

if(nfd_PLATFORM STREQUAL PLATFORM_LINUX)
  find_package(PkgConfig REQUIRED)
  # for Linux, we support GTK3 and xdg-desktop-portal
  option(NFD_PORTAL "Use xdg-desktop-portal instead of GTK" OFF)
  if(NOT NFD_PORTAL)
    pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
    message("Using GTK version: ${GTK3_VERSION}")
    list(APPEND SOURCE_FILES nfd_gtk.cpp)
  else()
    pkg_check_modules(DBUS REQUIRED dbus-1)
    message("Using DBUS version: ${DBUS_VERSION}")
    list(APPEND SOURCE_FILES nfd_portal.cpp)
  endif()
endif()

if(nfd_PLATFORM STREQUAL PLATFORM_MACOS)
  # For setting the filter list, macOS introduced allowedContentTypes in version 11.0 and deprecated allowedFileTypes in 12.0.
  # By default (set to ON), NFDe will use allowedContentTypes when targeting macOS >= 11.0.
  # Set this option to OFF to always use allowedFileTypes regardless of the target macOS version.
  # This is mainly needed for applications that are built on macOS >= 11.0 but should be able to run on lower versions
  # and should not be used otherwise.
  option(NFD_USE_ALLOWEDCONTENTTYPES_IF_AVAILABLE "Use allowedContentTypes for filter lists on macOS >= 11.0" ON)

  find_library(APPKIT_LIBRARY AppKit)
  if(NFD_USE_ALLOWEDCONTENTTYPES_IF_AVAILABLE)
    include(CheckCXXSourceCompiles)
    check_cxx_source_compiles(
      "
      #include <Availability.h>
      #if !defined(__MAC_OS_X_VERSION_MIN_REQUIRED) || !defined(__MAC_11_0) || __MAC_OS_X_VERSION_MIN_REQUIRED < __MAC_11_0
      static_assert(false);
      #endif
      int main() { return 0; }
      "
      NFD_USE_ALLOWEDCONTENTTYPES
    )
    if(NFD_USE_ALLOWEDCONTENTTYPES)
      find_library(UNIFORMTYPEIDENTIFIERS_LIBRARY UniformTypeIdentifiers)
      if(NOT UNIFORMTYPEIDENTIFIERS_LIBRARY)
        message(FATAL_ERROR "UniformTypeIdentifiers framework is not available even though we are targeting macOS >= 11.0")
      endif()
    endif()
  endif()
  list(APPEND SOURCE_FILES nfd_cocoa.m)
endif()

# Define the library
add_library(${TARGET_NAME} ${SOURCE_FILES})

if (BUILD_SHARED_LIBS)
  target_compile_definitions(${TARGET_NAME} PRIVATE NFD_EXPORT INTERFACE NFD_SHARED)
endif ()

# Allow includes from include/
target_include_directories(${TARGET_NAME}
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

if(nfd_PLATFORM STREQUAL PLATFORM_LINUX)
  if(NOT NFD_PORTAL)
    target_include_directories(${TARGET_NAME}
      PRIVATE ${GTK3_INCLUDE_DIRS})
    target_link_libraries(${TARGET_NAME}
      PRIVATE ${GTK3_LINK_LIBRARIES})
  else()
    target_include_directories(${TARGET_NAME}
      PRIVATE ${DBUS_INCLUDE_DIRS})
    target_link_libraries(${TARGET_NAME}
      PRIVATE ${DBUS_LINK_LIBRARIES})
    target_compile_definitions(${TARGET_NAME}
      PUBLIC NFD_PORTAL)
  endif()

  option(NFD_APPEND_EXTENSION "Automatically append file extension to an extensionless selection in SaveDialog()" OFF)
  if(NFD_APPEND_EXTENSION)
    target_compile_definitions(${TARGET_NAME} PRIVATE NFD_APPEND_EXTENSION)
  endif()
endif()

if(nfd_PLATFORM STREQUAL PLATFORM_MACOS)
  if(NFD_USE_ALLOWEDCONTENTTYPES)
    target_link_libraries(${TARGET_NAME} PRIVATE ${APPKIT_LIBRARY} ${UNIFORMTYPEIDENTIFIERS_LIBRARY})
    target_compile_definitions(${TARGET_NAME} PRIVATE NFD_MACOS_ALLOWEDCONTENTTYPES=1)
  else()
    target_link_libraries(${TARGET_NAME} PRIVATE ${APPKIT_LIBRARY})
    target_compile_definitions(${TARGET_NAME} PRIVATE NFD_MACOS_ALLOWEDCONTENTTYPES=0)
  endif()
endif()

if(nfd_COMPILER STREQUAL COMPILER_MSVC)
  string(REPLACE "/EHsc" "/EHs-c-" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  string(REPLACE "/GR" "/GR-" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  set_property(TARGET ${TARGET_NAME} APPEND_STRING PROPERTY STATIC_LIBRARY_OPTIONS /NODEFAULTLIB)
endif()

if(nfd_COMPILER STREQUAL COMPILER_CLANGCL)
  string(REPLACE "/EHsc" "/EHs-c-" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  string(REPLACE "/GR" "/GR-" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
endif()

if(nfd_COMPILER STREQUAL COMPILER_GNU)
  target_compile_options(${TARGET_NAME} PRIVATE -nostdlib -fno-exceptions -fno-rtti)
endif()

set_target_properties(${TARGET_NAME} PROPERTIES
                                       PUBLIC_HEADER "${PUBLIC_HEADER_FILES}"
                                       VERSION ${PROJECT_VERSION}
                                       SOVERSION ${PROJECT_VERSION_MAJOR})

if (NFD_INSTALL)
  include(GNUInstallDirs)

  install(TARGETS ${TARGET_NAME} EXPORT ${TARGET_NAME}-export
    LIBRARY DESTINATION ${LIB_INSTALL_DIR} ARCHIVE DESTINATION ${LIB_INSTALL_DIR} PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}    
  )
  install(EXPORT ${TARGET_NAME}-export
    DESTINATION lib/cmake/${TARGET_NAME}
    NAMESPACE ${TARGET_NAME}::
    FILE ${TARGET_NAME}-config.cmake
  )
endif()
