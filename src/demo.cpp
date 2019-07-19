/**********************************************************************************
 * Copyright (c) 2019 Michael.Chen. All rights reserved.
 * File name: demo.cpp
 * Created on 19th/Jul/2019
 * Author: Michael.Chen
 * Github: https://github.com/wpddmcmc/dnnDetector
 * Follow me: https://www.tgeek.tech
 * *******************************************************************************/
#include "detector.hpp"

/************************************************* 
    Function:       main
    Description:    function entrance
    Input:          None 
    Output:         None 
    Return:         return 0
    *************************************************/
int main(int argc, char **argv)
{
	// set up net
	Detector detector;
	cv::dnn::Net net = detector.net;
	// read video file or camera

	cv::VideoCapture capture;
	capture.open("../video/test.mp4");
	if(capture.isOpened())
		std::cout<<"INFO: Video file load sucessfully"<<std::endl;
	else
		std::cout<<"ERROR: Cannot find video file"<<std::endl;

	cv::Mat frame, output;
	while (capture.read(frame))
	{
		detector.thePredictor(frame, net);			// do detection
		frame.copyTo(output);
		// draw output
		detector.drawResult(output,detector.out_names,detector.out_boxes,detector.out_confidences,detector.out_centers,true);
		// display
		cv::imshow("demo",output);
		// read keyboard
		char c = cv::waitKey(5);
		if (c == 27)
		{ 
			// ESC to exit
			break;
		}
	}
	capture.release();
	return 0;
}
