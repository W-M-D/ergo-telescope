# ergo-telescope
A Linux based program that runs a GPS to track cosmic rays on a raspberry pi. 

#APT

If you don't want to build this from source you can add this line to your /etc/apt/sources.list file

deb http://packages.ergotelescope.org/debian stable/

you also need the key or else apt will not allow you to install the file

curl http://packages.ergotelescope.org/ergo.gpg.key | sudo apt-key add -

Then all you have to do is.

sudo apt-get update && sudo apt-get install ergo-telescope


#Building
If you would just like to build a bin file for testing run
autoreconf -iv
./configure
make 

If you would like to build a .deb enter the ergo-telescope dir and run 
autoreconf -iv 

You may have to change the version from 1.4 the current version. 

Then run

dpkg-checkbuilddeps 

and install missing build deps with apt-get 

Finally run 
dpkg-buildpackage

a .deb file will be created and can be installed with 

dpkg -i 

#Configuration 
As of version 1.2
To configure the raspi to your needs you can create a GPS.conf file with ucenter (https://www.u-blox.com/en/product/u-center-windows)
and move it to /etc/ERGO/GPS.conf 
If no GPS.conf is found the progrm will only use default setting. 

the baudrate must be 38400 for the raspi to work. 

#Debugging 
If any bugs are found please contact me. If you're really bored and want to look for bugs you can open up gdb, or radare2  in a screen
session after stoping the ergo service with the command sudo service ergo-telescope stop . However I run gperf with almost every build. 

This program does have some built in debugging features that can be activated by running the program directly with a number 1-4 after it 
(eg ergo-telescope 3) 
the number increases verbosity.
ONLY ONE ergo-telescope CAN RUN AT A TIME BECAUSE IT USES THE SAME PORT. 

#Logs
you can access logging and data by going to the /etc/ERGO directory 

Logs are under ERGO_LOGS.log and ERGO_HEX.log
Data is under ERGO_DATA.csv 
A data stamp will look like this 

Date    Time      ID    lat       lon       alt       nanoseconds 

11/25/14 19:32:41 442 +092365181 -289141422 -0002102 651011889


