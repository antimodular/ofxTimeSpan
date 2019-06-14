//
//  oneSpan.h
//  timeLine
//
//  Created by Stephan Schulz on 2018-04-13.
//

//one timeline span

#ifndef oneSpan_h
#define oneSpan_h

#include "ofxCurvesTool.h"
#include "ofxTiming.h"


class oneSpan {
    
public:
    int myID;
    string timelineName;
    string spanLabel;
    
    ofParameterGroup parameter_span;
    //    ofParameter<bool> bUse;
    //    ofParameter<bool> bShow;
    ofParameter<bool> bResetCurve;
    ofParameter<bool> bActive;
    ofParameter<int> curveTypeIndex;
    ofParameter<string> curveTypeName;
    string curveTypeNames[2];
    
    ofParameter<float> expo_value;
    bool old_bActive;
    bool makeActive;
    ofParameter<float> duration;
    
    ofParameter<float> lerpPercent;
    float old_duration;
    
    float lerpValue;
    float curveValue;
    float curveValueNormalized;
    
    bool bSave;
    bool bLoad;
    
    bool bDone;
    int amount;
    //    float cnt;
    
    ofParameter<float> endPause;
    float endTimer;
    bool bStartPause;
    
    LerpTimer lerpTimer;
    
    ofxCurvesTool curvesTool;
    
    bool justBegan;
    bool justEnded;
    
    void setup(int _id, string _timelineName, string _spanLabel){
        myID = _id;
        timelineName = _timelineName;
        spanLabel = _spanLabel;
        
        curveTypeNames[0] = "curveTool";
        curveTypeNames[1] = "exponential";
        
        parameter_span.setName("span_"+ofToString(myID)+"_"+spanLabel);
        parameter_span.add(bActive.set("bActive", false));
        parameter_span.add(curveTypeIndex.set("curveType", 0,0,1));
        parameter_span.add(curveTypeName.set("typeName",""));
        parameter_span.add(expo_value.set("expo_value", 1,0,5));
        
        parameter_span.add(bResetCurve.set("resetCurve", false));
        parameter_span.add(duration.set("duration",10,0,300));
        parameter_span.add(endPause.set("endPause",10,0,300));
        
        amount = 256;
        curvesTool.setup(amount);
        curvesTool.load("curves_"+ofToString(myID)+".yml"); //needed because it fills polyline
        
        //        lerpTimer.lerpToValue(amount);
        
    }
    void init(){
        bActive = false;
        old_bActive = bActive;
        makeActive = false;
        
        bResetCurve = false;
        
        bSave = false;
        bLoad = true;
        
        bDone = false;
        
        bStartPause = false;
        endTimer = ofGetElapsedTimef();
        
        lerpPercent = 0;
        
        checkGui();
        
        lerpTimer.setDuration(duration);
    }
    
    void update(){
        //        checkGui();
        
        
        //        ofLog()<<"curveTypeNames[curveTypeIndex] "<<curveTypeNames[curveTypeIndex];
        if(makeActive == true){
            makeActive = false;
            justBegan = true;
            justEnded = false;
            bActive = true;
            lerpTimer.setToValue(0);
            lerpTimer.lerpToValue(amount);
            bDone = false;
            ofLog()<<"span "<<myID<<" bActive "<<bActive;
        }
        
        //        if(old_bActive != bActive){
        //            
        //             ofLog()<<"span "<<myID<<" old_bActive = bActive "<<bActive;
        //             
        //            old_bActive = bActive;
        //            
        //            if(bActive == true){
        //                lerpTimer.setToValue(0);
        //                lerpTimer.lerpToValue(amount);
        //                bDone = false;
        //                ofLog()<<"span "<<myID<<" bActive "<<bActive;
        //            }
        //        }
        if(bActive){
            //            if(ofGetElapsedTimef() - myTimer >= duration){
            //                bActive = false;
            //            }else{
            //                
            //            }
            
            //            cnt = lerpTimer.getValue();
            lerpValue = lerpTimer.getValue();
            lerpPercent = lerpTimer.getProgress();
            
            if(curveTypeIndex == 0){
                curveValue = curvesTool.getAtPercent(lerpPercent);
                curveValueNormalized = curveValue/float(amount);
            } else if(curveTypeIndex == 1){
                curveValueNormalized =  pow(lerpPercent,expo_value);
                curveValue = curveValueNormalized* amount;
            }
            //            ofLog()<<myID<<" curveValue "<<curveValue<<" lerpPercent "<<lerpPercent; //lerpTimer.getTargetValue();
            
            if(endPause > 0){
                if(lerpPercent >= 0.97){
                    if(bStartPause == false){
                        bStartPause = true;
                        endTimer = ofGetElapsedTimef();
                    }
                    if(ofGetElapsedTimef() - endTimer > endPause){
                        bActive = false;
                        bDone = true; 
                    }
                } 
            }else{
                if(lerpPercent >= 0.97){
                    bActive = false;
                    bDone = true;    
                }
            }
            //             cnt = fmod(cnt,amount);
        }else{
            bStartPause = false;
        }
        
        
    }
    
    void draw(int _x, int _y){
        ofPushMatrix();
        ofTranslate(_x,_y);
        
        ofSetColor(255);
        
        
        
        ofSetColor(255);
        if(curveTypeIndex == 0){
            //use curveTools addon
            curvesTool.draw(0,0,lerpValue);
        }else if(curveTypeIndex == 1){
            //use exponential curve
            
            ofSetColor(20);
            ofFill();
            ofDrawRectangle(0, 0, 256, 256);
            
            
            // grid
            ofSetColor(50);
            for(int i = 0; i < 256; i += 64) {
                ofDrawLine(0, i, 256, i);
                ofDrawLine(i, 0, i, 256);
            }
            
            ofPolyline temp_pl;
            for(int i=0; i<amount; i++){
                temp_pl.addVertex(i,amount - pow(i/(float)amount,expo_value)*amount);
            }
            ofSetColor(255);
            temp_pl.draw();
        }
        
        if(bActive == true){
            ofNoFill();
            ofSetLineWidth(2);
            ofSetColor(255,255,0);
            ofDrawRectangle(-3,-3,amount+6,amount+6);
            ofSetLineWidth(1);
        }
        
        ofSetColor(255);
        int temp_y = 0;
        ofDrawBitmapString("span "+ofToString(myID) + " "+spanLabel, 1, temp_y+=15);
        ofDrawBitmapString("% "+ofToString(lerpPercent), 1, temp_y+=15);
        //        ofDrawBitmapString("from "+ofToString(lerpTimer.getStartValue()) +"  to "+ ofToString(lerpTimer.getTargetValue()), 0, temp_y+=15);
        //        ofDrawBitmapString("lerp value "+ofToString(lerpValue), 0, temp_y+=15);
        ofDrawBitmapString("curve perc "+ofToString(curveValue), 1, temp_y+=15);
        
        ofSetColor(ofColor::yellow);
        float temp_v = amount-curveValue;
        ofDrawLine(0, temp_v, amount,temp_v);
        ofDrawCircle(lerpValue,amount-curveValue,3);
        
        ofPopMatrix();
    }
    
    void checkGui(){
        
        curveTypeName = curveTypeNames[curveTypeIndex];
        
        
        if(bResetCurve){
            bResetCurve = false;
            curvesTool.clear();
            curvesTool.add(ofVec2f(0, 0));
            curvesTool.add(ofVec2f(255, 255));
            expo_value = 1;
        }
        if(bSave){
            bSave = false;
            string str = "curves_"+timelineName+"_"+spanLabel+"_"+ofToString(myID);
            curvesTool.save(str+".yml");
            ofLog()<<"curveTool save "<<str;
        }
        if(bLoad){
            bLoad = false;
            string str = "curves_"+timelineName+"_"+spanLabel+"_"+ofToString(myID);
            curvesTool.load(str+".yml");
            ofLog()<<"curveTool load "<<str;
        }
        
        if(old_duration != duration){
            old_duration = duration;
            lerpTimer.setDuration(duration);
        }
    }
};
#endif /* oneSpan_h */

