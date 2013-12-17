#include "testApp.h"

void testApp::exit()
{
    ofLog(OF_LOG_NOTICE, "%s", "Exiting OF app.");
    controller.removeListener(listener);
}
void testApp::setup()
{
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    ofEnableSmoothing();
    ofDisableArbTex();
    ofBackground(0);
    ofSetLogLevel(OF_LOG_VERBOSE);

    m_bfullscreen = 0;
    camera.setFov(20);
    logo.loadImage(ofToDataPath("g3860.png"));

    unibody.loadFont("Unibody8Pro-Bold.otf", 14, true, true);	

    controller.addListener(listener);
    if(ofIsGLProgrammableRenderer()) shader.load("leap");
}
void testApp::update()
{
}
void testApp::draw()
{
    ofEnableSmoothing();
    ofEnableBlendMode(OF_BLENDMODE_ADD);

    ofBackgroundGradient(ofColor(90, 90, 90), ofColor(30, 30, 30), OF_GRADIENT_LINEAR);
    camera.begin();

    ofTranslate(0, 0, -1500);

    ofPushMatrix();
    // ofScale(1, -1, 1);
    ofTranslate(-logo.getWidth()* 0.5 + ofGetWidth(), ofGetHeight() - logo.getHeight(), -2000);
    ofSetColor(132);
    logo.draw(0, 0);
    ofPopMatrix();

    ofScale(1, -1, 1);
    ofVec3f origin = ofVec3f(0.0);
    ofVec3f distance = origin - listener.hand_pos;

    float newheight = ofMap(listener.hand_pos.y, 30.0, 300.0, 0.0, 200.0);
    
    std::string hand_pos_str = "";
    
    ofColor status;
    status.r = 227;
    status.g = 7;
    status.b = 77;

    ofLog(OF_LOG_NOTICE, "%f", listener.hand_pos.z);
    if (distance.length() == 0)
    {		
        hand_pos_str = "Player lost";	
        status.r = 132;
        status.g = 132;
        status.b = 132;
    }	
    else
    {
        hand_pos_str = "Tracking player";
        rotate_x = listener.hand_pitch * RAD_TO_DEG * -1 + 90.0;
        rotate_z = listener.hand_roll * RAD_TO_DEG * -1;
    }

    // Draw hand status
    ofSetColor(status); 		
    ofCircle(0, 215, 10);
    float xpos = hand_pos_str.compare("Tracking player") == 0 ? -100 : -80;
    ofSetColor(245, 58, 135);
    ofPushMatrix();
    ofScale(1, -1, 1);
    unibody.drawString(hand_pos_str, xpos, -250);	
    ofPopMatrix();

    // shader.begin();
    ofSetColor(241, 113, 154, 100);
    ofPushMatrix();
    ofTranslate(0, -newheight + 200.0, 0);
    ofRotateX(rotate_x);
    ofRotateY(rotate_z);
    ofNoFill();
    ofTriangle(-100, 100, 0, 100, 100, 0, 0, -100, 0);
    ofFill();
    ofRectangle rect_pitch;
    rect_pitch.x = -5;
    rect_pitch.y = -90;
    rect_pitch.width = 10;
    rect_pitch.height = 190;
    ofRect(rect_pitch);
    ofPopMatrix();
    
    ofPushMatrix();
    ofTranslate(-ofGetWidth() * 0.5 + 110.0, -220.0, 0);

    ofRotateZ(rotate_z);
    ofNoFill();
    ofSetColor(241, 113, 154, 200);
    ofCircle(0, 0, 0, 20.0);

    ofFill();
    ofRectangle rect_roll;
    rect_roll.x = -20;
    rect_roll.y = 0;
    rect_roll.width = 40;
    rect_roll.height = 2;
    ofRect(rect_roll);
    ofNoFill();
    ofSetColor(241, 113, 154, 250);
    ofRect(rect_roll);
    ofPopMatrix();

    float throttle = ofMap(listener.hand_pos.y, 30.0, 300.0, 0.0, 300.0);
    setThrottle();
    setThrottle((int)throttle % 20);

    // shader.end();
    camera.end();
    ofDisableBlendMode();
    ofDisableSmoothing();
    ofSetWindowTitle(ofToString(ofGetFrameRate(), 2) + "fps");
}
void testApp::setThrottle(int index)
{
    int max = index == -1 ? 20 : index;
    ofFill();
    float yoffset = 230;	
    for (int i = 0; i < max; ++i)
    {
        ofPushMatrix();
        ofColor c = index == -1 ? ofColor(22, 22, 22) : ofColor(241, 113, 154, 255);
        ofSetColor(c);
        ofTranslate(-560, yoffset, 0);
        ofRectangle rect;
        rect.x = 0;
        rect.y = 0;
        rect.width = 50;
        rect.height = 10;
        ofRect(rect);	
        yoffset -= 15;	
        ofPopMatrix();
    }
}
void testApp::keyPressed(int key){}
void testApp::keyReleased(int key)
{
    switch (key)
    {
        case 'F':
        case 'f': m_bfullscreen = !m_bfullscreen; ofSetFullscreen(m_bfullscreen); break;
    }
}
void testApp::mouseMoved(int x, int y ){}
void testApp::mouseDragged(int x, int y, int button){}
void testApp::mousePressed(int x, int y, int button){}
void testApp::mouseReleased(int x, int y, int button){}
void testApp::windowResized(int w, int h){}
void testApp::gotMessage(ofMessage msg){}
void testApp::dragEvent(ofDragInfo dragInfo){}

 
// --------------  LeapListener  ----------------------
void LeapListener::onInit(const Controller& controller) 
{
    std::cout << "Initialized" << std::endl;
}
void LeapListener::onConnect(const Controller& controller) 
{
    std::cout << "Connected" << std::endl;
    controller.enableGesture(Gesture::TYPE_CIRCLE);
    controller.enableGesture(Gesture::TYPE_KEY_TAP);
    controller.enableGesture(Gesture::TYPE_SCREEN_TAP);
    controller.enableGesture(Gesture::TYPE_SWIPE);
}
void LeapListener::onDisconnect(const Controller& controller) 
{
    //Note: not dispatched when running in a debugger.
    std::cout << "Disconnected" << std::endl;
}
void LeapListener::onExit(const Controller& controller) 
{
    std::cout << "Exited" << std::endl;
}
void LeapListener::onFrame(const Controller& controller) 
{
    const Frame frame = controller.frame();
    // if (frame.hands().count() == 0 || frame.hands().empty()) return;

/*	std::cout << "Frame id: " << frame.id()
            << ", timestamp: " << frame.timestamp()
            << ", hands: " << frame.hands().count()
            << ", fingers: " << frame.fingers().count()
            << ", tools: " << frame.tools().count()
            << ", gestures: " << frame.gestures().count() << std::endl;*/

    // Get the first hand
    const Hand hand = frame.hands()[0];

    // Get the hand's normal vector and direction
    const Vector normal = hand.palmNormal();
    const Vector direction = hand.direction();

    // Get the hand's sphere radius and palm position
    // std::cout << "Hand sphere radius: " << hand.sphereRadius()
    //   		  << " mm, palm position: "  << hand.palmPosition() << std::endl;

    // Calculate the hand's pitch, roll, and yaw angles
/*	std::cout << "Hand pitch: " << direction.pitch() * RAD_TO_DEG 	<< " degrees, "
              << "roll: " << normal.roll() * RAD_TO_DEG << " degrees, "
              << "yaw: " << direction.yaw() * RAD_TO_DEG << " degrees" << std::endl;	*/

    hand_pitch = direction.pitch();
    hand_roll = normal.roll();
    hand_yaw = direction.yaw();
    hand_pos = ofVec3f(hand.palmPosition().x, hand.palmPosition().y, hand.palmPosition().z);
}

/*void LeapListener::onFrame(const Controller& controller) 
{
    // Get the most recent frame and report some basic information
    const Frame frame = controller.frame();
    std::cout << "Frame id: " << frame.id()
                        << ", timestamp: " << frame.timestamp()
                        << ", hands: " << frame.hands().count()
                        << ", fingers: " << frame.fingers().count()
                        << ", tools: " << frame.tools().count()
                        << ", gestures: " << frame.gestures().count() << std::endl;

    if (!frame.hands().empty()) 
    {
        // Get the first hand
        const Hand hand = frame.hands()[0];

        // Check if the hand has any fingers
        const FingerList fingers = hand.fingers();
        if (!fingers.empty()) 
        {
            // Calculate the hand's average finger tip position
            Vector avgPos;
            for (int i = 0; i < fingers.count(); ++i) 
            {
                avgPos += fingers[i].tipPosition();
            }
            avgPos /= (float)fingers.count();
            std::cout << "Hand has " << fingers.count()
                                << " fingers, average finger tip position" << avgPos << std::endl;
        }

        // Get the hand's sphere radius and palm position
        std::cout << "Hand sphere radius: " << hand.sphereRadius()
                            << " mm, palm position: "  << hand.palmPosition() << std::endl;

        // Get the hand's normal vector and direction
        const Vector normal = hand.palmNormal();
        const Vector direction = hand.direction();

        // Calculate the hand's pitch, roll, and yaw angles
        std::cout << "Hand pitch: " << direction.pitch() * RAD_TO_DEG 	<< " degrees, "
                        << "roll: " << normal.roll() * RAD_TO_DEG << " degrees, "
                            << "yaw: " << direction.yaw() * RAD_TO_DEG << " degrees" << std::endl;
    }

    // Get gestures
    const GestureList gestures = frame.gestures();
    for (int g = 0; g < gestures.count(); ++g) 
    {
        Gesture gesture = gestures[g];
        switch (gesture.type()) 
        {
            case Gesture::TYPE_CIRCLE:
            {
                CircleGesture circle = gesture;
                std::string clockwiseness;

                if (circle.pointable().direction().angleTo(circle.normal()) <= PI/4) 
                {
                    clockwiseness = "clockwise";
                } 
                else 
                {
                    clockwiseness = "counterclockwise";
                }

                // Calculate angle swept since last frame
                float sweptAngle = 0;
                if (circle.state() != Gesture::STATE_START) 
                {
                    CircleGesture previousUpdate = CircleGesture(controller.frame(1).gesture(circle.id()));
                    sweptAngle = (circle.progress() - previousUpdate.progress()) * 2 * PI;
                }
                std::cout << "Circle id: " << gesture.id()
                                    << ", state: " << gesture.state()
                                    << ", progress: " << circle.progress()
                                    << ", radius: " << circle.radius()
                                    << ", angle " << sweptAngle * RAD_TO_DEG
                                    <<  ", " << clockwiseness << std::endl;
                break;
            }
            case Gesture::TYPE_SWIPE:
            {
                SwipeGesture swipe = gesture;
                std::cout << "Swipe id: "<< gesture.id()
                                << ", state: " << gesture.state()
                                << ", direction: "<< swipe.direction()
                                << ", speed: " << swipe.speed() << std::endl;
                break;
            }
            case Gesture::TYPE_KEY_TAP:
            {
                KeyTapGesture tap = gesture;
                std::cout << "Key Tap id: "<< gesture.id()
                                << ", state: " << gesture.state()
                                << ", position: " << tap.position()
                                << ", direction: " << tap.direction()<< std::endl;
                break;
            }
            case Gesture::TYPE_SCREEN_TAP:
            {
                ScreenTapGesture screentap = gesture;
                std::cout << "Screen Tap id: " << gesture.id()
                                << ", state: " << gesture.state()
                                << ", position: " << screentap.position()
                                << ", direction: " << screentap.direction()<< std::endl;
                break;
            }
            default:
                std::cout << "Unknown gesture type." << std::endl;
                break;
        }
    }

    if (!frame.hands().empty() || !gestures.empty()) 
    {
        std::cout << std::endl;
    }
}*/

void LeapListener::onFocusGained(const Controller& controller) 
{
    std::cout << "Focus Gained" << std::endl;
}

void LeapListener::onFocusLost(const Controller& controller) 
{
    std::cout << "Focus Lost" << std::endl;
}