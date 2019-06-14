#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    //setup(string _name,int _spansAmount, int _x, int _y)
    timepan_object.setup("timepan_object",3,100,150);
    timepan_object.init();

    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    ofSetWindowTilte(ofToString(ofGetFrameRate()));
    timepan_object.update();
}

//--------------------------------------------------------------
void ofApp::draw(){

     
    int temp_y = 15;
    ofDrawBitmapStringHighlight("key t = trigger sequence ", 10,temp_y+=15);
    ofDrawBitmapStringHighlight("key s = save settings ", 10,temp_y+=15);

    ofDrawBitmapStringHighlight("activeIndex: "+ofToString(timepan_object.activeIndex) +" of "+ ofToString(timepan_object.spanAmount), 10,temp_y+=15);
    ofDrawBitmapStringHighlight("activeValueX: "+ofToString(timepan_object.activeValueX), 10,temp_y+=15);
     ofDrawBitmapStringHighlight("activeValueY: "+ofToString(timepan_object.activeValueY), 10,temp_y+=15);
    ofDrawBitmapStringHighlight("readyForNewTrigger: "+ofToString(timepan_object.readyForNewTrigger), 10,temp_y+=15);

    
//    if(timepan_object.readyForNewTrigger == true){
//        ofLog()<<"timepan_object.readyForNewTrigger "<<timepan_object.readyForNewTrigger;
//        timepan_object.readyForNewTrigger = false;
//    }
    
    ofPushMatrix();
    float temp_X = ofMap(timepan_object.activeValueX, 0, 1, 0, 256*3);

    ofTranslate(400 + temp_X, ofGetHeight() - 200);
    float temp_deg = ofMap(timepan_object.activeValueY, 0, 1, 0, 180);
    ofRotateZDeg(temp_deg);
    ofTranslate(-50, -100);
    ofFill();
    ofSetColor(255, 0, 0);
    ofDrawRectangle(0, 0, 100, 200);
    ofPopMatrix();
    
      timepan_object.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    if(key == 's')    timepan_object.save();
     if(key == 't')    timepan_object.bTrigger = true;
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

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
