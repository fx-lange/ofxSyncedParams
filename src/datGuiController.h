#pragma once

#include "ofMain.h"
#include "ofxJSON.h"
#include  "json.h"


class datGuiController {
    

public:
    
    string setFromOfParameterGroup ( ofParameterGroup & group );
    
    Json::Value parseParamGroup( ofParameterGroup & subGroup,  bool bInnerGroup);
    void setValuesFromJson( ofxJSONElement json, ofParameterGroup * subGroup, bool bInnerGroup );
    void setParamFromJson( ofxJSONElement json, ofParameterGroup * group);
    
    void toJson();
    void fromJson();
    
    
    void valuesChanged(string valuesFromJavascript);
    
    ofParameterGroup groupCopy;
    ofParameterGroup * groupPtr;
    
};
