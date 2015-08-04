#include "ofRemoteUIApp.h"

bool bSetup = false;
bool eOnInit = false;
Json::Value jsonInit, jsonUpdate;

bool bDrawGui = false;

bool onUpdate = false;

//--------------------------------------------------------------
void ofRemoteUIApp::setup(){
    ofBackground(0);
    ofSetLogLevel(OF_LOG_NOTICE);
    // client.connect("echo.websocket.org");

    // 1 - get default options
	ofxLibwebsockets::ClientOptions options = ofxLibwebsockets::defaultClientOptions();

	// 2 - set basic params
	options.host = "localhost";
	options.port = 9092;

	// 3 - set keep alive params
	// BIG GOTCHA: on BSD systems, e.g. Mac OS X, these time params are system-wide
	// ...so ka_time just says "check if alive when you want" instead of "check if
	// alive after X seconds"
	// Note: some have had issues with only setting one of these; may be an
	// all-or-nothing type option!
	options.ka_time     = 1;
	options.ka_probes   = 1;
	options.ka_interval = 1;

	// 4 - connect
	client.connect(options);

    client.addListener(this);
    ofSetFrameRate(60);

    //listen to parameter changes from ofxSynchedParams
	ofAddListener(syncedParams.paramChangedE,this,&ofRemoteUIApp::parameterChanged);
}

//--------------------------------------------------------------
void ofRemoteUIApp::parameterChanged( std::string & paramAsJsonString ){
	ofLogVerbose("ofRemoteUIApp::parameterChanged") << paramAsJsonString;
	if(!onUpdate){
		client.send( paramAsJsonString );
	}
}

//--------------------------------------------------------------
void ofRemoteUIApp::update(){

	if(eOnInit){
		eOnInit = false;

		//parse JSON to gui
		gui.setup();
		gui.add(syncedParams.setupFromJson(jsonInit));
		bDrawGui = true;
		bSetup = true;
	}else if(onUpdate){
		syncedParams.updateParamFromJson(jsonUpdate);
		onUpdate = false;
	}
}

//--------------------------------------------------------------
void ofRemoteUIApp::draw(){
    ofDrawBitmapString("press Return to synchronize with your client - if connected", 10,20);
    ofDrawBitmapString(client.isConnected() ? "Client is connected" : "Client disconnected :(", 10,50);
    if(bDrawGui){
    	gui.draw();
    }
}

//--------------------------------------------------------------
void ofRemoteUIApp::onConnect( ofxLibwebsockets::Event& args ){
    ofLogVerbose("ofRemoteUIApp")<<"on connected"<<endl;
}

//--------------------------------------------------------------
void ofRemoteUIApp::onOpen( ofxLibwebsockets::Event& args ){
    ofLogVerbose("ofRemoteUIApp")<<"on open"<<endl;
}

//--------------------------------------------------------------
void ofRemoteUIApp::onClose( ofxLibwebsockets::Event& args ){
    ofLogVerbose("ofRemoteUIApp")<<"on close"<<endl;
}

//--------------------------------------------------------------
void ofRemoteUIApp::onIdle( ofxLibwebsockets::Event& args ){
    ofLogVerbose("ofRemoteUIApp")<<"on idle"<<endl;
}

//--------------------------------------------------------------
void ofRemoteUIApp::onMessage( ofxLibwebsockets::Event& args ){
    ofLogVerbose("ofRemoteUIApp::onMessage") <<"got message " << args.message<<endl;

    if ( !args.json.isNull() ){
		ofLogNotice("ofRemoteUIApp::onMessage") << "json message: " << args.json.toStyledString() << " from " << args.conn.getClientName();

		if(args.json["type"] == "update"){
			jsonUpdate = args.json;
			onUpdate = true;
		}else if(args.json["type"] == "init" && !bSetup){
			jsonInit = args.json["params"];
			eOnInit = true;
		}
	}
}

//--------------------------------------------------------------
void ofRemoteUIApp::onBroadcast( ofxLibwebsockets::Event& args ){
    ofLogVerbose("ofRemoteUIApp")<<"got broadcast "<<args.message<<endl;
}

//--------------------------------------------------------------
void ofRemoteUIApp::keyPressed(int key){
	ofLogVerbose("keyPressed");

	if(key == 't'){
		string jsonString = "{\"gui\":{\"group1\":{\"myFloat\":{\"max\":1,\"min\":0,\"type\":\"float\",\"value\":0.5001}}}}";//,"myInt":{"max":100,"min":0,"type":"int","value":100},"myToggle":{"type":"bool","value":false}},"group2":{"group2a":{"myColor":{"type":"color","value":[107,142,35]}},"myFloat":{"max":10,"min":0,"type":"float","value":0.0001},"myInt":{"max":100,"min":0,"type":"int","value":0},"myToggle":{"type":"bool","value":true}}}";
		jsonInit = ofxJSONElement(jsonString);
		eOnInit = true;
	}else if(OF_KEY_RETURN){
		if(bSetup)
			return;
		Json::Value json;
		json["type"] = "initRequest";
		string jsonString = json.toStyledString();
		client.send(jsonString);
		ofLogNotice("keyPressed") << "send:\n" << jsonString;
	}
}

//--------------------------------------------------------------
void ofRemoteUIApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofRemoteUIApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofRemoteUIApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofRemoteUIApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void ofRemoteUIApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofRemoteUIApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofRemoteUIApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofRemoteUIApp::dragEvent(ofDragInfo dragInfo){

}
