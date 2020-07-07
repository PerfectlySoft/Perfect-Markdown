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

#if swift(>=5.0)
#else
	#if os(Linux)
	import Glibc
	#else
	import Darwin
	#endif
#endif
import upskirt

#if swift(>=5.0)
/// an OptionSet setting the markdown extensions to use when rendering using `.markdownToHTML` or `.markdownToXHTML`
public var markdownExtensionOptions = MarkdownExtensionOptions.default

/// an OptionSet setting the HTML rendering options to use when rendering using `.markdownToHTML` or `.markdownToXHTML`
public var markdownHTMLRenderOptions = HTMLRenderOptions.default
#endif

public extension String {

  /// parse a Markdown string into an HTML one, return nil if failed
  var markdownToHTML: String? {
		#if swift(>=5.0)
    return renderMarkdown()
		#else
    let terminated = self
    let OUTPUT_UNIT = 64
    let size = terminated.utf8.count
    let enabled_extensions = MKDEXT_TABLES.rawValue | MKDEXT_FENCED_CODE.rawValue | MKDEXT_EMAIL_FRIENDLY.rawValue
    let render_flags:UInt32 = 0 // HTML_SKIP_HTML | HTML_SKIP_STYLE | HTML_HARD_WRAP;
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
    let _ = sdhtml_renderer(&callbacks, &options, render_flags)
    let md = sd_markdown_new(enabled_extensions, 16, &callbacks, &options)
    let _ = sd_markdown_render(ob, ib.pointee.data, ib.pointee.size, md)
    let _ = sd_markdown_free(md)
    var buffer = Array(UnsafeBufferPointer(start: ob.pointee.data, count: ob.pointee.size))
    buffer.append(0)
    let htm = String(cString: buffer)
    let _ = sd_bufrelease(ib)
    let _ = sd_bufrelease(ob)
    return htm
		#endif
  }

	#if swift(>=5.0)
  /// parse a Markdown string into an XHTML one, return nil if failed
  var markdownToXHTML: String? {
    renderMarkdown(renderOptions: [markdownHTMLRenderOptions, .useXHTML])
  }
	#endif

	#if swift(>=5.0)
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
	#endif
}
