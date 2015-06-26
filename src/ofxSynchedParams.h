//
//  ofxSynchedParams
//  based on datGuiController originally by ofZach

#pragma once

#include "ofMain.h"
#include "ofxJSON.h"
#include  "json.h"

class ofxSynchedParams {
public:
    string setFromOfParameterGroup ( ofParameterGroup & group );
    void setParamFromJson( ofxJSONElement json, ofParameterGroup * group);

protected:
    Json::Value parseParamGroup( ofParameterGroup & subGroup,  bool bInnerGroup);
    
    ofParameterGroup groupCopy;
    ofParameterGroup * groupPtr;
};
