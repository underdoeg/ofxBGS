#include "package_bgs/IBGS.h"

#include "package_bgs/FrameDifferenceBGS.h"
#include "package_bgs/StaticFrameDifferenceBGS.h"
#include "package_bgs/WeightedMovingMeanBGS.h"
#include "package_bgs/WeightedMovingVarianceBGS.h"
#include "package_bgs/MixtureOfGaussianV1BGS.h"
#include "package_bgs/MixtureOfGaussianV2BGS.h"
#include "package_bgs/AdaptiveBackgroundLearning.h"

#include "package_bgs/dp/DPAdaptiveMedianBGS.h"
#include "package_bgs/dp/DPGrimsonGMMBGS.h"
#include "package_bgs/dp/DPZivkovicAGMMBGS.h"
#include "package_bgs/dp/DPMeanBGS.h"
#include "package_bgs/dp/DPWrenGABGS.h"
#include "package_bgs/dp/DPPratiMediodBGS.h"
#include "package_bgs/dp/DPEigenbackgroundBGS.h"

#include "package_bgs/tb/T2FGMM_UM.h"
#include "package_bgs/tb/T2FGMM_UV.h"
#include "package_bgs/tb/FuzzySugenoIntegral.h"
#include "package_bgs/tb/FuzzyChoquetIntegral.h"

#include "package_bgs/jmo/MultiLayerBGS.h"

#include "package_bgs/lb/LBSimpleGaussian.h"
#include "package_bgs/lb/LBFuzzyGaussian.h"
#include "package_bgs/lb/LBMixtureOfGaussians.h"
#include "package_bgs/lb/LBAdaptiveSOM.h"
#include "package_bgs/lb/LBFuzzyAdaptiveSOM.h"

//#include "package_analysis/ForegroundMaskAnalysis.h"

#include "ofxBGS.h"

ofxBGS::ofxBGS() {
	//it's not really a good idea to fill the vector from an enum like that... but yeah...
	for(int i=0;i<FuzzyAdaptiveSOM;i++){
		availableProcessors.push_back(static_cast<Processor>(i));
	}

	curProcessor = NULL;
	setProcessor(FrameDifference);

	configDir.open("../config/");
	if(!configDir.exists()){
		if(!configDir.create()){
			ofLogFatalError(BGS_MODULE, "could not create config directory");
		}
	}
}

ofxBGS::~ofxBGS() {
}

void ofxBGS::update(cv::Mat mat) {
	frame = mat;
	if(curProcessor == NULL) {
		ofLogWarning(BGS_MODULE, "no processor set");
		return;
	}

	cv::GaussianBlur(frame, frame, cv::Size(7,7), 1.5);

	curProcessor->process(frame, foreground);
	if(foreground.channels()==1) {
		foregroundBW = foreground;
	} else {
		cv::cvtColor(foreground, foregroundBW, CV_BGR2GRAY);
	}

	//cv::erode(foregroundMask, foregroundMask,cv::Mat(), cv::Point(-1,-1), 4);
	cv::threshold(foregroundBW, foregroundMask, 30, 255, cv::THRESH_BINARY);
}

void ofxBGS::update(ofPixelsRef pixels) {
	update(ofxCv::toCv(pixels));
}

void ofxBGS::draw(int x, int y) {
	ofPushMatrix();
	ofTranslate(x, y);
	ofPushStyle();

	ofSetColor(0);
	ofRect(0, 0, 660, 500);

	ofTranslate(10, 10);
	ofSetColor(255);
	ofxCv::drawMat(frame, 0, 0, 320, 240);
	ofDrawBitmapStringHighlight("INPUT FRAME", 5, 15);

	ofxCv::drawMat(foreground, 320, 0, 320, 240);
	ofDrawBitmapStringHighlight("FOREGROUND", 325, 15);

	ofxCv::drawMat(foregroundBW, 0, 240, 320, 240);
	ofDrawBitmapStringHighlight("FOREGROUND B&W", 5, 255);

	ofxCv::drawMat(foregroundMask, 320, 240, 320, 240);
	ofDrawBitmapStringHighlight("FOREGROUND MASK", 325, 255);

	ofPopStyle();
	ofPopMatrix();
}

ofImage ofxBGS::getForegroundImage() {
	ofxCv::toOf(foregroundMask, foregroundImg);
	return foregroundImg;
}

cv::Mat ofxBGS::getForegroundMat() {
	return foregroundMask;
}

ofPixelsRef ofxBGS::getForegroundPixels() {
	ofxCv::toOf(foregroundMask, foregroundImg);
	return foregroundImg.getPixelsRef();
}

void ofxBGS::setProcessor(Processor processor) {
	ProcessorMap::iterator it = processors.find(processor);
	if(it != processors.end()) {
		setProcessor(processor, it->second);
		return;
	}

	switch(processor) {
	case(AdaptiveBgLearning):
		setProcessor(processor, new AdaptiveBackgroundLearning());
		break;
	case(FrameDifference):
		setProcessor(processor, new FrameDifferenceBGS());
		break;
	case(MixtureOfGaussianV1):
		setProcessor(processor, new MixtureOfGaussianV1BGS());
		break;
	case(MixtureOfGaussianV2):
		setProcessor(processor, new MixtureOfGaussianV2BGS());
		break;
	case(StaticFrameDifference):
		setProcessor(processor, new StaticFrameDifferenceBGS());
		break;
	case(WeightedMovingMeans):
		setProcessor(processor, new WeightedMovingMeanBGS());
		break;
	case(WeightedMovingVariance):
		setProcessor(processor, new WeightedMovingVarianceBGS());
		break;
		///
	case(DPAdaptiveMedian):
		setProcessor(processor, new DPAdaptiveMedianBGS());
		break;
	case(DPGrimsonGMM):
		setProcessor(processor, new DPGrimsonGMMBGS());
		break;
	case(DPZivkovicAGMM):
		setProcessor(processor, new DPZivkovicAGMMBGS());
		break;
	case(DPMean):
		setProcessor(processor, new DPMeanBGS());
		break;
	case(DPWrenGA):
		setProcessor(processor, new DPWrenGABGS());
		break;
	case(DPPratiMediod):
		setProcessor(processor, new DPPratiMediodBGS());
		break;
	case(DPEigenbackground):
		setProcessor(processor, new DPEigenbackgroundBGS());
		break;
		///
	case(TBT2FGMM_UM):
		setProcessor(processor, new T2FGMM_UM());
		break;
	case(TBT2FGMM_UV):
		setProcessor(processor, new T2FGMM_UV());
		break;
	case(TBFuzzySugenoIntegral):
		setProcessor(processor, new FuzzySugenoIntegral());
		break;
	case(TBFuzzyChoquetIntegral):
		setProcessor(processor, new FuzzyChoquetIntegral());
		break;
		///
	case(MultiLayer):
		setProcessor(processor, new MultiLayerBGS());
		break;
		///
	case(SimpleGaussian):
		setProcessor(processor, new LBSimpleGaussian());
		break;
	case(FuzzyGaussian):
		setProcessor(processor, new LBFuzzyGaussian());
		break;
	case(MixtureOfGaussians):
		setProcessor(processor, new LBMixtureOfGaussians());
		break;
	case(AdaptiveSOM):
		setProcessor(processor, new LBAdaptiveSOM());
		break;
	case(FuzzyAdaptiveSOM):
		setProcessor(processor, new LBFuzzyAdaptiveSOM());
		break;
	}
}

void ofxBGS::setProcessor(Processor type, IBGS* p) {
	curProcessorIt = std::find(availableProcessors.begin(), availableProcessors.end(), type);
	processors[type] = p;
	curProcessor = p;
}

void ofxBGS::nextProcessor()
{
	curProcessorIt++;
	if(curProcessorIt == availableProcessors.end())
		curProcessorIt = availableProcessors.begin();
	setProcessor(*curProcessorIt);
}

void ofxBGS::previousProcessor()
{
	if(curProcessorIt == availableProcessors.begin())
		curProcessorIt = availableProcessors.end();
	curProcessorIt--;
	setProcessor(*curProcessorIt);
}

