## Redis

> 主要参考：https://xiaolincoding.com/redis

### 面试指北

#### Redis 概念

- redis 是一种基于内存的数据库，对数据的读写操作都是在内存中完成的，因此读写速度非常快，常用于**缓存、消息队列、分布式锁**等场景
- 提供很多数据类型：String、Hash、List、Set(集合)、Zset(有序集合)、Bitmaps、HyperLogLog(基数统计)、GEO(地理位置)、Stream(流)，一般在单线程中没有线程安全问题



**和 memcached 异同点**：

- 相同点：基于内存、有过期策略、性能高

- 不同点：
  - Redis 数据类型更丰富；Memcached 只支持 key-value
  - Redis 支持持久化，可以将数据保存在磁盘，重启时加载使用；Memcached 不支持
  - Redis 原生支持集群模式，Memcached 没有
  - Redis 支持发布订阅模型、Lua 脚本、事务等功能；Memcached 不支持



#### Redis 数据结构

**String**

- 底层使用 SDS (Simple Dynamic String) 实现
- 可以保存**文本数据和二进制数据**、O(1)时间内获取字符串长度
- 自动扩容、拼接字符串不会造成缓冲区溢出



**List**

- 底层使用**双向链表或者压缩列表**实现
- 压缩列表：列表元素个数少于 512 个、每个元素大小不超过 64 字节
- 否则就使用双向链表，3.2 版本之后底层都使用 **quicklist** 实现



**Hash**

- 底层使用**压缩列表或者哈希表**实现
- Redis 7.0 之后~~压缩列表~~数据结构被废弃，使用 **listpack** 代替



**Set**

- 底层使用**哈希表或者整数集合**实现
- 集合元素个数小于 512 就是整数集合，否则就使用哈希表



**Zset**

- 底层使用**压缩列表或跳表**实现
- 如果有序集合的元素个数小于 128 个，并且每个元素的值小于 64 字节时就使用~~压缩列表~~
- 否则就使用跳表实现



#### Redis 线程模型

严格来说 Redis 不是单线程的，一般的说的单线程是指 Redis **主线程**：「接受客户端请求 --> 解析请求 --> 进行数据读写等操作 --> 发送数据给客户端 」

- Redis 2.6 版本会有 2 个后台线程，分别处理**关闭文件以及APF刷盘**任务
- Redis 4.0 版本之后新增一个后台线程，异步释放 Redis 内存，也就是 **lazyfree 线程**

后台线程相当于消费者，都有各自的任务队列，不断轮询各自队列完成相应的任务



**CPU 并不是制约 Redis 性能表现的瓶颈所在**，因此 Redis 6.0 之前都采用单线程模型，在 Redis 6.0 版本之后，也采用了多个 I/O 线程来处理网络请求，**这是因为随着网络硬件的性能提升，Redis 的性能瓶颈有时会出现在网络 I/O 的处理上**。



#### Redis 持久化

三种数据持久化方式：

- AOF 日志：每执行一条写操作命令，就把该命令以追加的方式写入到一个文件里；
- RDB 快照：将某一时刻的内存数据，以二进制的方式写入磁盘；
- 混合持久化方式：Redis 4.0 新增的方式，集成了 AOF 和 RBD 的优点；



**AOF 将内核缓冲区数据写入磁盘时机**：

- Always：同步写回，可靠性高但是性能开销大
- Everysec：每秒写回，性能始终
- No：由操作系统控制写回，性能好但是宕机时丢失数据较多

AOF 日志过大时会触发 AOF 重写机制，由后台子进程 bgrewriteaof 完成



**RDB 快照**

AOF 日志记录的操作命令不是时机的数据，往往 AOF 日志比较多，恢复缓慢，为此引入 RDB 快照，记录某一时刻的内存数据，RDB 的效率比 AOF 高，因为只需要将 RDB 文件读入内存即可

- save 命令在主线程生 RDB 文件，会阻塞主线程
- bgsave 命令会场见一个子进程来生成 RDB 文件

bgsave 过程中 Redis 仍然可以继续处理操作命令，因为 bgsave 通过「读时共享，写时复制」完成



**混合持久化**

RDB 优点是数据恢复速度快，但是频率不太好把握；AOF 优点是丢失数据少但是数据恢复太慢。因此 Redis 4.0 提出了混合使用 AOF 日志和内存快照

- 混合持久化工作在 **AOF 日志重写过程**中，fork 子进程先将与主线程共享的内存数据以 RDB 方式写入 AOF 文件，然后主线程处理的操作命令会被记录在重写缓冲区里，重写缓冲区里的增量命令会以 AOF 方式写入到 AOF 文件
- 混合持久化，AOF 文件的**前半部分是 RDB 格式的全量数据，后半部分是 AOF 格式的增量数据**



#### Redis 高可用

- 主从复制：主服务器可以进行读写操作，从服务器只读
- 哨兵模式：监控主从服务，提供主从节点古装转移的功能
- 切片集群模型：当 Redis 缓存数据量达到一台服务器无法缓存是需要使用切片集群



#### Redis 过期和淘汰

每当我们对一个 key 设置了过期时间时，Redis 会把该 key 带上过期时间存储到一个**过期字典**（expires dict）中，也就是说「过期字典」保存了数据库中所有 key 的过期时间

- 懒惰删除：不会过多占用 CPU，但是过期的 key 一直存在内存
- 定期删除：每个一段时间「随机」从数据库中取出一定数量的 key 进行检查，并删除其中的过期的 key



**RDB 持久化时如何处理过期键？**

- RDB 文件生成阶段：**过期的键「不会」被保存到新的 RDB 文件中**
- RDB 加载阶段：
  - 主服务器：过期键不会被载入到数据库中
  - 从服务器：不论键是否过期都会被载入，但是主从服务器在进行数据同步时从服务器数据会被清空，因此载入过期键也没有影响



**AOF 持久化时如何处理过期键？**

- AOF 写入阶段：AOF 模式下，如果某个过期键未被删除，则 AOF 文件会保留该键。当该键被删除后，Redis 会通过追加 DEL 命令来显式地删除该键值
- AOF 重写阶段：执行 AOF 重写时，已过期的键不会被保存到重写后的 AOF 文件中，因此不会对 AOF 重写造成任何影响



**Redis 内存满了之后会触发内存淘汰机制**

- volatile-random：随机淘汰设置了过期时间的任意键值
- volatile-ttl：有限淘汰更早过期的键值
- volatile-lru：淘汰所有设置了过期时间的键值，最久未使用的键值
- volatile-lfu：淘汰所有设置了过期时间的键值，最少使用的键值
- allkeys-random：随机淘汰任意键值
- allkeys-lru：淘汰整个键值中最久未使用的键值，Redis 对象结构体中额外字段记录时间戳（24bit）
- allkeys-lfu：淘汰整个键值中最少使用的键值，高 16bit 存储 ldt(Last Decrement Time)，用来记录 key 的访问时间戳；低 8bit 存储 logc(Logistic Counter)，用来记录 key 的访问频次



#### Redis 缓存问题

**缓存雪崩**：大量缓存数据同一时间失效时如果此时有大量用户请求就会有大量请求直接访问数据库

- 将缓存失效时间随机打散
- 设置缓存不过期



**缓存击穿**：某个热点数据过期了，大量访问该数据的请求直接打到数据库

- 互斥锁方案
- 不给热点数据设置过期时间或者更新热点数据的过期时间



**缓存穿透**：一般由于业务误操作删除某个缓存数据或者黑客恶意攻击读取不在缓存中的数据

- 限制非法请求、设置空值或者默认值
- 使用布隆过滤器快速判断数据是否存在，避免通过查询数据库来判断数据是否存在



**三种更新缓存策略**

- Cache Aside 策略：**适用于读多写少的场景，不适合写多的场景**
  - 写策略：先更新数据库中的数据，再删除缓存中的数据（==顺序不能变==）
  - 读策略：命中直接返回，否则就从数据库读取数据然后写入缓存并返回
- Read/Write Through 策略
- Write Back 策略



#### Redis 实战

- ZSet 实现延迟队列
- 处理大 key（对应的 Value 比较大），需要分批次删除或者异步删除（lazyfree）
- Redis-cli 可以通过管道技术实现一次处理多个 Redis 命令
- Redis 事务没有回滚机制
- Redis 实现分布式锁 todo



### 数据类型

#### String

最基本的 key-value 结构，value 可以是具体的值、字符串或者数字，内部编码有三种：int、raw 和 embstr

应用场景：

- 缓存对象：`SET user:1 '{"name":"xiaolin", "age":18}'` 或者 `MSET user:1:name xiaolin user:1:age 18 user:2:name xiaomei user:2:age 20`
- 常规计数：因为 Redis 处理命令是单线程，所以执行命令的过程是原子的。因此 String 数据类型适合计数场景，比如计算访问次数、点赞、转发、库存数量等等

- 分布式锁：set 命令有一个 nx 参数可以实现「key 不存在时才插入」



#### List

3.2 版本之前由**双向链表或者压缩列表**实现，之后使用 quicklist 实现，常用命令有

```bash
LPUSH key value [value ...]
RPUSH key value [value ...]
LPOP key
RPOP key
LRANGE key start stop
```

应用场景：

消息队列：存取消息时需要满足**消息保序、处理重复的消息和保证消息可靠性**这些条件，其中

- 「消息保序」使用 LPUSH+RPOP 即可，另外 BRPOP 可以实现「阻塞读取」，比消费者不停循环读取更加有效，
- 生产者自行维护一个全局唯一的 ID 「处理重复消息」，最后使用 BRPOPLPUSH 保证消息的「可靠性」（消费者读取一条消息之后 Redis 会将这个消息插入另一个 list 做备份）



#### Hash

7.0 版本之前由**压缩列表或哈希表**实现，7.0 之后压缩列表被 listpack 替代

```bash
HSET key field value
HGET key field

HMSET key field value [field value...]
HMGET key field [field ...]

HDEL key field [field ...]

HLEN key
HGETALL key
HINCRBY key field n # 为哈希表 key 中field键的值加上增量 n
```

应用场景：注意和 String + Json 稍微区分一下，一般对象用 String + Json 存储，对象中某些**频繁变化**的属性可以考虑抽出来用 Hash 类型存储，例如购物车场景中「以用户 id 为 key，商品 id 为 field，商品数量为 value，恰好构成了购物车的3个要素」



#### Set

一个集合最多存储 2^23-1 个元素，支持集合内的增删改查以及集合之间的交集并集差集

底层使用**哈希表或者整数集合**实现，常用命令：

```bash
SADD key member [member ...]
SREM key member [member ...]

SMEMBERS key # 获取集合 key 中所有元素
SCARD key # 获取集合 key 中的元素个数

SISMEMBER key # 判断 member 元素是否存在集合 key 中

SRANDMEMBER key [count] # 从集合 key 中随机选出 count 个元素，元素不从 key 中删除
SPOP key [count] # 从集合 key 中随机选出 count 个元素，元素从 key 中删除

# 集合之间的运算时间复杂度较高
SINTER key [key ...] # 交集
SUNION key [key ...] # 并集
SDIFF key [key ...] # 差集
```

和 list 区别

- list 可以存储重复元素，set 不行
- list 存储元素有先后顺序，set 无序

使用场景：**文章点赞数、公众号共同关注好友数、抽奖活动 (SRANDMEMBER/SPOP)**



#### Zset

有序集合，相比 Set 多一个 score 属性，Zset 就按照 score 排序

底层使用**压缩列表或跳表**实现的，7.0 之后压缩列表被 listpack 替代，常用命令：

```bash
ZADD key score member [[score member] ...]
ZREM key member [member...]                 

ZSCORE key member # 返回有序集合key中元素member的分值
ZCARD key # 返回有序集合key中元素个数

ZINCRBY key increment member # 为有序集合key中元素member的分值加上increment

ZRANGE key start stop [WITHSCORES] # 正序获取有序集合key从start下标到stop下标的元素
ZREVRANGE key start stop [WITHSCORES] # 倒序获取有序集合key从start下标到stop下标的元素

# 返回有序集合中指定分数区间内的成员，分数由低到高排序。
ZRANGEBYSCORE key min max [WITHSCORES] [LIMIT offset count]

# 返回指定成员区间内的成员，按字典正序排列, 分数必须相同
ZRANGEBYLEX key min max [LIMIT offset count]
# 返回指定成员区间内的成员，按字典倒序排列, 分数必须相同
ZREVRANGEBYLEX key max min [LIMIT offset count]
```

应用场景：面对需要**展示最新列表、排行榜**等场景时，如果数据更新频繁或者需要分页显示，可以优先考虑使用 Sorted Set

- 排行榜：学生成绩的排名榜、游戏积分排行榜、视频播放排名、电商系统中商品的销量排名等
- 电话、姓名排序：使用 ZrangeBylex/ZRevRangeBylex 排序时需要确保 score 一致



#### BitMap

一串连续的二进制数组，可以通过偏移量定位元素，BitMap 通过最小的单位进行 0|1 的设置，表示某个元素的值或者状态，时间复杂度为 O(1)

底层使用 String 保存二值状态数组

```bash
SETBIT key offset value # 设置值，value 只能是 0/1
GETBIT key offset
BITCOUNT key [start end] # 统计 start 和 end 范围内的 1 的个数
```

应用场景：二值状态统计的场景

- 签到打卡：BITPOS 可以统计用户首次打卡时间
- 判断用户登录状态：SETBIT 设置登录、GETBIT 检查用户是否登录
- 连续签到用户总数：日期作为 key，userId 作为 offset，如果要统计3天连续打卡的用户数，就将三个 bitmap进行 AND 操作，并将结果保存到 destmap 中，然后对 destmap 进行 BITCOUNT 统计



#### HyperLogLog

主要作用就是**提供不精确的去重计数**，一种用于「统计基数」的数据集合类型，基数统计就是指统计一个集合中不重复的元素个数。但要注意，HyperLogLog 是统计规则是基于概率完成的，不是非常准确，标准误算率是 0.81%

在 Redis 里面，**每个 HyperLogLog 键只需要花费 12 KB 内存，就可以计算接近 `2^64` 个不同元素的基数**，和元素越多就越耗费内存的 Set 和 Hash 类型相比，HyperLogLog 就非常节省空间。

```bash
PFADD key element [element ...] # 指定元素添加到 HyperLogLog 中
PFCOUNT key [key ...] # 返回给定 HyperLogLog 的基数估算值
PFMERGE destkey sourcekey [sourcekey ...] # PFMERGE destkey sourcekey [sourcekey ...]
```

应用场景：百万级网页 UV 计数



#### GEO

用于存储地理位置信息，并对存储信息进行操作

应用场景：滴滴叫车，使用 GEOADD 存储指定的地理位置空间，然后基于用户的地理位置信息利用 GEORADIUS 获取周围指定范围内的车辆信息



#### Stream

Redis 5.0 版本之前使用 list 实现消息队列存在很多问题，例如不能重复消费、生产者需要自行实现全局唯一 ID 来避免重复的消息等等

stream 可以完美实现消息队列，支持消息的持久化、支持自动生成全局唯一 ID、支持 ack 确认消息的模式、支持消费者模式、让消费者队列更加的稳定和可靠



> **Redis 发布/订阅机制为什么不可以作为消息队列？**

发布订阅机制存在以下缺点，都是跟丢失数据有关：

1. 发布/订阅机制没有基于任何数据类型实现，所以不具备「数据持久化」的能力，也就是发布/订阅机制的相关操作，不会写入到 RDB 和 AOF 中，当 Redis 宕机重启，发布/订阅机制的数据也会全部丢失。
2. 发布订阅模式是“发后既忘”的工作模式，如果有订阅者离线重连之后不能消费之前的历史消息。
3. 当消费端有一定的消息积压时，也就是生产者发送的消息，消费者消费不过来时，如果超过 32M 或者是 60s 内持续保持在 8M 以上，消费端会被强行断开，这个参数是在配置文件中设置的，默认值是 `client-output-buffer-limit pubsub 32mb 8mb 60`。

所以，发布/订阅机制只适合即时通讯的场景，例如在C++集群聊天服务器中就使用 redis 的发布订阅模式实现一个简单的消息队列，参考：https://github.com/fixbug666/chatserver
