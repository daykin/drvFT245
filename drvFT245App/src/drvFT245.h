#ifndef DRVFT245_H
#define DRVFT245_H
#include <libftdi1/ftdi.h>

#include <asynPortDriver.h>
#include <epicsThread.h>
#include <epicsExport.h>
#include <iocsh.h>
#include <string.h>

#define serialNumberString      "serialNumber"
#define manufacturerString      "manufacturer"
#define deviceDescriptionString "deviceDescription"
#define deviceIdString          "deviceID"
#define pinDirectionString      "pinDirection"
#define pinSettingString        "pinSetting"
#define ftdiVersionString       "ftdiVersion"

#define FTDI_VENDOR_ID 0x0403
#define FT232BMLQ_PID  0x6001
#define FT245BMLQ_PID  0x6001
#define FT232RLQ_PID   0x6001
#define FT245RLQ_PID   0x6001
#define FT2232CDL_PID  0x6010
#define FT2232HL_PID   0x6010
#define FT4232HLQ_PID  0x6011
#define FT232HLQ_PID   0x6014

class drvFT245 : public asynPortDriver {
    public:
        drvFT245(const std::string& portName);
        drvFT245(const std::string& portName, const unsigned& deviceIndex);
        virtual asynStatus readUInt32Digital(asynUser *pasynUser, epicsUInt32 *value, epicsUInt32 mask);
        virtual asynStatus writeUInt32Digital(asynUser *pasynUser, epicsUInt32 value, epicsUInt32 mask);
        virtual asynStatus readOctet(asynUser *pasynUser, char *value, size_t maxChars, size_t *nActual, int *eomReason);
        //Must be public, called from C
        void pinPollTask();
        int serialNumber;
        #define FIRST_FT245_PARAM FT245serialNumber
        int manufacturer;
        int deviceDescription;
        int deviceID;
        int pinDirection;
        int pinSetting;
        int ftdiVersion;
        #define NUM_PARAMS 7
    private:
        struct ftdi_context *ftdi;
        struct ftdi_version_info version;
};
#endif //DRVFT245_H