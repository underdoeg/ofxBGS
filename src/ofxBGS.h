#ifndef OFXBGS_H
#define OFXBGS_H

#include "ofMain.h"
#include "ofxCv.h"

class ofxBGS
{
public:
	ofxBGS();
	~ofxBGS();

	void draw(int x=0, int y = 0);
	void update(cv::Mat mat);
	void update(ofPixelsRef pixels);

	ofPixelsRef getForegroundPixels();
	ofImage getForegroundImage();
	cv::Mat getForegroundMat();

};

#endif // OFXBGS_H
