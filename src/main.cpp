#include "OpenKNX.h"
//#include "Logic.h"
#ifdef IW_ChannelCount
#include "InternetWeatherModule.h"
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

void setup()
{    
    const uint8_t firmwareRevision = 1;
    openknx.init(firmwareRevision);
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
    openknx.addModule(3, openknxInternetWeatherModule);
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