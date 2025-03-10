# 英语学习软件（基于Qt）

## 项目概述

本项目旨在开发一个英语学习软件，主要功能包括数据解析、单词推演和句子与四级单词的关联处理。

## 功能模块

### 1. 数据解析

- 解析JSON文件中的各个数据。
- 提取并处理相关的语言学数据以便后续分析。

### 2. 单词推演

#### 2.1 名词推演

根据名词是否为不规则变化及其结尾字符的不同情况（如以s, x, z, o, ch, sh结尾，或以y, f, fe结尾等），生成复数形式或其他变形，并存入`QMap<QString, int> WordMap`中。

#### 2./XMLSchema 推演

首先检查是否为不规则变化。如果是，则直接获取其过去式、过去分词、现在分词及第三人称单数形式并存入`WordMap`。对于规则动词，根据单词结尾字符（如以e, y, 双辅音字母结尾等）生成相应形式，并存入`WordMap`。

#### 2.3 形容词推演

涉及比较级和最高级的形成。根据单词结尾字符（如以e结尾，或以辅音字母结尾等），生成比较级和最高级形式，并存入`WordMap`。

- 将每个单词的所有形态添加到扩展列表`expand`中。

### 3. 句子与四级单词关联

- 将例句转换为小写并分割成单个单词。
- 使用`WordMap`检查这些单词是否存在。如果存在，则认为该单词是四级词汇，并将句子与单词关联存储到数据库的`Relation`表中。
- 从例句中找出与`expand`列表匹配的部分，作为填空题的基础内容。

## 使用方法

1. 克隆仓库到本地：
   ```bash
   git clone https://github.com/yourusername/yourrepository.git
