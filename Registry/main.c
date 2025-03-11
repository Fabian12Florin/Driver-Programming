#include <windows.h>
#include <stdio.h>

#define SERVICES_KEY_PATH "SYSTEM\\CurrentControlSet\\Services"
#define VALUE_NAME "ImagePath"

void ExpandImagePath(char* imagePath, DWORD valueType) {
    if (valueType == REG_EXPAND_SZ) {
        char expandedPath[512];
        if (ExpandEnvironmentStringsA(imagePath, expandedPath, sizeof(expandedPath)) > 0) {
            printf(" (Expanded: %s)", expandedPath);
        }
    }
}

void ListAllServices() {
    HKEY hKey;
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, SERVICES_KEY_PATH, 0, KEY_READ, &hKey) != ERROR_SUCCESS) {
        printf("Failed to open registry key!\n");
        return;
    }

    char subKeyName[256];
    DWORD subKeyNameSize = 256;
    DWORD index = 0;
    DWORD totalServices = 0;

    while (RegEnumKeyExA(hKey, index, subKeyName, &subKeyNameSize, NULL, NULL, NULL, NULL) == ERROR_SUCCESS) {
        HKEY hSubKey;
        if (RegOpenKeyExA(hKey, subKeyName, 0, KEY_READ, &hSubKey) == ERROR_SUCCESS) {
            char imagePath[512] = { 0 };
            DWORD imagePathSize = sizeof(imagePath);
            DWORD valueType;

            LONG result = RegQueryValueExA(hSubKey, VALUE_NAME, NULL, &valueType, (LPBYTE)imagePath, &imagePathSize);
            if (result == ERROR_SUCCESS && (valueType == REG_SZ || valueType == REG_EXPAND_SZ)) {
                totalServices++;
                printf("Service: %s | ImagePath: %s", subKeyName, imagePath);
                ExpandImagePath(imagePath, valueType);
                printf("\n");
            }
            RegCloseKey(hSubKey);
        }

        subKeyNameSize = 256;
        index++;
    }

    printf("\nTotal: %d services\n", totalServices);
    RegCloseKey(hKey);
}

void ListOnlyDrivers(HKEY hKey) {
    char subKeyName[256];
    DWORD subKeyNameSize = 256;
    DWORD index = 0;
    DWORD totalDrivers = 0;

    while (RegEnumKeyExA(hKey, index, subKeyName, &subKeyNameSize, NULL, NULL, NULL, NULL) == ERROR_SUCCESS) {
        HKEY hSubKey;
        if (RegOpenKeyExA(hKey, subKeyName, 0, KEY_READ, &hSubKey) == ERROR_SUCCESS) {
            DWORD type;
            DWORD typeSize = sizeof(DWORD);
            if (RegQueryValueExA(hSubKey, "Type", NULL, NULL, (LPBYTE)&type, &typeSize) == ERROR_SUCCESS) {
                if (type == 0x01 || type == 0x02) {  // Only process drivers
                    char imagePath[512] = { 0 };
                    DWORD imagePathSize = sizeof(imagePath);
                    DWORD valueType;

                    LONG result = RegQueryValueExA(hSubKey, VALUE_NAME, NULL, &valueType, (LPBYTE)imagePath, &imagePathSize);
                    if (result == ERROR_SUCCESS && (valueType == REG_SZ || valueType == REG_EXPAND_SZ)) {
                        totalDrivers++;
                        printf("Driver: %s | ImagePath: %s", subKeyName, imagePath);
                        ExpandImagePath(imagePath, valueType);
                        printf("\n");
                    }
                    else {
                        printf("Driver: %s | No ImagePath (Error Code: %ld)\n", subKeyName, result);
                    }
                }
            }
            RegCloseKey(hSubKey);
        }
        subKeyNameSize = 256;
        index++;
    }

    printf("\nTotal: %d drivers\n", totalDrivers);
}

void SearchForDrivers() {
    HKEY hKey;
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, SERVICES_KEY_PATH, 0, KEY_READ, &hKey) != ERROR_SUCCESS) {
        printf("Failed to open registry key!\n");
        return;
    }

    ListOnlyDrivers(hKey);
    RegCloseKey(hKey);
}

int main() {
    int choice;

    printf("Select an option:\n");
    printf("1 - List all services\n");
    printf("2 - List only drivers\n");
    printf("3 - Search for drivers\n");
    printf("Option: ");
    scanf_s("%d", &choice);

    if (choice == 1) {
        ListAllServices();
    }
    else if (choice == 2 || choice == 3) {
        SearchForDrivers();
    }
    else {
        printf("Invalid option!\n");
    }

    system("pause");
    return 0;
}
