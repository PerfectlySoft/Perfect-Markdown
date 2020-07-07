// swift-tools-version:5.2
// The swift-tools-version declares the minimum version of Swift required to build this package.

import PackageDescription

let package = Package(
    name: "PerfectMarkdown",
    products: [
        .library(
            name: "PerfectMarkdown",
            targets: ["PerfectMarkdown"]),
    ],
    dependencies: [
        // Dependencies declare other packages that this package depends on.
        // .package(url: /* package url */, from: "1.0.0"),
    ],
    targets: [
      .target(name: "upskirt",dependencies: []),
      .target(name: "PerfectMarkdown", dependencies: ["upskirt"]),
      .testTarget(name: "PerfectMarkdownTests", dependencies: ["PerfectMarkdown"])
    ]
)
