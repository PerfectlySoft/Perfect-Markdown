import XCTest
@testable import PerfectMarkdown

class PerfectMarkdownTests: XCTestCase {
  func testTitle () {
    XCTAssertEqual(
      "# 标题1\n## 标题2\n### 标题3\n".markdownToHTML ?? "",
      "<h1>标题1</h1>\n\n<h2>标题2</h2>\n\n<h3>标题3</h3>\n"
    )
  }
  func testList () {
    XCTAssertEqual(
      "- 列表1\n- 列表2\n- 列表3\n\n".markdownToHTML ?? "",
      "<ul>\n<li>列表1</li>\n<li>列表2</li>\n<li>列表3</li>\n</ul>\n"
    )
  }
  func testNumbers () {
    XCTAssertEqual(
      "1. 清单1\n2. 清单2\n3. 清单3\n\n".markdownToHTML ?? "",
      "<ol>\n<li value=1>清单1</li>\n<li value=2>清单2</li>\n<li value=3>清单3</li>\n</ol>\n"
    )
  }
  func testInlineCode () {
    XCTAssertEqual(
      "`let x = \"🇨🇳🇨🇦\"`\n".markdownToHTML ?? "",
      "<p><code>let x = &quot;🇨🇳🇨🇦&quot;</code></p>\n"
    )
  }
  func testCodes () {
    XCTAssertEqual(
      "```\nlet x = \"🇨🇳🇨🇦\"\nlet y = 100\n```\n".markdownToHTML ?? "",
      "<pre><code>let x = &quot;🇨🇳🇨🇦&quot;\nlet y = 100\n</code></pre>\n"
    )
  }
  func testLink () {
    XCTAssertEqual(
      "[Perfect 官网](http://www.perfect.org)\n".markdownToHTML ?? "",
      "<p><a href=\"http://www.perfect.org\">Perfect 官网</a></p>\n"
    )
  }
  func testTable() {
      let a = "Number|Name|Date\n------|----|----\n1|CSwift|Mar 7, 2017\n2|SunDown文本处理器|2017年3月7日\n\n".markdownToHTML ?? ""
      let b = "<table><thead>\n<tr>\n<th>Number</th>\n<th>Name</th>\n<th>Date</th>\n</tr>\n</thead><tbody>\n<tr>\n<td>1</td>\n" +
      "<td>CSwift</td>\n<td>Mar 7, 2017</td>\n</tr>\n<tr>\n<td>2</td>\n<td>SunDown文本处理器</td>\n<td>2017年3月7日</td>\n</tr>\n</tbody></table>\n"
      XCTAssertEqual( a, b)
  }

    static var allTests : [(String, (PerfectMarkdownTests) -> () throws -> Void)] {
        return [
            ("testTitle", testTitle),
            ("testList", testList),
            ("testNumbers", testNumbers),
            ("testInlineCode", testInlineCode),
            ("testCodes", testCodes),
            ("testLink", testLink),
            ("testTable", testTable)
        ]
    }
}
