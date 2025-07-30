## 扫盲

C++ 后台开发面试时一般考察什么？ - 果冻虾仁的回答 - 知乎
https://www.zhihu.com/question/34574154/answer/95492549


【Apache是什么？核心原理和作用是什么？八分钟快速了解Apache一键部署网站！（附课件）】 https://www.bilibili.com/video/BV1kdKFziEuZ/?share_source=copy_web&vd_source=b2f3d4c4955b64c09967b0bf54faf9fe


【什么是nginx？两分钟给你讲明白】 https://www.bilibili.com/video/BV1cFPceqEhL/?share_source=copy_web&vd_source=b2f3d4c4955b64c09967b0bf54faf9fe

[关于CGI和FastCGI的理解 
](https://www.cnblogs.com/tssc/p/10255590.html)

### **一、后台开发的概念澄清**
1. **常见误解**  
   很多人将“后台开发”等同于“Web后台”（Java/PHP等框架开发），但实际C++后台开发主要指Linux系统级的服务端编程，二者是包含关系。
2. **技术栈差异**  
   - **Web后台**：聚焦HTTP协议，处理前端请求（如Java/PHP）。  
   - **C++大后台**：聚焦TCP协议、Socket通信、分布式系统内部服务调用（如游戏服务器、中间件等）。  
   - *补充*：C++也可通过# HTTP Server with CGI Demo



### **二、C++后台开发的核心技能**
#### **1. Linux系统基础**
- **基本操作**：常用命令（grep/sed/awk）、Shell脚本编写、日志分析工具。  
- **环境编程**（重点）：  
  - 熟悉POSIX API（文件IO、进程/线程管理、信号量）。  
  - 掌握Socket编程（TCP/UDP通信流程）、IO多路复用（select/poll/epoll）。  

#### **2. 操作系统原理**
- **进程与线程**：fork()与线程创建、同步机制（互斥锁/条件变量）、进程间通信（管道/共享内存/消息队列）。  
- **内存管理**：malloc/new的区别（可延伸至内存碎片、BSS段等）、虚拟内存机制。  
- **性能调优**：上下文切换成本、零拷贝技术等。

#### **3. 网络编程**
- **核心模型**：Reactor/Proactor、多线程/协程并发。  
- **协议基础**：TCP粘包处理、HTTP协议（GET/POST、Cookie/Session）。  

#### **4. 数据库**
- **SQL与优化**：索引原理（B+树）、执行计划分析。  
- **MySQL引擎**：InnoDB vs MyISAM、事务隔离级别。  

#### **5. 数据结构与算法**
- 必考内容（通用所有岗位）：二叉树、排序算法、动态规划等。  

---

### **三、面试策略**
1. **主动输出知识**  
   - 例如：回答“malloc vs new”时，可延伸讨论内存池设计、C++的placement new。  
2. **结合系统原理**  
   - 任何C++问题均可关联操作系统（如STL容器如何管理内存）。  
3. **项目经验**  
   - 突出网络编程、高并发场景的实践经验（如手写线程池、Epoll服务器）。  

---

### **四、补充说明**
- **应届生重点**：基础扎实比框架经验更重要，尤其是系统级理解。  
- **C++优势**：竞争较少，适合深耕底层/高性能场景（如金融、游戏）。  
- **学习资源推荐**：  
  - 《UNIX环境高级编程》（APUE）  
  - 《深入理解计算机系统》（CSAPP）  
  - 《Linux多线程服务端编程》（陈硕）  

--- 


