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
    let size = utf8.count
    guard let ib = sd_bufnew(size) else {
      return nil
    }//end guard
    let _ = sd_bufgrow(ib, size)
    ib.pointee.size = withCString { ptr->Int in
      memcpy(ib.pointee.data, ptr, size)
      return size
    }//end markdown
    guard let ob = sd_bufnew(Self.outputBufferReallocationUnit) else {
      return nil
    }//end guard
    var callbacks = sd_callbacks()
    var options = html_renderopt()
    let _ = sdhtml_renderer(&callbacks, &options, renderOptions.rawValue)
    let md = sd_markdown_new(markdownExtensions.rawValue, 16, &callbacks, &options)
    let _ = sd_markdown_render(ob, ib.pointee.data, ib.pointee.size, md)
    let _ = sd_markdown_free(md)
    var buffer = Array(UnsafeBufferPointer(start: ob.pointee.data, count: ob.pointee.size))
    buffer.append(0)
    let htm = String(cString: buffer)
    let _ = sd_bufrelease(ib)
    let _ = sd_bufrelease(ob)
    return htm
  }
}
