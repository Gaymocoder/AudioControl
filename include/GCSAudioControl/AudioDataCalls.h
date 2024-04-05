#ifndef __GCS_AUDIODATACALLS_H__
#define __GCS_AUDIODATACALLS_H__

#include <iostream>
#include <cinttypes>
#include <mmdeviceapi.h>

std::string getDeviceFriendlyName(IMMDevice* device);
void getDevice(const std::string &deviceFriendlyName, bool isSubstr, IMMDevice* &device);

#endif