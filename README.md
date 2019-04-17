ai捕获分析的软件

# ARDebug
ARDebug is a tool for monitoring and debugging swarm robotic and multi-robot experiments in real time using augmented reality techniques. The software provides a GUI for displaying internal data reported wirelessly by each robot within the swarm. This data is used to augment a real-time video feed of the robots and their environment, using visual representations of the data. The software aims to reduce the time taken to develop and debug swarm robotics experiments and behaviours by giving the developer more immediate access to decision making varaibles, sensor readings, and other key data, when compared to standard console or log-based debugging techniques. The software relies on a tracking system to locate the robots within the image in order to correctly overlay the augmented elements, and a modular software architecture is used to allow for easy integration with a variety of tracking systems. The tracking system used in this reference implementation makes use of [ArUco](https://www.uco.es/investiga/grupos/ava/node/26) fiducial markers and the associated [OpenCV](https://opencv.org/) image processing library.

## Installing dependencies
Currently, ARDebug must be compiled from source and may require minor modifications to work with your tracking set up. The software is known to work under Ubuntu 16.04 or later, and macOS 10.13.5.

### Ubuntu 18.04 LTS - Recommended
Installation under Ubuntu 18.04 is easy thanks to up-to-date versions of library dependencies available in the default repositories. To install all required libraries and tools simply run:

```
sudo apt-get install libopencv-dev libopencv-contrib-dev qt5-default qtconnectivity5-dev \
libqt5charts5-dev libgl1-mesa-dev git python
```

Once this install process has finished you should be able to compile the application using the steps below.

### Ubuntu 16.04 LTS, 17.04, and 17.10
Installation on older versions of Ubuntu is slightly more complex. ARDebug depends on further software packages, including OpenCV 3.X.X and `opencv_contrib` to detect ArUco tags.

The following commands can be used to download and build the appropriate source files.

From Ubuntu 17.04, the following PPA must be added in order to install `libjasper-dev`:

`sudo add-apt-repository "deb http://security.ubuntu.com/ubuntu xenial-security main"`

```
sudo apt-get update
sudo apt-get upgrade

sudo apt-get install build-essential cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev \
libswscale-dev python-dev python-numpy libtbb2 libtbb-dev libjpeg-dev libpng-dev libtiff-dev \
libjasper-dev libdc1394-22-dev libgl1-mesa-dev

git clone https://github.com/Itseez/opencv.git
git clone https://github.com/Itseez/opencv_contrib.git

cd opencv_contrib
git checkout 3.4.1

cd ../opencv
git checkout 3.4.1

mkdir build
cd build

cmake \
-D CMAKE_BUILD_TYPE=Release \
-D CMAKE_INSTALL_PREFIX=/usr/local \
-D OPENCV_EXTRA_MODULES_PATH=../../opencv_contrib/modules \
-D BUILD_opencv_bgsegm=OFF \
-D BUILD_opencv_bioinspired=OFF \
-D BUILD_opencv_ccalib=OFF \
-D BUILD_opencv_cnn_3dobj=OFF \
-D BUILD_opencv_cvv=OFF \
-D BUILD_opencv_datasets=OFF \
-D BUILD_opencv_dnn_objdetect=OFF \
-D BUILD_opencv_dnns_easily_fooled=OFF \
-D BUILD_opencv_dpm=OFF \
-D BUILD_opencv_face=OFF \
-D BUILD_opencv_fuzzy=OFF \
-D BUILD_opencv_freetype=OFF \
-D BUILD_opencv_hdf=OFF \
-D BUILD_opencv_line_descriptor=OFF \
-D BUILD_opencv_matlab=OFF \
-D BUILD_opencv_optflow=OFF \
-D BUILD_opencv_ovis=OFF \
-D BUILD_opencv_plot=OFF \
-D BUILD_opencv_reg=OFF \
-D BUILD_opencv_rgbd=OFF \
-D BUILD_opencv_saliency=OFF \
-D BUILD_opencv_sfm=OFF \
-D BUILD_opencv_stereo=OFF \
-D BUILD_opencv_structured_light=OFF \
-D BUILD_opencv_surface_matching=OFF \
-D BUILD_opencv_text=OFF \
-D BUILD_opencv_tracking=OFF \
-D BUILD_opencv_xfeatures2d=OFF \
-D BUILD_opencv_ximgproc=OFF \
-D BUILD_opencv_xobjdetect=OFF \
-D BUILD_opencv_xphoto=OFF \
..

make

sudo make install

sudo ldconfig
```

At this point all of the required OpenCV libraries should be installed.

To build the main application you must download a modern version of Qt (5.10.1 is recommended). The installer can be found [here](https://www.qt.io/download). Note that you will need to modify the installer's file permissions before you can run it:

```
chmod +x qt-unified-linux-x64-3.0.4-online.run
./qt-unified-linux-x64-3.0.4-online.run
```

Make a note of where Qt is installed, as you will need to specify the path to the new `qmake` binary when building ARDebug.

### macOS 10.3.5

Install [Homebrew](https://brew.sh/), then the following packages:

```
brew install opencv qt pkg-config
brew link --force qt
```

### Building ARDebug
Clone this repository, or download it as a zip file and extract it into a directory. Within this directory run the following commands:

```
mkdir build
cd build
qmake ..
```
Note that if you have a local installation of Qt that is newer than your system-wide installation (usually Qt 4.x), you will need to specify the path to `qmake` e.g.: `{QT_INSTALL_PATH}/{VERSION}/gcc_64/bin/qmake`.

You should now have a Makefile in the build directory. You can now build the application by running:

```
make
```
### Running ARDebug

The application can then be launched in Ubuntu by running:

```
./ardebug
```

or in macOS by running:

```
ardebug.app/Contents/MacOS/ardebug
```

## Features
ARDebug enables the visual display of internal robot data, which can be sent to the application in [JSON](https://www.json.org/) format. The software takes this data and displays it to the user in a number of forms. This includes the augmented video view of the robots, which can be overlaid data transmitted by each robot, as well a robot's ID, position, and orientation. For wireless communication ARDebug supports both Wi-Fi and Bluetooth, as well as a combination of both, which can be useful for heterogeneous swarm experiments.

## Interfacing with ARDebug
ARDebug receives data from the robots as a JSON object, which can either be sent via Bluetooth, or as UDP packets to the network port specified in the GUI. The only requirement is that each submitted JSON object must have an 'id' value that identifies which robot the data pertains to.

Code for communicating packets in this format via Wi-Fi from a Linux-based robot is provided in <i>ardebug/RobotAPI</i>, primarily for illustrative purposes. An example of how this can be integrated into an example ARGoS robot controller is provided in <i>ardebug/ExampleRobotController/e-puck</i>. An example controller for Psi Swarm robots using Bluetooth communication is also provided in <i>ardebug/ExampleRobotController/psi-swarm</i>. Note that only the main file is provided here - example projects for the Psi Swarm robot and the associated libraries can be found at the [mbed team page](http://os.mbed.com/teams/Psi-Swarm-Robot).

## Tracking System
Tracking information regarding the position and orientation of each robot is passed through the same interface as the other data, meaning that the code can be extended to support a different tracking system from the one based on ArUco that is included, or even receive tracking data via the network from a seperate machine.

When using the ArUco interface, a configuration file is used to map the numeric IDs of ArUco tags to the textual IDs of the robots used within the application. This configuration file is a JSON file containing an array of objects with `aruco_id` and `robot_id` attributes. If no configuration file is found on application startup then a default one named `RobotConfig.json` is created, which can be used as a basis for your own configuration.

Robot positions are described using a simple 'proportional' coordinate system, where both the X and Y coordinate of the robot are stored as a value between 0 and 1, describing the robots position on that axis as a proportion of the length of the camera viewport in that direction. Orientation is simply an angle in degrees, measured clockwise from zero pointing straight up along the Y axis.

## UI Layout
When the application is first launched the user is presented with a video feed which will be drawn from the first USB camera feed as found by OpenCV.

At the bottom of the application is a tabbed region which contains both a "Console" tab, where useful information will be reported, and a "Data Visualisation" tab, where robot data can be viewed and where you can select which values are displayed in the visualiser.

To the right of the application is a second tabbed section, where the list of known robots is shown by default. There are extra tabs to open a network port to receive JSON data, and to configure which robots are connected via Bluetooth. Below this second tabbed area is a chart display region, in which graphs can be drawn relating to various types of data received by the application.

## Charts
The type of chart displayed depends upon the type of value selected - pie charts for string values (useful for displaying state distributions across the swarm), line graphs for numerical values (useful for tracking values over time), and bar charts for arrays of numeric values (useful for visualising IR sensor readings, for example).

To display one of these charts simply select a robot from the "Robots" tab. The "Data Visualisation" tab will now display the data known about the selected robot. A chart can be drawn by double-clicking on any value in the table. If the selected value is in a format which can currently be graphed by the application then the appropriate graph will appear in the chart display region.

## ArUco
By default the application uses the built in `DICT_6X6_50` tag dictionary. To generate the appropriate tags refer to [this page](https://docs.opencv.org/3.2.0/d5/dae/tutorial_aruco_detection.html). If you would like to use a different set of tags then the dictionary in use should be specified in `Application/Tracking/aruco.cpp`.

A sample document containing ArUco tags is provided in this repository.

## Test script
A python script is provided in this repository both to test the application and to demonstrate how data could be submitted. The script defines a small set of virtual robots and reports various pieces of data to the application. By default, an initial set of poses are generated and do not change throughout the lifetime of the script. This shows how pose data can be submitted through the network interface if a different tracking system were to be used.

To use the test script run `testDataSource.py` using a python interpreter, then in ARDebug click on the "Network" tab and "Start Listening" on port 8888.

The script can also be used to test simulated robot data in conjunction with ArUco tags. To do so, run the script with the optional `aruco` argument as follows: `testDataSource.py aruco`. In this mode, the test script will no longer output pose data - instead pose data is obtained from ArUco tags detected by the camera. Simply print page one of ArUcoMarkers/allMarker.pdf (tag IDs 0-7) and place the sheet of paper in front of your camera. The simulated robot data from the test script will match up with these tags, and can be visualised in ARDebug.

## Adding camera sources
It is possible to add new camera sources to the application by writing a C++ interface to it. For a simple example of how this works see `Application/Tracking/usbcamerathread.{h,cpp}`.

The USB camera thread extends the ARCameraThread class, which acts as a common base class for all possible camera sources. As most consumers of camera frames are likely to be doing complex computation they may not be able to process frames as quickly as they are available. To prevent frames from building up in the Qt event queue camera threads should disconnect all consumers after a frame is emitted. Consumers are then responsible for reregistering when they have finished processing and are ready for the next frame. To ensure that consumers are reconnected in such a way that they are not immediately disconnected without receiving an emitted signal (possible if the connect is issued from a consumer thread after the camera thread emits a frame but before the disconnect call) the ARCameraThread class maintains a list of "pre-emit calls". This is a vector of function calls that will be executed before each frame is emitted and can be used to reregister a consumer. A typical consumer slot may therefore look like:

```
void processFrame(cv::Mat& image)
{
	// Process the frame here
	// This is likely to take longer than the time between frames

	cameraThread->addPreEmitCall([&](){ connect(cameraThread, SIGNAL(newFrame(cv::Mat&)), this, SLOT(processFrame(cv::Mat&))); });
}
```

This makes use of C++11 lambda functions to provide the reconnect call to the camera thread. The camera thread's main loop may then contain a loop similar to:

```
while(shouldRun)							// shouldRun will be cleared by the ARCameraThread class when quit() is called on the thread
{
	/* Capture image */
	executePreEmitCalls();						// Loop through and execute all submitted pre-emit calls, then clear them from the list
	emit newVideoFrame(image);					// Emit the new frame
	disconnect(this, SIGNAL(newVideoFrame(cv::Mat&)), nullptr, 0);	// Disconnect all connected slots
}
```

The `shouldRun` variable and the `executePreEmitCalls()` function are provided by the ARCameraThread base class.

The newly implemented class can then be instantiated in Application/Core/mainwindow.cpp by replacing the line:

```
cameraThread = new USBCameraThread;
```

## Adding visualisations

To add new visualisations to the software, simply subclass `VisElement` (see `VisPosition` and `VisText` for examples), and draw geometric primitives using Qt's [QPainter](http://doc.qt.io/qt-5/qpainter.html) class. Then, instantiate your subclass in the constructor of the `Visualiser` class (Application/Visualiser/visualiser.cpp), and push the object into the `config.elements` vector so they are rendered by ARDebug.
