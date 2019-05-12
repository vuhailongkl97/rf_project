main periperal using in this project :
	Arduino Nano + Uno R3
	NRF24L01 SPI
	lcd5110  GPIO
	joystick ADC
	resistor 
	
feature:
	+ 1. using only 1 channel for transfer/receiver data 

description:
	+ 1.master(user) using RF wave to control robot 
	+ 2.slave response it' power to master 	
	+ 3.master display information about power to LCD5110 

how to run ?
	+ 1. you wire correctly both master and slave
	+ 2. update code in directory rf_project/phat/phat.ino to master 
	+ 3. update code in directory rf_project/thu/thu.ino to slave 


