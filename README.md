# ofxSyncedParams

OpenFrameworks addon to control your ofParameters with "any" remote UI, independent of language, platform and connection. Use-Cases could be fine tuning your installation with a smartphone/tablet, remote controlling over THE internet or a dedicated GUI for a multi PC setup. 

ofxSyncedParams parses your ofParameterGroup (or ofxGui) to JSON and keeps your parameters in sync in both directions. Check out the examples and let me know if you run in any issues.

Examples
---------
* *example_datGui*: This example uses datGui (JS) as a web interface to control the parameters of your ofApp. Run the app and press ENTER to start the web UI in your default browser. The datGui will show up after a short delay and will be in sync already. Feel free to play around and add more parameters to the ofApp. 

* *example_remoteUI*: Instead of using a thrid party UI, in this example the remote controller is written in oF itself. Run the first example before you start this remote controller and hit ENTER to connect them. Both ofxGui instances should be in sync.

Writing your own remote (web) UI
--------------------------------

At the moment datGui is the only non-oF GUI which is supported out of the box. Check out the `example_datGui/bin/data/web/js/guiGlueMod.js` as a starting point to write your own Json->UI parser. Basicly you need handle three cases:

* walking through the initial JSON message which contains all infos to setup the remote UI
* sending updates per parameter onChange()
* updating your parameters on JSON update messages

Best contact me in advance - happy to support.

Dependencies
------------
* ofxLibwebsockets
* ofxJSON
* current openFrameworks master ~0.9.0 (27 July 2015). It won't work with oF 0.8.4 even if it would compile. Let me know if you need a 0.8.4 version.

Credits
--------
* Based on datGuiController by [ofZach](https://github.com/ofZach). Thanks for sharing!
* Using a modified version of [guiGlue.js](https://github.com/dmvaldman/guiGlue) by David Valdman.
