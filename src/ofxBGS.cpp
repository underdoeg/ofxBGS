#include "WeightedMovingVarianceBGS.h"
#include "WeightedMovingMeanBGS.h"
#include "MixtureOfGaussianV2BGS.h"
#include "MixtureOfGaussianV1BGS.h"
#include "FrameDifferenceBGS.h"
#include "ofxBGS.h"
#include "AdaptiveBackgroundLearning.h"

ofxBGS::ofxBGS() {
	curProcessor = NULL;
	setProcessor(FrameDifference);
}

ofxBGS::~ofxBGS() {
}

void ofxBGS::update(cv::Mat mat) {
	frame = mat;
	if(curProcessor == NULL) {
		ofLogWarning(BGS_MODULE, "no processor set");
		return;
	}

	curProcessor->process(frame, foreground);
	if(foreground.channels()==1){
		foregroundBW = foreground;
	}else{
		cv::cvtColor(foreground, foregroundBW, CV_BGR2GRAY);
	}
	cv::threshold(foregroundBW, foregroundMask, 30, 255, cv::THRESH_BINARY);
}

void ofxBGS::update(ofPixelsRef pixels) {
	update(ofxCv::toCv(pixels));
}

void ofxBGS::draw(int x, int y) {
	ofPushMatrix();
	ofPushStyle();
	ofSetColor(255);

	ofxCv::drawMat(frame, 0, 0, 320, 240);
	ofxCv::drawMat(foreground, 320, 0, 320, 240);
	ofxCv::drawMat(foregroundBW, 0, 240, 320, 240);
	ofxCv::drawMat(foregroundMask, 320, 240, 320, 240);

	ofPopStyle();
	ofPopMatrix();
}

ofImage ofxBGS::getForegroundImage() {
}

cv::Mat ofxBGS::getForegroundMat() {
}

ofPixelsRef ofxBGS::getForegroundPixels() {
}

void ofxBGS::setProcessor(Processor processor) {
	ProcessorMap::iterator it = processors.find(processor);
	if(it != processors.end()) {
		curProcessor = it->second;
		return;
	}

	/*
		AdaptiveBackgroundLearning,
		FrameDifference,
		MixtureOfGaussianV1,
		MixtureOfGaussianV2,
		StaticFrameDifference,
		WeightedMovingMeans,
		WeightedMovingVariance
	*/

	switch(processor) {
	case(AdaptiveBgLearning):
		addProcessor(AdaptiveBgLearning, new AdaptiveBackgroundLearning());
		break;
	case(FrameDifference):
		addProcessor(FrameDifference, new FrameDifferenceBGS());
		break;
	case(MixtureOfGaussianV1):
		addProcessor(FrameDifference, new MixtureOfGaussianV1BGS());
		break;
	case(MixtureOfGaussianV2):
		addProcessor(FrameDifference, new MixtureOfGaussianV2BGS());
		break;
	case(WeightedMovingMeans):
		addProcessor(FrameDifference, new WeightedMovingMeanBGS());
		break;
	case(WeightedMovingVariance):
		addProcessor(FrameDifference, new WeightedMovingVarianceBGS());
		break;
	}
}

void ofxBGS::addProcessor(Processor type, IBGS* p) {
	processors[type] = p;
	curProcessor = p;
}
