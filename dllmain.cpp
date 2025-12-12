#include "pch.h"
#include <windows.h>
#include <slpublic.h>

#pragma comment(lib, "slwga.lib")

// Function pointer type definition
typedef HRESULT(WINAPI* SLGetLicensingStatusInformation_t)(
    HSLC hSLC,
    SLID* pAppID,
    SLID* pProductSkuId,
    PWSTR pwszRightName,
    UINT* pnStatusCount,
    SL_LICENSING_STATUS** ppLicensingStatus
);

// Original function pointer
SLGetLicensingStatusInformation_t OriginalSLGetLicensingStatusInformation = nullptr;

// Global variable
BOOL bIsHeartbeatRegistryModified = FALSE;

// Helper function to check if product is in grace period
BOOL IsGracePeriodProduct(HSLC hSLC, SLID* pSkuId) {
    // Implement logic based on specific product SKU ID
    // Currently returns FALSE, implement your logic as needed
    return FALSE;
}

// Helper function to modify heartbeat registry
void ModifyHeartbeatRegistry() {
    HKEY hKey;
    DWORD dwDisposition;
    
    // Create or open registry key
    if (RegCreateKeyExW(HKEY_LOCAL_MACHINE,
        L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\SoftwareProtectionPlatform",
        0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, &dwDisposition) == ERROR_SUCCESS) {
        
        DWORD dwValue = 1;
        RegSetValueExW(hKey, L"HeartbeatModified", 0, REG_DWORD, (BYTE*)&dwValue, sizeof(dwValue));
        
        RegCloseKey(hKey);
    }
}

// Hook function implementation
HRESULT WINAPI SLGetLicensingStatusInformationHook(
    HSLC hSLC,
    SLID* pAppID,
    SLID* pProductSkuId,
    PWSTR pwszRightName,
    UINT* pnStatusCount,
    SL_LICENSING_STATUS** ppLicensingStatus
) {
    // Call original function
    HRESULT hResult = OriginalSLGetLicensingStatusInformation(
        hSLC,
        pAppID,
        pProductSkuId,
        pwszRightName,
        pnStatusCount,
        ppLicensingStatus
    );

    if (hResult != S_OK)
        return hResult;

    // Modify licensing status information
    for (UINT i = 0; i < *pnStatusCount; i++) {
        if ((*ppLicensingStatus + i)->eStatus == 0) continue;
        if (IsGracePeriodProduct(hSLC, &((*ppLicensingStatus + i)->SkuId))) continue;

        (*ppLicensingStatus + i)->eStatus = (SLLICENSINGSTATUS)1;
        (*ppLicensingStatus + i)->dwGraceTime = 0;
        (*ppLicensingStatus + i)->dwTotalGraceDays = 0;
        (*ppLicensingStatus + i)->hrReason = 0;
        (*ppLicensingStatus + i)->qwValidityExpiration = 0;
    }

    // Modify heartbeat registry
    if (!bIsHeartbeatRegistryModified) {
        ModifyHeartbeatRegistry();
        bIsHeartbeatRegistryModified = TRUE;
    }

    return hResult;
}

// DLL entry point
BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved) {
    HMODULE hSLWGA = NULL;
    
    switch (dwReason) {
    case DLL_PROCESS_ATTACH:
        // Get original function address
        hSLWGA = GetModuleHandleW(L"slwga.dll");
        if (hSLWGA) {
            OriginalSLGetLicensingStatusInformation = (SLGetLicensingStatusInformation_t)
                GetProcAddress(hSLWGA, "SLGetLicensingStatusInformation");
        }
        break;
        
    case DLL_PROCESS_DETACH:
        // Cleanup
        break;
    }
    return TRUE;
}