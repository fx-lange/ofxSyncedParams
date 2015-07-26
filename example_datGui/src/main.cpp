#include "ofMain.h"
#include "ofAppGlutWindow.h"
#include "ofDatGuiApp.h"

int main() {
	ofAppGlutWindow window;
	ofSetupOpenGL(&window, 500,600, OF_WINDOW);
	ofRunApp(new ofDatGuiApp());
}
