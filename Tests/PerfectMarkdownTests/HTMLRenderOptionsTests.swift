import XCTest
import PerfectMarkdown

#if swift(>=5.0)
class HTMLRenderOptionsTests: XCTestCase {

  func assertWithOption(
    option: HTMLRenderOptions,
    withoutOptions: HTMLRenderOptions = .none,
    markdownString: String,
    withOptionString: String,
    withoutOptionString: String
  ) {

    markdownHTMLRenderOptions = option

    XCTAssertEqual(
      markdownString.renderMarkdown(renderOptions: option) ?? "",
      withOptionString + "\n"
    )

    markdownHTMLRenderOptions = withoutOptions

    XCTAssertEqual(
      markdownString.renderMarkdown(renderOptions: withoutOptions) ?? "",
      withoutOptionString + "\n"
    )
  }

  func testSkipHTML() {
    assertWithOption(
      option: .skipHTML,
      markdownString: "Skipped: <i>Skipped</i>",
      withOptionString: "<p>Skipped: Skipped</p>",
      withoutOptionString: "<p>Skipped: <i>Skipped</i></p>"
    )
  }

  func testSkipStyle() {
    assertWithOption(
      option: .skipStyle,
      markdownString: "Style: <style>Skipped</style>",
      withOptionString: "<p>Style: Skipped</p>",
      withoutOptionString: "<p>Style: <style>Skipped</style></p>"
    )
  }

  func testSkipImages() {
    assertWithOption(
      option: .skipImages,
      markdownString: "Image: <img src='example.jpeg'>",
      withOptionString: "<p>Image: </p>",
      withoutOptionString: "<p>Image: <img src='example.jpeg'></p>"
    )
  }

  func testSkipLinks() {
    assertWithOption(
      option: .skipLinks,
      markdownString: "[Link](https://www.perfect.org)",
      withOptionString: "<p>[Link](https://www.perfect.org)</p>",
      withoutOptionString: "<p><a href=\"https://www.perfect.org\">Link</a></p>"
    )
  }

  func testSafeLinks() {
    assertWithOption(
      option: .safeLinks,
      markdownString: "[Link](unknown:example.org)",
      withOptionString: "<p>[Link](unknown:example.org)</p>",
      withoutOptionString: "<p><a href=\"unknown:example.org\">Link</a></p>"
    )
  }

  func testTableOfContents() {
    assertWithOption(
      option: .tableOfContents,
      markdownString: "# Header",
      withOptionString: "<h1 id=\"toc_0\">Header</h1>",
      withoutOptionString: "<h1>Header</h1>"
    )
  }

  func testHardWrap() {
    assertWithOption(
      option: .hardWrap,
      markdownString: "Hard\nwrap",
      withOptionString: "<p>Hard<br>\nwrap</p>",
      withoutOptionString: "<p>Hard\nwrap</p>"
    )
  }

  func testUseXHTML() {
    assertWithOption(
      option: .useXHTML,
      markdownString: "----",
      withOptionString: "<hr/>",
      withoutOptionString: "<hr>"
    )
  }

  func testEscapeHTML() {
    assertWithOption(
      option: .escapeHTML,
      markdownString: "<p>Skipped</p>",
      withOptionString: "<p>&lt;p&gt;Skipped&lt;/p&gt;</p>",
      withoutOptionString: "<p>Skipped</p>"
    )
  }

  func testCreateOutline() {
    assertWithOption(
      option: .createOutline,
      markdownString: "# Header",
      withOptionString: "<section class=\"section1\">\n<h1>Header</h1>\n\n</section>",
      withoutOptionString: "<h1>Header</h1>"
    )
  }

}
#endif
