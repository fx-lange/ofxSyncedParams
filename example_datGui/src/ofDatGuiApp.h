#pragma once

#include "ofxSyncedParams.h"
#include "ofMain.h"
#include "ofxLibwebsockets.h"

using namespace std;

class ofDatGuiApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);

	void parameterChanged( std::string & );

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

    // websocket methods
	void onMessage( ofxLibwebsockets::Event& args );
	void onConnect( ofxLibwebsockets::Event& args ){}
	void onOpen( ofxLibwebsockets::Event& args ){}
	void onClose( ofxLibwebsockets::Event& args ){}
	void onIdle( ofxLibwebsockets::Event& args ){}
	void onBroadcast( ofxLibwebsockets::Event& args ){}
};
