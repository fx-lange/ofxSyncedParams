//
//  ofxSynchedParams
//  based on datGuiController originally by ofZach

#pragma once

#include "ofMain.h"
#include "ofxJSON.h"
#include "ofxGui.h"
#include  "json.h"

class ofxSynchedParams {
public:
	ofxSynchedParams();

	void setupFromGui(ofxPanel & gui);
	void setupFromParamGroup(ofParameterGroup & group);

	string parseParamsToJson ( );
	void setParamFromJson( ofxJSONElement json );

protected:
    Json::Value parseParamGroup( ofParameterGroup & subGroup,  bool bInnerGroup); //TODO use pointer
    ofParameterGroup * rootGroup;
    bool bSetup;
};
