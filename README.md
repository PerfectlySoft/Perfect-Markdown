# Perfect-Markdown [简体中文](README.zh_CN.md)

<p align="center">
    <a href="http://perfect.org/get-involved.html" target="_blank">
        <img src="http://perfect.org/assets/github/perfect_github_2_0_0.jpg" alt="Get Involed with Perfect!" width="854" />
    </a>
</p>

<p align="center">
    <a href="https://github.com/PerfectlySoft/Perfect" target="_blank">
        <img src="http://www.perfect.org/github/Perfect_GH_button_1_Star.jpg" alt="Star Perfect On Github" />
    </a>  
    <a href="http://stackoverflow.com/questions/tagged/perfect" target="_blank">
        <img src="http://www.perfect.org/github/perfect_gh_button_2_SO.jpg" alt="Stack Overflow" />
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
        <img src="https://img.shields.io/badge/Swift-4.0-orange.svg?style=flat" alt="Swift 4.0">
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
    <a href="http://perfect.ly" target="_blank">
        <img src="http://perfect.ly/badge.svg" alt="Slack Status">
    </a>
</p>



This project provides a solution to convert markdown text into html presentations.

This package builds with Swift Package Manager and is part of the [Perfect](https://github.com/PerfectlySoft/Perfect) project but can also be used as an independent module.

## Acknowledgement

Perfect-Markdown is directly building on  [GerHobbelt's "upskirt"](https://github.com/GerHobbelt/upskirt) project.


## Swift Package Manager

Add dependencies to your Package.swift

``` swift
.package(url: "https://github.com/PerfectlySoft/Perfect-Markdown.git", 
	from: "3.0.0")

// on target section:
.target(
            // name: "your project name",
            dependencies: ["PerfectMarkdown"]),
```

## Import Perfect Markdown Library

Add the following header to your swift source code:

``` swift
import PerfectMarkdown
```

## Get HTML from Markdown Text

Once imported, a new String extension `markdownToHTML` would be available:

```
let markdown = "# some blah blah blah markdown text \n\n## with mojo 🇨🇳 🇨🇦"

guard let html = markdown.markdownToHTML else {
  // conversion failed
}//end guard

print(html)
```

## Further Information
For more information on the Perfect project, please visit [perfect.org](http://perfect.org).
