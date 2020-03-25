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

extension String {
  /// parse a Markdown string into an HTML one, return nil if failed
  public var markdownToHTML: String? {
    renderMarkdown()
  }

  /// parse a Markdown string into an XHTML one, return nil if failed
  public var markdownToXHTML: String? {
    renderMarkdown(renderOptions: [markdownHTMLRenderOptions, .useXHTML])
  }

  /// the smallest number of bytes to expand the output buffer by at a time
  private static let outputBufferReallocationUnit = 64

  public func renderMarkdown(
    markdownExtensions: MarkdownExtensionOptions = markdownExtensionOptions,
    renderOptions: HTMLRenderOptions = markdownHTMLRenderOptions
  ) -> String? {
    let inputBufferSize = self.utf8.count
    guard let inputBuffer = sd_bufnew(inputBufferSize) else {
      return nil
    }//end guard
    defer {
      sd_bufrelease(inputBuffer)
    }

    sd_bufgrow(inputBuffer, inputBufferSize)
    inputBuffer.pointee.size = self.withCString { ptr->Int in
      memcpy(inputBuffer.pointee.data, ptr, inputBufferSize)
      return inputBufferSize
    }//end markdown

    guard let outputBuffer = sd_bufnew(Self.outputBufferReallocationUnit) else {
      return nil
    }//end guard
    defer {
      sd_bufrelease(outputBuffer)
    }

    var callbacks = sd_callbacks()
    var options = html_renderopt()
    sdhtml_renderer(&callbacks, &options, renderOptions.rawValue)

    let markdownContext = sd_markdown_new(markdownExtensions.rawValue, 16, &callbacks, &options)
    sd_markdown_render(outputBuffer, inputBuffer.pointee.data, inputBuffer.pointee.size, markdownContext)
    sd_markdown_free(markdownContext)

    var stringBuffer = Array(UnsafeBufferPointer(start: outputBuffer.pointee.data, count: outputBuffer.pointee.size))
    stringBuffer.append(0)

    return String(cString: stringBuffer)
  }
}
