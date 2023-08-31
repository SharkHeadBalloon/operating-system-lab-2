# operating-system-lab-2
  生产者消费者问题，北京理工大学计算机学院大三操作系统课程实验。
## 实验内容
  在Windows系统下实现生产者-消费者问题。实验步骤如下：
(1) 创建一个包含6个缓冲区的缓冲池，初始为空，每个缓冲区能存放一个长度为10个字符的字符串。
(2) 创建2个生产者进程，要求每个生产者进程：
随机等待一段时间后，向缓冲区中添加数据；
若缓冲区已满，则等待消费者取走数据后再继续添加数据；
重复添加数据12次。
(3) 创建3个消费者进程，要求每个消费者进程：
随机等待一段时间后，从缓冲区中读取数据；
若缓冲区为空，则等待生产者添加数据后再继续读取数据；
重复读取数据8次。
(4) 输出显示每次添加或读取数据的时间，以及缓冲区的映像。
实验使用的语言为C艹，使用Windows.h库中的函数CreateSemaphore、CreateMutex建立信号量，使用函数WaitFOrSingleObject进行P操作，使用函数ReleaseMutex、ReleaseSemaphore实现V操作。
在缓冲区方面，通过创建可共享的文件映射作为共享内存，用于放置缓冲区。通过CreateFileMapping函数创建文件映射，通过MapViewOfFile函数将文件映射对象映射到当前程序的内存空间，通过OpenFileMapping打开已存在的文件映射。
在多进程方面，通过CreateProcess函数创建子进程。
程序的主进程位于main.exe，由main.cpp实现，主进程负责创建缓冲区、创建5个子进程等工作；生产者进程位于producer.exe，由producer.cpp实现。消费者进程位于consumer.exe，由consumer.cpp实现。

## 运行方式
  首先生成三个可执行程序。如果已经在VSCode上安装CMake插件或者安装了CMake，可以直接用项目中提供的CMake提供的CMakelist生成三个可执行程序。当然也可以不用CMake而是手动编译生成，但是注意在编译时要与os_2.h和os_2.cpp链接。
  生成完可执行程序后，只需在控制台中运行main.exe即可。

## 运行效果
![image](https://github.com/SharkHeadBalloon/operating-system-lab-2/assets/106319321/976fa731-e60b-4669-9d2f-b0ed2db17cb3)

![image](https://github.com/SharkHeadBalloon/operating-system-lab-2/assets/106319321/e562b8b6-98be-4d3b-991f-a91badc10101)

## 参考资料
  忘了，貌似只有https://learn.microsoft.com/zh-cn/
