import upskirt

#if swift(>=5.0)
public struct MarkdownExtensionOptions: OptionSet {
  public let rawValue: UInt32
  public init(rawValue: UInt32) {
    self.rawValue = rawValue
  }

  private init(_ enumValue: mkd_extensions) {
    self.rawValue = enumValue.rawValue
  }
}

public extension MarkdownExtensionOptions {
  static let noIntraEmphasis     = MarkdownExtensionOptions(MKDEXT_NO_INTRA_EMPHASIS)
  static let tables              = MarkdownExtensionOptions(MKDEXT_TABLES)
  static let fencedCode          = MarkdownExtensionOptions(MKDEXT_FENCED_CODE)
  static let autolink            = MarkdownExtensionOptions(MKDEXT_AUTOLINK)
  static let strikethrough       = MarkdownExtensionOptions(MKDEXT_STRIKETHROUGH)
  static let insertion           = MarkdownExtensionOptions(MKDEXT_INS)
  static let strictHeaderSpacing = MarkdownExtensionOptions(MKDEXT_SPACE_HEADERS)
  static let superscript         = MarkdownExtensionOptions(MKDEXT_SUPERSCRIPT)
  static let laxSpacing          = MarkdownExtensionOptions(MKDEXT_LAX_SPACING)
  static let emailFriendly       = MarkdownExtensionOptions(MKDEXT_EMAIL_FRIENDLY)
//static let listAlphaRoman      = MarkdownExtensionOptions(MKDEXT_LIST_ALPHA_ROMAN) // broken in upskirt

  static let none = MarkdownExtensionOptions([])
  static let `default` = MarkdownExtensionOptions([.tables, .fencedCode, .emailFriendly])
}

public struct HTMLRenderOptions: OptionSet {
  public let rawValue: UInt32
  public init(rawValue: UInt32) {
    self.rawValue = rawValue
  }

  private init(_ enumValue: html_render_mode) {
    self.rawValue = enumValue.rawValue
  }
}

public extension HTMLRenderOptions {
  static let skipHTML        = HTMLRenderOptions(HTML_SKIP_HTML)
  static let skipStyle       = HTMLRenderOptions(HTML_SKIP_STYLE)
  static let skipImages      = HTMLRenderOptions(HTML_SKIP_IMAGES)
  static let skipLinks       = HTMLRenderOptions(HTML_SKIP_LINKS)
//static let expandTabs      = HTMLRenderOptions(HTML_EXPAND_TABS) // broken in upskirt
  static let safeLinks       = HTMLRenderOptions(HTML_SAFELINK)
  static let tableOfContents = HTMLRenderOptions(HTML_TOC)
  static let hardWrap        = HTMLRenderOptions(HTML_HARD_WRAP)
  static let useXHTML        = HTMLRenderOptions(HTML_USE_XHTML)
  static let escapeHTML      = HTMLRenderOptions(HTML_ESCAPE)
  static let createOutline   = HTMLRenderOptions(HTML_OUTLINE)

  static let none = HTMLRenderOptions([])
  static let `default` = Self.none
}
#endif
