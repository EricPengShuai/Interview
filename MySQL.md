## MySQL

> 施磊 MySQL 课程

### 核心 SQL

#### limit

```mysql
-- limit 在一些没有索引的查询上是有优化的
-- explain 可以查看 SQL 语句大致的执行计划，但不准确

select name, age from limit 5; -- 限制前 5 行
select name, age from limit 1, 5; -- 偏移 1 行之后再限制前 5 行
select name, age from limit 5 offset 1; -- 偏移 1 行之后再限制前 5 行
```

分页操作

```mysql
select * freom user limit (M-1)*N, N; -- 瓶颈在 M
select * from user where id > 上一页最后一条数据的id值 limit N; -- 利用索引查询更好
```

Create procedure 语法

```mysql
delimiter $

Create Procedure add_t_user (IN n INT) BEGIN
DECLARE i INT;
SET i=0;
WHILE i<n DO
INSERT INTO t_user VALUES(NULL,CONCAT(i+1,'@fixbug.com'),i+1); SET i=i+1;
END WHILE;
END$

delimiter ;

call add_t_user(2000000);
```



#### order

- 排序性能和「索引」以及「select 的属性」相关
- 观察 explain 输出的  extra 字段：using filesort, using index, null



#### group

- 本身涉及排序，因此 GROUP BY 后面的字段使用索引很关键
- 观察 explain 输出的  extra 字段：using temporary, using filesort, using index



#### join

**内连接**

- on a.uid = c.uid 底层区分大表和小表，按照数据量来区分，小表永远是整表扫描，然后区大表搜索，因此在大表上建立索引更好
- **注意这里小表和大表是在 where 过滤之后比较的**
- 对于 inner join 内连接，限制/过滤条件写在 where 的后面和 on 连接条件里面，效果是一样的

**外连接**

- 左外连接和右外连接
- **外连接中过滤条件写在 on 后面和 where 后面区别是很大的**



#### 常用信息语言

```mysql
show indexes from student; -- 查看 student 表的所有索引信息
show create table student\G -- 查看建表的信息（其中的索引结构信息可能不太准）

desc stduent;
desc student\g -- 查看具体表结构

create index ageidx on student(age); -- 在 name 字段上创建索引
drop index ageidx on student; -- 删除 student 表的 nameidx 索引

select * from student force index(ageidx) where name='linfeng' and age=22; -- 强制索引
```



### 存储引擎以及索引

#### MyISAM vs InnoDB

三个主要的元素：表结构、表数据、表索引，不同存储引擎存储方式不同

- MyISAM：\*.frm 存储表结构、\*.MYD 存储表数据 MYDATA、\*.MYI 存储表索引 MYIndex

- InnoDB：\*.frm 存储表结构、\*.ibd 存储表数据和索引

InnoDB 中数据和索引一起存放，所以如果没有指定主键，会有一个默认的idx作为主键索引，但是 MyISAM 不会，索引和数据一起存放一般叫做**聚集索引或者聚簇索引**，MyISAM 的索引树都是非聚集索引

memory 是**基于内存**的存储引擎：使用存在内存中的内容来创建表。每个MEMORY 表实际只对应一个磁盘文件，格式 是.frm(表结构定义)。MEMORY 类型的表访问非常快，因为它的数据是放在内存中的，并且默认使用**HASH 索引** (不适合做范围查询)，**一旦服务关闭，表中的数据就会丢失掉**。

| 种类   | 锁机制 | B-树索引 | 哈希索引 | 外键   | 事务   | 索引缓存 | 数据缓存 |
| ------ | ------ | -------- | -------- | ------ | ------ | -------- | -------- |
| MyISAM | 表锁   | 支持     | 不支持   | 不支持 | 不支持 | 支持     | 不支持   |
| InnoDB | 行锁   | 支持     | 不支持   | 支持   | 支持   | 支持     | 支持     |
| Memory | 表锁   | 支持     | 支持     | 不支持 | 不支持 | 支持     | 支持     |



#### B+ 树索引

MySQL 访问索引也是需要磁盘 IO 的，并且访问磁盘是以「页」/「区」为单位的（以磁盘块为单位），因此磁盘 IO 是一个瓶颈

- B 树对比 AVL 树的好处就是磁盘 IO 次数少，搜索某个值的时间复杂度都是二分查找 log N 

[BTree](https://img2020.cnblogs.com/blog/1326194/202009/1326194-20200926221256872-871859524.png)

![](https://img-blog.csdnimg.cn/20210305171108195.png)

- B+ 树改进 B 树结构，基于以下几点：
  - B 树每一个非叶子节点不仅仅存储索引 key 也要存储索引对应的数据 data；但是 B+ 树每个非叶子节点只存放 key，不存储 data，这样一个节点存放的 key 值就更多，**B+ 树在理论上来说，层数就会更低**，搜索的效率就更好
  - B 树数据内存分散在不同的节点上，离根节点近搜索就快；离根结点远搜索就慢，因此**花费的磁盘 IO 次数不平均，每行数据搜索花费的时间也不平均**；但是 B+ 树只有在叶子节点上存储所有的索引以及数据，这样搜索每一个索引对应的数据时间是非常平均的
  - B 树不方便做范围搜索，整表遍历起来也不方便，B+ 树叶子结点被串在一个链表中，形成了一个**有序的链表**，如果要进行索引树的搜索或者整表搜索，直接遍历叶子节点就可以，**范围搜索很方便**

![B+Tree](https://img-blog.csdnimg.cn/img_convert/00d064acdfab2a62e4bf4f5618e7aa20.png)



#### 哈希索引

- 没办法处理磁盘上的数据，哈希索引是加载到内存上构建高效的搜索索引结构
- 只适合做等值搜索，不适合做范围查询、排序等



#### 自适应哈希索引

InnoDB 存储监测到同样的二级索引不断被使用（涉及不断地搜索或者回表），那么它就会根据这个二级索引，在内存上根据二级索引树（B+树）上的二级索引值，在内存上构建一个哈希索引来加速搜索

自适应哈希索引本身的数据维护也是要耗费性能的，并不是说自适应哈希索引在任何情况下都会提升二级索引的查询性能，应该根据**性能指标**来具体分析是否应该打开或者关闭自适应哈希索引

```mysql
show engine innodb status\G -- 查看性能指标参数，关注下面两个重要的信息
```

- RW-latch 等待的线程数量（自适应哈希索引默认分配了 8 个分区），同一个分区等待的线程数量过多就需要关注是否 diable adaptive hash index
- `hash serches/s`走自适应哈希索引搜索的频率（低）和 `non-hash second`二级索引树搜索的频率（高）

![自适应哈希索引.png](https://s2.loli.net/2023/08/20/pEdRViqKrkANDXn.png)



**注意**

- 一次查询同一张表只能使用一个索引
- where 等过滤条件不一定使用索引，比较区分度，例如过滤性别字段直接 ALL 全表扫描就可以
- 并不是 in 和 not in 一定不使用索引，有几个别情况，可以优化为 range 范围搜索



**实际项目中如何分析耗时的 sql 以及优化的索引？** :fire:

1. 慢查询日志：slow_query_log，设置合理的、业务可以接收的慢查询时间

```mysql
mysql> show variables like '%slow_query%';
+---------------------+---------------------------------------+
| Variable_name       | Value                                 |
+---------------------+---------------------------------------+
| slow_query_log      | OFF                                   |
| slow_query_log_file | /usr/local/mysql/data/ps-mba-slow.log |
+---------------------+---------------------------------------+
2 rows in set (0.03 sec)

mysql> set global slow_query_log=ON; # 注意这是全局变量

mysql> show variables like 'long%';
+-----------------+-----------+
| Variable_name   | Value     |
+-----------------+-----------+
| long_query_time | 10.000000 |
+-----------------+-----------+
1 row in set (0.00 sec)

mysql> set long_query_time = 1; # 注意，单位是秒 
Query OK, 0 rows affected (0.00 sec)
```

2. 压测执行各种业务
3. 查看慢查询日志，找出所有执行耗时的sql
4. 使用 explain 分析耗时 sql，例如如果 extra 字段有 using filesort 说明有外部排序了，可以考虑建立联合索引



### MySQL 事务

一个事务是由**一条或者多条对数据库操作的SQL语句**所组成的一个不可分割的单元，只有当事务中的所 有操作都正常执行完了，整个事务才会被提交给数据库；如果有部分事务处理失败，那么事务就要回退 到最初的状态，因此，事务要么全部执行成功，要么全部失败。

> MyISAM 不支持事务，InnoDB 支持事务和行锁（最大的特点）

- Atomic、Consistency、Isolation、Duration
- ACD 通过 redo/undo log 机制保证，I 隔离性通过 mysql 事务的锁机制来保证



#### 并发一致性问题

- 脏读：事务 B 读取了事务 A 尚未提交的数据，这是一定要杜绝的！
- 不可重复读：事务 B 读取了事务 A 已提交的数据
- 虚读/幻影读：事务 B 读取了事务 A 新增加的数据或者读不到事务 A 删除的数据

> NonRepeatable Read 和 Phantom Read 是可以接受的，就看我们需要的隔离级别



#### 隔离级别

- **TRANSACTION_READ_UNCOMMITTED** 未提交读：说明在提交前一个事务可以看到另一个事务的变化。这样读脏数据，不可重复读和虚读都是被允许的

- **TRANSACTION_READ_COMMITTED** 已提交读：说明读取未提交的数据是不允许的。这个级别仍然允许不可重复读和虚读产生

- **TRANSACTION_REPEATABLE_READ** 可重复读：说明事务保证能够再次读取相同的数据而不会失败，但虚读仍然会出现

  > MySQL 默认隔离级别，实际上可以解决部分的虚读问题，但是不能防止update更新产生的虚读问题，要禁止虚读产生，还是需要设置串行化隔离级别（看看施磊老师视频 48...）

- **TRANSACTION_SERIALIZABLE** 串行化：是最高的事务级别，它防止读脏数据，不可重复读和虚读



#### 事务处理命令

```mysql
# 查看 MySQL 是否自动提交事务
mysql> select @@autocommit;
+--------------+
| @@autocommit |
+--------------+
|            1 |
+--------------+
1 row in set (0.00 sec)

mysql> set autocommit=0;
Query OK, 0 rows affected (0.00 sec)

BEGIN; 	# 开启一个事务
COMMIT; # 提交一个事务
ROLLBACK; # 回滚一个事务到初始的位置

SAVEPOINT point1;   # 设置一个名字为point1的保存点
ROLLBACK TO point1; # 事务回滚到保存点point1，而不是回滚到初始状态 

SET TX_ISOLATION='REPEATABLE-READ'; # 设置事务的隔离级别
SELECT @@ TX_ISOLATION; # 查询事务的隔离级别

mysql> SELECT @@TX_ISOLATION; # MySQL 默认的隔离级别是可重复读
+-----------------+
| @@TX_ISOLATION  |
+-----------------+
| REPEATABLE-READ |
+-----------------+
1 row in set, 1 warning (0.00 sec)
```



### MySQL 锁

**可串行化隔离级别在执行事务时是主动加锁的**，也就说 select 就是获取读锁、update 等就是获取写锁，但是可重复读隔离级别在执行事务时不会主动加锁，需要使用以下命令显示加锁

```mysql
# 显示加锁命令
select ... lock in share mode; # 强制获取共享锁
select ... for update; # 获取排它锁
```

InnoDB 的行锁是加在索引项上面的，**是给索引在加锁**，并不是给单纯的行记录在加锁，**如果过滤没有索引的话，使用的就是表锁，而不是行锁！**



#### 行级锁

InnoDB存储引擎支持事务处理，表支持行级锁定，并发能力更好。

1. InnoDB行锁是通过给索引上的**索引项**加锁来实现的，而不是给表的行记录加锁实现的，这就意味着**只有通过索引条件检索数据，InnoDB才使用行级锁，否则InnoDB将使用表锁。**

2. 由于InnoDB的行锁实现是针对索引字段添加的锁，不是针对行记录加的锁，因此虽然访问的是 InnoDB引擎下表的不同行，但是如果使用**相同的索引字段作为过滤条件（主要看主键索引树）**，依然会发生锁冲突，只能串行进行，不能并发进行。

3. 即使SQL中使用了索引，但是经过MySQL的优化器后，如果认为全表扫描比使用索引效率更高，此时会放弃使用索引，因此也不会使用行锁，而是使用表锁，**比如对一些很小的表，MySQL就不会去使用索引**。



#### 间隙锁

「可重复读」隔离级别无法彻底解决幻读问题，**「可串行化」隔离级别通过间隙锁可以解决幻读问题**

- 理解“间隙”的含义
- 直到间隙的范围

**等值查询**

- 主键索引：record lock

  ```mysql
  select * from user where id=9; # id 是主键索引（值不可以重复）
  ```

- 辅助索引：record lock + gap lock = next-key lock

  ```mysql
  select * from user where age=18; # age 是辅助索引（值可以重复）
  ```

**范围查询**

- 主键索引：record lock + gap lock = next-key lock

  ```mysql
  select * from user where id > 9; # id 是主键，是自增的
  
  # 此时如果另一个事务 insert 一条记录由于「间隙锁的存在」就会阻塞
  ```

- 辅助索引：record lock + gap lock = next-key lock

  ```mysql
  select * from user where age > 20; # age 是辅助索引
  
  # 此时如果另一个事务 insert 一条记录需要分析「gap 图」
  ```

  

#### MVCC

多版本并发控制，依赖 undo log 实现

undo log 主要作用就是事务发生错误时回滚 rollback 以及提供MVCC 的非锁定读（**快照读**）

- 快照读 (snapshot read) 读的是记录的可见版本，不用加锁。如 select

- 当前读 (current read) 读取的是记录的最新版本，并且当前读返回的记录。如 insert，delete，update，select...lock in share mode/for update

MVCC：每一行记录实际上有多个版本，每个版本的记录除了数据本身之外，增加了其它字段，比较重要的两个字段为：

- DB_TRX_ID:记录当前事务ID
- DB_ROLL_PTR：指向undo log日志上数据的指针

**已提交读**：“每次”执行语句的时候都重新生成一次快照 (Read View)，每次select查询时。

> 为什么无法解决「不可重复读」？
>
> 因为**每一次 select 都会重新产生一次数据快照**，其他事务更新之后而且已提交的数据，可以实时反馈到当前事务的 select 结果中！
>
> 为什么无法解决「幻读」
>
> 因为每一次 select 都会重新产生一次数据快照，其他事务增加了和当前事务查询条件相同的新数据并且成功提交后，就会导致当前事务再次以同样的条件查询时，数据多了！

**可重复读**：同一个事务开始的时候生成一个当前事务全局性的快照 (Read View)，第一次select查询时。

> 为什么解决了「不可重复读」？
>
> 第一次 select 产生数据快照，其他事务虽然修改了最新的数据，但是当前事务 select 时依然查看的是**最初的快照数据**
>
> 为什么部分解决了「幻读」？
>
> B 事务 select 数据产生快照，A 事务插入了一条满足 B 事务过滤条件的数据，A 提交之后 B 事务再次 select 还是原先的数据不会增加 A 事务插入的数据，这看似解决了幻读，但是当 B 事务 update 对应数据之后（**当前读**），再次 select 就会出现新增的数据，也即是幻读，因为**快照读对于当前事务内自己的更新是可以读到的**



#### 死锁

两个事务获取锁的顺序不同导致死锁问题，mysqld 有事务阻塞的超时时间，并且可以**直接能够监测到死锁问题，并且进行事务的回滚**



### 日志

#### 错误日志

错误日志是 MySQL 中最重要的日志之一，它记录了当 mysqld 启动和停止时，以及服务器在运行过程 中发生任何严重错误时的相关信息。当数据库出现任何故障导致无法正常使用时，可以首先查看此日志。

mysqld 使用错误日志名 host_name.err(host_name 为主机名) 并默认在参数 DATADIR(数据目录) 指定的目录中写入日志文件。



#### 查询日志

查询日志记录了客户端的所有语句。由于上线项目sql特别多，开启查询日志IO太多导致MySQL效率 低，只有在调试时才开启，比如通过查看sql发现热点数据进行缓存。

```mysql
mysql> show global variables like "%genera%";
```



#### 二进制日志

二进制日志 (BINLOG) 记录了所有的 **DDL(数据定义语言)语句**和 **DML(数据操纵语言) 语句**，但是不包括数据查询语句。语句以“事件”的形式保存，它描述了数据的更改过程。 此日志对于灾难时的数据恢复起 着极其重要的作用。

- 数据恢复
- 主从复制

```mysql
shell> mysqlbinlog --no-defaults --database=school --base64-output=decode-rows
-v --start-datetime='2021-05-01 00:00:00' --stop-datetime='2021-05-10 00:00:00'
mysql-bin.000001 | more # 解析binlog

mysql> show binary logs; -- 查看 binlog
```



#### 事务日志

##### redo log（物理日志）

重做日志，用于记录事务操作的变化，**确保事务的持久性**。redo log 是在事务开始后就开始记录，不管事务是否提交都会记录下来，在异常发生时(如数据持久化过程中掉电)，InnoDB 会使用 redo log 恢复到掉电前的时刻，保证数据的完整性。

innodb_log_buffer_size 默认是 16M，就是 redo log 缓冲区的大小，**它随着事务开始，就开始写 redo log**，如果事务比较大，为了避免事务执行过程中花费过多磁盘 IO，可以设置比较大的 redo log 缓存，节省磁盘IO。

InnoDB 修改操作数据，**不是直接修改磁盘上的数据，实际只是修改 Buffer Pool 中的数据**。InnoDB总是先把Buffer Pool 中的数据改变记录到 redo log中，用来进行崩溃后的数据恢复。 **优先记录 redo log，然后再慢慢的将 Buffer Pool 中的脏数据刷新到磁盘上。**

![redoLog.png](https://s2.loli.net/2023/08/20/7m8Clp9Khesu6W1.png)

##### undo log（逻辑日志）

回滚日志，保存了事务发生之前的数据的一个版本，用于事务执行时的回滚操作，同时也是 实现多版本并发控制 (MVCC) 下读操作的关键技术。

- DB_TRX_ID：事务ID

- DB_ROLL_PTR：回滚指针



#### 数据备份

```mysql
mysqldump -u root -p --all-databases > ~/all.sql
mysqldump -u root -p --databases school > ~/school.sql
mysqldump -u root -p school user > ~/user.sql
```



### MySQL 集群

#### 主从复制

![主从复制.png](https://s2.loli.net/2023/08/20/rR4dHuTsobtfayV.png)

主从复制的流程:

两个日志 (binlog二进制日志&relay log日志) 和三个线程 (master的一个线程和 slave的二个线程)

1. 主库的更新操作写入 binlog 二进制日志中
2. master 服务器创建一个 **binlog 转储线程**，将二进制日志内容发送到从服务器
3. slave 机器执行 START SLAVE 命令会在从服务器创建一个 **IO 线程**，接收 master 的 binary log 复制到其中继日志。首先 slave 开始一个工作线程(I/O线程)，I/O线程在master上打开一个普通的连接，然后开始 binlog dump process，binlog dump process 从 master 的二进制日志中读取事件，如果已经跟上 master，它会睡眠并等待 master 产生新的事件，I/O线程将这些事件写入中继日志
4. **sql slave thread** (sql从线程) 处理该过程的最后一步，sql 线程从中继日志中读取事件，并重放其中的事件而更新 slave 机器的数据，使其与 master 的数据一致。只要该线程与 I/O 线程保持一致，中继日志通常会位于 os 缓存中，所以中继日志的开销很小



#### 读写分离

![读写分离.png](https://s2.loli.net/2023/08/20/8HMijkNga6qvZhp.png)

一般引入中间件 Mycat 来实现，JDK 1.7 版本以上

**配置文件**

- server.xml: 配置登录Mycat的账号信息
- schema.xml: 配置逻辑库和数据源、续写分离、分库分表信息等

**重要参数**

- balance
  - "0": 不开启读写分离
  - "1": 全部的 readHost 和 stand by writeHost 参与 select 语句的负载
  - "2": 所有读操作随机在 readHost 和 writeHost 上分发 
  - "3": 所有读请求随机分发到 writeHost 对应的 readHost 上执行

- writeType="0": 所有写操作发送到配置的第一个writeHost，第一个挂掉切换到还生存的第二个 writeHost

- switchType
  - "-1": 不自动切换
  - "1": 自动切换，根据心跳 select user() 
  - "2": 基于 MySQL 的主从同步状态决定是否进行切换 show slave status

**端口**

- 数据端口 8066
- 管理端口 9066



#### 分库分表

- 垂直切分：分不同的数据库处理，Mycat 配置可以两个逻辑库
- 水平切分：分不同的表处理数据，Mycat 可以按照设置的 rule 配置 insert 到两个不同节点的表中



### 扩展

#### 处理流程

- **连接器**: 管理连接，权限验证
- **解析器**: 词法以及语法分析 
- **优化器**: 生成执行计划，选择合适索引 
- **执行器**: 操作引擎获取结果 
- **存储引擎**: 存储数据，提供读写接口

![Server-Engine](https://cdn.xiaolincoding.com/gh/xiaolincoder/mysql/sql%E6%89%A7%E8%A1%8C%E8%BF%87%E7%A8%8B/mysql%E6%9F%A5%E8%AF%A2%E6%B5%81%E7%A8%8B.png)
