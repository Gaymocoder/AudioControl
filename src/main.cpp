#include <iostream>
#include <string>

void ChangeDeviceVolume(const std::string &deviceName, const short int &newVolume)
{
    printf("Device \"%s\" volume has been changed to %i\n", deviceName.c_str(), newVolume);
}

int main(int argc, char** argv)
{
    short int newVolume = 0;
    if (argc < 2)
    {
        fprintf(stderr, "Wrong numer of command-line arguments was entered\n");
        return 1;
    }

    try
    {
        newVolume = std::stoi(argv[1]);
    }
    catch (std::invalid_argument const &e)
    {
        fprintf(stderr, "Wrong command-line argument on position 1\n");
        return 2;
    }

    ChangeDeviceVolume("USB PnP Audio Device", newVolume);
    return 0;
}