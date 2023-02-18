## 流媒体相关



### RTMP

RTMP 全称是 Real Time Messaging Protocol 即**实时消息传输协议**，是基于TCP协议的应用层协议，默认通信端口1935，RTMP是一种设计用来进行实时通信的网络协议，主要用来在 Flash 平台和支持 RTMP协 议的流媒体/交互服务器之间进行以视频和数据通信。**直播场景**中使用RTMP协议比较多。

<img src="https://p1-jj.byteimg.com/tos-cn-i-t2oaga2asx/gold-user-assets/2020/1/28/16feae92c410303b~tplv-t2oaga2asx-zoom-in-crop-mark:4536:0:0:0.awebp" alt="RTMP通信过程" style="zoom:80%;" />

#### 握手过程

> RTMP首先在TCP层面是有三次握手的过程的，在TCP连接建立以后，再进行RTMP协议层次的握手。

要建立一个有效的 RTMP Connection 链接，首先要“握手”：客户端要向服务器发送 C0,C1,C2（按序）三个chunk，服务器向客户端发送 S0,S1,S2（按序）三个chunk，然后才能进行有效的信息传输。RTMP协议本身并没有规定这6个Message的具体传输顺序，但 RTMP 协议的实现者需要保证这几点如下：

1. 客户端要等收到 S1 之后才能发送 C2
2. 客户端要等收到 S2 之后才能发送其他信息（控制信息和真实音视频等数据）
3. 服务端要等到收到 C0 之后发送 S1
4. 服务端必须等到收到 C1 之后才能发送 S2
5. 服务端必须等到收到 C2 之后才能发送其他信息（控制信息和真实音视频等数据）

<img src="https://s2.loli.net/2023/02/17/971BjW2zoAyN3XI.png" alt="729.png" style="zoom:35%;" />



**总结**

- 握手开始于客户端发送 C0、C1 块。服务器收到 C0 或 C1 后发送 S0 和 S1。
- 当客户端收齐 S0 和 S1 后，开始发送 C2。当服务器收齐C0和C1后，开始发送S2。
- 当客户端和服务器分别收到 S2 和 C2后，握手完成。

> RTMP握手的这个过程就是完成了两件事：
>
> 1. 校验客户端和服务器端RTMP协议版本号
> 2. 发了一堆随机数据，校验网络状况。



#### 消息类型（Message）

**RTMP  消息头格式**

```bash
 # 1字节消息类型（Message Type）
 # 3字节负载消息长度
 # 4字节时间戳
 # 3字节Stream ID，区分消息流
 0                   1                   2                   3
  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 | Message Type |               Payload length                   |
 |   (1 byte)   |                   (3 bytes)                    |
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 |                          Timestamp                            |
 |                          (4 bytes)                            |
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 |                          Stream ID            |
 |                          (3 bytes)            |
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
                         Message Header
```

- MT 分为协议控制消息（MT=1-6）、**音频数据消息（MT=8）**、**视频数据消息（MT=9）**、元数据消息（MT=15/18）、共享对象消息（MT=16/19）、命令消息（MT=17/20）、聚合消息（MT=22）等

- 接收到命令消息后通过 AMF 解读消息内容

  > Action Message Format: A compact binary format that is used to serialize ActionScript object graphs。有两个版本：AMF0和AMF3
  >
  > 1. AMF3 用作 Flash Playe 9 的 ActionScript 3.0 的默认序列化格式
  >
  > 2. AMF0 用作旧版的 ActionScript 1.0 和 2.0 的序列化格式
  >
  > 在网络传输数据方面，AMF3 比 AMF0 更有效率。AMF3 能将 int 和 uint 对象作为整数（integer）传输，并且能序列化 ActionScript 3.0 才支持的数据类型, 比如 ByteArray，XML 和 Iexternalizable



#### 消息分块（Chunk）

**为什么 RTMP 要将 Message 拆分成不同的 Chunk 呢？**

> 通过拆分，数据量较大的 Message 可以被拆分成较小的 “Message”，这样就可以**避免优先级低的消息持续发送阻塞优先级高的数据**，比如在视频的传输过程中，会包括**视频帧，音频帧和RTMP控制信息**，如果持续发送音频数据或者控制数据的话可能就会造成视频帧的阻塞，然后就会造成看视频时最烦人的卡顿现象。同时对于数据量较小的Message，可以通过对 Chunk Header 的字段来压缩信息，从而减少信息的传输量。



<img src="https://images2015.cnblogs.com/blog/693621/201706/693621-20170627103318493-1240078246.png" alt="Chunk" style="zoom:90%;" align="left"/>

Chunk 由 Header + Data 组成，Header 由 Basic Header + Message Header [+ Extended Timestamp] 组成

```bash
+-------+     +--------------+----------------+  
| Chunk |  =  | Chunk Header |   Chunk Data   |  
+-------+     +--------------+----------------+

+--------------+     +-------------+----------------+-------------------+ 
| Chunk Header |  =  | Basic header| Message Header |Extended Timestamp |  
+--------------+     +-------------+----------------+-------------------+

# Basic Header(1-3 bytes): fmt(2 bit --> Chunk Message Header) + csid
+-+-+-+-+-+-+-+-+
|fmt|   cs id   |
+-+-+-+-+-+-+-+-+

# Chunk Message Header(11/7/3/0 bytes)
0                   1                   2                   3   
0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+  
|                     timestamp(3 byte)         |               :           
+-------------------------------+---------------+---------------+  
:    message length(3 byte)     |message type id|               : 
+-------------------------------+---------------+---------------+ 
:           message stream id(4 byte)           |  
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

# Extended Timestamp(0/4 bytes)
0                   1                   2                   3   
0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 
|                           timestamp                           |   
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

# 用户层面上真正想要发送的与协议无关的数据，长度在(0,chunkSize]之间
# chunk size默认为128字节
+-----------+
|Chunk Data |
+-----------+
```

**Basic  Header**

- fmt：占 2bit，对应 Chunk Message Header 四种类型
- csid：长度待定，表示 Chunk Stream Id，最多支持 65597 个流，范围是 3-65599

**Chunk Message Header**

- timestamp: 最大表示16777215=0xFFFFFF=2^24-1，超出这个值置为1，将实际数据转存到 Extended Timestamp 字段中
- message length: 表示实际发送的消息的数据如音频帧、视频帧等数据的长度，注意这里是Message的长度，也就是chunk属于的Message的总数据长度，而不是chunk本身Data的数据的长度
- message type id: 占 1 byte, 表示实际发送的数据的类型，如**8代表音频数据、9代表视频数据**
- message stream id: 占 4byte 表示该 chunk 所在的流的 ID，和 Basic Header 的 csid 一样，message stream id 采用**小端存储**，RTMP都是大端模式，所以发送数据，包头，交互消息都要填写大端模式的，但是只有 stream id 是小端模式
  - 如果包头 Message Type ID 为 0x08 或 0x09，数据(chunk data) 是 FLV 的 tag data (没有tag header)，FLV格式封装请见 [FLV格式解析](https://juejin.cn/post/6844903994008010766)
  - 也可以用新类型 Message Type ID 为0x16，数据(chunk data) 是一个完整 FLV 的 tag(tag header + tag data) 

**总结**

- Chunk Size: RTMP 是按照 chunk size 进行分块，chunk size 指的是 chunk 的payload 部分的大小，不包括chunk basic header 和 chunk message header 长度。客户端和服务器端各自维护了两个 chunk size, 分别是自身分块的 chunk size 和 对端 的 chunk size, **默认的这两个 chunk size 都是128字节**。通过向对端发送set chunk size 消息可以告知对方更改了 chunk size的大小。
- Chunk Type: RTMP 消息分成的 Chunk 有4种类型，可以通过 chunk basic header 的高两位 (fmt) 指定，一般在拆包的时候会把一个RTMP消息拆成以`fmt=0`开始的chunk，之后的包拆成`fmt=3` 类型的 chunk

> 如果第二个 message 和第一个 message 的 message stream ID 相同，并且第二个message的长度也大于了chunk size，那么该如何拆包？当时查了很多资料，都没有介绍。后来看了一些源码，如 SRS，FFMPEG中的实现，发现第二个message可以拆成`fmt=1`类型一个chunk， message剩余的部分拆成`fmt=3`类型的chunk。FFMPEG中就是这么做的。



#### 命令类型

**NetConnection 命令**

- connect
- createStream

**NetStream 命令**

- play
- play2
- deleteStream
- receiveAudio
- receiveVideo
- publish
- seek
- pause

> 具体参考：https://www.cnblogs.com/Kingfans/p/7083100.html



#### 变体协议

- RTMPS：基于 **TLS/SSL** 连接的 RTMP。与 RTMPE 相比，设置和使用 RTMPS 要复杂一些，但是能够确保一定程度的安全性
- RTMPE：使用了包含 Diffie–Hellman key exchange 和 HMACSHA256 的行业标准加密。它生成了一对 **RC4 密钥**，其中：第一个密钥用于加密从服务器向客户端发出的媒体数据；第二个密钥用于加密向服务器发送的数据
- RTMP Proper: 是指使用 1935 端口、基于 TCP 的 RTMP 的别名
- RTMPT：建立在 **HTTP 协议**之上，是通过 HTTP 封装后的 RTMP 协议。它允许 RTMP 信息穿过防火墙，封装的信息可以是 RTMP Proper、RTMPS 或 RTMPE 数据包
- RTMFP：基于 **UDP协议**  且没有使用 RTMP Chunk Stream。Real Time Media Flow Protocol设计用于直接在 P2P 之间进行低延迟、实时的音频和视频通信，而无需通过 RTMP 服务器



#### 参考

- [深入理解rtmp(四)之协议实现分析](https://juejin.cn/post/6844904052325613575)
- [RTMP的工作原理 (thepaper.cn)](https://m.thepaper.cn/baijiahao_18276826)
- [RTMP 协议规范(中文版) ](https://www.cnblogs.com/Kingfans/p/7083100.html)



### FLV 封装协议

FLV 是一种封装格式，主要用于流媒体系统，体积轻巧、封装播放简单，很适合网络应用，嵌入 Flash 的浏览器就可以播放



#### FLV Header

所有的 FLV 格式文件都以 FLV Header 开头，具体类型结构如下：

```cpp
typedef struct {
	UI8 Signature;	// 'F'(0x46)
    UI8 Signature;	// 'L'(0x4C)
    UI8 Signature;	// 'V'(0x56)
    UI8 Version;	// FLV 版本号
    UI8 TypeFlags;	// b[0] 是否存在视频流 b[2] 是否存在音频流 05 表示音视频都有
    UI32 DataOffset; // 文件头长度，4个字节，一般长度为9
} FLVHEADER;
```



#### FLV Body

一个 FLV 文件，除开头的 FLV Header 外，剩余部分就是 FLV Body。FLV Body 由一系列 back-pointer 和 tag 交织构成。**back-pointer 表示前一 tag 大小**。FLV Body 类型是 FLVBODY，FLVBODY 定义如下：

```cpp
typedef struct {
    UI32 PreviousTagSize0;	// 0
    FLVTAG Tag1;			// 第一个 Tag
    UI32 PreviousTagSize1;	// 前一个 Tag 大小
    FLVTAG Tag2;
    ...
    UI32 PreviousTagSizeN-1;
    FLVTAG TagN;
    UI32 PreviousTagSizeN;
} FLVBODY;
```



**FLV Tag** 包含音频、视频或脚本元数据、可选的加密元数据和 payload。FLV Tag 类型是 FLVTAG，FLVTAG 定义如下：（UI24 占3个字节）

```cpp
typedef struct {
    UB[2] Reserved;	// 0，保留字段
    UB[1] Filter;	// 表示 packet 是否需要预处理：0-不需要，1-需要
    UB[5] TagType;	// 数据类型：8-音频，9-视频，18-脚本数据
    UI24 DataSize;	// Header + Data 字段长度?
    UI24 Timestamp;	// 解码时间戳 DTS(ms)，第一个 Tag 的 DTS 为0
    UI8 TimestampExtended;	// 和 TimeStamp 一起扩展至32位，此字段为高8位
    UI24 StreamID;	// 0
  IF TagType == 8	// 数据区
    AudioTagHeader Header;
  IF TagType == 9
    VideoTagHeader Header;
  IF TagType == 8
    AUDIODATA Data;
  IF TagType == 9
    VIDEODATA Data;
  IF TagType == 18
    SCRIPTDATA Data;
} FLVTAG;
```

一个 FLVTAG 中，**前 11 个字节是通用 TagHeader**，后面紧跟跟着音频 Tag、视频 Tag 或脚本 Tag，其中音频 Tag 和视频 Tag 都包含 TagHeader 和 TagBody 两部分，脚本 Tag 只有 TagBody 部分。



Audio Tag = AudioTagHeader + AudioTagBody

```cpp
typedef struct {
    UB [4] SoundFormat;
    UB [2] SoundRate;
    UB [1] SoundSize;
    UB [1] SoundType;
  IF SoundFormat == 10
    UI8 AACPacketType;
} AudioTagHeader;

typedef struct {
  IF Encrypted
    EncryptedBody Body
  else
    AudioTagBody Body;
} AUDIODATA;

typedef struct {
  IF SoundFormat == 10
    AACAUDIODATA SoundData;
  ELSE
    Varies by format
} AudioTagBody;
```



Video Tag = VideoTagHeader + VideoTagBody

Data Tag / Script TagBody：script 类型的 tag 通常会更在 FLVHEADER 后面，并且只有一个。其中的数据区会存放一些关于 FLV 音视频的参数信息，例如 duration、width、height 等

**FLV完整的封装结构如下：**

<img src="https://leisure_chn.gitee.io/blog/figure/containers/flv.jpg" alt="FLV" style="zoom:80%;" align="left"/>



#### 参考

- 格式详解：
  - https://www.cnblogs.com/leisure_chn/p/10662941.html
  - https://cloud.tencent.com/developer/article/1814062
- 分析工具：https://www.cnblogs.com/renhui/p/10348629.html
- 代码分解：https://blog.csdn.net/weixin_40840000/article/details/107859776



### RTSP

#### 概念

RTSP是一个实时传输流协议，是一个应用层的协议，通常说的RTSP包括RTSP协议、RTP协议、RTCP协议

- RTSP协议：负责服务器与客户端之间的请求与响应

- RTP协议：负责传输媒体数据

- RTCP协议：在RTP传输过程中提供传输信息

rtsp承载与rtp和rtcp之上，rtsp并不会发送媒体数据，而是使用rtp协议传输，**rtp并没有规定发送方式，可以选择udp发送或者tcp发送**

> RTP 使用一个 偶数 UDP port；而RTCP 则使用 RTP 的下一个 port，也就是一个奇数 port。RTCP与RTP联合工作，RTP实施实际数据的传输，RTCP则负责将控制包送至电话中的每个人。其主要功能是就 RTP 正在提供的服务质量做出反馈



#### 通信过程

```bash
# 信息交互交互过程基于 SDP 协议
客户端->>服务器: OPTIONS
服务器->>客户端: 200 OK
客户端->>服务器: DESCRIBE
服务器->>客户端: 200 OK
客户端->>服务器: SETUP
服务器->>客户端: 200 OK
客户端->>服务器: PLAY
服务器->>客户端: (RTP包)
客户端->>服务器: TEARDOWN
服务器->>客户端: 200 OK
```



1. 首先实现RTSP**建立连接**过程，创建套接字，基于TCP，socket，setsockopt，bind，listen

   | 方法     | 描述                                                         |
   | -------- | ------------------------------------------------------------ |
   | OPTIONS  | 获取服务端提供的可用方法                                     |
   | DESCRIBE | 向服务端获取对应会话的媒体描述信息                           |
   | SETUP    | 向服务端发起建立请求，建立连接会话（获取**客户端的RTP和RTCP端口号**） |
   | PLAY     | 向服务端发起播放请求（发送数据包）                           |
   | TEARDOWN | 向服务端发起关闭连接会话请求                                 |

2. 然后创建通信用的RTP和RTCP套接字，基于UDP，socket，setsockopt，bind，accept

3. 使用RTP的套接字**发送数据**

   1. 解析H264文件结构：单元结构是NALU，每个NALU之间通过 start code `00 00 00 01` 或者`00 00 01`分隔开，然后 NUAL header 是主要的数据
   2. 封装成RTP包：
      - 比较小的NALU包直接打包
      - 比较大的NALU包分片打包

4. 使用VLC客户端播放

> 代码参考：https://gitee.com/ericps/cpp-stream-learning/tree/master/rtspV1



**为什么RTP和RTCP是基于UDP的，而其上层的RTSP可以使用TCP传输数据？**

该协议定义了一对多应用程序如何有效地通过IP网络传送多媒体数据. **RTSP在体系结构上位于RTP和RTCP之上, 它使用TCP或RTP完成数据传输**. RTSP被用于建立的控制媒体流的传输，它为多媒体服务扮演“网络远程控制”的角色。尽管有时可以把RTSP控制信息和媒体数据流交织在一起传送，**但一般情况RTSP本身并不用于转送媒体流数据。媒体数据的传送可通过RTP/RTCP等协议来完成**。



#### RTSP Interleaved Frame

一般 RTP/RTCP 通过 UDP 传输数据，往往前者传输音视频数据，后者传输控制信息。如果使用 TCP 传输数据，有时候出于安全设计, 防火墙可能要求 RTSP 控制方法（RTCP）和流数据（RTP）公用一个通信通道，进行交错传输。此时就需要区分 RTP 通道和 RTSP 通道，为此在 RTP 上加一层 RTSP Interleaved Frame，如下图

<img src="https://upload-images.jianshu.io/upload_images/1720840-c723a819613748e1.png" alt="RTSP Interleaved Frame" style="zoom:50%;" />

#### 参考

- [从零开始写一个RTSP服务器](https://blog.csdn.net/weixin_42462202/category_9293806.html)
- [最详细的流媒体传输协议-rtsp协议详解](https://blog.csdn.net/m0_60259116/article/details/123401239)
- [RTP/AVP & RTP/AVP/TCP - 简书 (jianshu.com)](https://www.jianshu.com/p/7b9793eb2f4e)



### DASH

DASH 全称是 Dynamic Adaptive Streaming over HTTP，即基于HTTP的动态自适应的比特率流。在2011年底MPEG和ISO共同制定了MPEG-DASH标准，并于2014年成为首个基于HTTP协议的自适应流媒体技术的国际标准。



#### MPD

DASH 的 Manifest 文件名为 Media Presentation Descrption(MPD)，使用XML格式，对音视频流作了多个维度的划分，下面我们对其**结构和内容**做一个分析。

MPD文件的结构由外向内分别是 **Period（周期）→AdaptationSet（自适应子集）→Representation（码流）→Segment（片段）**

- **Period:** 一个 Period 代表一个时间段，在直播中一般只使用一个 Period，多 Period 只有在一些特殊场景下才会使用。
- **Adaptation Set:** 一个 Period 由一个或者多个 Adaptation Set 组成，一组可供切换的不同码率的码流组合成一个自适应子集。
- **Representation:** 每个 Adaptation Set 包含了一个或者多个 Representation，每个 Representation 代表一路**音频流或视频流**。同一个 Adaptation Set 的多个 Representation 之间代表他们由同一路源流产生的不同的码率、分辨率、帧率等等的码流。
- **Segment:** 每个 Representation 包含多个 Segment。与 HLS 类似，每个 Segment 代表一小段音频或视频数据，其中 DASH Segment 常用的载体是使用fmp4格式。



MPD 文件中有用于描述该DASH流特点的字段参数，如maxSegmentDuration、minBufferTime、minimumUpdatePeriod、publishTime、type等等。也有用于描述视频流信息的字段参数，如bandwidth、codecs、width、height等等。比较重要的两个参数是：

- **minimumUpdatePeriod (MPD最低限度更新时间) :** 告诉播放器MPD内容更新间隔，播放器会根据此值来控制MPD轮询更新时间，其值过大会导致内容更新不及时导致卡顿。用于直播场景，点播场景应该不存在。
- **minBufferTime (最小缓存时间): ** 播放器最小的缓存音视频时长，其值需要为最小的 segment 时长。



**下载流程**

1. 下载 MPD 文件，解析 DASH 相关信息；
2. 下载视频的 Initialization Segment 和音频的 Initialization Segment；
3. 下载视频的第一个分片，下载音频的第一个分片；
4. 当视频和音频的第一个分片都下载完，播放器内部再进行一些相关处理后，就可以开始播放出画面。后续就是不断轮询更新MPD文件和下载后续的音频和视频分片。



#### 参考

- [DASH, MPD文件, 多码率切换](https://cloud.tencent.com/developer/article/1895146)
- [MPD 文件结构剖析-CSDN博客](https://blog.csdn.net/luoxueqian/article/details/82982188)
- [目前几种实时视频流协议对比_leadersmallsmile的博客-CSDN博客](https://blog.csdn.net/leadersmallsmile/article/details/105121995)
