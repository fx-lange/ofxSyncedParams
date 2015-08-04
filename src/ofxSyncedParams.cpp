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

	Json::Value json;
	json["params"] = parseParamGroup(*rootGroup, false);
	json["type"] = "init";

    std::string str = json.toStyledString();

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
		p.setWithoutEventNotifcations(json["value"].asFloat());
	}else if(type==typeid(ofParameter<int>).name()){
		ofParameter<int> & p = param.cast<int>();
		p.setWithoutEventNotifcations(json["value"].asInt());
	}else if(type==typeid(ofParameter<bool>).name()){
		ofParameter<bool> & p = param.cast<bool>();
		p.setWithoutEventNotifcations(json["value"].asBool());
	}
	else if(type==typeid(ofParameter<ofColor>).name()){
		ofParameter<ofColor> p = param.cast<ofColor>();
		ofxJSONElement elem = json["value"];
		if (elem.isArray()){ // get out RGBA
			float r = elem[0].asInt();
			float g = elem[1].asInt();
			float b = elem[2].asInt();
			p.setWithoutEventNotifcations( ofColor(r,g,b));
		} else {
			std::string hex = elem.asString();
			hex.erase(0, 1);
			unsigned int color = strtoul(hex.c_str(), NULL, 16);
			ofColor c;
			c.setHex(color);
			p.setWithoutEventNotifcations(c);
		}
	}
}

Json::Value ofxSyncedParams::parseParamGroup(ofParameterGroup & _parameters, bool bInnerGroup = false){
    
    string name = _parameters.getName();
    Json::Value json;
    
    for(int i=0; i < (int)_parameters.size(); i++){
        
		string type = _parameters.getType(i);
        bool bKnownParameter = false;

        Json::Value valToAddSub;
		if(type==typeid(ofParameter<int>).name()){
			
            ofParameter<int> p = _parameters.getInt(i);
            valToAddSub["type"] = "int";
            valToAddSub["value"] = p.get();
            valToAddSub["min"] = p.getMin();
            valToAddSub["max"] = p.getMax();
            bKnownParameter = true;

		}else if(type==typeid(ofParameter<float>).name()){
			 
            ofParameter<float> p = _parameters.getFloat(i);
            valToAddSub["type"] = "float";
            valToAddSub["value"] = p.get() + 0.0001;     //TODO ok it's unhappy if see an "int" so this is a hack...
            valToAddSub["min"] = p.getMin();
            valToAddSub["max"] = p.getMax();
            bKnownParameter = true;
            
		}else if(type==typeid(ofParameter<bool>).name()){

			ofParameter<bool> p = _parameters.getBool(i);
            valToAddSub["type"] = "bool";
            valToAddSub["value"] = p.get();
            bKnownParameter = true;

		}else if(type==typeid(ofParameter<ofColor>).name()){

			ofParameter<ofColor> p = _parameters.getColor(i);
            
            ofColor temp = p;
            Json::Value jsonArray;
            jsonArray.append(temp.r);
            jsonArray.append(temp.g);
            jsonArray.append(temp.b);
            
            valToAddSub["type"] = "color";
            valToAddSub["value"] = jsonArray;
            bKnownParameter = true;

		}else if(type==typeid(ofParameterGroup).name()){

			ofParameterGroup p = _parameters.getGroup(i);
            Json::Value jsonTemp = parseParamGroup (p, true);
			json[ p.getName() ]["members"] = jsonTemp;
            json[ p.getName() ]["orderIdx"] = i;

		}else{
            ofLogWarning() << "ofxBaseGroup; no control for parameter of type " << type;
		}

		if(bKnownParameter){
			valToAddSub["orderIdx"] = i;
			json[ _parameters.getName(i) ] = valToAddSub;
		}
    }
    
    if (!bInnerGroup){
        Json::Value jsonOuter;
        jsonOuter[name]["members"] = json;
        jsonOuter[name]["orderIdx"] = 0;
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
//	list<string> hierarchy;
//	ofParameterGroup * parentGroup = parameter.getParent();
//	while(parentGroup != rootGroup){
//		hierarchy.push_front(parentGroup->getName());
//		parentGroup = parentGroup->getParent();
//	}
//	hierarchy.push_front(parentGroup->getName());
//	list<string>::iterator it = hierarchy.begin();
//	for(;it!=hierarchy.end();++it){
//		path.append(*it);
//	}
//	json["path"] = path;

	/* TODO getParent is private so the version above doesn't work anymore.
	the new version depends on group naming and could break if a sub group has the same name as the root group*/

	//get group hierarchy path until rootGroup
	vector<string> treeHierarchy = parameter.getGroupHierarchyNames();
	list<string> hierarchy;
	//walk from first parent [size()-2] to rootGroup [-2 because paramName is also part of the hierarchy]
	for(size_t i=treeHierarchy.size()-2;i>=0; --i){ //
		string & groupName = treeHierarchy[i];
		hierarchy.push_front(groupName);
		if(groupName == rootGroup->getName()){
			break;
		}
	}
	for(list<string>::iterator it = hierarchy.begin();it!=hierarchy.end();++it){
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
	rootGroup = new ofParameterGroup();
//	//TODO needs refactoring! don't like it and memory leak ...

	std::string rootName = jsonInit.getMemberNames()[0];
	rootGroup->setName(rootName);
	unfurl(jsonInit[rootName],*rootGroup);

	ofxGuiGroup * group = new ofxGuiGroup();
	group->setup(*rootGroup);
//	rootGroup = &(ofParameterGroup&)group->getParameter();
	//TODO same here - pointer <-> reference madness

	ofAddListener(rootGroup->parameterChangedE(),this,&ofxSyncedParams::parameterChanged);

	return group;
}

void ofxSyncedParams::unfurl(Json::Value & obj, ofParameterGroup & parentGroup){
	ofLogNotice("ofRemoteUIApp::unfurl") << obj.toStyledString();

	/* instead of just going through all members in one loop,
		a second loop (outer) is needed to find the members in the right order,
		because many c++ json implementation don't keep track of the insertion order. */

	std::vector<std::string> members = obj["members"].getMemberNames();
	for(int i=0;i<(int)members.size();++i){
		for(size_t k=0;k<members.size();++k){

			std::string objName = members[k];
			Json::Value subObj = obj["members"].get(objName,"");
			if(subObj["orderIdx"].asInt()!= i)
				continue;

			if(subObj.isObject() && !subObj.isMember("members")){ //is leaf?
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
