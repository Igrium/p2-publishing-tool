# Building the Portal 2 Map Uploader

The tool is a Qt 6 / CMake application that talks to Steam through the Steamworks SDK.
It builds on **Linux** (GCC or Clang) and **Windows** (MSVC).

## Requirements

| Dependency | Version / notes |
|---|---|
| CMake | 3.15 or newer |
| C++ compiler | Linux: GCC or Clang with C++20 support. Windows: MSVC 2019 or 2022 (C++23). |
| Qt | **Exactly 6.7.2**, with the Core, Gui, Widgets and Network modules (plus DBus on Linux, which comes with the standard desktop install). CMake copies the `6.7.2` libraries by name, so other versions won't configure. |
| Steamworks SDK | Download it from <https://partner.steamgames.com/downloads/list> (needs a Steamworks account). |
| git | Needed to fetch the `SpeedyKeyV` submodule. |

## 1. Get the source

```bash
git clone --recursive https://github.com/Igrium/p2-publishing-tool.git
```

If you already cloned without `--recursive`, fetch the submodule from inside the repo:

```bash
git submodule update --init --recursive
```

## 2. Add the Steamworks SDK

Extract the SDK so that its contents (the `public/` and `redistributable_bin/` folders) are directly inside `libs/Steam-API/`. That folder is gitignored. The build expects these paths:

```
libs/Steam-API/
├── public/steam/steam_api.h
└── redistributable_bin/
    ├── linux64/libsteam_api.so          (Linux)
    └── win64/steam_api64.dll, steam_api64.lib   (Windows)
```

**Windows only:** CMake also copies two OpenSSL 1.1 DLLs, which are *not* part of the official SDK:

```
libs/Steam-API/openssl/libcrypto-1_1-x64.dll
libs/Steam-API/openssl/libssl-1_1-x64.dll
```

Put 64-bit OpenSSL 1.1 builds of those DLLs there, or configure will fail.

## 3. Install Qt 6.7.2

Use the official [Qt online installer](https://www.qt.io/download-qt-installer) and select *Qt 6.7.2 → Desktop*, or use [aqtinstall](https://github.com/miurahr/aqtinstall):

```bash
pip install aqtinstall
```

Linux:

```bash
aqt install-qt linux desktop 6.7.2 linux_gcc_64 -O ~/Qt
```

Windows:

```bash
aqt install-qt windows desktop 6.7.2 win64_msvc2019_64 -O C:\Qt
```

This produces `~/Qt/6.7.2/gcc_64` on Linux, which is also the location CMake assumes by default, or `C:\Qt\6.7.2\msvc2019_64` on Windows.
This directory is called `QT_BASEDIR` below.

## 4a. Build on Linux

Install a compiler, CMake and the OpenGL headers Qt needs. On Debian/Ubuntu:

```bash
sudo apt install build-essential cmake git libgl1-mesa-dev libxcb-cursor0
```

(`libxcb-cursor0` is needed at runtime by Qt's X11 plugin.)

Configure and build from the repository root:

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release -DQT_BASEDIR=$HOME/Qt/6.7.2/gcc_64 -DCMAKE_PREFIX_PATH=$HOME/Qt/6.7.2/gcc_64
```

```bash
cmake --build build -j
```

Run it:

```bash
./build/p2_map_uploader
```

CMake copies the Qt libraries, plugins and `libsteam_api.so` into `build/`.

**Native file dialogs:** on Linux the app opens file pickers through `xdg-desktop-portal`, so you get the desktop's own dialog (Dolphin-style on KDE Plasma, GTK on GNOME).
This needs `xdg-desktop-portal` and a backend for your desktop, such as `xdg-desktop-portal-kde` or `xdg-desktop-portal-gtk`. Most desktops install these by default.
Without them the app falls back to Qt's built-in dialog.
To force a different Qt platform theme, set `QT_QPA_PLATFORMTHEME` yourself before launching.

## 4b. Build on Windows

Open an **"x64 Native Tools Command Prompt for VS 2022"** (or 2019) and run from the repository root:

```bat
cmake -G Ninja -B build -DCMAKE_BUILD_TYPE=Release -DQT_BASEDIR=C:/Qt/6.7.2/msvc2019_64 -DCMAKE_PREFIX_PATH=C:/Qt/6.7.2/msvc2019_64
```

```bat
cmake --build build
```

Run `build\p2_map_uploader.exe`. CMake copies the required Qt DLLs, plugins and `steam_api64.dll` next to the executable.
On Windows, `QT_BASEDIR` is mandatory.

## Running

- **Steam must be running and logged in**, or the app exits with "SteamAPI_Init() failed".
- By default it uploads to Portal 2 (app ID `644`). Use `--appid <id>` to target a different app:

  ```bash
  ./build/p2_map_uploader --appid 620
  ```

## Troubleshooting

| Problem | Fix |
|---|---|
| `Could not find a package configuration file provided by "Qt6"` | Pass `-DCMAKE_PREFIX_PATH=<QT_BASEDIR>`. |
| `configure_file Problem configuring file` mentioning `libQt6Core.so.6.7.2`, `libicu*.so.73.2` or a Qt plugin | `QT_BASEDIR` doesn't point to a Qt **6.7.2** `gcc_64` / `msvc2019_64` directory. |
| `Please define your QT install dir with -DQT_BASEDIR=...` | On Windows, pass `-DQT_BASEDIR`. |
| `steam_api.h: No such file or directory` or a missing `libsteam_api.so` / `steam_api64.dll` | The Steamworks SDK isn't extracted into `libs/Steam-API/` as shown in step 2. |
| Windows configure fails on `libcrypto-1_1-x64.dll` / `libssl-1_1-x64.dll` | Add the OpenSSL DLLs described in step 2. |
| `KeyValue.h: No such file or directory` | Run `git submodule update --init --recursive`. |
| `qt.qpa.plugin: From 6.5.0, xcb-cursor0 or libxcb-cursor0 is needed` | Install `libxcb-cursor0`. |
| "SteamAPI_Init() failed" at startup | Start Steam and log in first. |
