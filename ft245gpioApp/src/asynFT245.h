#ifndef ASYN_FT245_H
#define ASYN_FT245_H
#include <asynPortDriver.h>
#include <libftdi1/ftdi.h>
#include <unistd.h>

#define serialNumberString      "serialNumber"
#define manufacturerString      "manufacturer"
#define deviceDescriptionString "deviceDescription"
#define deviceIdString          "deviceID"
#define pinDirectionString      "pinDirection"
#define pinSettingString        "pinSetting"
#define ftdiVersionString       "ftdiVersion"

#define FT245_VENDOR_ID 0x0403
#define FT245_DEVICE_ID 0x6001

class drvFT245 : public asynPortDriver {
    public:
        drvFT245(const std::string& portName);
        drvFT245(const std::string& portName, const unsigned& deviceIndex);
        virtual asynStatus readUInt32Digital(asynUser *pasynUser, epicsUInt32 *value, epicsUInt32 mask);
        virtual asynStatus writeUInt32Digital(asynUser *pasynUser, epicsUInt32 value, epicsUInt32 mask);
        int serialNumber;
        #define FIRST_FT245_PARAM serialNumber
        int manufacturer;
        int deviceDescription;
        int deviceId;
        int pinDirection;
        int pinSetting;
        int ftdiVersion;
        #define NUM_PARAMS 7
    private:
        struct ftdi_context *ftdi;
        struct ftdi_version_info version;
        void pinPollTask();
};
#endif //ASYN_FT245_H