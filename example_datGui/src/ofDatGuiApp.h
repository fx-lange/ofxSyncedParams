#pragma once

#include "ofxSyncedParams.h"
#include "ofMain.h"
#include "ofxLibwebsockets.h"

class ofDatGuiApp : public ofBaseApp {
public:
	void setup();
	void setupGui();

	void parameterChanged( std::string & );

	void update();
	void draw();

	//----
	ofxPanel gui;
	ofParameterGroup group1,group2,group21;
	ofParameter<int> intParam1, intParam2;
	ofParameter<float> floatParam1, floatParam2;
	ofParameter<ofColor> color1;
	ofParameter<bool> toggle1, toggle2;

	ofxSyncedParams paramSync;

	//--- WebSocket
    ofxLibwebsockets::Server server;
    ofxJSONElement paramUpdate;
    bool bSetup;

    // websocket methods
	void onMessage( ofxLibwebsockets::Event& args );
	void onConnect( ofxLibwebsockets::Event& args ){}
	void onOpen( ofxLibwebsockets::Event& args ){}
	void onClose( ofxLibwebsockets::Event& args ){}
	void onIdle( ofxLibwebsockets::Event& args ){}
	void onBroadcast( ofxLibwebsockets::Event& args ){}
};
