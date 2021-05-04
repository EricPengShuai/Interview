## set VS map

- 底层都是**红黑树**，插入、删除、查询操作都在O(logn)时间内完成
- 内部都是自动排序的
- **set**中的元素都是**value**，**map**中的元素是**value-key键值对**，map根据key内部排序
- **set**不能包含两个相同的元素，**map**如果插入已经存在的value的key会进行覆盖

> set底层：https://blog.csdn.net/xiamentingtao/article/details/55517304
>
> map底层：https://blog.csdn.net/qq_21989927/article/details/108068998



## unordered_set VS unordered_map

- 底层都是**哈希表**
- 内部是无序的
- 如果需要内部元素自动排序，使用map，不需要排序使用unordered_map

> 参考：https://blog.csdn.net/qq_30815237/article/details/91047041