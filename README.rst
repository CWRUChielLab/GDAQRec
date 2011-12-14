GDAQrec
=======

GDAQrec is a quick a dirty program for recording analog signals from a DAQ that
is supported by the COMEDI api for Linux.  We have older versions running on 
OS/X using the NIDAQmx base library for National Instruments boards, but I have
not tried building this target recently.

This code was originally thrown together as a quick way for recording
electrophysiology data for an experiment I was running, and builds on example
graphing code code from the book "C++ GUI Programming with Qt 4" by Jasmin
Blanchette and Mark Summerfield.  While the code is messy and was not intended
to last the year, it has lasted several years and is now being used across
multiple laboratories.  I still hope to replace it, and when I do I will add a
note to this readme.  

Build instructions 
------------------

1) Install Qt 4.  
2) uncompress the files and run "qmake DAQLIB=comedi"
3) run "make"
4) run "./GDAQrec" to run the program
