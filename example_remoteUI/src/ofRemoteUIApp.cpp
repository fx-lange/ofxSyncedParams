#include "ofRemoteUIApp.h"

//--------------------------------------------------------------
void ofRemoteUIApp::setup(){
    ofBackground(0);
    ofSetLogLevel(OF_LOG_VERBOSE);
    // client.connect("echo.websocket.org");

    // 4 - connect
    client.connect( "localhost", 9092 );
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

    client.send("Hello");
    cout << "sending hello" <<endl;
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
