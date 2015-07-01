//
//  ofxSynchedParams
//  based on datGuiController originally by ofZach

#include "ofxSynchedParams.h"

void myReplace(std::string& str, const std::string& oldStr, const std::string& newStr){
    size_t pos = 0;
    while((pos = str.find(oldStr, pos)) != std::string::npos){
        str.replace(pos, oldStr.length(), newStr);
        pos += newStr.length();
    }
}

void PrintJSONValue( const Json::Value &val )
{
    if( val.isString() ) {
        printf( "string(%s)", val.asString().c_str() );
    } else if( val.isBool() ) {
        printf( "bool(%d)", val.asBool() );
    } else if( val.isInt() ) {
        printf( "int(%d)", val.asInt() );
    } else if( val.isUInt() ) {
        printf( "uint(%u)", val.asUInt() );
    } else if( val.isDouble() ) {
        printf( "double(%f)", val.asDouble() );
    }
    else
    {
        printf( "unknown type=[%d]", val.type() );
    }
}

bool PrintJSONTree( const Json::Value &root, unsigned short depth /* = 0 */)
{
    depth += 1;
    printf( " {type=[%d], size=%d}", root.type(), root.size() );
    
    if( root.size() > 0 ) {
        printf("\n");
        for( Json::ValueIterator itr = root.begin() ; itr != root.end() ; itr++ ) {
            // Print depth.
            for( int tab = 0 ; tab < depth; tab++) {
                printf("-");
            }
            printf(" subvalue(");
            PrintJSONValue(itr.key());
            printf(") -");
            PrintJSONTree( *itr, depth);
        }
        return true;
    } else {
        printf(" ");
        PrintJSONValue(root);
        printf( "\n" );
    }
    return true;
}

ofxSynchedParams::ofxSynchedParams(){
	bSetup = false;
	rootGroup = NULL;
}

void ofxSynchedParams::setupFromGui(ofxPanel & gui){
	setupFromParamGroup(((ofParameterGroup&)gui.getParameter()));
}

void ofxSynchedParams::setupFromParamGroup(ofParameterGroup & group){
	rootGroup = &group;
	bSetup = true;
	//TODO error checking?
}

std::string ofxSynchedParams::parseParamsToJson ( ){
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

void ofxSynchedParams::setParamFromJson(ofxJSONElement json){
	ofParameterGroup group = *rootGroup; //TODO why pointer -> object and not pointer -> pointer?
    ofLogNotice("use") << group.getName();

    ofxJSONElement path(json["path"]);
	if(path.isArray()){
		ofLogNotice() << "path array found - ";
		for(int i=1;i<(int)path.size();++i){ //skip the first one
			std::string groupName = path[i].asString();
			ofLogNotice() << groupName;
			group = group.getGroup(groupName);
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

	//TODO boolean & integer at least
}

Json::Value ofxSynchedParams::parseParamGroup(ofParameterGroup & _parameters, bool bInnerGroup = false){
    
    
    string name = _parameters.getName();
    Json::Value json;
    //Json::Value jsonArray;
    
    for(int i=0; i < _parameters.size(); i++){
        
		string type = _parameters.getType(i);
        
		if(type==typeid(ofParameter<int>).name()){
			
            ofParameter<int> p = _parameters.getInt(i);
            Json::Value valToAddSub;
            valToAddSub["display"] = "range";
            valToAddSub["value"] = p.get();
            valToAddSub["min"] = p.getMin();
            valToAddSub["max"] = p.getMax();
            valToAddSub["step"] = 1;
            valToAddSub["listen"] = true;
            json[ p.getName() ] = valToAddSub;
            
            cout << "INT! " << p.getName() << endl;
            
            //valToAdd[ p.getName() ] = valToAddSub;
            //jsonArray.append(valToAdd);
		}else if(type==typeid(ofParameter<float>).name()){
			 
            ofParameter<float> p = _parameters.getFloat(i);
            Json::Value valToAddSub;
            valToAddSub["display"] = "range";
            valToAddSub["value"] = p.get() + 0.0001;     // ok it's unhappy if see an "int" so this is a hack...
            valToAddSub["min"] = p.getMin();
            valToAddSub["max"] = p.getMax();
            
            //valToAddSub["step"] = 0.1; //(p.getMax()-p.getMin())/300.0; // 300 steps?
            valToAddSub["listen"] = true;
            json[ p.getName() ] = valToAddSub;
            
            cout << "FLOAT! " << p.getName() << endl;
            
            
		}else if(type==typeid(ofParameter<bool>).name()){
			ofParameter<bool> p = _parameters.getBool(i);
            
            Json::Value valToAddSub;
            valToAddSub["display"] = "bool";
            valToAddSub["value"] = p.get();
            valToAddSub["listen"] = true;
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
            
            valToAddSub["display"] = "color";
            valToAddSub["value"] = jsonArray;
            valToAddSub["listen"] = true;
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
			
            //ofLogWarning() << "ofxBaseGroup; no control for parameter of type " << type;
		}
    }
    
    //json[name] = jsonArray;
    
    if (!bInnerGroup){
        Json::Value jsonOuter;
        jsonOuter[name] = json;
        return jsonOuter;
    } else {
        return json;
    }
}
