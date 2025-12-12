#pragma once

#include <windows.h>
#include <slpublic.h>

// Export declaration for the hook function
extern "C" __declspec(dllexport) HRESULT WINAPI SLGetLicensingStatusInformationHook(
    HSLC hSLC,
    SLID* pAppID,
    SLID* pProductSkuId,
    PWSTR pwszRightName,
    UINT* pnStatusCount,
    SL_LICENSING_STATUS** ppLicensingStatus
);