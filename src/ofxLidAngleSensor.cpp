#include "ofxLidAngleSensor.h"
#include <vector>
#include <cmath>


static CFNumberRef cfnum(int v) { return CFNumberCreate(kCFAllocatorDefault, kCFNumberIntType, &v); }


CFMutableDictionaryRef ofxLidAngleSensor::makeMatchDict(uint32_t usagePage, uint32_t usage) {
    auto d = CFDictionaryCreateMutable(kCFAllocatorDefault, 0,
                                       &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
    auto up = cfnum((int)usagePage);
    auto us = cfnum((int)usage);
    CFDictionarySetValue(d, CFSTR(kIOHIDDeviceUsagePageKey), up);
    CFDictionarySetValue(d, CFSTR(kIOHIDDeviceUsageKey), us);
    CFRelease(up); CFRelease(us);
    return d;
}


CFMutableDictionaryRef ofxLidAngleSensor::makeVidPidDict(uint32_t vid, uint32_t pid) {
    auto d = CFDictionaryCreateMutable(kCFAllocatorDefault, 0,
                                       &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
    auto v = cfnum((int)vid);
    auto p = cfnum((int)pid);
    CFDictionarySetValue(d, CFSTR(kIOHIDVendorIDKey), v);
    CFDictionarySetValue(d, CFSTR(kIOHIDProductIDKey), p);
    CFRelease(v); CFRelease(p);
    return d;
}


bool ofxLidAngleSensor::setup(uint32_t vid, uint32_t pid, uint32_t usagePage, uint32_t usage) {
    matchVID_ = vid; matchPID_ = pid; matchUsagePage_ = usagePage; matchUsage_ = usage;
    
    
    manager_ = IOHIDManagerCreate(kCFAllocatorDefault, kIOHIDOptionsTypeNone);
    if (!manager_) return false;
    
    
    CFMutableArrayRef matches = CFArrayCreateMutable(kCFAllocatorDefault, 0, &kCFTypeArrayCallBacks);
    CFArrayAppendValue(matches, makeMatchDict(matchUsagePage_, matchUsage_));
    CFArrayAppendValue(matches, makeVidPidDict(matchVID_, matchPID_));
    IOHIDManagerSetDeviceMatchingMultiple(manager_, matches);
    CFRelease(matches);
    
    
    if (IOHIDManagerOpen(manager_, kIOHIDOptionsTypeNone) != kIOReturnSuccess) return false;
    
    
    return openFirstMatchingDevice();
}


bool ofxLidAngleSensor::openFirstMatchingDevice() {
    if (!manager_) return false;

    CFSetRef devSet = IOHIDManagerCopyDevices(manager_);
    if (!devSet) return false;

    CFIndex count = CFSetGetCount(devSet);
    if (count <= 0) {
        CFRelease(devSet);
        return false;
    }

    std::vector<IOHIDDeviceRef> devices(count);
    CFSetGetValues(devSet, (const void**)devices.data());
    CFRelease(devSet);

    for (IOHIDDeviceRef d : devices) {
        if (!d) continue;
        if (IOHIDDeviceOpen(d, kIOHIDOptionsTypeNone) == kIOReturnSuccess) {
            device_ = d;
            double tmpDeg; int tmpRaw;
            if (readAngleOnce(tmpDeg, tmpRaw)) {
                angleDeg_ = tmpDeg;
                rawAngle_ = tmpRaw;
                return true;
            }
            IOHIDDeviceClose(d, kIOHIDOptionsTypeNone);
            device_ = nullptr;
        }
    }
    return false;
}


bool ofxLidAngleSensor::readAngleOnce(double& outDeg, int& outRaw) {
    if (!device_) return false;
    
    uint8_t buf[64] = {0};
    buf[0] = 0x01;
    CFIndex len = sizeof(buf);
    
    
    IOReturn r = IOHIDDeviceGetReport(device_, kIOHIDReportTypeFeature, buf[0], buf, &len);
    if (r != kIOReturnSuccess || len < 3) return false;
    
    
    uint16_t raw = (uint16_t)buf[1] | ((uint16_t)buf[2] << 8); // little-endian
    
    
    double deg = (raw > 360) ? (raw / 100.0) : (double)raw;
    
    
    if (deg < 0) deg = 0;
    if (deg > 360) deg = 360;
    
    
    outDeg = deg; outRaw = (int)raw;
    return true;
}


bool ofxLidAngleSensor::update() {
    double d; int raw;
    if (readAngleOnce(d, raw)) { angleDeg_ = d; rawAngle_ = raw; return true; }
    
        if (device_) {
        IOHIDDeviceClose(device_, kIOHIDOptionsTypeNone);
        device_ = nullptr;
    }
    return openFirstMatchingDevice();
}


void ofxLidAngleSensor::exit() {
    if (device_) {
        IOHIDDeviceClose(device_, kIOHIDOptionsTypeNone);
        device_ = nullptr;
    }
    if (manager_) {
        IOHIDManagerClose(manager_, kIOHIDOptionsTypeNone);
        CFRelease(manager_);
        manager_ = nullptr;
    }
}
