# abcc-example-starterkit
This is a stand-alone example application using the Anybus CompactCom Driver API ([abcc-driver-api](https://github.com/hms-networks/abcc-driver-api)) ported for the [Anybus CompactCom Starter Kit](https://anybus.com/starterkit40) evaluation platform.
### Purpose
To enable easy evaluation and inspiration to [Anybus CompactCom](https://www.hms-networks.com/embedded-network-interfaces) prospects.

## Prerequisites
### System
- This example application shall be built for and run in a Windows environment.
### Anybus Transport Provider
- The free Anybus Transport Provider DLL is required. [Download](https://hmsnetworks.blob.core.windows.net/nlw/docs/default-source/products/anybus/monitored/software/hms-anybus-transport-provider-1.zip?sfvrsn=e636aad6_44) and install this DLL.
### CMake
- If you do not yet have CMake, [download](https://cmake.org/download/) and install it before continuing.
### Visual Studio
- This example targets Visual Studio with the "Desktop development with C++" workload. Install [Visual Studio](https://visualstudio.microsoft.com/downloads/) with that workload enabled before continuing.

  *CMake will automatically detect the Visual Studio version installed on your computer and generate the corresponding project files.*
### Git
- Of course, you will need to have Git installed. See [this tutorial](https://github.com/git-guides/install-git) on how to install Git.

## Cloning
### Flag? What flag?
This repository contains three submodules: [abcc-driver-api](https://github.com/hms-networks/abcc-api), [abcc-driver](https://github.com/hms-networks/abcc-driver) and [abcc-abp](https://github.com/hms-networks/abcc-abp). To initialize them automatically during cloning, you must use the `--recurse-submodules` flag.

:warning: Important: The "Download ZIP" link (found under the green "Code" button at the top of this page) does not include the bundled submodules. To get the complete source code with all dependencies, run:

```
git clone --recurse-submodules https://github.com/hms-networks/abcc-example-starterkit.git
```
#### (In case you missed it...)
If you did not pass the flag `--recurse-submodules` when cloning, the following command can be run:
```
git submodule update --init --recursive
```

## Build and run
### Step 1. CMake
This example generates a complete Visual Studio project by using the `abcc-driver-api.cmake` helper script, which is included by the top-level CMakeLists.txt. The script automatically configures the driver library and its dependencies.

To generate the project, run the lines below, starting in the repository root.
```
mkdir build
```
```
cd build
```
```
cmake ..
```

### Step 2. Visual Studio
Open the generated project solution *.sln* / *.slnx* file and run the Local Windows Debugger (green play button). The Project should build and run!
#### (Nothing is happening...)
Make sure that your Starter Kit is powered on and the USB cable is plugged in. See the [Starter Kit Reference Guide](https://hmsnetworks.blob.core.windows.net/nlw/docs/default-source/products/anybus/manuals-and-guides---manuals/hms-hmsi-27-224.pdf?sfvrsn=8dfb9d6_20) for more details.
