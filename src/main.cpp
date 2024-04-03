#include "GCS.Extras.h"

#include <cstdint>
#include <string>

#include <mmdeviceapi.h>
#include <Windows.h>
#include <endpointvolume.h>
#include <initguid.h>
#include <FunctionDiscoveryKeys_devpkey.h>

void ChangeDeviceVolume(const std::string &deviceName, const double &newVolumeLevel)
{
    CoInitialize(NULL);
    HRESULT hr;
    uint32_t deviceCount = 0;
    IMMDevice* requestedDevice = NULL;
    IMMDeviceEnumerator* deviceEnumerator = NULL;
    IMMDeviceCollection* deviceCollection = NULL;

    hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL,
        __uuidof(IMMDeviceEnumerator), (LPVOID*) &deviceEnumerator);
    hr = deviceEnumerator->EnumAudioEndpoints(eAll, DEVICE_STATE_ACTIVE,
        &deviceCollection);
    deviceCollection->GetCount(&deviceCount);

    for(uint32_t i = 0; i < deviceCount; ++i)
    {
        IMMDevice* device = NULL;
        fprintf(stdout, "Getting device #%i — ", i+1);
        if (FAILED(deviceCollection->Item(i, &device)))
        {
            fprintf(stderr, "Failed to get device #%i\n", i+1);
            continue;
        }
    
        IPropertyStore *propertyStore;
        PROPVARIANT propVariant;
        if (FAILED(device->OpenPropertyStore(STGM_READ, &propertyStore)))
        {
            PropVariantClear(&propVariant);
            propertyStore->Release();

            fprintf(stderr, "Failed to open property store for requested device\n");
            continue;
        }

        if (FAILED(propertyStore->GetValue(PKEY_Device_FriendlyName, &propVariant)))
        {
            PropVariantClear(&propVariant);
            propertyStore->Release();

            fprintf(stderr, "Failed to get friendly name for requested device");
            continue;
        }
        std::string deviceFriendlyName = wstrToStr(propVariant.pwszVal);

        PropVariantClear(&propVariant);
        propertyStore->Release();

        fprintf(stderr, "— %s — ", deviceFriendlyName.c_str());

        if (deviceFriendlyName == deviceName)
        {
            if (deviceEnumerator) deviceEnumerator->Release();
            if (deviceCollection) deviceCollection->Release();
            requestedDevice = device;

            break;
        }

        fprintf(stderr, "wrong device\n");
    }

    if (requestedDevice == NULL)
    {
        fprintf(stderr, "Cannot find requested audio device (%s)\n", deviceName.c_str());
        return;
    }
    printf("device has been found\n");
    
    IAudioEndpointVolume *deviceEndpointVolume = NULL;
    HRESULT result;

    if (FAILED(requestedDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL, NULL,(LPVOID*) &deviceEndpointVolume)))
    {
        if (deviceEndpointVolume) deviceEndpointVolume->Release();
        if (requestedDevice) requestedDevice->Release();
        CoUninitialize();

        fprintf(stderr, "Failed to activate the device\n");
        return;
    }

    if (FAILED(deviceEndpointVolume->SetMasterVolumeLevelScalar(newVolumeLevel, NULL)))
    {
        if (deviceEndpointVolume) deviceEndpointVolume->Release();
        if (requestedDevice) requestedDevice->Release();
        CoUninitialize();
        
        fprintf(stderr, "Failed to set the volume for the device\n");
        return;
    }
    
    if (deviceEndpointVolume) deviceEndpointVolume->Release();
    if (requestedDevice) requestedDevice->Release();
    CoUninitialize();
    
    fprintf(stderr, "Volume of the \"%s\" device has been set to %i%% successfully\n\n", deviceName.c_str(), (int) (newVolumeLevel * 100));

}

int main(int argc, char** argv)
{
    setNormalLocale();
    double newVolume = 0;
    if (argc < 2)
    {
        fprintf(stderr, "Wrong numer of command-line arguments was entered\n");
        return 1;
    }

    try
    {
        newVolume = ((double) std::stoul(argv[1])) / 100;
    }
    catch (std::invalid_argument const &e)
    {
        fprintf(stderr, "Wrong command-line argument on position 1\n");
        return 2;
    }

    ChangeDeviceVolume("Динамики (Realtek(R) Audio)", newVolume);
    return 0;
}