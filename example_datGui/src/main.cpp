#include "ofMain.h"
#include "ofAppGlutWindow.h"
#include "ofDatGuiApp.h"

int main() {
	ofAppGlutWindow window;
	ofSetupOpenGL(&window, 400,500, OF_WINDOW);
	ofRunApp(new ofDatGuiApp());
}
