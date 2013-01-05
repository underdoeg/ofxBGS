#ifndef OFXBGS_H
#define OFXBGS_H

#include "ofMain.h"
#include "ofxCv.h"
#include "IBGS.h"

#define BGS_MODULE "ofxBGS"

class ofxBGS
{
public:
	enum Processor{
		AdaptiveBgLearning,
		FrameDifference,
		MixtureOfGaussianV1,
		MixtureOfGaussianV2,
		StaticFrameDifference,
		WeightedMovingMeans,
		WeightedMovingVariance,
		///
		DPAdaptiveMedian,
		DPGrimsonGMM,
		DPZivkovicAGMM,
		DPMean,
		DPWrenGA,
		DPPratiMediod,
		DPEigenbackground,
		///
		TBT2FGMM_UM,
		TBT2FGMM_UV,
		TBFuzzySugenoIntegral,
		TBFuzzyChoquetIntegral,
		///
		MultiLayer,
		///
		SimpleGaussian,
		FuzzyGaussian,
		MixtureOfGaussians,
		AdaptiveSOM,
		FuzzyAdaptiveSOM
	};

	typedef std::map<int, IBGS*> ProcessorMap;
	typedef std::vector<Processor> ProcessorList;

	ofxBGS();
	~ofxBGS();

	void draw(int x=0, int y = 0);
	void update(cv::Mat mat);
	void update(ofPixelsRef pixels);

	ofPixelsRef getForegroundPixels();
	ofImage getForegroundImage();
	cv::Mat& getForegroundMat();

	void setProcessor(Processor processor);
	void nextProcessor();
	void previousProcessor();
private:
	void setProcessor(Processor type, IBGS* p);
	ProcessorMap processors;

	ProcessorList availableProcessors;
	ProcessorList::iterator curProcessorIt;

	IBGS* curProcessor;
	cv::Mat frame;
	cv::Mat frameBW;
	cv::Mat foreground;
	cv::Mat foregroundBW;
	cv::Mat foregroundMask;
	cv::Mat frameMasked;
	ofImage foregroundImg;

	ofDirectory configDir;
};

#endif // OFXBGS_H
