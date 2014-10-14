#ifndef __CONTROL_HPP__
#define __CONTROL_HPP__

namespace wezside
{
	class Control
	{
		private:
			int throttle;
			ofTrueTypeFont unibody, unibodyLarge;

		public:
			// Default constructor
			Control() 
			{
				throttle = 0;
			}

			// Copy constructor
			Control (const Control& s) 
			{
				throttle = s.throttle;
			}

			// Copy assignment constructor
			Control& operator=(Control& s) 
			{
				throttle = s.throttle; 
				return *this;
			};

			// Destructor
			virtual ~Control() {}	

			void draw()
			{
				drawRoll();
				drawPitch();
				drawThrottle(); // grey'd out
				drawThrottle(throttle); 
			}

			void drawRoll() 
			{
				ofPushMatrix();
				ofSetLineWidth(5);
				ofSetCircleResolution(100);
				ofTranslate(ofGetWidth() * 0.5 - 120.0f, 0.0 , 0);
				ofRotateZ(roll.z);
				ofNoFill();
				ofSetColor(241, 113, 154, 200);
				ofCircle(0, 0, 0, 100.0);

				ofPushMatrix();
				ofScale(1, -1, 1);
				ofRectangle rotate_rect = unibody.getStringBoundingBox(ofToString(roll.z), 0, 0);
				unibody.drawString(ofToString(roll.z), -rotate_rect.width * 0.5, 0);	
				ofPopMatrix();

				ofSetColor(255.0);
				float centerx = 0.0;
				float centery = 0.0;
				ofSetLineWidth(1);
				for (int i = 0; i < 360; i+= 4)
				{
					float inner_radius = 80;
					float outer_radius = 95;
					float x1 = inner_radius * cos(i * DEG_TO_RAD) + centerx;
					float y1 = inner_radius * sin(i * DEG_TO_RAD) + centery;
					float x2 = outer_radius * cos(i * DEG_TO_RAD) + centerx;
					float y2 = outer_radius * sin(i * DEG_TO_RAD) + centery;
					ofLine(x1, y1, x2, y2);
				}
				for (int i = 0; i < 360; i+= 2)
				{
					float inner_radius = 90;
					float outer_radius = 95;
					float x1 = inner_radius * cos(i * DEG_TO_RAD) + centerx;
					float y1 = inner_radius * sin(i * DEG_TO_RAD) + centery;
					float x2 = outer_radius * cos(i * DEG_TO_RAD) + centerx;
					float y2 = outer_radius * sin(i * DEG_TO_RAD) + centery;
					ofLine(x1, y1, x2, y2);
				}				
				ofPopMatrix();
			}
			void drawPitch() {}
			void drawThrottle(int index = -1) 
			{
				int max = index == -1 ? 30 : ofMap(index, 0, 100, 0, 30);
				if (max > 30) max = 30;
				ofFill();
				float yoffset = 230;	
				for (int i = 0; i < max; ++i)
				{
					ofPushMatrix();
					ofTranslate(-ofGetWidth() * 0.5 + 20, yoffset, 0);
					ofColor c = index == -1 ? ofColor(22, 22, 22) : ofColor(241, 113, 154, 255);
					ofSetColor(c);
					ofRectangle rect;
					rect.x = 0;
					rect.y = 0;
					rect.width = 50;
					rect.height = 10;
					ofRect(rect);	
					yoffset -= 15;	
					ofPopMatrix();	
				}				

				ofPushMatrix();
				ofScale(1, -1, 1);
				ofSetColor(122);
				int val = (int)throttle;
				if (val < 0) val = 0;
				if (val > 100) val = 100;
				ofRectangle throttle_rect = unibodyLarge.getStringBoundingBox(ofToString(val), 0, 0);
				unibodyLarge.drawString(ofToString((int)throttle), -ofGetWidth() * 0.5 + 60, -ofGetHeight() * 0.5 + 50);	
				ofPopMatrix();
			}
			void setThrottle(int t) 
			{
				if (t <= 0) throttle = 0;
				else if (t <= 100) throttle = t;
				else throttle = 100;
			}

			void setRollFont(ofTrueTypeFont& fm) {unibody = fm;}
			void setThrottleFont(ofTrueTypeFont& fm) {unibodyLarge = fm;}

			ofVec3f roll;
	};
}
#endif //__CONTROL_HPP__