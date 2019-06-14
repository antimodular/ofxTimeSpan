//
//  ofxTimeSpan.h
//  timeLine
//
//  Created by Stephan Schulz on 2018-04-13.
//

#ifndef ofxTimeSpan_h
#define ofxTimeSpan_h

#include "ofxGui.h"
#include "oneSpan.h"


class ofxTimeSpan{
    
public:
    string timeLineName;
    
    ofxPanel gui_spans;
    
    ofParameter<bool> bShow;
    ofParameter<bool> bCombineSpans;
    
    //     ofParameter<float> pauseBeforeRepeatDuration;
    //    float pauseBeforeRepeatTimer;
    
    //    vector<ofParameter<float>> pauseEndDuration;
    //    vector<float> pauseEndTimer;
    
    vector<oneSpan> allSpans;
    oneSpan * currentSpan;
    
    //    vector<ofParameter<int>> spanOrder;
    
    bool bTrigger;
    bool bRunning;
    bool justBegan;
    int spanAmount;
    vector<string> spanLabels;
    
    ofParameter<int> activeIndex;
    ofParameter<float> activeValueX;
    ofParameter<float> activeValueY;
    bool readyForNewTrigger;
    
    void setup(string _name,  vector<string> _labels){
        spanLabels = _labels;
        setup(_name, spanLabels.size());
    }
    
    void setup(string _name,int _spansAmount){
        
        timeLineName = _name;
        spanAmount = _spansAmount;
        allSpans.resize(spanAmount);
        
        //if no labels were passed in
        if(spanLabels.size() != spanAmount){
            spanLabels.resize(spanAmount);
            fill(spanLabels.begin(),spanLabels.end(),"");
        }
        //        spanOrder.resize(_spans);
        
        for(int i=0; i<allSpans.size(); i++){
            allSpans[i].setup(i,_name, spanLabels[i]);
        }
        //        
        //        pauseEndDuration.resize(_spans);
        //        pauseEndTimer.resize(_spans);
        
        gui_spans.setup();
        gui_spans.setName(timeLineName);
        //        gui_spans.setPosition(_x,_y);
        gui_spans.setHeaderBackgroundColor(ofColor(255,0,0));
        gui_spans.add(bShow.set("bShow", false));
        gui_spans.add(bCombineSpans.set("combineSpans", false));
        
        gui_spans.add(activeIndex.set("activeIndex",0,0,allSpans.size()-1));
        gui_spans.add(activeValueX.set("activeValueX",0,0,1));
        gui_spans.add(activeValueY.set("activeValueY",0,0,1));
        
        //        gui_spans.add(pauseBeforeRepeatDuration.set("pauseRepeatDur",30,0,300));
        
        //        for(int i=0; i<spanOrder.size(); i++){
        //            gui_spans.add(spanOrder[i].set("span_"+ofToString(i),-1,-1,spanOrder.size()));
        //        }
        for(int i=0; i<allSpans.size(); i++){
            gui_spans.add(allSpans[i].parameter_span);
        }
        gui_spans.loadFromFile("gui_spans.xml");
        
        for(int i=0; i<allSpans.size(); i++){
            allSpans[i].init();
        }
        
        activeValueX = 0;
        activeValueY = 0;
        
        bTrigger = false;
        bRunning = false;
        justBegan = false;
        readyForNewTrigger = false;
    }
    
    void init(){
        activeIndex = 0;
        
    }
    void exit(){
        
    }
    
    void triggerIt(){
        justBegan = true;
        bTrigger = true;
        readyForNewTrigger = false;
    }
    void update(){
        
        for(int i=0; i<allSpans.size(); i++){
            allSpans[i].checkGui();
        }
        
        //        if(activeIndex >= 0 && activeIndex< spanAmount){
        //            currentSpan = allSpans[activeIndex];
        //        }
        if(bTrigger == true){
            //no path was triggered 
            
            bTrigger = false;
            bRunning = true;
            activeIndex = 0;
            currentSpan = &allSpans[activeIndex];
            currentSpan->makeActive = true;
            //              ofLog()<<timeLineName<<" allSpans[activeIndex].old_bActive "<<allSpans[activeIndex].old_bActive;
            ofLog()<<timeLineName<<" bTrigger "<<bTrigger<<" activeIndex "<<activeIndex;
        }
        if(bRunning == true){
            for(int i=0; i<allSpans.size(); i++){
                allSpans[i].update();
            }
            
            if(bCombineSpans == false){
                activeValueX = currentSpan->lerpPercent;
                activeValueY = currentSpan->curveValueNormalized;
            }else{
                
                activeValueX = 1 / float(allSpans.size());
                activeValueX *= activeIndex;
                activeValueX += currentSpan->lerpPercent / float(allSpans.size());
                
                activeValueY = currentSpan->curveValueNormalized;
                //                activeValueY = activeIndex/float(allSpans.size()) + allSpans[activeIndex].curveValueNormalized / float(allSpans.size());
            }
            
            //             ofLog()<<timeLineName<<" activeValue "<<activeValue;
            
            if(currentSpan->bDone == true){
                activeIndex++;
                if(activeIndex >= allSpans.size()){
                    bRunning = false;
                    
                    readyForNewTrigger = true;
                    ofLog()<<"readyForNewTrigger new path due to timer expiration";
                }else{
                    currentSpan = &allSpans[activeIndex];
                    currentSpan->makeActive = true;
                }
                ofLog()<<timeLineName<<" allSpans[activeIndex].bDone == true ";
                ofLog()<<timeLineName<<" bRunning "<<bRunning;
            }
            
            //            if(bRunning == false && activeIndex == allSpans.size()){
            //                pauseBeforeRepeatTimer = ofGetElapsedTimef();
            //            }
        }
        
        //        if(bRunning == false && activeIndex == allSpans.size() && ofGetElapsedTimef() - pauseBeforeRepeatTimer > pauseBeforeRepeatDuration){
        ////            bTrigger = true;
        //            readyForNewTrigger = true;
        //            ofLog()<<"readyForNewTrigger new path due to timer expiration";
        //        }
        
    }
    
    void draw(){
        ofPushMatrix();
        //        ofTranslate(gui_spans.getPosition().x + gui_spans.getWidth()+3,gui_spans.getPosition().y);
        //        ofTranslate(gui_spans.getPosition().x,gui_spans.getPosition().y + gui_spans.getHeight()+3);
        ofTranslate(gui_spans.getPosition().x + gui_spans.getWidth()+3,gui_spans.getPosition().y);
        
        for(int i=0; i<allSpans.size(); i++){
            //            allSpans[i].draw(0,i*258);
            allSpans[i].draw(i*258,0);
        }
        ofPopMatrix();
        gui_spans.draw();
    }
    
    void save(){
        gui_spans.saveToFile("gui_spans.xml");
        for(int i=0; i<allSpans.size(); i++){
            allSpans[i].bSave = true;
        }
    }
private:
    
};

#endif /* ofxTimeSpan_h */

