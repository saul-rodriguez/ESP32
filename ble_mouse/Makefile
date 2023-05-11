# Makefile template for ESP32 projects
# Author: Saul Rodriguez
# Date:	2023-04-12
#
#  The following variables need to be set for each project:
#   NAME = project name i.e. gio
# 	PORT = USB port name for flashing binary i.e. /dev/ttyUSB0 
#	

PORT = /dev/ttyUSB0
NAME = ble_mouse

#compile elf file from source
create: 
	idf.py create-project $(NAME)
	
config:
	idf.py set-target esp32
	idf.py menuconfig
	
bin:
	idf.py build
	
clean: 
	idf.py clean

flash:
	idf.py -p $(PORT) flash
	
monitor:
	idf.py -p $(PORT) monitor


