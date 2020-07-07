import upskirt

public class MarkdownRenderer {
  typealias InputBuffer = IOBuffer
  typealias OutputBuffer = IOBuffer

  class IOBuffer {
    let buffer: UnsafeMutablePointer<sd_buf>

    init?(unitSize: Int) {
      guard
        let bufferPointer = sd_bufnew(unitSize)
      else {
        return nil
      }

      self.buffer = bufferPointer
    }

    init?(input: String) {
      let bufferSize = input.utf8.count
      guard
        let bufferPointer = sd_bufnew(bufferSize)
      else {
        return nil
      }

      self.buffer = bufferPointer
      sd_bufgrow(buffer, bufferSize)

      buffer.pointee.size = input.withCString { ptr->Int in
        memcpy(buffer.pointee.data, ptr, bufferSize)
        return bufferSize
      }
    }

    deinit {
      sd_bufrelease(buffer)
    }

    var value: String {
      var stringBuffer = Array(UnsafeBufferPointer(start: buffer.pointee.data, count: buffer.pointee.size))
      stringBuffer.append(0)

      return String(cString: stringBuffer)
    }
  }

  let outputBufferUnitSize: Int

  let markdownExtensions: MarkdownExtensionOptions
  let renderOptions: HTMLRenderOptions

  var callbacks = sd_callbacks()
  var options = html_renderopt()

  let markdownContext: OpaquePointer?

  init(
    outputBufferUnitSize: Int = 64,
    markdownExtensions: MarkdownExtensionOptions = MarkdownExtensionOptions.default,
    renderOptions: HTMLRenderOptions = HTMLRenderOptions.default
  ) {
    self.outputBufferUnitSize = outputBufferUnitSize

    self.markdownExtensions = markdownExtensions
    self.renderOptions = renderOptions

    sdhtml_renderer(&callbacks, &options, self.renderOptions.rawValue)
    self.markdownContext = sd_markdown_new(self.markdownExtensions.rawValue, 16, &callbacks, &options)
  }

  deinit {
    sd_markdown_free(markdownContext)
  }

  func render(inputBuffer: InputBuffer, outputBuffer: OutputBuffer) {
    sd_markdown_render(outputBuffer.buffer, inputBuffer.buffer.pointee.data, inputBuffer.buffer.pointee.size, markdownContext)
  }

  public func callAsFunction(input: String) -> String? {
    guard
      let inputBuffer = InputBuffer(input: input),
      let outputBuffer = OutputBuffer(unitSize: outputBufferUnitSize)
    else {
      return nil
    }

    render(inputBuffer: inputBuffer, outputBuffer: outputBuffer)

    return outputBuffer.value
  }
}
