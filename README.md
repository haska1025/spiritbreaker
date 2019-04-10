# spiritbreaker

此项目是从webrtc源码中移植了rtc_base, api ,p2p相关模块。目的是用于后续支持ice协议的流媒体服务器开发。

依赖的第三方库：boringssl, abseil-cpp, jsoncpp, libsrtp

## build

由于webrtc原有的build是采用gn生成nijia项目的方式，不太方便，所以整改成cmake编译的方式。

首先将spiritbreaker clone到本地。

### boringssl 安装

github路径：https://github.com/google/boringssl.git

boringssl 依赖go，安装参考：https://tecadmin.net/install-go-on-ubuntu/

将boringssl克隆到spiritbreadk/src/thirdparty目录

```c
cd spiritbreadk/src/thirdparty
git clone https://github.com/google/boringssl.git
cd boringssl
mkdir build
cmake ..
```

### Abseil安装

将Abseil克隆到spiritbreadk/src/thirdparty目录

参考：https://abseil.io/docs/cpp/quickstart.html#running-the-abseil-hello-world

### jsoncpp安装

直接通过deb软件源安装。

```c
sudo apt-get install libjsoncpp-dev
```

### libsrtp

github 路径：https://github.com/cisco/libsrtp

这里用的是cisco开源的一个库，clone到本地，然后：

 ./configure --enable-openssl CPPFLAGS="-DGCM"
 
 make
 
 需要注意的是：webrtc用的时候，需要用到AES-GCM模式，因此需要加上宏GCM
 


