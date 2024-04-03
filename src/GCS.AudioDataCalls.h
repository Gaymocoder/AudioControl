#ifndef __GCS_AUDIODATACALLS_H__
#define __GCS_AUDIODATACALLS_H__

#include <iostream>
#include <cinttypes>
#include <mmdeviceapi.h>

std::string getDeviceFriendlyName(IMMDevice* device);
IMMDevice* getDevice(const std::string& deviceFriendlyName);

#endif