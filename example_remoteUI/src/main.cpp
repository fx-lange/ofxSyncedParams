#include "ofMain.h"
#include "ofAppGlutWindow.h"
#include "ofRemoteUIApp.h"

int main() {
	ofAppGlutWindow window;
	ofSetupOpenGL(&window, 800 + 32, (128 + 32) * 4, OF_WINDOW);
	ofRunApp(new ofRemoteUIApp());
}
