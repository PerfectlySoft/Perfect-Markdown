import XCTest
import PerfectMarkdown

class MarkdownExtensionTests: XCTestCase {

  func assertWithOption(
    option: MarkdownExtensionOptions,
    withoutOptions: MarkdownExtensionOptions = .none,
    markdownString: String,
    withOptionString: String,
    withoutOptionString: String
  ) {

    markdownExtensionOptions = option

    XCTAssertEqual(
      markdownString.markdownToHTML ?? "",
      withOptionString + "\n"
    )

    markdownExtensionOptions = withoutOptions

    XCTAssertEqual(
      markdownString.markdownToHTML ?? "",
      withoutOptionString + "\n"
    )
  }

  func testNoIntraEmphasis() {
    assertWithOption(
      option: .noIntraEmphasis,
      markdownString:      "intra_emphasis_word",
      withOptionString:    "<p>intra_emphasis_word</p>",
      withoutOptionString: "<p>intra<em>emphasis</em>word</p>"
    )
  }

  func testTables() {
    assertWithOption(
      option: .tables,
      markdownString:      "Table|Header\n-----|-----\nRow|1",
      withOptionString:    "<table><thead>\n<tr>\n<th>Table</th>\n<th>Header</th>\n</tr>\n</thead><tbody>\n<tr>\n<td>Row</td>\n<td>1</td>\n</tr>\n</tbody></table>",
      withoutOptionString: "<p>Table|Header\n-----|-----\nRow|1</p>"
    )
  }

  func testFencedCode() {
    assertWithOption(
      option: .fencedCode,
      markdownString: "```\nlet this = some code...\n```",
      withOptionString:    "<pre><code>let this = some code...\n</code></pre>",
      withoutOptionString: "<p><code>\nlet this = some code...\n</code></p>"
    )
  }

  func testAutolink() {
    assertWithOption(
      option: .autolink,
      markdownString:      "https://www.perfect.org",
      withOptionString:    "<p><a href=\"https://www.perfect.org\">https://www.perfect.org</a></p>",
      withoutOptionString: "<p>https://www.perfect.org</p>"
    )
  }

  func testStrikethrough() {
    assertWithOption(
      option: .strikethrough,
      markdownString:      "~~insertion~~",
      withOptionString:    "<p><del>insertion</del></p>",
      withoutOptionString: "<p>~~insertion~~</p>"
    )
  }

  func testInsertion() {
    assertWithOption(
      option: .insertion,
      markdownString:      "++insertion++",
      withOptionString:    "<p><ins>insertion</ins></p>",
      withoutOptionString: "<p>++insertion++</p>"
    )
  }

  func testSpaceHeaders() {
    assertWithOption(
      option: .strictHeaderSpacing,
      markdownString:      "#Header",
      withOptionString:    "<p>#Header</p>",
      withoutOptionString: "<h1>Header</h1>"
    )
  }

  func testSuperscript() {
    assertWithOption(
      option: .superscript,
      markdownString:      "Text^(superscript)",
      withOptionString:    "<p>Text<sup>superscript</sup></p>",
      withoutOptionString: "<p>Text^(superscript)</p>"
    )
  }

  func testLaxSpacing() {
    assertWithOption(
      option: .laxSpacing,
      markdownString:      "Lax\n<p>spacing</p>\nblock",
      withOptionString:    "<p>Lax</p>\n\n<p>spacing</p>\n\n<p>block</p>",
      withoutOptionString: "<p>Lax\n<p>spacing</p>\nblock</p>"
    )
  }

  func testEmailFriendly() {
    assertWithOption(
      option: .emailFriendly,
      markdownString:      "Email\n> friendly\nblockquotes",
      withOptionString:    "<p>Email</p>\n\n<blockquote>\n<p>friendly</p>\n</blockquote>\n\n<p>blockquotes</p>",
      withoutOptionString: "<p>Email</p>\n\n<blockquote>\n<p>friendly\nblockquotes</p>\n</blockquote>"
    )
  }

}
