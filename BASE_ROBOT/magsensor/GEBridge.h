#ifndef GEBRIDGE_H
#define GEBRIDGE_H

#include <ArduinoJson.h>
#include "SerialCommand.h"

class GEBridge {
private:
    void connect();
    GEBridge();
public:
    static GEBridge* sharedInstance;
    
    void setup(String name);
    void loop();
    void setName(String serialName);
    void sendData(JsonObject& data);
    SerialCommand& getSerial();
};

#endif

