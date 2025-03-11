# abcc-example-starterkit
This is a stand-alone example application using the Anybus CompactCom Driver API ([abcc-api](https://github.com/hms-networks/abcc-driver-api)) ported for the [Anybus CompactCom Starter Kit](https://anybus.com/starterkit40) evaluation platform.
### Purpose
To enable easy evaluation and inspiration to [Anybus CompactCom](https://www.hms-networks.com/embedded-network-interfaces) prospects.

## Prerequisites
### System
- This example application shall be built for and ran in a Windows environment.
### Anybus Transport Provider
- The free Anybus Transport Provider DLL is required. [Download](https://hmsnetworks.blob.core.windows.net/nlw/docs/default-source/products/anybus/monitored/software/hms-anybus-transport-provider-1.zip?sfvrsn=e636aad6_44) and install this DLL.
### CMake
- If you do not yet have CMake, [download](https://cmake.org/download/) and install it before continuing.

## Cloning
### Flag? What flag?
This repository contain submodules [abcc-driver-api](https://github.com/hms-networks/abcc-api), ([abcc-driver](https://github.com/hms-networks/abcc-driver) and [abcc-abp](https://github.com/hms-networks/abcc-abp) that must be initialized. Therefore, pass the flag `--recurse-submodules` when cloning.

*It's suggested to clone the repository into your projects `lib/` folder.*
```
git clone --recurse-submodules https://github.com/hms-networks/abcc-example-starterkit.git
```
#### (In case you missed it...)
If you did not pass the flag `--recurse-submodules` when cloning, the following command can be run:
```
git submodule update --init --recursive
```

## Build and run
### CMake
This example application utilizes the Anybus CompactCom Driver API's CMake module file in a top level CMakelLists.txt file to generate a complete Visual Studio project. To generate the project, run the lines below, starting in the repository root.
```
mkdir build
```
```
cd build
```
```
cmake ..
```

### Visual Studio
Open the generated project solution *.sln* file and run the Local Windows Debugger (green play button). The Project should build and run!
#### (Nothing is happening...)
Make sure that your Starter Kit is powered on and the USB cable is plugged in. See the [Starter Kit Reference Guide](https://hmsnetworks.blob.core.windows.net/nlw/docs/default-source/products/anybus/manuals-and-guides---manuals/hms-hmsi-27-224.pdf?sfvrsn=8dfb9d6_20) for more details.

