import XCTest
import PerfectMarkdown

/**
 *  Contains tests covering issues reported at https://github.com/PerfectlySoft/Perfect-Markdown and their fixes.
 */
class BugFixTests: XCTestCase {

  /// #3: Crash when calling `.markdownToHTML` on an empty string
  func testIssue3() {
    let string = ""
    let html = string.markdownToHTML

    XCTAssertEqual(html, "")
  }

}
