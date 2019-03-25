# spiritbreaker

此项目是从webrtc源码中移植了rtc_base, api ,p2p相关模块。目的是用于后续支持ice协议的流媒体服务器开发。

依赖的第三方库：boringssl, absl

## build

由于webrtc原有的build是采用gn生成nijia项目的方式，不太方便，所以整改成cmake编译的方式。



