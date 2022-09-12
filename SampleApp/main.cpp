#include "Spectrum/Spectrum.h"
#include <iostream>
#include <Windows.h>
#include <hidsdi.h>
#include <setupapi.h>
#include <string.h>
#include <array>
#include <vector>
GUID HIDClassGuid;

int main(int argc, char** argv)
{
	BOOL found = false;
	HidD_GetHidGuid(&HIDClassGuid);
	HDEVINFO DeviceInfoSet = SetupDiGetClassDevs(&HIDClassGuid, NULL, NULL, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
	//HDEVINFO DeviceInfoSet = SetupDiGetClassDevs(NULL, NULL, NULL, DIGCF_PRESENT | DIGCF_ALLCLASSES);
	UINT16 index = 0;;
	SP_DEVINFO_DATA DeviceInfoData;
	SP_DEVICE_INTERFACE_DATA deviceData;
	ZeroMemory(&deviceData, sizeof(SP_DEVICE_INTERFACE_DATA));
	ZeroMemory(&DeviceInfoData, sizeof(SP_DEVINFO_DATA));
	deviceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
	DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
	HANDLE DeviceHandle = NULL;
	DWORD interfaceInfoSize = 0;
	HIDD_ATTRIBUTES DeviceAttributes;
	PHIDP_PREPARSED_DATA PreparsedData;
	HIDP_CAPS Caps;
	while (!found && SetupDiEnumDeviceInfo(DeviceInfoSet, index, &DeviceInfoData))
	{
		SetupDiEnumDeviceInterfaces(DeviceInfoSet, NULL, &HIDClassGuid, index, &deviceData);
		SetupDiGetDeviceInterfaceDetail(DeviceInfoSet, &deviceData, NULL, 0, &interfaceInfoSize, NULL);
		PSP_DEVICE_INTERFACE_DETAIL_DATA deviceInterfaceDetail = (PSP_DEVICE_INTERFACE_DETAIL_DATA)malloc(interfaceInfoSize);
		if (deviceInterfaceDetail)
		{
			deviceInterfaceDetail->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

			ZeroMemory(deviceInterfaceDetail->DevicePath, sizeof(deviceInterfaceDetail->DevicePath));
		}
		if (SetupDiGetDeviceInterfaceDetail(DeviceInfoSet, &deviceData, deviceInterfaceDetail, interfaceInfoSize, NULL, NULL))
		{
			DeviceHandle = CreateFile(deviceInterfaceDetail->DevicePath, (GENERIC_WRITE | GENERIC_READ),
				FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
			if (DeviceHandle != INVALID_HANDLE_VALUE) {
				if (HidD_GetAttributes(DeviceHandle, &DeviceAttributes))
				{
					if (DeviceAttributes.VendorID == 0x048D && (DeviceAttributes.ProductID == 0xC977))
					{
						std::cout << "Found: ";
						std::wcout << (deviceInterfaceDetail->DevicePath) << "\n";
						DWORD result = HidD_GetPreparsedData(DeviceHandle, &PreparsedData);

						result = HidP_GetCaps(PreparsedData, &Caps);
						HidD_FreePreparsedData(PreparsedData);
						if (Caps.FeatureReportByteLength == 960)
						{
							found = true;
							std::cout << "Found 960: ";
							std::wcout << (deviceInterfaceDetail->DevicePath) << "\n";
						}
					}
					else
					{
						CloseHandle(DeviceHandle);
					}
				}
			}

		}
		else
		{
			wchar_t str[256];
			FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				str, (sizeof(str) / sizeof(wchar_t)), NULL);
			std::wcout << "err: " << str << "\n";
		}
		free(deviceInterfaceDetail);

		index++;
	}
	SetupDiDestroyDeviceInfoList(DeviceInfoSet);


	//verify that the handle has already been created
	if (found)
	{

		Spectrum Spectrum;
		uint8_t* data = Spectrum.SetBacklight(7);
		HidD_SetFeature(DeviceHandle, data, 960);

		data = Spectrum.SetProfile_1(4);
		HidD_SetFeature(DeviceHandle, data, 960);

		data = Spectrum.SetProfile_2(4);
		HidD_SetFeature(DeviceHandle, data, 960);

		Spectrum.AddEffect(4, { KeyMap.at("Esc") }, {{255,255,255}}, Always, 0, 0, 0, ColorList, 0);
		Spectrum.AddEffect(4, { KeyMap.at("F1") }, { {255,0,0},{0,0,255} }, Color_Wave, 0, 0, 0, ColorList, 0);
		data = Spectrum.AddEffect(4, { KeyMap.at("F2") }, { {25,45,0},{0,45,0} }, Color_Pulse, 0, 0, 0, ColorList, 0);

		HidD_SetFeature(DeviceHandle, data, 960);

		Sleep(500);
	}

}
