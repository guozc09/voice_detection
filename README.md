<!--
 * @Description: readme.md
 * @version: 1.0
 * @Author: Zhc Guo
 * @Date: 2020-06-18 09:39:34
 * @LastEditors: Zhc Guo
 * @LastEditTime: 2020-06-24 11:25:06
 -->

# voice detection

## 功能说明

- 关键词识别（Keyword Detection Mode）
- 语音触发（Voice Activity Detection Mode, VAD）

## 目录结构

``` CXX

| voice_detection
|-- include
|-- keyword_detection
|   |-- kwd
|   |   |-- include
|   |   |-- src
|   |   |-- test
|-- voice_activity_detection
|   |-- vad
|   |   |-- include
|   |   |-- src
|   |   |-- test
|-- doc
|-- README.md

```

## 构建

1. cd voice_detection
2. ./configure --help
3. ./configure --host=arm-none-linux-gnueabi --prefix=prefix_path --with-system-lib-path=lib_path --with-system-include-path=include_path
4. 根据实际情况修改 3 中的 prefix_path(工程安装路径)，lib_path(依赖的第三方库路径)，include_path(依赖的第三方头文件路径)
5. make all
6. make install

## Keyword Detection Mode

- 对应路径在 keyword_detection 下 .cpp .h 为外部直接调用实现体。
- kwd/include 存放 header 档。
- kwd/src 存放 kwd 算法源码。
- kwd/test 存放Demo测试档。

## Voice Activity Detection Mode

- 对应路径在 voice_activity_detection 下 .cpp .h 为外部直接调用实现体。
- vad/include 存放 header 档。
- vad/src 存放 vad 算法源码。
- vad/test 存放Demo测试档。
