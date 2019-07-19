/*********************************************************************************
 * Copyright (c) 2019 Michael.Chen. All rights reserved.
 * File name: detector.hpp
 * Created on 19th/Jul/2019
 * Author: Michael.Chen
 * Github: https://github.com/wpddmcmc/dnnDetector
 * Follow me: https://www.tgeek.tech
 * ******************************************************************************/
#include <opencv2/dnn.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

/************************************************* 
    Class:       Detector
    Description:    class for prediction
    *************************************************/
class Detector
{
public:
    /************************************************* 
    Function:       Detector
    Description:    initialization of detector
    Input:          None 
    Output:         None 
    *************************************************/
    Detector()
    {
        // Read configration file
        cv::FileStorage setting_fs("../param/param.xml", cv::FileStorage::READ);           
        setting_fs["net_type"]>>net_type;                  
        setting_fs["thresh"]>>thresh;
        setting_fs["nms_thresh"]>>nms_thresh;

        // If use YoloV3
        if (net_type){
            std::cout << "INFO: Found \"net_type==1\", using **YoloV3** network" << std::endl;
            width = 416;
            height = 416;
            setting_fs["Yolo_config"] >> net_structure;
            setting_fs["Yolo_model"] >> model;
            setting_fs["coco_name"] >> name_file;
            setting_fs["Yolo_scaleFactor"]>>scaleFactor;
            setting_fs["Yolo_meanVal"]>>meanVal;
        }

        // If use SSD
        else{
            std::cout << "INFO: Found \"net_type==0\", using **SSD** network" << std::endl;
            width = 300;
            height = 300;
            setting_fs["ssd_config"] >> net_structure;
            setting_fs["ssd_model"] >> model;
            setting_fs["ssd_name"] >> name_file;
            setting_fs["ssd_scaleFactor"]>>scaleFactor;
            setting_fs["ssd_meanVal"]>>meanVal;
        }

        // Set network
        net = cv::dnn::readNet(net_structure, model);
        if (net.empty()){
            std::cerr << "ERROR: Can't load network by using the following files: " << std::endl;
            exit(-1);
        }
        else    std::cout<<"INFO: Load network sucessfully"<<std::endl;

        // Read lable file
        std::ifstream ifs(name_file.c_str());
        std::string line;
        while (std::getline(ifs, line)) classes.push_back(line);
    }

    cv::dnn::Net net;

    // output results
    std::vector<cv::Rect> out_boxes;
    std::vector<cv::Point> out_centers;
    std::vector<std::string> out_names;
    std::vector<float> out_confidences;

    // predictation
    void thePredictor(cv::Mat frame, cv::dnn::Net net);  
    // draw prediction results   
    void drawResult(cv::Mat& frame, std::vector<std::string> out_names, std::vector<cv::Rect> out_boxes,std::vector<float> confidences,std::vector<cv::Point> out_centers,bool if_fps);   

private:
    // network structure
    int net_type;                   
    std::string net_structure;
    std::string model;
    std::string name_file;

    // network configrations
    double thresh;          // threshold for confidence
    double nms_thresh;      // threshold for nms
    size_t width;
    size_t height;
    float meanVal;
    float scaleFactor;
    std::vector<std::string> classes;

    double fps;
    // get the names of output layers
    std::vector<std::string> getOutputsNames(cv::dnn::Net &net);      
    // draw prediction results (Not Used)
    void drawPred(int classId, float conf, int left, int top, int right, int bottom, cv::Mat& frame,std::vector<std::string> classes);
    // yolo detection
    void yoloProcess(cv::Mat &frame, const std::vector<cv::Mat> &outs, float confThreshold, float nmsThreshold);
    // ssd detection
    void ssdPropcess(cv::Mat &frame, std::vector<cv::Mat> outs);
};
