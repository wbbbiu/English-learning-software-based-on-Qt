# English learning software based on Qt
 # 四级英语学习App - 基于Qt与链式学习

这是一个基于Qt框架开发的英语四级词汇学习应用程序，采用了链式学习法。通过将单词与例句关联，不仅帮助用户记忆单词，还能通过复习包含多个单词的例句来巩固所学内容。例如，在学习“email”时，用户会遇到例句：“Please email the documents to me.” 这个句子中的“document”同样是四级词汇，自然成为下一个学习目标。

## 项目结构

### 数据处理模块 (GetData)
- **功能**：获取并处理学习所需的数据库内容。
- **数据源**：解析`CET4_3.json`格式的四级单词数据（[更多资源](https://github.com/kajweb/dict)）。
- **原理**：创建一个`QMap<QString, int>`存储单词及其存在状态（1表示存在）。根据词性推演所有可能形式，转换并分割例句为单个单词，检查这些单词是否存在于WordMap中，并将相关联的句子与单词存储到数据库的Relation表中，以生成填空题的基础内容。
- **使用方法**：使用Qt Creator打开并运行该部分代码，查看应用程序输出窗口确认数据库连接成功的信息，其中会显示处理好的数据库路径。

### 用户界面与业务逻辑模块 (CET4_QML)
- **技术栈**：Qt框架结合C++实现后端业务逻辑，前端使用QML制作，相比传统的QWidget，提供更强的互动性和更美观的界面。
- **关键组件**：
  - `main.cpp`：程序入口点。
  - `datastorage.cpp`：负责数据库操作及数据处理，向QML提供可调用函数。
  - `ac_signal.cpp`：存放交流信号，供QML使用。
  - `mainwindow.cpp`：将需要在QML中使用的类注入到QML上下文中，使得QML可以通过注入对象名调用相应类的方法。

## 使用指南
1. 确保已安装Qt环境。
2. 打开Qt Creator，加载项目。
3. 分别运行`GetData`和`CET4_QML`两个模块，开始体验四级英语单词的学习之旅。
   
# 项目结果展示
## 学习功能
学习功能的话第一阶段的掌握会代表这个单词你认为自己认识，就不在展开学习，选择学习的话就是进行接下来几个阶段的学习
<iframe src="//player.bilibili.com/player.html?bvid=BV1EcXQYUEM9&page=1" scrolling="no" border="0" frameborder="no" framespacing="0" allowfullscreen="true"> </iframe>

对于复习功能的话也是类似不过会先判断你当前对这个单词的掌握在不同程度复习。
感谢[kajweb/dict](https://github.com/kajweb/dict)提供的数据支持。
