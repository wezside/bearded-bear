#include "testApp.h"

void testApp::exit()
{
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

    ofBackgroundGradient(ofColor(90, 90, 90), ofColor(30, 30, 30),  OF_GRADIENT_BAR);
    camera.begin();
    ofTranslate(0, 0, -500);
    ofScale(1, -1, 1);

    ofVec3f origin = ofVec3f(0.0);
    ofVec3f distance = origin - listener.hand_pos;
    ofLog(OF_LOG_NOTICE, "%f", distance.length());
    
    float rotate_x = 0.0;
    float rotate_z = 0.0;
    std::string hand_pos_str = "";
    
    ofColor status;
    status.r = 0;
    status.g = 255;
    status.b = 0;

    if (distance.length() == 0 || distance.length() > 200.0)
    {
        status.r = 255;
        status.g = 0;
        status.b = 0;
        hand_pos_str = "Move your hand closer";
    }
    else
    {
        hand_pos_str = "Perfect";
        rotate_x = listener.hand_pitch * RAD_TO_DEG + 90.0;
        rotate_z = listener.hand_roll * RAD_TO_DEG * -1;
    }	
    // Draw hand status
    ofSetColor(status); 		
    ofCircle(10, 10, 20);
    ofDrawBitmapString(hand_pos_str, 10,100);

    shader.begin();
    ofPushMatrix();
    ofRotateX(rotate_x);
    ofRectangle rect_pitch;
    rect_pitch.x = -5;
    rect_pitch.y = -100;
    rect_pitch.width = 10;
    rect_pitch.height = 200;
    ofRect(rect_pitch);
    ofPopMatrix();

    ofPushMatrix();
    ofRotateZ(rotate_z);
    ofRectangle rect_roll;
    rect_roll.x = -100;
    rect_roll.y = -5;
    rect_roll.width = 200;
    rect_roll.height = 10;
    ofRect(rect_roll);
    ofPopMatrix();

    float newheight = ofClamp(ofMap(listener.hand_pos.y, 35.0, 300.0, 0.0, 200.0), 0.0, 250.0);
    ofPushMatrix();
    ofRectangle rect_throttle;
    rect_throttle.x = -200;
    rect_throttle.y = -newheight;
    rect_throttle.width = 50;
    rect_throttle.height = newheight;
    ofRect(rect_throttle);
    ofPopMatrix();

    shader.end();
    camera.end();
    ofDisableBlendMode();
    ofDisableSmoothing();
    ofSetWindowTitle(ofToString(ofGetFrameRate(), 2)+"fps");
}
void testApp::keyPressed(int key){}
void testApp::keyReleased(int key){}
void testApp::mouseMoved(int x, int y ){}
void testApp::mouseDragged(int x, int y, int button){}
void testApp::mousePressed(int x, int y, int button){}
void testApp::mouseReleased(int x, int y, int button){}
void testApp::windowResized(int w, int h){}
void testApp::gotMessage(ofMessage msg){}
void testApp::dragEvent(ofDragInfo dragInfo){}

 
// --------------  LeapListener  --------------------------------
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