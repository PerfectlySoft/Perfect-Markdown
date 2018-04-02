# Perfect-Markdown [English](README.md)

<p align="center">
    <a href="http://perfect.org/get-involved.html" target="_blank">
        <img src="http://perfect.org/assets/github/perfect_github_2_0_0.jpg" alt="Get Involed with Perfect!" width="854" />
    </a>
</p>

<p align="center">
    <a href="https://github.com/PerfectlySoft/Perfect" target="_blank">
        <img src="http://www.perfect.org/github/Perfect_GH_button_1_Star.jpg" alt="Star Perfect On Github" />
    </a>  
    <a href="https://gitter.im/PerfectlySoft/Perfect" target="_blank">
        <img src="http://www.perfect.org/github/Perfect_GH_button_2_Git.jpg" alt="Chat on Gitter" />
    </a>  
    <a href="https://twitter.com/perfectlysoft" target="_blank">
        <img src="http://www.perfect.org/github/Perfect_GH_button_3_twit.jpg" alt="Follow Perfect on Twitter" />
    </a>  
    <a href="http://perfect.ly" target="_blank">
        <img src="http://www.perfect.org/github/Perfect_GH_button_4_slack.jpg" alt="Join the Perfect Slack" />
    </a>
</p>

<p align="center">
    <a href="https://developer.apple.com/swift/" target="_blank">
        <img src="https://img.shields.io/badge/Swift-4.1-orange.svg?style=flat" alt="Swift 4.1">
    </a>
    <a href="https://developer.apple.com/swift/" target="_blank">
        <img src="https://img.shields.io/badge/Platforms-OS%20X%20%7C%20Linux%20-lightgray.svg?style=flat" alt="Platforms OS X | Linux">
    </a>
    <a href="http://perfect.org/licensing.html" target="_blank">
        <img src="https://img.shields.io/badge/License-Apache-lightgrey.svg?style=flat" alt="License Apache">
    </a>
    <a href="http://twitter.com/PerfectlySoft" target="_blank">
        <img src="https://img.shields.io/badge/Twitter-@PerfectlySoft-blue.svg?style=flat" alt="PerfectlySoft Twitter">
    </a>
    <a href="https://gitter.im/PerfectlySoft/Perfect?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge" target="_blank">
        <img src="https://img.shields.io/badge/Gitter-Join%20Chat-brightgreen.svg" alt="Join the chat at https://gitter.im/PerfectlySoft/Perfect">
    </a>
    <a href="http://perfect.ly" target="_blank">
        <img src="http://perfect.ly/badge.svg" alt="Slack Status">
    </a>
</p>


该项目提供了在Swift中直接从Markdown文本生成HTML的方法

该软件使用SPM进行编译和测试，本软件也是[Perfect](https://github.com/PerfectlySoft/Perfect)项目的一部分，但也可以独立使用。

请确保您已经安装并激活了最新版本的 Swift tool chain 工具链。

## 致谢

Perfect-Markdown 直接基于  [GerHobbelt 的 "upskirt（超短裙）"](https://github.com/GerHobbelt/upskirt) 项目.


## 使用说明

请首先修改您的 Package.swift 文件增加依存关系：

``` swift
.package(url: "https://github.com/PerfectlySoft/Perfect-Markdown.git", 
	from: "3.0.0")

// on target section:
.target(
            // name: "your project name",
            dependencies: ["PerfectMarkdown"]),
```

## 引用库函数

请将下列头文件增加到源代码

``` swift
import PerfectMarkdown
```

## 从 Markdown 文本中获取 HTML 字符串

一旦引用成功，String 类型会增加一个名为 `markdownToHTML` 的扩展属性:

```
let markdown = "# 这是一个 markdown 文档 \n\n## with mojo 🇨🇳 🇨🇦"

guard let html = markdown.markdownToHTML else {
  // 转换失败
}//end guard

print(html)
```

## 更多信息
关于本项目更多内容，请参考[perfect.org](http://perfect.org).
