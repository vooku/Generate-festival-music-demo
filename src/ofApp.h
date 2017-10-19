#pragma once

#include "ofMain.h"
#include "Oscillator.h"
#include "Granulizer.h"

class ofApp : public ofBaseApp{
public:
    enum class Mode {
        NOISE,
        SINE,
        SQUARE,
        TRIANGLE,
        AM,
        FM,
        LOOP,
        GRANULAR
    };

	void setup();
	//void update();
	void draw();

	void keyPressed  (int key);
	//void keyReleased(int key);
	void mouseMoved(int x, int y );
	//void mouseDragged(int x, int y, int button);
	//void mousePressed(int x, int y, int button);
	//void mouseReleased(int x, int y, int button);
	//void mouseEntered(int x, int y);
	//void mouseExited(int x, int y);
	//void windowResized(int w, int h);
	//void dragEvent(ofDragInfo dragInfo);
	//void gotMessage(ofMessage msg);
		
	void audioOut(float * input, int bufferSize, int nChannels);
		
private:
	ofSoundStream soundStream_;
    
    Mode mode_;
    Oscillator osc_;
    Oscillator modOsc_;
    Granulizer granulizer_;

    float frequency_;
    float modFrequency_;
	float volume_;
    bool soundOn_;
    
	std::vector<float> lAudio_;
	std::vector<float> rAudio_;
};
