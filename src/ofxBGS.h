#ifndef OFXBGS_H
#define OFXBGS_H

#include "ofMain.h"
#include "ofxCv.h"
#include "IBGS.h"

#define BGS_MODULE "ofxBGS"

class ofxBGS
{
public:
	typedef std::map<int, IBGS*> ProcessorMap;

	enum Processor{
		AdaptiveBgLearning,
		FrameDifference,
		MixtureOfGaussianV1,
		MixtureOfGaussianV2,
		StaticFrameDifference,
		WeightedMovingMeans,
		WeightedMovingVariance
	};

	ofxBGS();
	~ofxBGS();

	void draw(int x=0, int y = 0);
	void update(cv::Mat mat);
	void update(ofPixelsRef pixels);

	ofPixelsRef getForegroundPixels();
	ofImage getForegroundImage();
	cv::Mat getForegroundMat();

	void setProcessor(Processor processor);
private:
	void addProcessor(Processor type, IBGS* p);
	ProcessorMap processors;
	IBGS* curProcessor;
	cv::Mat frame;
	cv::Mat frameBW;
	cv::Mat foreground;
	cv::Mat foregroundBW;
	cv::Mat foregroundMask;
};

#endif // OFXBGS_H
