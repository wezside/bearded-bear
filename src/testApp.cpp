#include "testApp.h"

void testApp::exit()
{
	ofLog(OF_LOG_NOTICE, "%s", "Exiting OF app.");
	//controller.removeListener(listener);
}
void testApp::setup()
{
	ofSetVerticalSync(true);
	ofSetFrameRate(60);

	ofDisableArbTex();
	ofBackground(0);

	logo.loadImage(ofToDataPath("g3860.png"));

 	unibody.loadFont("Unibody8Pro-Bold.otf", 14, true, true);	
 	unibodyLarge.loadFont("Unibody8Pro-Bold.otf", 164, true, true);	
 	
 	// Set the font to use for the throttle value 
 	control.setRollFont(unibody);
 	control.setThrottleFont(unibodyLarge);

 	// Initialize serial comms
 	if (!serial.setup("/dev/ttyACM0", 115200))
 	{
 		ofLog(OF_LOG_NOTICE, "%s", "Couldn't initialise Serial"); 		
 	}

	controller.addListener(listener);
	if(ofIsGLProgrammableRenderer()) shader.load("leap");
}
void testApp::update()
{
	if (listener.hand_pos.y > 0) throttle = ofMap(listener.hand_pos.y, 50, 300, 0, 100, true);
	else if (throttle > 0)
	{
		throttle -= 0.5;
	}
	else throttle = 0;

	// The height from Leap mapped to value usable on screen
	if (listener.hand_pos.y > 0.0f) newheight = ofMap(listener.hand_pos.y, 30.0, 300.0, 0.0, 250.0, true);
	else if (listener.hand_pos.y == 0.0f && newheight > 0.0f) newheight -= 1;
	else newheight = 0.0f;

	control.setThrottle((int)throttle);

	status.r = 227;
	status.g = 7;
	status.b = 77;

	ofVec3f origin = ofVec3f(0.0);
	ofVec3f distance = origin - listener.hand_pos;
	// Distance will be zero when a hand is not tracked 
	if (distance.length() == 0)
	{		
		status.r = 132;
		status.g = 132;
		status.b = 132;
		if (static_cast<int>(control.roll.x) > 90.0f) control.roll.x -= 1;
		else if (static_cast<int>(control.roll.x) < 90.0f) control.roll.x += 1;
		else control.roll.x = 90.0f;
		
		// if (static_cast<int>(control.roll.z) > 0.0f) control.roll.z -= 1;
		if (static_cast<int>(control.roll.z) < 0.0f) control.roll.z += 1;
		else control.roll.z = 0.0f;
		hand_pos_str = "Not tracking";	
	}	
	else
	{
		status.r = 245;
		status.g = 58;
		status.b = 135;
		hand_pos_str = "Tracking";
		control.roll.x = listener.hand_pitch * RAD_TO_DEG * -1 + 90.0;
		control.roll.z = listener.hand_roll * RAD_TO_DEG * -1;
	}
	if (ofGetFrameNum() % 2 == 0) serial.writeByte(throttle);
}
void testApp::draw()
{
	ofEnableSmoothing();
	ofEnableAntiAliasing();
	ofEnableBlendMode(OF_BLENDMODE_ADD);

	ofBackground(ofColor(30));
	camera.begin();

	// Translate camera 
	ofTranslate(0, 0, -500);

	// Draw Logo 
	ofPushMatrix();
	ofTranslate(ofGetWidth() - logo.getWidth() - 40, ofGetHeight() - logo.getHeight() - 40, -500);
	ofSetColor(132);
	logo.draw(0, 0);
	ofPopMatrix();

	ofScale(1, -1, 1);	

	// Draw hand status
	ofRectangle player_rect = unibody.getStringBoundingBox(hand_pos_str, 0, 0);
	ofSetColor(status);
	ofPushMatrix();
	ofScale(1, -1, 1);
	unibody.drawString(hand_pos_str, -player_rect.width * 0.5, ofGetHeight() * 0.5 - unibody.getLineHeight() * 2 - 40);	
	ofPopMatrix();

	// Draw the craft
	ofSetColor(241, 113, 154, 100);
	ofPushMatrix();
	ofTranslate(0, -newheight + 170.0f, 0);
	ofRotateX(control.roll.x);
	ofRotateY(control.roll.z);
	ofTriangle(-100, 100, 0, 100, 100, 0, 0, -100, 0);
	ofFill();
	ofRectangle rect_pitch;
	rect_pitch.x = -5;
	rect_pitch.y = -90;
	rect_pitch.width = 10;
	rect_pitch.height = 190;
	ofRect(rect_pitch);
    ofPopMatrix();
	
	// Draw controls
 	control.draw();
	
	// shader.end();
	camera.end();
	ofDisableBlendMode();
	ofDisableAntiAliasing();
	ofDisableSmoothing();
	ofSetWindowTitle(ofToString(ofGetFrameRate(), 2) + "fps");
}
void testApp::drawThrottle(int index)
{
	
}
void testApp::keyPressed(int key){}
void testApp::keyReleased(int key)
{
	switch (key)
	{
		case 'F':
		case 'f': ofToggleFullscreen(); break;
		case '+': serial.writeByte(throttle++); break;
		case '-': serial.writeByte(throttle--); break;
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

	// ofLog(OF_LOG_NOTICE, "%s %d", "Total fingers", hand.fingers().count());

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