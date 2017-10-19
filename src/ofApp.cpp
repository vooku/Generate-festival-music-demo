#include "ofApp.h"

#define SAMPLE_RATE 44100
#define VOL 0.4f
#define LOOP_NAME "data/Jazz10.wav"
#define LOOP_GAIN 3.0f

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(34, 34, 34);

    osc_.setSampleRate(SAMPLE_RATE);
    modOsc_.setSampleRate(SAMPLE_RATE);
	
	int bufferSize = 512;
	volume_	= VOL;
    mode_ = Mode::SINE;
    soundOn_ = true;
    
    granulizer_.loadLoop(LOOP_NAME);
    
	lAudio_.assign(bufferSize, 0.0);
	rAudio_.assign(bufferSize, 0.0);
	
	soundStream_.printDeviceList();
	
	//if you want to set the device id to be different than the default
	//soundStream.setDeviceID(1); 	//note some devices are input only and some are output only 

	soundStream_.setup(this, 2, 0, SAMPLE_RATE, bufferSize, 4);
	
	ofSetFrameRate(60);
}

//--------------------------------------------------------------
void ofApp::draw(){

	ofSetColor(225);
	ofDrawBitmapString("AUDIO OUTPUT EXAMPLE", 32, 32);
	ofDrawBitmapString("'s' to pause / unpause the audio\n'1' for noise\n'2' for sine wave\n'3' for square wave\n'4' for triangle wave", 32, 62);
    ofDrawBitmapString("'5' for amplitude (ring) modulation\n'6' for frequency modulation\n'7' for looping a sample\n'8' for granular synthesis", 432, 62);
    
	ofNoFill();
	
	// draw the left channel:
	ofPushStyle();
		ofPushMatrix();
		ofTranslate(32, 150, 0);
			
		ofSetColor(225);
		ofDrawBitmapString("Left Channel", 4, 18);
		
		ofSetLineWidth(1);	
		ofDrawRectangle(0, 0, 900, 200);

		ofSetColor(245, 58, 135);
		ofSetLineWidth(3);
					
			ofBeginShape();
			for (unsigned int i = 0; i < lAudio_.size(); i++){
				float x =  ofMap(i, 0, lAudio_.size(), 0, 900, true);
				ofVertex(x, 100 -lAudio_[i]*180.0f);
			}
			ofEndShape(false);
			
		ofPopMatrix();
	ofPopStyle();

	// draw the right channel:
	ofPushStyle();
		ofPushMatrix();
		ofTranslate(32, 350, 0);
			
		ofSetColor(225);
		ofDrawBitmapString("Right Channel", 4, 18);
		
		ofSetLineWidth(1);	
		ofDrawRectangle(0, 0, 900, 200);

		ofSetColor(245, 58, 135);
		ofSetLineWidth(3);
					
			ofBeginShape();
			for (unsigned int i = 0; i < rAudio_.size(); i++){
				float x =  ofMap(i, 0, rAudio_.size(), 0, 900, true);
				ofVertex(x, 100 -rAudio_[i]*180.0f);
			}
			ofEndShape(false);
			
		ofPopMatrix();
	ofPopStyle();
	
		
	ofSetColor(225);
	string reportString = "volume: ("+ofToString(volume_, 2)+") modify with -/+ keys\nsynthesis: ";
    switch (mode_) {
        case Mode::NOISE:
            reportString += "noise";
            break;
        case Mode::SINE:
            reportString += "sine " + ofToString(frequency_, 2) + "hz";
            break;
        case Mode::SQUARE:
            reportString += "square " + ofToString(frequency_, 2) + "hz";
            break;
        case Mode::AM:
            reportString += "am " + ofToString(frequency_, 2) + "hz (mouse y), " + ofToString(modFrequency_, 2) +" hz (mouse x)";
            break;
        case Mode::FM:
            reportString += "fm " + ofToString(frequency_, 2) + "hz (mouse y), " + ofToString(modFrequency_, 2) + " hz (mouse x)";
            break;
        case Mode::LOOP:
            reportString += "looping " + std::string(LOOP_NAME);
            break;
        case Mode::GRANULAR:
            reportString += "granular " + ofToString(granulizer_.getGrainLen() * 1000, 2) + " ms";
            break;
    }
	
	ofDrawBitmapString(reportString, 32, 579);

}

//--------------------------------------------------------------
void ofApp::keyPressed  (int key){
	if (key == '-' || key == '_' ){
		volume_ -= 0.05;
		volume_ = MAX(volume_, 0);
	} else if (key == '+' || key == '=' ){
		volume_ += 0.05;
		volume_ = MIN(volume_, 1);
	}
	
	if ( key == 's' ){
        soundOn_ = !soundOn_;
        if (soundOn_) {
            soundStream_.start();
        } else {
            soundStream_.stop();
        }
	}

    switch (key) {
        case '1':
            mode_ = Mode::NOISE;
            break;
        case '2':
            mode_ = Mode::SINE;
            break;
        case '3':
            mode_ = Mode::SQUARE;
            break;
        case '4':
            mode_ = Mode::TRIANGLE;
            break;
        case '5':
            mode_ = Mode::AM;
            break;
        case '6':
            mode_ = Mode::FM;
            break;
        case '7':
            mode_ = Mode::LOOP;
            break;
        case '8':
            mode_ = Mode::GRANULAR;
        default:
            break;
    }    	
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
	float width = (float)ofGetWidth();
	float height = (float)ofGetHeight();
	float heightPct = (height-y) / height;
    float widthPct = (width-(width - x)) / width;
	frequency_ = 2000.0f * heightPct;
    modFrequency_ = 500.0f * widthPct;
    granulizer_.setGrainLen(granulizer_.getLoopLen() * widthPct);
}

//--------------------------------------------------------------
void ofApp::audioOut(float * output, int bufferSize, int nChannels) {
    for (int i = 0; i < bufferSize; i++) {
        float left, right;
        
        switch (mode_)
            {
            case Mode::NOISE:
                left = osc_.noise();
                right = osc_.noise();
                break;
            case Mode::SINE:
                left = right = osc_.sine(frequency_);
                break;
            case Mode::SQUARE:
                left = right = osc_.square(frequency_);
                break;
            case Mode::TRIANGLE:
                left = right = osc_.triangle(frequency_);
                break;
            case Mode::AM:
                left = right = osc_.sine(frequency_) * modOsc_.sine(modFrequency_);
                break;
            case Mode::FM:
                left = right = osc_.sine(frequency_ + 500 * modOsc_.sine(modFrequency_));
                break;
            case Mode::LOOP: {
                auto frame = granulizer_.straightFrame();
                left = LOOP_GAIN * frame.first;
                right = LOOP_GAIN * frame.second;
                break;
            }
            case Mode::GRANULAR: {
                auto frame = granulizer_.grainedFrame();
                left = LOOP_GAIN * frame.first;
                right = LOOP_GAIN * frame.second;
                break;
            }
            default:
                break;
        }

        lAudio_[i] = output[i*nChannels] = volume_ * left;
        rAudio_[i] = output[i*nChannels + 1] = volume_ * right;
    }
}
