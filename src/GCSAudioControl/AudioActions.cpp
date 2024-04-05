#include "GCSAudioControl/AudioActions.h"
#include "GCSAudioControl/AudioDataCalls.h"
#include "GCSAudioControl/Extras.h"

#include <fstream>
#include <Windows.h>
#include <endpointvolume.h>
#include <initguid.h>
#include <FunctionDiscoveryKeys_devpkey.h>

double RequestedParams::new_volume_level = 0.0;
std::string RequestedParams::device_friendly_name = "";

int RequestedParams::setupParams(const char* path)
{
    std::string buf = "";
    std::ifstream paramsFile(path, std::ios::in);
    if (!std::getline(paramsFile, buf))
    {
        fprintf(stderr, "Wrong file format: 0 lines exist while 2 have been requested\nThe file: %s\n", path);
        return 0;
    }

    try
    {
        RequestedParams::new_volume_level = ((double) std::stoul(buf)) / 100;
    }
    catch (std::invalid_argument err)
    {
        fprintf(stderr, "Wrong file format: first line must be an integer\nThe file: \"%s\"\n", path);
        return 0;
    }
    
    if (!std::getline(paramsFile, buf))
    {
        fprintf(stderr, "Wrong file format: 0 lines exist while 2 have been requested\nThe file: %s\n", path);
        return 0;
    }
    RequestedParams::device_friendly_name = buf;

    paramsFile.close();
    return 1;
}

double RequestedParams::newVolumeLevel()
{
    return RequestedParams::new_volume_level;
}

std::string RequestedParams::deviceFriendlyName()
{
    return RequestedParams::device_friendly_name;
}

void RequestedParams::applyParams()
{
    CoInitialize(NULL);
    IAudioEndpointVolume *deviceEndpointVolume = NULL;
    HRESULT result;

    double newVolumeLevel = RequestedParams::newVolumeLevel();
    std::string deviceFriendlyName = RequestedParams::deviceFriendlyName();
    IMMDevice* requestedDevice = NULL;
    getDevice(deviceFriendlyName, true, requestedDevice);
    if (!requestedDevice)
    {
        CoUninitialize();
        return;
    }

    printf("device has been found\n");
    deviceFriendlyName = getDeviceFriendlyName(requestedDevice);

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
    
    fprintf(stderr, "Volume of the \"%s\" device has been set to %i%% successfully\n\n", deviceFriendlyName.c_str(), (int) (newVolumeLevel * 100));
}

