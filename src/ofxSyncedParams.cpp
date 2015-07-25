//
//  ofxSyncedParams
//  based on datGuiController originally by ofZach

#include "ofxSyncedParams.h"

ofxSyncedParams::ofxSyncedParams(){
	bSetup = false;
	rootGroup = NULL;
}

void ofxSyncedParams::setupFromGui(ofxPanel & gui){
	//TODO store a pointer to the gui?!
	setupFromParamGroup(((ofParameterGroup&)gui.getParameter()));
}

void ofxSyncedParams::setupFromParamGroup(ofParameterGroup & group){
	rootGroup = &group;
	bSetup = true;
	//TODO error checking?

	ofAddListener(group.parameterChangedE(),this,&ofxSyncedParams::parameterChanged);
}

std::string ofxSyncedParams::parseParamsToJson ( ){
	if(!bSetup){
		ofLogWarning("ofxSynchedParams::parseParamsToJson") << "parsing not possible - call setup first";
		return "";
	}

    std::string str = parseParamGroup(*rootGroup, false).toStyledString();

    // remove spaces and new lines...
    std::string::iterator end_pos = std::remove(str.begin(), str.end(), ' ');
    str.erase(end_pos, str.end());
    end_pos = std::remove(str.begin(), str.end(), '\n');
    str.erase(end_pos, str.end());
    return str;
}

void ofxSyncedParams::updateParamFromJson(ofxJSONElement json){
	ofParameterGroup group = *rootGroup; //TODO why pointer -> object and not pointer -> pointer?
//    ofLogNotice("use") << group.getName();

    ofxJSONElement path(json["path"]);
	if(path.isArray()){
//		ofLogNotice() << "path array found - ";
		for(int i=1;i<(int)path.size();++i){ //skip the first one
			std::string groupName = path[i].asString();
//			ofLogNotice() << groupName;
			group = group.getGroup(groupName); //TODO crash if subgroup doesn't exist
		}
	}else{
		ofLogError() << "path not found";
		return;
	}

	std::string name = json["name"].asString();
	ofAbstractParameter & param = group.get(name);

	std::string type = param.type();

	if(type==typeid(ofParameter<float>).name()){
		ofParameter<float> & p = param.cast<float>();
		p = json["value"].asFloat();
	}else if(type==typeid(ofParameter<int>).name()){
		ofParameter<int> & p = param.cast<int>();
		p = json["value"].asInt();
	}else if(type==typeid(ofParameter<bool>).name()){
		ofParameter<bool> & p = param.cast<bool>();
		p = json["value"].asBool();
	}
	else if(type==typeid(ofParameter<ofColor>).name()){
		ofParameter<ofColor> p = param.cast<ofColor>();
		ofxJSONElement elem = json["value"];
		if (elem.isArray()){ // get out RGBA
			float r = elem[0].asInt();
			float g = elem[1].asInt();
			float b = elem[2].asInt();
			p.set( ofColor(r,g,b));
		} else {
			std::string hex = elem.asString();
			hex.erase(0, 1);
			unsigned int color = strtoul(hex.c_str(), NULL, 16);
			ofColor c;
			c.setHex(color);
			p.set(c);
		}
	}
}

Json::Value ofxSyncedParams::parseParamGroup(ofParameterGroup & _parameters, bool bInnerGroup = false){
    
    
    string name = _parameters.getName();
    Json::Value json;
    //Json::Value jsonArray;
    
    for(int i=0; i < _parameters.size(); i++){
        
		string type = _parameters.getType(i);
        
		if(type==typeid(ofParameter<int>).name()){
			
            ofParameter<int> p = _parameters.getInt(i);
            Json::Value valToAddSub;
            valToAddSub["type"] = "int";
            valToAddSub["value"] = p.get();
            valToAddSub["min"] = p.getMin();
            valToAddSub["max"] = p.getMax();
            json[ p.getName() ] = valToAddSub;
            
            //valToAdd[ p.getName() ] = valToAddSub;
            //jsonArray.append(valToAdd);
		}else if(type==typeid(ofParameter<float>).name()){
			 
            ofParameter<float> p = _parameters.getFloat(i);
            Json::Value valToAddSub;
            valToAddSub["type"] = "float";
            valToAddSub["value"] = p.get() + 0.0001;     // ok it's unhappy if see an "int" so this is a hack...
            valToAddSub["min"] = p.getMin();
            valToAddSub["max"] = p.getMax();
            
            json[ p.getName() ] = valToAddSub;
            
		}else if(type==typeid(ofParameter<bool>).name()){
			ofParameter<bool> p = _parameters.getBool(i);
            
            Json::Value valToAddSub;
            valToAddSub["type"] = "bool";
            valToAddSub["value"] = p.get();
            json[ p.getName() ] = valToAddSub;

		}else if(type==typeid(ofParameter<ofVec2f>).name()){
			ofParameter<ofVec2f> p = _parameters.getVec2f(i);

		}else if(type==typeid(ofParameter<ofVec3f>).name()){
			ofParameter<ofVec3f> p = _parameters.getVec3f(i);

		}else if(type==typeid(ofParameter<ofVec4f>).name()){
			ofParameter<ofVec4f> p = _parameters.getVec4f(i);

		}else if(type==typeid(ofParameter<ofColor>).name()){
			ofParameter<ofColor> p = _parameters.getColor(i);
            
            ofColor temp = p;
            Json::Value valToAddSub;
            
            Json::Value jsonArray;
            jsonArray.append(temp.r);
            jsonArray.append(temp.g);
            jsonArray.append(temp.b);
            
            valToAddSub["type"] = "color";
            valToAddSub["value"] = jsonArray;
            json[ p.getName() ] = valToAddSub;
            

		}else if(type==typeid(ofParameter<ofShortColor>).name()){
			ofParameter<ofShortColor> p = _parameters.getShortColor(i);

		}else if(type==typeid(ofParameter<ofFloatColor>).name()){
			ofParameter<ofFloatColor> p = _parameters.getFloatColor(i);

		}else if(type==typeid(ofParameter<string>).name()){
            ofParameter<string> p = _parameters.getString(i);
            
		}else if(type==typeid(ofParameterGroup).name()){
			ofParameterGroup p = _parameters.getGroup(i);
            Json::Value jsonTemp = parseParamGroup (p, true);
            json[ p.getName() ] = jsonTemp;
		}else{
            ofLogWarning() << "ofxBaseGroup; no control for parameter of type " << type;
		}
    }
    
    if (!bInnerGroup){
        Json::Value jsonOuter;
        jsonOuter[name] = json;
        return jsonOuter;
    } else {
        return json;
    }
}

void ofxSyncedParams::parameterChanged( ofAbstractParameter & parameter ){
	ofLogVerbose("ofxSyncedParams::parameterChanged");

	Json::Value json, path;

	json["type"] = "update";

	//get path like getGroupHierarchyNames but only until rootGroup
	list<string> hierarchy;
	ofParameterGroup * parentGroup = parameter.getParent();
	while(parentGroup != rootGroup){
		hierarchy.push_front(parentGroup->getName());
		parentGroup = parentGroup->getParent();
	}
	hierarchy.push_front(parentGroup->getName());

	list<string>::iterator it = hierarchy.begin();
	for(;it!=hierarchy.end();++it){
		path.append(*it);
	}
	json["path"] = path;

	//get name & value
	if(parameter.type()==typeid(ofParameter<int>).name()){
		const ofParameter<int> & p = parameter.cast<int>();
		json[ "value" ] = p.get();
		json[ "name" ] = p.getName();
	}else if(parameter.type()==typeid(ofParameter<float>).name()){
		const ofParameter<float> & p = parameter.cast<float>();
		json[ "value" ] = p.get();
		json[ "name" ] = p.getName();
	}else if(parameter.type()==typeid(ofParameter<bool>).name()){
		const ofParameter<bool> & p = parameter.cast<bool>();
		json[ "value" ] = p.get();
		json[ "name" ] = p.getName();
	}else if(parameter.type()==typeid(ofParameter<ofColor>).name()){
		const ofParameter<ofColor> & p = parameter.cast<ofColor>();

        Json::Value jsonArray;
        jsonArray.append(p.get().r);
        jsonArray.append(p.get().g);
        jsonArray.append(p.get().b);

        json[ "value" ] = jsonArray;
        json[ "name" ] = p.getName();
	}else{
	}

	changedParamInJson = json.toStyledString();
	ofNotifyEvent(paramChangedE,changedParamInJson);
}

ofxGuiGroup * ofxSyncedParams::setupFromJson(Json::Value & jsonInit){
	ofParameterGroup * groupOwner = new ofParameterGroup();
	//TODO needs refactoring! don't like it and memory leak ...
	unfurl(jsonInit,*groupOwner);

	ofxGuiGroup * group = new ofxGuiGroup();
	group->setup((ofParameterGroup&)groupOwner->get(0));
	rootGroup = &(ofParameterGroup&)group->getParameter();
	//TODO same here - pointer <-> reference madness

	ofAddListener(rootGroup->parameterChangedE,this,&ofxSyncedParams::parameterChanged);

	return group;
}

void ofxSyncedParams::unfurl(Json::Value & obj, ofParameterGroup & parentGroup){
	ofLogNotice("ofRemoteUIApp::unfurl") << obj.toStyledString();

	std::vector<std::string> members = obj.getMemberNames();
	for(size_t i=0;i<members.size();++i){

		std::string objName = members[i];
		Json::Value subObj = obj.get(objName,"");

		if(subObj.isObject() && subObj.isMember("type")){ //is leaf?
			//add to group
			addToGroup(objName,subObj,parentGroup);
		}else{
			//add subgroup
			ofParameterGroup subGroup;
			subGroup.setName(objName);
			unfurl(subObj,subGroup);
			parentGroup.add(subGroup);
		}
	}
}

void ofxSyncedParams::addToGroup(string & name, Json::Value & obj, ofParameterGroup & group){
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
