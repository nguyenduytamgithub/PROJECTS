#include "GEBridge.h"

SerialCommand serial;
bool isConnected = false;
String serialName;

void connected();

GEBridge* GEBridge::sharedInstance = new GEBridge();

GEBridge::GEBridge() {
}

void GEBridge::setup(String name) {
    serialName = String(name);
    serial.addCommand("ge_bridge_identified", connected);
}

void GEBridge::loop() {
    serial.readSerial();
    this->connect();
}

void GEBridge::sendData(JsonObject& data) {
    if (!isConnected) {
        return;
    }
    data["type"] = "send_data";
    data["serial_name"] = serialName;
    data.prettyPrintTo(Serial);
}

void GEBridge::connect() {
    if (isConnected) {
        return;
    }
    StaticJsonBuffer<200> jsonBuffer;
    JsonObject& data = jsonBuffer.createObject();
    data["type"] = "connect";
    data["serial_name"] = serialName;
    data.prettyPrintTo(Serial);
}

SerialCommand& GEBridge::getSerial() {
    return serial;
}

void connected() {
    isConnected = true;
}
