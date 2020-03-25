import upskirt

public class MarkdownRenderer {
  let outputBufferReallocationUnit: Int

  let markdownExtensions: MarkdownExtensionOptions
  let renderOptions: HTMLRenderOptions

  var callbacks = sd_callbacks()
  var options = html_renderopt()

  let markdownContext: OpaquePointer?

  init(
    outputBufferReallocationUnit: Int = 64,
    markdownExtensions: MarkdownExtensionOptions = MarkdownExtensionOptions.default,
    renderOptions: HTMLRenderOptions = HTMLRenderOptions.default
  ) {
    self.outputBufferReallocationUnit = outputBufferReallocationUnit

    self.markdownExtensions = markdownExtensions
    self.renderOptions = renderOptions

    sdhtml_renderer(&callbacks, &options, self.renderOptions.rawValue)
    self.markdownContext = sd_markdown_new(self.markdownExtensions.rawValue, 16, &callbacks, &options)
  }

  deinit {
    sd_markdown_free(markdownContext)
  }

  func callAsFunction(input: String) -> String? {
    let inputBufferSize = input.utf8.count
    guard let inputBuffer = sd_bufnew(inputBufferSize) else {
      return nil
    }//end guard
    defer {
      sd_bufrelease(inputBuffer)
    }

    sd_bufgrow(inputBuffer, inputBufferSize)
    inputBuffer.pointee.size = input.withCString { ptr->Int in
      memcpy(inputBuffer.pointee.data, ptr, inputBufferSize)
      return inputBufferSize
    }//end markdown

    guard let outputBuffer = sd_bufnew(outputBufferReallocationUnit) else {
      return nil
    }//end guard
    defer {
      sd_bufrelease(outputBuffer)
    }

    sd_markdown_render(outputBuffer, inputBuffer.pointee.data, inputBuffer.pointee.size, markdownContext)

    var stringBuffer = Array(UnsafeBufferPointer(start: outputBuffer.pointee.data, count: outputBuffer.pointee.size))
    stringBuffer.append(0)

    return String(cString: stringBuffer)
  }
}
