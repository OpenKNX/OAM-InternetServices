#include "OpenKNX.h"
//#include "Logic.h"
#ifdef IW_ChannelCount
//#include "InternetWeatherModule.h"
#endif
#ifdef ARDUINO_ARCH_ESP32
#include "WLANModule.h"
#include "OTAUpdateModule.h"
#endif
#ifdef SHC_ChannelCount
#include "ShutterControllerModule.h"
#endif
#ifdef SIP_ChannelCount
#include "SIPModule.h"
#endif
#ifdef NET_ModuleVersion
#include "NetworkModule.h"
#endif
#ifdef ARDUINO_ARCH_RP2040
#include "UsbExchangeModule.h"
#include "FileTransferModule.h"
#endif
#ifdef LOG_ChannelCount
#include "Logic.h"
#endif
#ifdef USE_AUTO_CONNECT
#include <AutoConnect.h>
#include <WebServer.h>
WebServer webServer(80);
AutoConnect Portal(webServer);    
AutoConnectConfig config;
#endif

#include "HTTPClient.h"
#include "time.h"
#include <sys/time.h>

#define Abc_LeftShift 2 
#define Abc_Shift 1
#define Abc_Mask 0x3E 
#define Abc_Sign 0x20
#define Abc_Neg 0xE0

void test()
{
    Serial.println("Test");
    uint8_t config = 0b11110111;
    int8_t result = (int8_t)(config << Abc_LeftShift) >> (Abc_LeftShift + Abc_Shift);
    Serial.println("config: ");
    Serial.println(config, BIN);
    Serial.println("result: ");
    Serial.println(result, BIN);
    Serial.println(result, DEC);

    int8_t result2 = ((config & Abc_Mask) >> Abc_Shift) | ((config & Abc_Sign) > 0 ? Abc_Neg : 0);
    Serial.println("result2: ");
    Serial.println(result2, BIN);
    Serial.println(result2, DEC);

    config = 0b00110110;
    result = (int8_t)(config << Abc_LeftShift) >> (Abc_LeftShift + Abc_Shift);
    Serial.println("config: ");
    Serial.println(config, BIN);
    Serial.println("result: ");
    Serial.println(result, BIN);
    Serial.println(result, DEC);

    result2 = ((config & Abc_Mask) >> Abc_Shift) | ((config & Abc_Sign) > 0 ? Abc_Neg : 0);
    Serial.println("result2: ");
    Serial.println(result2, BIN);
    Serial.println(result2, DEC);
}

void setLocalTime()
{

    // Set local time
    setenv("TZ", "CET-1CEST,M3.5.0/02,M10.5.0/03", 1);

    tm tm{}; // Zero initialise
    tm.tm_year = 2074 - 1900; 
    tm.tm_mon = 10 - 1; 
    tm.tm_mday = 27; 
    tm.tm_hour = 2;
    tm.tm_min = 30;
    tm.tm_isdst = 1; 
    std::time_t epoch = mktime(&tm); 
    struct timeval tv;
    tv.tv_sec = epoch;
    tv.tv_usec = 0;

    settimeofday(&tv, NULL);


    time_t now;
    time(&now);
    Serial.println(sizeof(time_t));
    Serial.print("Local: ");
    Serial.println(asctime(localtime(&now)));
    Serial.print("UTC: ");
    Serial.println(asctime(gmtime(&now)));

}

void setUTCTime()
{
    setenv("TZ", "CET-1CEST,M3.5.0/02,M10.5.0/03", 1);

    // Set utc time
  
    tm tm{}; // Zero initialise
    tm.tm_year = 2024 - 1900; 
    tm.tm_mon = 10 - 1; 
    tm.tm_mday = 27; 
    tm.tm_hour = 03;
    tm.tm_min = 30;
    
    std::time_t epoch = mktime(&tm) - _timezone;   
  

    struct timeval tv;
    tv.tv_sec = epoch;
    tv.tv_usec = 0; 
    settimeofday(&tv, NULL);



    time_t now;
    time(&now);
   
    Serial.print("Local: ");
    Serial.println(asctime(localtime(&now)));
    Serial.print("UTC: ");
    Serial.println(asctime(gmtime(&now)));

}

void setup()
{    


    const uint8_t firmwareRevision = 1;
    openknx.init(firmwareRevision);


    setLocalTime();
   //     setUTCTime();
    test();
#ifdef WLAN_WifiSSID    
    openknx.addModule(1, openknxWLANModule);
#endif
#ifdef NET_ModuleVersion
    openknx.addModule(1, openknxNetwork);
#endif    
#ifdef LOG_ChannelCount
    openknx.addModule(2, openknxLogic);
#endif
#ifdef IW_ChannelCount
   // openknx.addModule(3, openknxInternetWeatherModule);
#endif
#ifdef ARDUINO_ARCH_ESP32    
#ifdef WLAN_WifiSSID 
   openknx.addModule(4, openknxOTAUpdateModule);
#endif
#endif
#ifdef ARDUINO_ARCH_RP2040
    openknx.addModule(5, openknxUsbExchangeModule);
    openknx.addModule(6, openknxFileTransferModule);
#endif
#ifdef SIP_ChannelCount
    openknx.addModule(7, openknxSIPModule);
#endif
#ifdef SHC_ChannelCount
    openknx.addModule(8, openknxShutterControllerModule);
#endif
    openknx.setup();
#ifdef USE_AUTO_CONNECT
    config.apid ="OpenKNX";
    config.password = "12345678";
    Portal.config(config);
    Portal.begin();
#endif


}

#define Abc_Shift 1
#define Abc_Offset 0x3E

void loop()
{
#ifdef USE_AUTO_CONNECT
    Portal.handleClient();
#endif
   openknx.loop();
}

#ifdef OPENKNX_DUALCORE
void setup1()
{
    openknx.setup1();
}

void loop1()
{
    openknx.loop1();
}
#endif