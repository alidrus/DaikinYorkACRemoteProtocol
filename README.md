# Daikin/York Air Conditioner Remote Control Protocol

This Arduino IDE library generates raw IR timings for some Daikin and York
Air Conditioners. These timings can be used to send IR signals to the Air
Conditioner using [IRremote](https://github.com/z3t0/Arduino-IRremote)'s
sendraw() method.


**********************
WARNING AND DISCLAIMER
**********************

This library is a WORK IN PROGRESS. A lot of it has NOT BEEN TESTED. Sending
malformed or illegal codes to your AC could potentially damage it permanently.
I will not be held liable for anything bad happening to your AC as a result of
using this software. Use at your own risk!

**********************


## Supported AC Models

This library is made for any Daikin or York AC model which uses the following remotes:
- Daikin DGS01
- York ECGS01-i

The remote protocol may be compatible with some Acson AC models as well (unverified).


## Supported Boards / Controllers

Any microcontroller / board supported by the
[IRremote](https://github.com/z3t0/Arduino-IRremote) library.


## Install

Download the latest ZIP archive of the library by visiting the
[Releases](https://github.com/alidrus/DaikinYorkACRemoteProtocol/releases) page
and downloading the ZIP file of the version you wish to install.

Then, from the Arduino IDE menus, click on **Sketch > Include Library > Add ZIP
Library …** and navigate to the ZIP file you just downloaded and then click on
the **Choose** button.


## Uninstall

Navigate to your Arduino IDE's **libraries** folder and remove the folder with
the same name as the ZIP file you installed.


## Usage

In order to use the C++ classes provided by this library, the following header
file needs to be included:
```
   #include <DaikinYorkACRemoteProtocol.h>
```

### Classes and Methods

TO BE ADDED

### Examples

TO BE ADDED
