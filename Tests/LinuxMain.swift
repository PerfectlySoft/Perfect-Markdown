import XCTest

#if swift(>=5.0)
import PerfectMarkdownTests

var tests = [XCTestCaseEntry]()
tests += PerfectMarkdownTests.__allTests()

XCTMain(tests)
#else
@testable import PerfectMarkdownTests
XCTMain([
     testCase(PerfectMarkdownTests.allTests),
])
#endif
