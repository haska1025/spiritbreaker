#介绍

主要支持ICE协议，webrtc终端。实现基于webrtc协议栈，网络层来完成。

##架构

本系统包含信令子系统，媒体子系统两部分。

信令子系统java实现。

媒体子系统c++实现，协议栈基于webrtc。

信令子系统和媒体子系统通过消息队列来实现。

### 信令子系统

### 媒体子系统

1. Publisher

发布者共享自己本地音视频。拥有独立的PeerConnection，只是发送数据不接收数据。

调用CreateOffer, setLocalSession, 通过信令通道发送sdp给信令服务器。

2. Subscriber

订阅远端的音视频。拥有独立的PeerConnection，只是接收数据，不发送数据。

3. Peer
  
   一个终端对应一个Peer对象。一个Peer对象有一个Publisher？但是可以有多个Subscriber？分别对应多个Peer的不同Publisher？

Publisher将自己的sdp发送到spiritbreaker
  
  a) spiritbreaker需要createAnswer，并且回馈answer

  b) gather local candidate，发送给offerer.


####流程

1. setRemoteSession

2. gather native candidate

3. OnCandidate

4. answer

