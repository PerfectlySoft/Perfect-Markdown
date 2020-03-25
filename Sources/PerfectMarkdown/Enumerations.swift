import upskirt

public struct MarkdownExtensionOptions: OptionSet {
  public let rawValue: UInt32
  public init(rawValue: UInt32) {
    self.rawValue = rawValue
  }
}

public extension MarkdownExtensionOptions {
  private init(_ enumValue: mkd_extensions) {
    self.rawValue = enumValue.rawValue
  }

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
  // static let listAlphaRoman      = MarkdownExtensionOptions(MKDEXT_LIST_ALPHA_ROMAN) // broken in upskirt

  static let none = MarkdownExtensionOptions([])
  static let `default` = MarkdownExtensionOptions([.tables, .fencedCode, .emailFriendly])
}
