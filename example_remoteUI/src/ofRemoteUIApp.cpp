#include "ofRemoteUIApp.h"

//--------------------------------------------------------------
void ofRemoteUIApp::setup(){
    ofBackground(0);
    ofSetLogLevel(OF_LOG_VERBOSE);
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
    ofSetLogLevel(OF_LOG_ERROR);

    client.addListener(this);
    ofSetFrameRate(60);
}

//--------------------------------------------------------------
void ofRemoteUIApp::update(){
}

//--------------------------------------------------------------
void ofRemoteUIApp::draw(){
    ofDrawBitmapString("Type anywhere to send 'hello' to your server\nCheck the console for output!", 10,20);
    ofDrawBitmapString(client.isConnected() ? "Client is connected" : "Client disconnected :(", 10,50);
}

//--------------------------------------------------------------
void ofRemoteUIApp::onConnect( ofxLibwebsockets::Event& args ){
    cout<<"on connected"<<endl;
}

//--------------------------------------------------------------
void ofRemoteUIApp::onOpen( ofxLibwebsockets::Event& args ){
    cout<<"on open"<<endl;
}

//--------------------------------------------------------------
void ofRemoteUIApp::onClose( ofxLibwebsockets::Event& args ){
    cout<<"on close"<<endl;
}

//--------------------------------------------------------------
void ofRemoteUIApp::onIdle( ofxLibwebsockets::Event& args ){
    cout<<"on idle"<<endl;
}

//--------------------------------------------------------------
void ofRemoteUIApp::onMessage( ofxLibwebsockets::Event& args ){
    cout<<"got message "<<args.message<<endl;
}

//--------------------------------------------------------------
void ofRemoteUIApp::onBroadcast( ofxLibwebsockets::Event& args ){
    cout<<"got broadcast "<<args.message<<endl;
}

//--------------------------------------------------------------
void ofRemoteUIApp::keyPressed(int key){

	Json::Value json;
	json["type"] = "initRequest";
	string jsonString = json.toStyledString();
    client.send(jsonString);
    ofLogNotice("keyPressed") << "send:\n" << jsonString;
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
