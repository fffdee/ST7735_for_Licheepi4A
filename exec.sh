#!/bin/bash
export LD_LIBRARY_PATH=/home/sipeed/TFT_demo/lib:$LD_LIBRARY_PATH
export PATH=/home/sipeed/TFT_demo/bin:$PATH
sudo chmod o+rw /dev/gpiochip4
sudo chmod o+rw /dev/spidev2.0
