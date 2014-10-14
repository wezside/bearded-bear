#pragma once

#include "ofMain.h"
#include "Leap.h"
#include "control.hpp"

using namespace Leap;

class LeapListener : public Listener 
{
	public:
		virtual void onInit(const Controller&);
		virtual void onConnect(const Controller&);
		virtual void onDisconnect(const Controller&);
		virtual void onExit(const Controller&);
		virtual void onFrame(const Controller&);
		virtual void onFocusGained(const Controller&);
		virtual void onFocusLost(const Controller&);

		ofVec3f hand_pos;
		float hand_pitch, hand_roll, hand_yaw;
};

class testApp : public ofBaseApp
{
	private:
		Controller controller;
		LeapListener listener;
		
		ofCamera camera;
		ofImage logo;
		ofShader shader;
		ofTrueTypeFont unibody, unibodyLarge;
		ofSerial serial;
		ofColor status;

		float rotate_x;
		float rotate_z;
		unsigned char throttle;
		bool serialInit;
		std::string hand_pos_str;

	public:
		void exit();
		void setup();
		void update();
		void draw();
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		void drawThrottle(int index = -1);

		wezside::Control control;
		float hand_pitch, hand_roll, hand_yaw, newheight;
};
