# dnnDetector
![](https://img.shields.io/badge/TGeek-Projects-blue.svg)

![](https://img.shields.io/badge/linux%20build-pass-green.svg)

Object detection based on Opencv.dnn, easy to use

[中文](README_ch.md)

------

Author: Michael.Chen

Website: www.tgeek.tech

Contact: m.c.chen@outlook.com

---

Project build and test o OpenCV4.1, Any OpenCV>=3.3 will be ok

## Requirements

OpenCV

OpenCV_contrib

## Files structure

**src/:** source file, the example programe - demo.cpp	

**include/:** the head file - detector.hpp

**lib/:** libary file - libdnnDetector.so

**param/:** parameter file - param.xml

**dnn_nets/:** Dnn networks configration, weight and labels files

**video/:** videos for test

## How to use

### Build

#### Create a folder to build project

```bash
mkdir build
```

#### Compile

If you have more than one version of OpenCV. Set  path to which you want to use by uncommiting and modifying #line9 of ```CMakeLists.txt```

For example, your OpenCV path if "/home/user/opencv_4.1". Then modify ```CMakeLists.txt``` as

```cmake
set(OpenCV_DIR "/home/user/opencv_4.1/build/")
```

 Then run ```cmake``

```
cd build/
cmake ..
```

Run ```make ``` to compile

```bash
make
```

## Run 

```bash
./dnnDetector
```

## Change paramters

Any change of paramters **do not need to rebuild** the project

Modify ```param/param.xml``` to change paramters

### Prediction configrations

line5 - line7

```xml
<net_type>0</net_type>              <!-->0-ssd 1-yolo<-->
<thresh>0.35</thresh>               <!-->confidence threshold<-->
<nms_thresh>0.25</nms_thresh>       <!-->nms threshold<-->
```

### for Yolo network

line10 - line14, you can change to any Yolo network, any Yolo model you want to use

```xml
<Yolo_meanVal>1</Yolo_meanVal> 
<Yolo_scaleFactor>0.003921569</Yolo_scaleFactor>
<Yolo_config>../dnn_nets/yolo/yolov3-tiny.cfg</Yolo_config>
<Yolo_model>../dnn_nets/yolo/yolov3-tiny.weights</Yolo_model>
<coco_name>../dnn_nets/yolo/coco.names</coco_name>
```

### for SSD network

line17 - line22, you can change to any SSD network, any SSD model you want to use

```xml
<ssd_meanVal>127.5</ssd_meanVal> 
<ssd_scaleFactor>0.007843</ssd_scaleFactor>
<ssd_config>../dnn_nets/ssd/deploy.prototxt</ssd_config>
<ssd_model>../dnn_nets/ssd/mobilenet_iter_73000.caffemodel</ssd_model>
<ssd_name>../dnn_nets/ssd/ssd.names</ssd_name>
</opencv_storage>
```

## Demo

对demo.cpp进行详解

### head file

add head file

```c++
#include "detector.hpp"
```

all the head files used was writen in ```include/detector.hpp```

```c++
#include <opencv2/dnn.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
```

### Network loading

 Instantiate```Detector``` class, and read network

```c++
	Detector detector;
	cv::dnn::Net net = detector.net;
```

network initialization was writen in ```include/detector.hpp``` 

### Video or camera reading

```c++
cv::VideoCapture capture;
capture.open("../video/test.mp4");
if(capture.isOpened())
	std::cout<<"INFO: Video file load sucessfully"<<std::endl;
else
	std::cout<<"ERROR: Cannot find video file"<<std::endl;
cv::Mat frame, output;
while (capture.read(frame))
{
  // Detection
}
```

### Detection

Write in ```while``` 

#### Get results

Use ```Detector.thePredictor(cv::Mat frame, cv::dnn::Net net)```

```c++
detector.thePredictor(frame, net);			// do detection
```

#### Draw results and display

```c++
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
```

end of loop

#### Release video

```c++
capture.release();
return 0;
```

## class Detector  public functions and variables

### Constructor Detector()

#### Read configuration file

```c++
// Read configration file
cv::FileStorage setting_fs("../param/param.xml", cv::FileStorage::READ);       
```

#### Load prediction parameter

Network type，network confidence threshold，non maximum suppression threshold

```c++
setting_fs["net_type"]>>net_type;                  
setting_fs["thresh"]>>thresh;
setting_fs["nms_thresh"]>>nms_thresh;
```

#### If use YOLO, load YOLO configurations

```c++
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
```

#### If use SSD, load SSD configurations

```c++
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
```

#### Read network structure

```c++
// Set network
net = cv::dnn::readNet(net_structure, model);
if (net.empty()){
std::cerr << "ERROR: Can't load network by using the following files: " << std::endl;
exit(-1);
}
else    std::cout<<"INFO: Load network sucessfully"<<std::endl;
```

#### Read lables file

```c++
// Read lable file
std::ifstream ifs(name_file.c_str());
std::string line;
while (std::getline(ifs, line)) classes.push_back(line);
```

### Public variables

```c++
std::vectorcv::Rect out_boxes		// Bounding box

std::vectorcv::Point out_centers	// Bounding box center positions

std::vectorstd::string out_names	// name of objects was detected

std::vector<float> out_confidences	// confidence of objects was detected

```

### Public functions

```c++
// do prediction
// input		-image to be predicted
// 				-Network
void thePredictor(cv::Mat frame, cv::dnn::Net net);  
// Draw result  
// Inputs		-input/output image
//				-vector of object names
//				-vector of BBOX
//				-vector of confidences
//				-vector of objects centers
//				-wether draw fps
void drawResult(cv::Mat& frame, std::vector<std::string> out_names, std::vector<cv::Rect> out_boxes,std::vector<float> confidences,std::vector<cv::Point> out_centers,bool if_fps); 
```

