//
//  PerfectMarkdown.swift
//  Perfect-Markdown
//
//  Created by Rockford Wei on 2017-03-08.
//  Copyright © 2017 PerfectlySoft. All rights reserved.
//
//===----------------------------------------------------------------------===//
//
// This source file is part of the Perfect.org open source project
//
// Copyright (c) 2017 - 2018 PerfectlySoft Inc. and the Perfect project authors
// Licensed under Apache License v2.0
//
// See http://perfect.org/licensing.html for license information
//
//===----------------------------------------------------------------------===//
//

#if os(Linux)
import Glibc
#else
import Darwin
#endif
import upskirt

/// an OptionSet setting the markdown extensions to use when rendering using `.markdownToHTML` or `.markdownToXHTML`
public var markdownExtensionOptions = MarkdownExtensionOptions.default

/// an OptionSet setting the HTML rendering options to use when rendering using `.markdownToHTML` or `.markdownToXHTML`
public var markdownHTMLRenderOptions = HTMLRenderOptions.default

public extension String {
  /// parse a Markdown string into an HTML one, return nil if failed
  var markdownToHTML: String? {
    renderMarkdown()
  }

  /// parse a Markdown string into an XHTML one, return nil if failed
  var markdownToXHTML: String? {
    renderMarkdown(renderOptions: [markdownHTMLRenderOptions, .useXHTML])
  }

  /// renders a Markdown string using `markdownExtensions` and `renderOptions`
  func renderMarkdown(
    markdownExtensions: MarkdownExtensionOptions = markdownExtensionOptions,
    renderOptions: HTMLRenderOptions = markdownHTMLRenderOptions
  ) -> String? {
    MarkdownRenderer(
      markdownExtensions: markdownExtensions,
      renderOptions: renderOptions
    ).callAsFunction(input: self)
  }
}
