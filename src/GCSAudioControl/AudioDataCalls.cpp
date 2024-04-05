#include "GCSAudioControl/AudioDataCalls.h"
#include "GCSAudioControl/Extras.h"

#include <fstream>
#include <endpointvolume.h>
#include <initguid.h>
#include <FunctionDiscoveryKeys_devpkey.h>

std::string getDeviceFriendlyName(IMMDevice* device)
{
    IPropertyStore *propertyStore;
    PROPVARIANT propVariant;
    if (FAILED(device->OpenPropertyStore(STGM_READ, &propertyStore)))
    {
        PropVariantClear(&propVariant);
        propertyStore->Release();

        fprintf(stderr, "Failed to open property store for requested device\n");
        return "";
    }

    if (FAILED(propertyStore->GetValue(PKEY_Device_FriendlyName, &propVariant)))
    {
        PropVariantClear(&propVariant);
        propertyStore->Release();

        fprintf(stderr, "Failed to get friendly name for requested device");
        return "";
    }
    std::string deviceFriendlyName = wstrToStr(propVariant.pwszVal);

    PropVariantClear(&propVariant);
    propertyStore->Release();
    return deviceFriendlyName;
}

void getDevice(const std::string &deviceFriendlyName, bool isSubstr, IMMDevice* &device)
{
    fprintf(stderr, "Seeking for device \"%s\"...\n", deviceFriendlyName.c_str());
    uint32_t deviceCount;
    device = NULL;
    IMMDeviceEnumerator* deviceEnumerator = NULL;
    IMMDeviceCollection* deviceCollection = NULL;

    CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL,
        __uuidof(IMMDeviceEnumerator), (LPVOID*) &deviceEnumerator);
    deviceEnumerator->EnumAudioEndpoints(eAll, DEVICE_STATE_ACTIVE,
        &deviceCollection);
    deviceCollection->GetCount(&deviceCount);

    for(uint32_t i = 0; i < deviceCount; ++i)
    {
        fprintf(stdout, "Getting device #%i — ", i+1);
        if (FAILED(deviceCollection->Item(i, &device)))
        {
            fprintf(stderr, "Failed to get device #%i\n", i+1);
            continue;
        }
        std::string currDeviceFriendlyName = getDeviceFriendlyName(device);
        fprintf(stderr, "— %s — ", currDeviceFriendlyName.c_str());

        if (isSubstr && currDeviceFriendlyName.find(deviceFriendlyName) != std::string::npos
            || !isSubstr && currDeviceFriendlyName == deviceFriendlyName)
        {
            if (deviceEnumerator) deviceEnumerator->Release();
            if (deviceCollection) deviceCollection->Release();

            return;
        }

        fprintf(stderr, "wrong device\n");
    }

    if (device) device->Release();
    if (deviceEnumerator) deviceEnumerator->Release();
    if (deviceCollection) deviceCollection->Release();
    
    device = NULL;

    fprintf(stderr, "Requested device has not been found.\n");
    return;
}