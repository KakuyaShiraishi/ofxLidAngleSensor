#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    bool ok = sensor.setup();
}

//--------------------------------------------------------------
void ofApp::update(){
    sensor.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0);
    ofSetColor(255);
    double a = sensor.getAngle();
    ofDrawBitmapStringHighlight("Lid angle: " + ofToString(a, 1) + " deg", 20, 40);
    
    ofPushMatrix();
    ofTranslate(250, 250);
    ofSetColor(160);
    ofDrawCircle(0,0, 120);
    ofSetColor(255);
    ofRotateDeg(-90);
    ofDrawLine(0,0, 120 * cos(ofDegToRad(a)), 120 * sin(ofDegToRad(a)));
    ofPopMatrix();
}
//--------------------------------------------------------------
void ofApp::exit(){
    sensor.exit();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY){
    
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}
