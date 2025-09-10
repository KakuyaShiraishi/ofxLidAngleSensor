#pragma once

#include <CoreFoundation/CoreFoundation.h>
#include <IOKit/hid/IOHIDManager.h>
#include <limits>
#include <cstdint>

class ofxLidAngleSensor {
public:
    bool setup(uint32_t vid = 0x05AC, uint32_t pid = 0x8104,
               uint32_t usagePage = 0x20, uint32_t usage = 0x8A);
    void exit();
    bool isConnected() const { return device_ != nullptr; }
    bool update();
    double getAngle() const { return angleDeg_; }
    int getRawAngle() const { return rawAngle_; }
    
private:
    IOHIDManagerRef manager_ = nullptr;
    IOHIDDeviceRef device_ = nullptr;
    
    
    double angleDeg_ = std::numeric_limits<double>::quiet_NaN();
    int rawAngle_ = -1;
    
    
    uint32_t matchVID_ = 0x05AC;
    uint32_t matchPID_ = 0x8104;
    uint32_t matchUsagePage_ = 0x20;
    uint32_t matchUsage_ = 0x8A;
    
    
    bool openFirstMatchingDevice();
    bool readAngleOnce(double& outDeg, int& outRaw);
    
    
    static CFMutableDictionaryRef makeMatchDict(uint32_t usagePage, uint32_t usage);
    static CFMutableDictionaryRef makeVidPidDict(uint32_t vid, uint32_t pid);
};
