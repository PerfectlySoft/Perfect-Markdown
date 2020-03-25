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

public var markdownExtensionOptions = MarkdownExtensionOptions.default
public var markdownHTMLRenderOptions = HTMLRenderOptions.default

extension String {
  /// parse a Markdown string into an HTML one, return nil if failed
  public var markdownToHTML: String? { get {
    let terminated = self
    let OUTPUT_UNIT = 64
    let size = terminated.utf8.count
    guard let ib = sd_bufnew(size) else {
      return nil
    }//end guard
    let _ = sd_bufgrow(ib, size)
    ib.pointee.size = terminated.withCString { ptr->Int in
      memcpy(ib.pointee.data, ptr, size)
      return size
    }//end markdown
    guard let ob = sd_bufnew(OUTPUT_UNIT) else {
      return nil
    }//end guard
    var callbacks = sd_callbacks()
    var options = html_renderopt()
    let _ = sdhtml_renderer(&callbacks, &options, markdownHTMLRenderOptions.rawValue)
    let md = sd_markdown_new(markdownExtensionOptions.rawValue, 16, &callbacks, &options)
    let _ = sd_markdown_render(ob, ib.pointee.data, ib.pointee.size, md)
    let _ = sd_markdown_free(md)
    var buffer = Array(UnsafeBufferPointer(start: ob.pointee.data, count: ob.pointee.size))
    buffer.append(0)
    let htm = String(cString: buffer)
    let _ = sd_bufrelease(ib)
    let _ = sd_bufrelease(ob)
    return htm
  } }
}
