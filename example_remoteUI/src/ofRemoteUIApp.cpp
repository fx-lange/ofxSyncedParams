#include "ofRemoteUIApp.h"

bool bIsSetup = false;
bool eOnInit = false;
Json::Value jsonInit;

bool bDrawGui = false;

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

    client.addListener(this);
    ofSetFrameRate(60);
}

//--------------------------------------------------------------
void ofRemoteUIApp::update(){

	if(eOnInit){
		eOnInit = false;

		//parse JSON to gui
		ofParameterGroup rootGroup;
		unfurl(jsonInit,rootGroup);
		gui.setup((ofParameterGroup&)rootGroup.get(0));
		bDrawGui = true;
	}
}

void ofRemoteUIApp::unfurl(Json::Value & obj, ofParameterGroup & parentGroup){
	ofLogNotice("ofRemoteUIApp::unfurl") << obj.toStyledString();

	std::vector<std::string> members = obj.getMemberNames();
	for(size_t i=0;i<members.size();++i){

		std::string objName = members[i];
		Json::Value subObj = obj.get(objName,"");

		if(subObj.isObject() && subObj.isMember("type")){ //is leaf?
			ofLogVerbose() << "is leaf -> is a parameter";
			//add to group
			addToGroup(objName,subObj,parentGroup);
		}else{
			ofLogVerbose() << "is a parameter group";
			ofParameterGroup subGroup;
			subGroup.setName(objName);
			unfurl(subObj,subGroup);
			parentGroup.add(subGroup);
		}
	}
}

void ofRemoteUIApp::addToGroup(string & name, Json::Value & obj, ofParameterGroup & group){
	string type = obj["type"].asString();

	if(type == "int"){
		ofParameter<int> param;
		param.setName(name);
		param.setMin(obj["min"].asInt());
		param.setMax(obj["max"].asInt());
		param.set(obj["value"].asInt());
		group.add(param);
	}else if(type == "float"){
		ofParameter<float> param;
		param.setName(name);
		param.setMin(obj["min"].asInt());
		param.setMax(obj["max"].asInt());
		param.set(obj["value"].asFloat());
		group.add(param);
	}else if(type == "bool"){
		ofParameter<bool> param;
		param.setName(name);
		param.set(obj["value"].asFloat());
		group.add(param);
	}else if(type == "color"){
		ofParameter<ofColor> param;
		param.setName(name);
		ofColor color;
		for(int i=0;i<3;++i){
			color[i] = obj["value"][i].asInt();
		}
		param.set(color);
		param.setMin(ofColor(0,0));
		param.setMax(ofColor(255,255));
		group.add(param);
	}
}

//--------------------------------------------------------------
void ofRemoteUIApp::draw(){
    ofDrawBitmapString("Type anywhere to send 'hello' to your server\nCheck the console for output!", 10,20);
    ofDrawBitmapString(client.isConnected() ? "Client is connected" : "Client disconnected :(", 10,50);
    if(bDrawGui){
    	gui.draw();
    }
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
    ofLogNotice("") <<"got message " << args.message<<endl;

    if ( !args.json.isNull() ){
		ofLogVerbose("ofRemoteUIApp::onMessage") << "json message: " << args.json.toStyledString() << " from " << args.conn.getClientName();

		jsonInit = args.json;
		eOnInit = true;
	}
}

//--------------------------------------------------------------
void ofRemoteUIApp::onBroadcast( ofxLibwebsockets::Event& args ){
    cout<<"got broadcast "<<args.message<<endl;
}

//--------------------------------------------------------------
void ofRemoteUIApp::keyPressed(int key){
	ofLogVerbose("keyPressed");

	if(key == 't'){
		string jsonString = "{\"gui\":{\"group1\":{\"myFloat\":{\"max\":1,\"min\":0,\"type\":\"float\",\"value\":0.5001}}}}";//,"myInt":{"max":100,"min":0,"type":"int","value":100},"myToggle":{"type":"bool","value":false}},"group2":{"group2a":{"myColor":{"type":"color","value":[107,142,35]}},"myFloat":{"max":10,"min":0,"type":"float","value":0.0001},"myInt":{"max":100,"min":0,"type":"int","value":0},"myToggle":{"type":"bool","value":true}}}";
		jsonInit = ofxJSONElement(jsonString);
		eOnInit = true;
	}else if(OF_KEY_RETURN){
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
