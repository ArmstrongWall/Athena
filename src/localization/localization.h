//
// Created by wzq on 18-7-7.
//

#ifndef ATHENA_LOCALIZATION_H
#define ATHENA_LOCALIZATION_H

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv/cv.hpp>
#include <opencv/highgui.h>
#include <thread>
#include <locale.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <chrono>

#include <boost/thread.hpp>
#include <gflags/gflags.h>
#include <config.pb.h>
#include <glog/logging.h>

#include "IOWrapper/Output3DWrapper.h"
#include "IOWrapper/ImageDisplay.h"

#include "util/settings.h"
#include "util/globalFuncs.h"
#include "util/DatasetReader.h"
#include "util/globalCalib.h"

#include "util/NumType.h"
#include "FullSystem/FullSystem.h"
#include "OptimizationBackend/MatrixAccumulators.h"
#include "FullSystem/PixelSelector2.h"

#include "IOWrapper/Pangolin/PangolinDSOViewer.h"
#include "IOWrapper/OutputWrapper/SampleOutputWrapper.h"

#include "sensor/camera/setero_camera.h"
#include "config/until.h"

namespace athena {
namespace localization {

class Localization {
public:
    Localization():run_(true){}
    ~Localization() = default;

    bool init();
    void parseArgument();
    void settingsDefault(int preset);
    std::thread localization_thread_;
    void localization_live_thread_func();
    void localization_dataset_thread_func();

public:
    std::unique_ptr<sensor::SeteroCamera> setero_camera_;
    std::string name_;
    IOWrap::PangolinDSOViewer* viewer = 0;

    //VO  Config
private:
    std::string source_file_;
    std::string calib_;
    std::string gammaCalib_ ;
    std::string vignette_;
    athena::config::Config config_;
    cv::Mat left_image_,right_image_;
    float playbackSpeed=0;	// 0 for linearize (play as fast as possible, while sequentializing tracking & mapping). otherwise, factor on timestamps.
    bool preload=false;

private:
    bool dataset_init();
    bool live_init();

    //VO
private:
    int start = 0;
    int end   = 100000;
    bool useSampleOutput=false;
    // thread run or not
    bool run_;

    std::unique_ptr<ImageFolderReader> reader;
    std::unique_ptr<ImageFolderReader> reader_right;
    std::unique_ptr<FullSystem> fullSystem;



};

}
}
#endif //ATHENA_LOCALIZATION_H
