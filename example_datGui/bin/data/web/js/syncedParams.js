	var optionsGUI = {};
	var params;
	var isInit = false;
	
	function śyncGui( val ){
		console.log("json -> gui:");

		var obj = JSON.parse(val);
		console.log(obj);

		if(obj["type"]=="update"){
			console.log("Update");
			var param = params;
			for(i=0;i<obj["path"].length;++i){
				param = param[obj["path"][i]];
			}
			
			param[obj["name"]] = obj["value"];
		}else if(obj["type"] == "init"){
			console.log("Init");
			if(!isInit){
				isInit = true;
				params = guiGlue(obj["params"], optionsGUI);
			}else{
				console.log("ignore");
			}
		}
	};