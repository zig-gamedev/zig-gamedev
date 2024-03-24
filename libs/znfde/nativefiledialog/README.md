
# Native File Dialog Extended

![GitHub Actions](https://github.com/btzy/nativefiledialog-extended/workflows/build/badge.svg)

A small C library with that portably invokes native file open, folder select and file save dialogs.  Write dialog code once and have it pop up native dialogs on all supported platforms.  Avoid linking large dependencies like wxWidgets and Qt.

This library is based on Michael Labbe's Native File Dialog ([mlabbe/nativefiledialog](https://github.com/mlabbe/nativefiledialog)).

Features:

- Lean C API, static library &mdash; no C++/ObjC runtime needed
- Supports Windows (MSVC, MinGW, Clang), MacOS (Clang), and Linux (GTK, portal) (GCC, Clang)
- Zlib licensed
- Friendly names for filters (e.g. `C/C++ Source files (*.c;*.cpp)` instead of `(*.c;*.cpp)`) on platforms that support it
- Automatically append file extension on platforms where users expect it
- Support for setting a default folder path
- Support for setting a default file name (e.g. `Untitled.c`)
- Consistent UTF-8 support on all platforms
- Native character set (UTF-16 `wchar_t`) support on Windows
- Initialization and de-initialization of platform library (e.g. COM (Windows) / GTK (Linux GTK) / D-Bus (Linux portal)) decoupled from dialog functions, so applications can choose when to initialize/de-initialize
- Multiple file selection support (for file open dialog)
- Support for Vista's modern `IFileDialog` on Windows
- No third party dependencies
- Modern CMake build system
- Works alongside [SDL2](http://www.libsdl.org) on all platforms
- Optional C++ wrapper with `unique_ptr` auto-freeing semantics and optional parameters, for those using this library from C++

**Comparison with original Native File Dialog:**

The friendly names feature is the primary reason for breaking API compatibility with Michael Labbe's library (and hence this library probably will never be merged with it).  There are also a number of tweaks that cause observable differences in this library.

Features added in Native File Dialog Extended:

- Friendly names for filters
- Automatically appending file extensions
- Support for setting a default file name
- Native character set (UTF-16 `wchar_t`) support on Windows
- xdg-desktop-portal support on Linux that opens the "native" file chooser (see "Usage" section below)
- Initialization and de-initialization of platform library decoupled from file dialog functions
- Modern CMake build system
- Optional C++ wrapper with `unique_ptr` auto-freeing semantics and optional parameters

There is also significant code refractoring, especially for the Windows implementation.

The [wiki](https://github.com/btzy/nativefiledialog-extended/wiki) keeps track of known language bindings and known popular projects that depend on this library.

# Basic Usage

```C
#include <nfd.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    
    NFD_Init();

    nfdchar_t *outPath;
    nfdfilteritem_t filterItem[2] = { { "Source code", "c,cpp,cc" }, { "Headers", "h,hpp" } };
    nfdresult_t result = NFD_OpenDialog(&outPath, filterItem, 2, NULL);
    if (result == NFD_OKAY)
    {
        puts("Success!");
        puts(outPath);
        NFD_FreePath(outPath);
    }
    else if (result == NFD_CANCEL)
    {
        puts("User pressed cancel.");
    }
    else 
    {
        printf("Error: %s\n", NFD_GetError());
    }

    NFD_Quit();
    return 0;
}
```

See [NFD.h](src/include/nfd.h) for more options.

If you are using a platform abstraction framework such as SDL or GLFW, also see the "Usage" section below.

# Screenshots #

![Windows 10](screens/open_win10.png?raw=true#gh-light-mode-only)
![Windows 10](screens/open_win10_dark.png?raw=true#gh-dark-mode-only)
![MacOS 10.13](screens/open_macos_11.0.png?raw=true#gh-light-mode-only)
![MacOS 10.13](screens/open_macos_11.0_dark.png?raw=true#gh-dark-mode-only)
![GTK3 on Ubuntu 20.04](screens/open_gtk3.png?raw=true#gh-light-mode-only)
![GTK3 on Ubuntu 20.04](screens/open_gtk3_dark.png?raw=true#gh-dark-mode-only)

# Building

## CMake Projects
If your project uses CMake,
simply add the following lines to your CMakeLists.txt:
```
add_subdirectory(path/to/nativefiledialog-extended)
target_link_libraries(MyProgram PRIVATE nfd)
```
Make sure that you also have the needed [dependencies](#dependencies).

When included as a subproject, sample programs are not built and the install target is disabled by default.
Add `-DNFD_BUILD_TESTS=ON` to build sample programs and `-DNFD_INSTALL=ON` to enable the install target.

## Standalone Library
If you want to build the standalone static library,
execute the following commands (starting from the project root directory):

For GCC and Clang:
```
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```

For MSVC:
```
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

The above commands will make a `build` directory,
and build the project (in release mode) there.
If you are developing NFDe, you may want to do `-DCMAKE_BUILD_TYPE=Debug`/`--config Debug`
to build a debug version of the library instead.

When building as a standalone library, sample programs are built and the install target is enabled by default.
Add `-DNFD_BUILD_TESTS=OFF` to disable building sample programs and `-DNFD_INSTALL=OFF` to disable the install target.

On Linux, if you want to use the Flatpak desktop portal instead of GTK, add `-DNFD_PORTAL=ON`.  (Otherwise, GTK will be used.)  See the "Usage" section below for more information.

See the [CI build file](.github/workflows/cmake.yml) for some example build commands.

### Visual Studio on Windows
Recent versions of Visual Studio have CMake support built into the IDE. 
You should be able to "Open Folder" in the project root directory,
and Visual Studio will recognize and configure the project appropriately.
From there, you will be able to set configurations for Debug vs Release,
and for x86 vs x64. 
For more information, see [the Microsoft Docs page]([https://docs.microsoft.com/en-us/cpp/build/cmake-projects-in-visual-studio?view=vs-2019](https://docs.microsoft.com/en-us/cpp/build/cmake-projects-in-visual-studio?view=vs-2019)).
This has been tested to work on Visual Studio 2019,
and it probably works on Visual Studio 2017 too.

### Compiling Your Programs

 1. Add `src/include` to your include search path.
 2. Add `nfd.lib` or `nfd_d.lib` to the list of static libraries to link against (for release or debug, respectively).
 3. Add `build/<debug|release>/<arch>` to the library search path.

## Dependencies

### Linux

#### GTK (default)
Make sure `libgtk-3-dev` is installed on your system.

#### Portal
Make sure `libdbus-1-dev` is installed on your system.

### MacOS
On MacOS, add `AppKit` and `UniformTypeIdentifiers` to the list of frameworks.

### Windows
On Windows (both MSVC and MinGW), ensure you are building against `ole32.lib`, `uuid.lib`, and `shell32.lib`.

# Usage

See `NFD.h` for API calls.  See the `test` directory for example code (both C and C++).

If you turned on the option to build the `test` directory (`-DNFD_BUILD_TESTS=ON`), then `build/bin` will contain the compiled test programs.

## File Filter Syntax

Files can be filtered by file extension groups:

```C
nfdfilteritem_t filterItem[2] = { { "Source code", "c,cpp,cc" }, { "Headers", "h,hpp" } };
```

A file filter is a pair of strings comprising the friendly name and the specification (multiple file extensions are comma-separated).

A list of file filters can be passed as an argument when invoking the library.

A wildcard filter is always added to every dialog.

*Note: On MacOS, the file dialogs do not have friendly names and there is no way to switch between filters, so the filter specifications are combined (e.g. "c,cpp,cc,h,hpp").  The filter specification is also never explicitly shown to the user.  This is usual MacOS behaviour and users expect it.*

*Note 2: You must ensure that the specification string is non-empty and that every file extension has at least one character.  Otherwise, bad things might ensue (i.e. undefined behaviour).*

*Note 3: On Linux, the file extension is appended (if missing) when the user presses down the "Save" button.  The appended file extension will remain visible to the user, even if an overwrite prompt is shown and the user then presses "Cancel".*

*Note 4: On Windows, the default folder parameter is only used if there is no recently used folder available.  Otherwise, the default folder will be the folder that was last used.  Internally, the Windows implementation calls [IFileDialog::SetDefaultFolder(IShellItem)](https://docs.microsoft.com/en-us/windows/desktop/api/shobjidl_core/nf-shobjidl_core-ifiledialog-setdefaultfolder).  This is usual Windows behaviour and users expect it.*

## Iterating Over PathSets

A file open dialog that supports multiple selection produces a PathSet, which is a thin abstraction over the platform-specific collection.  There are two ways to iterate over a PathSet:

### Accessing by index

This method does array-like access on the PathSet, and is the easiest to use.
However, on certain platforms (Linux, and possibly Windows),
it takes O(N<sup>2</sup>) time in total to iterate the entire PathSet,
because the underlying platform-specific implementation uses a linked list.

See [test_opendialogmultiple.c](test/test_opendialogmultiple.c).

### Using an enumerator (experimental)

This method uses an enumerator object to iterate the paths in the PathSet.
It is guaranteed to take O(N) time in total to iterate the entire PathSet.

See [test_opendialogmultiple_enum.c](test/test_opendialogmultiple_enum.c).

This API is experimental, and subject to change.

## Customization Macros

You can define the following macros *before* including `nfd.h`/`nfd.hpp`:

- `NFD_NATIVE`: Define this before including `nfd.h` to make non-suffixed function names and typedefs (e.g. `NFD_OpenDialog`) aliases for the native functions (e.g. `NFD_OpenDialogN`) instead of aliases for the UTF-8 functions (e.g. `NFD_OpenDialogU8`).  This macro does not affect the C++ wrapper `nfd.hpp`.
- `NFD_THROWS_EXCEPTIONS`: (C++ only)  Define this before including `nfd.hpp` to make `NFD::Guard` construction throw `std::runtime_error` if `NFD_Init` fails.  Otherwise, there is no way to detect failure in `NFD::Guard` construction.

Macros that might be defined by `nfd.h`:

- `NFD_DIFFERENT_NATIVE_FUNCTIONS`: Defined if the native and UTF-8 versions of functions are different (i.e. compiling for Windows); not defined otherwise.  If `NFD_DIFFERENT_NATIVE_FUNCTIONS` is not defined, then the UTF-8 versions of functions are aliases for the native versions.  This might be useful if you are writing a function that wants to provide overloads depending on whether the native functions and UTF-8 functions are the same.  (Native is UTF-16 (`wchar_t`) for Windows and UTF-8 (`char`) for Mac/Linux.)

## Usage with a Platform Abstraction Framework

NFDe is known to work with SDL2 and GLFW, and should also work with other platform abstraction framworks.  However, you should initialize NFDe _after_ initializing the framework, and probably should deinitialize NFDe _before_ deinitializing the framework.  This is because some frameworks expect to be initialized on a "clean slate", and they may configure the system in a different way from NFDe.  `NFD_Init` is generally very careful not to disrupt the existing configuration unless necessary, and `NFD_Quit` restores the configuration back exactly to what it was before initialization.

An example with SDL2:

```
// Initialize SDL2 first
if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0) {
    // display some error here
}

// Then initialize NFDe
if (NFD_Init() != NFD_OKAY) {
    // display some error here
}

/*
Your main program goes here
*/

NFD_Quit(); // deinitialize NFDe first

SDL_Quit(); // Then deinitialize SDL2
```

## Using xdg-desktop-portal on Linux

On Linux, you can use the portal implementation instead of GTK, which will open the "native" file chooser selected by the OS or customized by the user.  The user must have `xdg-desktop-portal` and a suitable backend installed (this comes pre-installed with most common desktop distros), otherwise `NFD_ERROR` will be returned.

To use the portal implementation, add `-DNFD_PORTAL=ON` to the build command.

*Note:  Setting a default path is not supported by the portal implementation, and any default path passed to NFDe will be ignored.  This is a limitation of the portal API, so there is no way NFDe can work around it.  If this feature is something you desire, please show your interest on https://github.com/flatpak/xdg-desktop-portal/pull/874.*

*Note 2:  The folder picker is only supported on org.freedesktop.portal.FileChooser interface version >= 3, which corresponds to xdg-desktop-portal version >= 1.7.1.  `NFD_PickFolder()` will query the interface version at runtime, and return `NFD_ERROR` if the version is too low.

### What is a portal?

Unlike Windows and MacOS, Linux does not have a file chooser baked into the operating system.  Linux applications that want a file chooser usually link with a library that provides one (such as GTK, as in the Linux screenshot above).  This is a mostly acceptable solution that many applications use, but may make the file chooser look foreign on non-GTK distros.

Flatpak was introduced in 2015, and with it came a standardized interface to open a file chooser.  Applications using this interface did not need to come with a file chooser, and could use the one provided by Flatpak.  This interface became known as the desktop portal, and its use expanded to non-Flatpak applications.  Now, most major desktop Linux distros come with the desktop portal installed, with file choosers that fit the theme of the distro.  Users can also install a different portal backend if desired.  There are currently two known backends: GTK and KDE.  (XFCE does not currently seem to have a portal backend.)

## Platform-specific Quirks

### MacOS

- If the MacOS deployment target is ≥ 11.0, the [allowedContentTypes](https://developer.apple.com/documentation/appkit/nssavepanel/3566857-allowedcontenttypes?language=objc) property of NSSavePanel is used instead of the deprecated [allowedFileTypes](https://developer.apple.com/documentation/appkit/nssavepanel/1534419-allowedfiletypes?language=objc) property for file filters.  Thus, if you are filtering by a custom file extension specific to your application, you will need to define the data type in your `Info.plist` file as per the [Apple documentation](https://developer.apple.com/documentation/uniformtypeidentifiers/defining_file_and_data_types_for_your_app).  (It is possible to force NFDe to use allowedFileTypes by adding `-DNFD_USE_ALLOWEDCONTENTTYPES_IF_AVAILABLE=OFF` to your CMake build command, but this is not recommended.  If you need to support older MacOS versions, you should be setting the correct deployment target instead.)

# Known Limitations #

 - No support for Windows XP's legacy dialogs such as `GetOpenFileName`.  (There are no plans to support this; you shouldn't be still using Windows XP anyway.)
 - No Emscripten (WebAssembly) bindings.  (This might get implemented if I decide to port Circuit Sandbox for the web, but I don't think there is any way to implement a web-based folder picker.)
 - GTK dialogs don't set the existing window as parent, so if users click the existing window while the dialog is open then the dialog will go behind it.  GTK writes a warning to stdout or stderr about this.
 - Portal dialogs (the alternative to GTK on Linux) don't support a default path.  Any default path you supply will be ignored.
 - This library is not compatible with the original Native File Dialog library.  Things might break if you use both in the same project.  (There are no plans to support this; you have to use one or the other.)
 - This library does not explicitly dispatch calls to the UI thread.  This may lead to crashes if you call functions from other threads when the platform does not support it (e.g. MacOS).  Users are generally expected to call NFDe from an appropriate UI thread (i.e. the thread performing the UI event loop).

# Reporting Bugs #

Please use the GitHub issue tracker to report bugs or to contribute to this repository.  Feel free to submit bug reports of any kind.

# Credit #

Bernard Teo (me) and other contributors for everything that wasn't from Michael Labbe's [Native File Dialog](https://github.com/mlabbe/nativefiledialog).

[Michael Labbe](https://github.com/mlabbe) for his awesome Native File Dialog library, and the other contributors to that library.

Much of this README has also been copied from the README of original Native File Dialog repository.

## License ##

Everything in this repository is distributed under the ZLib license, as is the original Native File Dialog library.

## Support ##

I don't provide any paid support.  [Michael Labbe](https://github.com/mlabbe) appears to provide paid support for his [library](https://github.com/mlabbe/nativefiledialog) at the time of writing.
