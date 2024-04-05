#include "GCSAudioControl/AudioActions.h"
#include "GCSAudioControl/Extras.h"

int main(int argc, char** argv)
{
    setNormalLocale();

    if (argc < 2)
    {
        fprintf(stderr, "No params for audio actions have been requested\n");
        PressEnter();
        return 1;
    }

    if (!RequestedParams::setupParams(argv[1]))
    {
        PressEnter();
        return 1;
    }

    RequestedParams::applyParams();
    PressEnter();
    return 0;
}