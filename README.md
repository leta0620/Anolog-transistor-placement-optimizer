# Anolog-transistor-placement-optimizer
A small C++ project organized with CMake so it can be built on Visual Studio (Windows) and g++/Clang (Linux/macOS).
使用 CMake 管理，支援 Visual Studio（Windows） 與 g++/Clang（Linux/macOS）編譯。

## Directory Layout | 專案結構
```
├─ CMakeLists.txt
├─ header/
│  ├─ InitialPlacement.h
│  ├─ SAManager.h
|  ├─ CostTableManager.h
│  ├─ TableManager.h
|  ├─ DeviceUnit.h
|  └─ test.h
├─ src/
│  ├─ main.cpp
│  ├─ InitialPlacement.cpp
│  ├─ SAManager.cpp
|  ├─ CostTableManager.cpp
│  ├─ TableManager.cpp
|  ├─ DeviceUnit.cpp
|  └─ test.cpp
└─ build/          # out-of-source build directory（建議用來放所有編譯產物，此資料夾中的內容會被ignore忽略，不會汙染倉庫）
```

## Prerequisites | 先決條件
```
1. 安裝Visual Studio 2022。
2. 安裝CMAKE(CMake ≥ 3.20)。
```

## Build & Run on Windows (Visual Studio) | 在 Windows / VS 編譯與執行
用 CMake 產生 .sln，使用CMAKE，並輸入：
```
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Debug
```

接著用 VS 打開：build/AnalogTransistorPlacementOptimizer.sln
在 Solution Explorer 對 atpo 右鍵 → 設為啟動專案 → ctrl+F5執行。

## Testinng
在傳入的參數中，若僅傳入一個參數，會進入測試模式(test class)，請不要更動main funtion中的內容，將你的test function定義在test class中，並透過test class的建構式運行實作(test.cpp)。

## Coding Style
變數：首字母小寫，後續字母大寫開頭。
``` e.g. string circuitType;```

函式、類別：全字母開頭大寫。
``` 
e.g. InItialPlacenent initialPlacement();
e.g. class SAManager{};
```

若要新增variable/function，請新增在類別中的private or protected中，保持class的封裝，需新增public variable/function請先和其他人確認此部分是否會影響並討論其合理性。