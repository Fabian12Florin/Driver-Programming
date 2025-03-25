#include <windows.h>
#include <setupapi.h>
#include <initguid.h>
#include <devguid.h>
#include <cfgmgr32.h>
#include <iostream>

#pragma comment(lib, "setupapi.lib")

int main() {
    // Obține lista dispozitivelor din clasa USB
    HDEVINFO usbSet = SetupDiGetClassDevs(
        &GUID_DEVCLASS_USB, // tipul clasei: USB
        nullptr,
        nullptr,
        DIGCF_PRESENT | DIGCF_PROFILE
    );

    if (usbSet == INVALID_HANDLE_VALUE) {
        std::cerr << "Eroare: nu s-a putut accesa lista de dispozitive USB.\n";
        return -1;
    }

    SP_DEVINFO_DATA deviceData;
    deviceData.cbSize = sizeof(SP_DEVINFO_DATA);

    int index = 0;

    while (SetupDiEnumDeviceInfo(usbSet, index, &deviceData)) {
        TCHAR desc[1024];
        DWORD type = 0, length = 0;

        if (SetupDiGetDeviceRegistryProperty(
            usbSet,
            &deviceData,
            SPDRP_DEVICEDESC,
            &type,
            (PBYTE)desc,
            sizeof(desc),
            &length
        )) {
            std::wcout << L"USB #" << index << L": " << desc << std::endl;
        }

        index++;
    }

    SetupDiDestroyDeviceInfoList(usbSet);
    return 0;
}
