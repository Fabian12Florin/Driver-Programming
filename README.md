# USB Devices Enumerator 🔌

This branch contains the **USB Devices Enumerator** project, part of the driver programming labs.

## 🛠️ Features
- Lists all connected USB devices using Windows SetupAPI.
- Uses:
  - `SetupDiGetClassDevs()`
  - `SetupDiEnumDeviceInfo()`
  - `SetupDiGetDeviceRegistryProperty()`
- Displays device descriptions directly in the console.

## 🚀 How to Run
1. Open **Visual Studio 2022** and create a new Console App project.
2. Replace `main.cpp` with the provided source code.
3. Select **Release | x64** from the toolbar.
4. Build the project: **Build → Build Solution (Ctrl + Shift + B)**.
5. Run the `.exe` from the output folder: `x64\Release\`.

> ✅ You can also run from inside Visual Studio (F5), but make sure to run it as **Administrator** for full access.
