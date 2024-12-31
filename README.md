# IRViewer - 和os没什么关系的os大作业

本项目可以分析linux kernel的llvm IR，并对给定的函数绘制控制流图。绘制的控制流图支持点击显示IR源码、拖拽。

# 技术特点

- 本项目使用[这个项目](https://github.com/ZHYfeng/Generate_Linux_Kernel_Bitcode)生成内核的llvm IR文件
- 输入函数点击查询，程序遍历llvm控制流图，生成由QGraphicsItem组成的新图，交由Qt Graphics System进行可视化，并实现拖拽功能
- 由于分析IR时间较长，处理IR的过程均在另一个线程中完成，与主线程采用信号槽方式进行通信
- 所有使用new分配的内存或者交由智能指针管理，或者交由Qt元对象系统

# 技术细节

本项目主要包含两个类：IRViewer与IRParser。IRViewer继承自QMainWindow，管理窗口与Qt Graphics System；IRParser继承自QThread，负责在后台处理llvm Module。

对生成的控制流图，定义了一系列数据结构：BlockNode、BlockEdge均继承自QGraphicsObject，分别表示控制流图的基本块、边。通过重写Paint方法与点击、拖拽事件，实现只需加入到QGraphicsScene中便可显示、交互。ControlFlowGraph是对整个控制流图的包装，构造时传入函数即自动建图。

# 不足

- 由于内核IR过于庞大，导致程序启动和生成图速度较慢。可以考虑通过ctags定位到单个文件后再进行查找
- 由于时间关系，未对图的生成进行进一步优化，导致可读性差，比较杂乱

# 编译方法

```
make build
cd build
cmake ..
make
```

将built-in.bc放在可执行程序同一目录下即可启动。
