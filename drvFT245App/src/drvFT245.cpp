#include "drvFT245.h"

static const char* driverName = "asynFT245";

static void pinPollTaskC(void* drvPvt){
    drvFT245 *pPvt = (drvFT245 *)drvPvt;
    pPvt->pinPollTask();
}

drvFT245::drvFT245(const std::string& portName)
    :asynPortDriver(portName.c_str(), //portName
    0,//Max Signals
    asynUInt32DigitalMask|asynOctetMask|asynInt32Mask|asynDrvUserMask,//iface mask
    asynUInt32DigitalMask,//interrupt mask
    0,//asynFlags
    1,//autoConnect
    0,//priority
    0) //stackSize)
    {
        drvFT245(portName, 0);
    }

drvFT245::drvFT245(const std::string& portName, const unsigned& deviceIndex)
    :asynPortDriver(portName.c_str(),
    0,//Max Signals
    asynUInt32DigitalMask|asynOctetMask|asynDrvUserMask,//iface mask
    asynUInt32DigitalMask,//interrupt mask
    0,//asyn flags
    1,//autoConnect
    0,//priority
    0)//stackSize
    {
        const char *functionName = "drvFT245";
        int status = asynSuccess;
        status |= createParam(serialNumberString,       asynParamOctet,         &serialNumber);
        status |= createParam(manufacturerString,       asynParamOctet,         &manufacturer);
        status |= createParam(deviceDescriptionString,  asynParamOctet,         &deviceDescription);
        status |= createParam(deviceIdString,           asynParamInt32,         &deviceID);
        status |= createParam(pinDirectionString,       asynParamUInt32Digital, &pinDirection);
        status |= createParam(pinSettingString,         asynParamUInt32Digital, &pinSetting);
        status |= createParam(ftdiVersionString,        asynParamOctet,         &ftdiVersion);
        if((ftdi=ftdi_new())==0){
            printf("%s:%s: Unable to initialize libftdi context.\n", driverName, functionName);
            status=asynError;
        }
        else{
            printf("%s:%s: Initialized libftdi context.\n", driverName, functionName);
        }
        char *versionString[8];
        version = ftdi_get_library_version();
        sprintf(*versionString, "%d.%d.%d", version.major, version.minor, version.micro);
        //status |= setStringParam(ftdiVersion, (const char*)versionString);

        struct ftdi_device_list *devices, *device, *deviceToOpen;
        int nDevices = ftdi_usb_find_all(ftdi, &devices, 0x0, 0x0);
        if(nDevices < 0){
            printf("%s:%s: Unable to enumerate FT245 devices (usb_find_all() failed).\n", driverName, functionName);
        }
        else{
            char mfr[128], desc[128], serial[128];
            int i=0;
            int usedDeviceIndex=deviceIndex;
            if (deviceIndex>(unsigned)nDevices){
                printf("%s:%s: Index %d was specified but there are only %d devices seen. Defaulting to 0.\n", driverName, functionName, nDevices, deviceIndex);
                usedDeviceIndex = 0;
            }
            printf("%s:%s: I see the following devices:\n", driverName, functionName);
            //default initialization to first/only available device
            deviceToOpen=devices;
            device=devices;
            do{
                ftdi_usb_get_strings(ftdi, device->dev, (char*)mfr, 128, (char*)desc, 128, (char*)serial, 128);
                printf("Index %d: Manufacturer %s, Description %s, Serial %s\n", i, mfr, desc, serial);
                if(i==usedDeviceIndex){
                    deviceToOpen=device;
                    lock();
                    setIntegerParam(deviceID, i);
                    setStringParam(serialNumber,(strlen(serial)==0)?"No serial specified":serial);
                    setStringParam(deviceDescription, (strlen(desc)==0)?"No description specified":desc);
                    setStringParam(manufacturer, (strlen(mfr)==0)?"No manufacturer Specified":mfr);
                    unlock();
                }
                if(device->next!=NULL)device=device->next;
                i++;
            }while(device->next!=NULL);
            if(ftdi_usb_open_dev(ftdi, deviceToOpen->dev)<0){
                printf("%s:%s: Unable to open FTDI device (usb_open() failed).\n", driverName, functionName);
            }
            else{
                printf("%s:%s: Opened device connection.\n", driverName, functionName);
            }
            ftdi_list_free(&devices);
            epicsThreadCreate("pinPollTask", 0, 0, &pinPollTaskC, this);
        }
    }

asynStatus drvFT245::readUInt32Digital(asynUser *pasynUser, epicsUInt32 *value, epicsUInt32 mask){
    int status = asynSuccess;
    const char *functionName="readUInt32Digital";
    const int function(pasynUser->reason);
    if(function==pinSetting){
        unsigned char pinStates = 0x0;
        if(ftdi_read_pins(ftdi, &pinStates) <0){
            asynPrint(pasynUserSelf, ASYN_TRACE_ERROR, "%s:%s:Failed to read pin states.\n", driverName, functionName);
            status = asynError;
        }
        else{
            asynPrint(pasynUserSelf, ASYN_TRACEIO_DEVICE, "%s:%s pin state: %#04x", driverName, functionName, (unsigned)pinSetting);
            *value = pinStates;
        }
    }
    asynPortDriver::readUInt32Digital(pasynUser, value, mask);
    return (asynStatus)status;
}

asynStatus drvFT245::readOctet(asynUser *pasynUser, char *value, size_t maxChars, size_t *nActual, int *eomReason){
    asynStatus status =  asynPortDriver::readOctet(pasynUser, value, maxChars, nActual, eomReason);
    callParamCallbacks();
    return status;
}

asynStatus drvFT245::writeUInt32Digital(asynUser *pasynUser, epicsUInt32 value, epicsUInt32 mask){
    int status = asynSuccess;
    const char *functionName="writeUInt32Digital";
    int function(pasynUser->reason);
    if(function == pinDirection){
        value &= mask & 0xFF;
        if(ftdi_set_bitmode(ftdi, (unsigned char)value, BITMODE_BITBANG) < 0){
            asynPrint(pasynUserSelf, ASYN_TRACE_ERROR, "%s:%s:Failed to set pin directions.\n", driverName, functionName);
            status = asynError;
        }
        else{
            asynPrint(pasynUserSelf, ASYN_TRACEIO_DEVICE, "%s:%s set pin directions: %#04x", driverName, functionName, (unsigned)value);
        }
    }
    else if(function == pinSetting){
        value &= mask & 0xFF;
        if(ftdi_write_data(ftdi, (unsigned char*)&value, 1) < 0){
            asynPrint(pasynUserSelf, ASYN_TRACE_ERROR, "%s:%s:Failed to set pin values.\n", driverName, functionName);
            status = asynError;
        }
        else{
            asynPrint(pasynUserSelf, ASYN_TRACEIO_DEVICE, "%s:%s pin values written: %#04x", driverName, functionName, (unsigned)value);
        }
    }
    status |= asynPortDriver::writeUInt32Digital(pasynUser, value, mask);
    return (asynStatus)status;
}


void drvFT245::pinPollTask(){
    //No interrupt line is available. continuously sample D0-D7 at 10Hz
    //raise a software I/O Intr and update pinStates when something changes
    unsigned char lastValue = 0x00;
    unsigned char value = 0x00;
    int ret = 0;
    const char *functionName = "pinPollTask";
    while(1){
        epicsThreadSleep(0.1);
        ret = ftdi_read_pins(ftdi, &value);
        if(ret < 0){
            asynPrint(pasynUserSelf, ASYN_TRACE_ERROR, "%s:%s: Driver Failed to read pins (%d)\n", driverName, functionName, ret);
        }
        if(value != lastValue){
            lock();
            setUIntDigitalParam(pinSetting, (epicsUInt32)value, 0xFF);
            unlock();
            callParamCallbacks();
            asynPrint(pasynUserSelf, ASYN_TRACE_FLOW, "%s:%s:Detected change on pins (%#04x->%#04x)\n",driverName, functionName, lastValue, value);
        }
        lastValue=value;
    }
}

extern "C" int drvFT245Configure(const char *portName, const unsigned& deviceIndex){
    new drvFT245(portName, deviceIndex);
    return asynSuccess;
}

static const iocshArg FT245DriverConfigArg0 = {"Port Name", iocshArgString};
static const iocshArg FT245DriverConfigArg1 = {"Device Index", iocshArgInt};

static const iocshArg * const FT245DriverConfigArgs[] = {&FT245DriverConfigArg0,
                                                         &FT245DriverConfigArg1};

static const iocshFuncDef configFT245Driver = {"drvFT245Configure", 2, FT245DriverConfigArgs};
static void configFT245DriverCallFunc(const iocshArgBuf *args)
{
    drvFT245Configure(args[0].sval, args[1].ival);
}

static void drvFT245Register(void)
{
    iocshRegister(&configFT245Driver, configFT245DriverCallFunc);
}

extern "C" {
    epicsExportRegistrar(drvFT245Register);
}