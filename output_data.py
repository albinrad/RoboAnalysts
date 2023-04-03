# -*- coding:utf-8 -*-
'''!
  @file  output_data.py
  @brief  A use example for the DAC, execute it to output different values from different channels.
  @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
  @license  The MIT License (MIT)
  @author  [tangjie](jie.tang@dfrobot.com)
  @version  V1.0
  @date  2022-03-07
  @url  https://github.com/DFRobot/DFRobot_GP8403
'''
from __future__ import print_function
import sys
import os
import time

sys.path.append(os.path.dirname(os.path.dirname(os.path.realpath(__file__))))
from DFRobot_GP8403 import *

DAC = DFRobot_GP8403(0x58)  
status = DAC.begin()
if(status != 0):
  while True:time.sleep(1)
  
print("init succeed")
#Set output range  
DAC.set_DAC_outrange(OUTPUT_RANGE_10V)
while True:
  #Output value from DAC channel 0
  DAC.set_DAC_out_voltage(3000,1)
  #DAC.set_DAC_out_voltage(3000,1)
  for voltage in range(10000, 3000, -2000):
    DAC.set_DAC_out_voltage(voltage, 0)
    time.sleep(1)

  
