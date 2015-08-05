//returns params, a stripped version of paramsGUI, without all the GUI fluff
function guiGlue(paramsGUI, optionsGUI){

    //pass options to GUI e.g., { autoPlace: false }
    optionsGUI = optionsGUI || {};

    //extra parameter whether you want folders open or closed
    optionsGUI.folded = optionsGUI.folded || false;

    // params is a stripped version of paramsGUI, where everything
    // but its default attributes have been removed
    var params = {};
    var gui = renderParameters(paramsGUI, optionsGUI, params);

    //return stripped parameter object
    return params;

    function renderParameters(paramsGUI, optionsGUI, params){
    
        //initial creation    
        var gui = new dat.GUI(optionsGUI);

        //handle root
        var rootKey = Object.keys(paramsGUI)[0];
        var rootFolder = gui.addFolder(rootKey);
        if (!optionsGUI.folded)
            rootFolder.open();

        params[rootKey] = {};

        //walk the parameter tree
        unfurl(paramsGUI[rootKey], rootFolder, params[rootKey]);

        function unfurl(obj, folder, params){

            /*
                instead of just going through all members of obj in one loop,
                a second loop (outer) is needed to find the members in the right order,
                because many json implementation don't keep track of the insertion order.
                for example jsoncpp used by ofxSyncedParams does not ...
            */
            
            var size = Object.keys(obj["members"]).length;
            for(var i=0;i<size;++i){

                for (var key in obj["members"]){

                    var subObj = obj["members"][key];
                    if(subObj["orderIdx"] != i)
                        continue;

                    var leaf = isLeaf(subObj);
                    
                    if (leaf){
                        addToFolder(key, subObj, folder, params);
                    }
                    else{ 
                        //is folder
                        var subfolder = folder.addFolder(key);
                        if (!optionsGUI.folded)
                            subfolder.open();

                        params[key] = {};
                        unfurl(obj["members"][key], subfolder, params[key]);
                    }
                }

            }

            //a leaf object is one that contains no other objects
            //it is critical that none of the tracked parameters is itself an object
            function isLeaf(obj){

                return !obj.hasOwnProperty('members');

            }

        }

        function addToFolder(key, options, folder, params){

            var handle;
            params[key] = options.value;

            //type almost equals type (except float&int)
            var type = options.type || '';

            switch (type){
                case 'int':
                    handle = folder.add(params, key, options.min, options.max).step(1);
                    break;
                case 'float':
                    handle = folder.add(params, key, options.min, options.max);
                    break;
                case 'selector':
                    handle = folder.add(params, key, options.choices);
                    break;
                case 'color':
                    handle = folder.addColor(params, key);
                    break;
                case 'none':
                    break;
                default:
                    handle = folder.add(params, key);
                    break;
            }

            //if (handle && options.onChange)
            //    handle.onChange(options.onChange);


            // pass all changes through to OF 
            if (handle){

                //listen for sync in both directions
                handle.listen(true);

                var myObj = {};
                var path = [];
                var parent = folder;
                while(parent.name !== undefined){
                    path.unshift(parent.name);
                    parent = parent.parent;
                }

                
                var f = function(val){
                    myObj["value"] = val;
                    myObj["name"] = key;
                    myObj["path"] = path;
                    var jsonString = JSON.stringify(myObj);
                    console.log(jsonString);
                    socket.send(jsonString);
                }
                handle.onChange(f);

            }

            if (handle && options.onFinishChange)
                handle.onChange(options.onFinishChange);

            if (handle && options.listen)
                handle.listen();

        }

        return gui;

    }

}
