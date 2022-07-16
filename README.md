<p align="center">
  <img src="https://github.com/agvitaglione/ptmonitor/blob/main/img/logo.png" width="500">
</p>
PtMonitor is a GUI-based monitoring applcation running on GNU/Linux system for the collection, elaboration and visualization on screen of vehicle tyre temperature and pressure values. The values are sent though a CAN bus connected to the system whith a USB2CAN module. 
The GUI is developed using GTK3.0 and [slope](https://github.com/bytebrew/slope) toolkit. 

## Installation
Install the required development packages: 
'''bash
sudo apt-get install build-essential cmake libgtk-3-dev
'''

Install the slope toolkit
cd ~
git clone https://github.com/bytebrew/slope
cd slope
mkdir build
cd build
cmake ..
make
sudo make install
sudo mv /usr/local/lib/libslope.so /lib

Install CAN utilities and set USB2CAN module
sudo apt-get install can-utils
sudo ip link set can0 type can bitrate 250000
sudo ifconfig can0 up

Install xinit in environment without GUI




