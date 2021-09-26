#include "utils.h"

int check_admin(){
	int isAdmin = 0;
	HANDLE hToken;

	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
		CloseHandle(hToken);
		return 0;
	}

	DWORD requiredSize;
	TOKEN_ELEVATION elevation;

	if (GetTokenInformation(hToken, TokenElevation, &elevation, sizeof(elevation), &requiredSize)) {
		isAdmin = elevation.TokenIsElevated;
	}
	else {
		const DWORD err = GetLastError();
		if (err == ERROR_INVALID_PARAMETER) {
			printf("check_admin(): Windows versions older than Vista are not supported.\n");
		}
	}

	CloseHandle(hToken);
	return isAdmin;
}

int enum_NIC() {
	HANDLE handle = SetupDiGetClassDevs(&GUID_DEVCLASS_NET, NULL, NULL, DIGCF_PRESENT);
	if (handle == INVALID_HANDLE_VALUE) {
		return 0;
	}

	SP_DEVINFO_DATA deviceInfoData;
	deviceInfoData.cbSize = sizeof(deviceInfoData);

	for (DWORD i = 0; SetupDiEnumDeviceInfo(handle, i, &deviceInfoData); ++i) {
		DWORD requiredSize;

		if (!SetupDiGetDeviceRegistryProperty(handle, &deviceInfoData, SPDRP_FRIENDLYNAME, NULL, NULL, 0, &requiredSize)) {
			const DWORD err = GetLastError();
			if (err != ERROR_INSUFFICIENT_BUFFER) {
				return 0;
			}
		}

		LPCWSTR name = (LPCWSTR)malloc(requiredSize);
		memset(name, 0, requiredSize);

		if (!SetupDiGetDeviceRegistryProperty(handle, &deviceInfoData, SPDRP_FRIENDLYNAME, NULL, name, requiredSize, NULL)) {
			return 0;
		}

		printf("Index: %d\n", i);
		wprintf("Name: %ws\n", name);
	}

	const DWORD err = GetLastError();

	if (err != ERROR_NO_MORE_ITEMS) {
		return 0;
	}

	return 1;
}

int set_NIC(int nic_index, int operation) {
	HANDLE handle = SetupDiGetClassDevs(&GUID_DEVCLASS_NET, NULL, NULL, DIGCF_PRESENT);
	if (handle == INVALID_HANDLE_VALUE || !check_admin()) {
		return 0;
	}

	SP_DEVINFO_DATA deviceInfoData;
	deviceInfoData.cbSize = sizeof(deviceInfoData);

	if (!SetupDiEnumDeviceInfo(handle, nic_index, &deviceInfoData)) {
		return 0;
	}

	SP_PROPCHANGE_PARAMS propChangeParams;
	switch (operation) {
	case NIC_CONTROL_ENABLE:
		propChangeParams.StateChange = DICS_ENABLE;
		break;
	case NIC_CONTROL_DISABLE:
		propChangeParams.StateChange = DICS_DISABLE;
		break;
	default:
		break;
	}
	
	propChangeParams.HwProfile = 0;
	propChangeParams.Scope = DICS_FLAG_CONFIGSPECIFIC;
	propChangeParams.ClassInstallHeader.InstallFunction = DIF_PROPERTYCHANGE;
	propChangeParams.ClassInstallHeader.cbSize = sizeof(propChangeParams.ClassInstallHeader);

	if (!SetupDiSetClassInstallParams(handle, &deviceInfoData, &propChangeParams.ClassInstallHeader, sizeof(propChangeParams))) {
		return 0;
	}

	if (!SetupDiCallClassInstaller(DIF_PROPERTYCHANGE, handle, &deviceInfoData)) {
		SetupDiSetClassInstallParams(handle, &deviceInfoData, NULL, 0);
		return 0;
	}
	return 1;
}
