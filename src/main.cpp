#include "ofMain.h"
#include "testApp.h"
#include "ofAppGlutWindow.h"

//========================================================================
int main( )
{
    // say that we're going to *use* the fancy new renderer
    ofSetCurrentRenderer(ofGLProgrammableRenderer::TYPE);

    ofAppGlutWindow window;  
    window.setGlutDisplayString("rgba double samples>=2");
    ofSetupOpenGL(&window, 1280, 576, OF_WINDOW);    

    //ofSetupOpenGL(1280,576,OF_WINDOW);			// <-------- setup the GL context

    // this kicks off the running of my app
    // can be OF_WINDOW or OF_FULLSCREEN
    // pass in width and height too:
    ofRunApp(new testApp());

}
