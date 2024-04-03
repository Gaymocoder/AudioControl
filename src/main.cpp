#include "GCS.AudioDataCalls.h"
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

    IMMDevice* requestedDevice = getDevice(deviceName);

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