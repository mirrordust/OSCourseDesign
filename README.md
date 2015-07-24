### 小组成员
| 学号     | 姓名   | 班级号 |
| :------  | :----  | :----- |
| 1352921  | 廖山河 |42028701|
| 1352922  | 夏浩茗 |42028701|
| 1352906  | 李星辰 |42028701|


### Git托管地址
https://github.com/mirrordust/OSCourseDesign

### 项目说明
项目对文件系统部分的源码进行了增改，具体修改为增加了lseek, stat, dup, pwread和pwrite几个文件调用，以及将源码的扁平结构改为多级目录结构。
运行方法：Ubuntu环境在src目录下打开终端，输入make image，再输入bochs -f bochsrc。