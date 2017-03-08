import XCTest
@testable import PerfectMarkdown

class PerfectMarkdownTests: XCTestCase {
    func testExample() {
      let markdown = "# Swift调用C语言自建函数库的方法\n\n本程序示范了如何用Swift调用自定义C语言模块的方法。您可以直接下载本程序，或者按照以下教程逐步完成。\n\n## 快速上手\n\n本程序需要Swift 3.0以上版本。\n\n### 下载、编译和测试\n\n```\n$ git clone https://github.com/RockfordWei/CSwift.git\n$ cd CSwift\n$ swift build\n$ swift test\n```\n\n### Module Map\n\n下一步是修理一下目标的模块映射表。请把module.modulemap修改为如下程序：\n\n``` swift\nmodule CSwift [system] {\n\theader \"CSwift.h\"\n\tlink \"CSwift\"\n\texport *\n}\n```\n\n## 其他\n\nNumber|Name|Date\n------|----|----\n1|CSwift|Mar 7, 2017\n2|SunDown文本处理器|2017年3月7日\n\n"
      guard let html = markdown.markdownToHTML else {
        XCTFail("fault")
        return
      }//end guard
      print(html)
    }


    static var allTests : [(String, (PerfectMarkdownTests) -> () throws -> Void)] {
        return [
            ("testExample", testExample),
        ]
    }
}
