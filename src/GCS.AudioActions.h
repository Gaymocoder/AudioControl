#ifndef __GCS_AUDIOACTIONS_H__
#define __GCS_AUDIOACTIONS_H__

#include <iostream>

class RequestedParams
{
    private:
        static double new_volume_level;
        static std::string device_friendly_name;

    public:
        static double newVolumeLevel();
        static std::string deviceFriendlyName();

        static int setupParams(const char* path);
        static void applyParams();
};

#endif