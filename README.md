# spiritbreaker

此项目是从webrtc源码中移植了rtc_base, api ,p2p相关模块。目的是用于后续支持ice协议的流媒体服务器开发。

依赖的第三方库：boringssl, abseil-cpp, jsoncpp, libsrtp，ffmpeg,libjpeg,libyuv，opus，openh264，libvpx，rnnoise，usrsctplib

由于webrtc rtc_event_log 用到了protobuf，所以需要安装protobuf3.0以上版本

项目最终编译生成一个 libsbs.so

## build

### 第三方库的依赖build

由于webrtc原有的build是采用gn生成nijia项目的方式，不太方便，所以整改成cmake编译的方式。

首先将spiritbreaker clone到本地。

#### boringssl 安装

github路径：https://github.com/google/boringssl.git

boringssl 依赖go，安装参考：https://tecadmin.net/install-go-on-ubuntu/

将boringssl克隆到spiritbreadk/src/thirdparty目录

注意borringssl需要编译生成一个共享库。

```c
cd spiritbreadk/src/thirdparty
git clone https://github.com/google/boringssl.git
cd boringssl
mkdir build
cd build
cmake -DCMAKE_CXX_FLAGS="-lrt" -DBUILD_SHARED_LIBS=1 ..
```
CMAKE_CXX_FLAGS="-lrt" 增加一个rt库的依赖，主要是链接clock_gettime函数。


#### Abseil安装

将Abseil克隆到spiritbreadk/src/thirdparty目录

参考：https://abseil.io/docs/cpp/quickstart.html#running-the-abseil-hello-world

需要编译共享库，打开顶级 CMakeLists.txt 增加如下内容：

set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)

```c
cd abseil-cpp
mkdir build
cd build
cmake ..
```

#### jsoncpp安装

直接通过deb软件源安装。

```c
sudo apt-get install libjsoncpp-dev
```

#### libsrtp

github 路径：https://github.com/cisco/libsrtp

这里用的是cisco开源的一个库，clone到本地，然后：

 ./configure --enable-openssl CPPFLAGS="-DGCM"
 
 make
 
 需要注意的是：webrtc用的时候，需要用到AES-GCM模式，因此需要加上宏GCM
 
 #### ffmpeg
 
 可能用到一些音频，视频的 编解码库？
 
 github路径：https://github.com/FFmpeg/FFmpeg.git
 
 #### libjpeg
 
 视频图像编解码依赖的库
 
 github路径：https://github.com/LuaDist/libjpeg.git
 
 ```c
 ./configure
 make
 ```

 #### libyuv
 
 视频图像编解码依赖的库
 
 github路径：https://github.com/bilibili/libyuv

```c
cd spiritbreadk/src/thirdparty
git clone https://github.com/google/boringssl.git
cd libyuv
mkdir build
cd build
cmake ..
```

 #### opus
 
现代音频编解码库
 
 github路径：https://git.xiph.org/opus.git

 #### openh264
 
 视频图像编解码依赖的库
 
 github路径： https://github.com/cisco/openh264.git
 
 命令行执行
 
 make

因为不需要configure配置

 #### libvpx
 
 视频图像编解码依赖的库，主要是google vp8/vp9
 
 github路径： https://github.com/webmproject/libvpx.git
 
 ```c
 cd libvpx
 ./configure
 make
 ```
 
 注意，貌似用 github 版本，不能使用。目前用webrtc自带的。
 
 #### rnnoise
 
 基于神经网络算法的噪音处理？
 
 这个还是用webrtc自带的吧，github实现貌似是c实现，webrtc自带是c++实现。
 
 这个我们自己生成了一个CMake工程。
 
 ```c
 cd rnnoise
 mkdir build
 cd build
 cmake ..
 make
 ```
 
 #### usrsctplib
 
  ```c
 cd usrsctplib
 mkdir build
 cd build
 cmake ..
 make
 ```
 
 ### spiritbreaker build
 
spiritbreaker会一同编译webrtc，webrtc在编译audio device module的时候，需要依赖到系统声卡驱动相关api，所以做如下安装：
 
sudo apt-get install libasound2-dev
  
sudo apt-get install libpulse-dev 
 
首先clone code到本地
 
然后执行如下命令：
```c
 cd spiritbreaker
 mkdir build
 cd build
 cmake ..
 make
```
 
