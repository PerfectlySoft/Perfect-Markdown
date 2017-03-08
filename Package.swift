import PackageDescription

let package = Package(
    name: "PerfectMarkdown",
    targets: [
      Target(name: "upskirt", dependencies: []),
      Target(name: "PerfectMarkdown", dependencies: ["upskirt"])
    ]
)
