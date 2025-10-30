# Anolog-transistor-placement-optimizer
A small C++ project organized with CMake so it can be built on Visual Studio (Windows) and g++/Clang (Linux/macOS).
本專案使用 CMake 管理，支援 Visual Studio（Windows） 與 g++/Clang（Linux/macOS） 交叉平台編譯。

## Directory Layout | 專案結構
<repo-root>/
├─ CMakeLists.txt
├─ header/
│  ├─ InitialPlacement.h
│  ├─ SAManager.h
│  └─ TableManager.h
├─ src/
│  ├─ main.cpp
│  ├─ InitialPlacement.cpp
│  ├─ SAManager.cpp
│  └─ TableManager.cpp
└─ build/          # out-of-source build directory（建議用來放所有編譯產物，此資料夾中的內容會被ignore忽略，不會汙染倉庫）

## Prerequisites | 先決條件
###Windows (Visual Studio 2022)
安裝「使用 C++ 的桌面開發」工作負載，勾選 C++ CMake tools（Open Folder 模式需要）
安裝CMAKE(CMake ≥ 3.20)

## Build & Run on Windows (Visual Studio) | 在 Windows / VS 編譯與執行
用 CMake 產生 .sln，使用CMAKE，並輸入：

cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Debug

接著用 VS 打開：build/AnalogTransistorPlacementOptimizer.sln
在 Solution Explorer 對 atpo 右鍵 → 設為啟動專案 → ctrl+F5執行。

