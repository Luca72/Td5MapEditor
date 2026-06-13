# Td5MapEditor

**Td5MapEditor** is a cross-platform editor/viewer for Land Rover Td5 ECU map files.

The project was originally developed with Code::Blocks and wxWidgets, and has recently been refreshed to build with modern wxWidgets 3.2 toolchains on Linux and Windows. The application embeds a collection of Td5 base maps directly inside the executable, so it can be distributed as a mostly self-contained tool without requiring an external map database folder.

> **Warning**
>
> Td5MapEditor is still under development and considered UNSTABLE. ECU map editing can damage an engine or vehicle if used incorrectly. This software is intended for technical/educational use by people who understand Td5 ECU files and the risks involved. Always keep verified backups of original ECU files.

## Main features

- Open and inspect Land Rover Td5 map files.
- Compare and work with embedded base maps.
- Graph/table-oriented wxWidgets interface.
- Portable design: reference maps are compiled into the executable.
- Linux and Windows build targets.
- Code::Blocks project files retained for compatibility with the original workflow.
- Makefile-based builds added for faster incremental builds.

## User manual

See [USER_MANUAL.md](USER_MANUAL.md).

## Project layout

Typical layout:

```text
Td5MapEditor/
├── src/                    # C++ source files
│   ├── tinyxml/             # Bundled TinyXML sources
│   └── baseMaps.cpp/.h      # Generated embedded map resources
├── linux_x64/               # Linux build output
├── win64/                   # Windows build output
├── .vscode/                 # VS Code tasks/debug configuration
├── Makefile                 # Linux Makefile
├── Makefile.win             # Windows MinGW Makefile
└── td5mapeditor.cbp         # Code::Blocks project
```

`baseMaps.cpp` is intentionally large because it contains embedded binary map resources. It should normally be treated as a generated file.

## Dependencies

### Linux

Install a C++ toolchain and wxWidgets 3.2 development files:

```bash
sudo apt update
sudo apt install build-essential gdb make libwxgtk3.2-dev
```

The Linux build uses:

```bash
wx-config --cflags
wx-config --libs
```

so `wx-config` must be available in `PATH`.

### Windows

The current Windows configuration targets:

```text
Code::Blocks 25.03
Bundled MinGW-W64 x86_64-ucrt-posix-seh
GCC 14.2.0
wxWidgets 3.2.x, static build
```

Expected default wxWidgets path:

```text
C:/wxWidgets-3.2
```

Expected static wxWidgets library folder:

```text
C:/wxWidgets-3.2/lib/gcc1420UCRT_x64
```

If your local paths differ, edit these variables in `Makefile.win`:

```makefile
MINGW_DIR ?= C:/Program Files/CodeBlocks/MinGW
WX_DIR    ?= C:/wxWidgets-3.2
WX_LIB    ?= $(WX_DIR)/lib/gcc1420UCRT_x64
```

## Building on Linux

### Using Makefile

Build Debug:

```bash
make debug
```

Build Release:

```bash
make release
```

Clean build outputs:

```bash
make clean
```

The Linux Makefile performs incremental builds and stores object files under:

```text
linux_x64/debug/obj/vscode
linux_x64/release/obj/vscode
```

Executables are generated as:

```text
linux_x64/debug/td5mapeditor
linux_x64/release/td5mapeditor
```

The Makefile automatically uses parallel jobs. By default it uses roughly half of the available CPU cores. You can override it manually:

```bash
make -j8 release
```

### Using VS Code on Linux

The repository can be opened directly in VS Code.

Recommended extensions:

- **C/C++** by Microsoft

Useful commands:

```text
Ctrl+Shift+B       Build default target, usually Debug
F5                 Build Debug and start the debugger
Ctrl+Shift+P       Tasks: Run Task → choose Debug/Release/Clean
```

The VS Code configuration calls the Makefile, so builds are incremental.

Typical `.vscode/tasks.json` targets:

```text
Build Td5MapEditor Debug
Build Td5MapEditor Release
Clean Td5MapEditor
```

Typical `.vscode/launch.json` target:

```text
Debug Td5MapEditor
```

### Using Code::Blocks on Linux

Open:

```text
td5mapeditor.cbp
```

Then select one of the Linux targets:

```text
Linux x64 Debug
Linux x64 Release
```

The Code::Blocks Linux configuration uses `wx-config` directly.

## Building on Windows

There are two supported Windows workflows:

1. Code::Blocks project build.
2. `Makefile.win` with MinGW-w64.

Both expect the same compiler/wxWidgets ABI combination.

### Using Code::Blocks on Windows

Open:

```text
td5mapeditor.cbp
```

Then select:

```text
Win64 Debug
Win64 Release
```

Current expected wxWidgets paths in the project:

Debug include paths:

```text
C:/wxWidgets-3.2/lib/gcc1420UCRT_x64/mswud
C:/wxWidgets-3.2/include
```

Release include paths:

```text
C:/wxWidgets-3.2/lib/gcc1420UCRT_x64/mswu
C:/wxWidgets-3.2/include
```

Library path:

```text
C:/wxWidgets-3.2/lib/gcc1420UCRT_x64
```

Important compiler options:

```text
-std=gnu++17
-funsigned-char
-D__WXMSW__
-mthreads
```

Do **not** define `WXUSINGDLL` when using the static wxWidgets build.

### Using Makefile.win on Windows

From the project root, run:

```bat
mingw32-make -f Makefile.win debug
```

or:

```bat
mingw32-make -f Makefile.win release
```

Clean:

```bat
mingw32-make -f Makefile.win clean
```

Outputs:

```text
win64/debug/td5mapeditor.exe
win64/release/td5mapeditor.exe
```

Object files are stored under:

```text
win64/debug/obj/make
win64/release/obj/make
```

`Makefile.win` does not use `wx-config`, because `wx-config` is normally not available in a standard Windows MinGW setup. Instead, it explicitly defines:

- wxWidgets include paths.
- wxWidgets library path.
- wxWidgets libraries.
- Required Windows system libraries.

This is intentional.

## Building wxWidgets statically on Windows

The Windows static build requires wxWidgets to be built with the same MinGW-w64 toolchain used by Code::Blocks.

Example from a Windows command prompt configured for Code::Blocks MinGW:

```bat
cd C:\wxWidgets-3.2\build\msw
mingw32-make -f makefile.gcc BUILD=release SHARED=0 UNICODE=1
mingw32-make -f makefile.gcc BUILD=debug   SHARED=0 UNICODE=1
```

Expected result:

```text
C:/wxWidgets-3.2/lib/gcc1420UCRT_x64/mswu/wx/setup.h
C:/wxWidgets-3.2/lib/gcc1420UCRT_x64/mswud/wx/setup.h
C:/wxWidgets-3.2/lib/gcc1420UCRT_x64/libwxmsw32u_core.a
C:/wxWidgets-3.2/lib/gcc1420UCRT_x64/libwxmsw32ud_core.a
```

If your wxWidgets build creates a different folder name, either rename it consistently or update `WX_LIB` in `Makefile.win` and the Code::Blocks search paths.

## Development status

This is an old project refreshed for current toolchains. The source remains compatible with the original Code::Blocks workflow while adding Makefile and VS Code support for easier modern development.

## Licence

This project is licensed under the terms of the GNU license.
