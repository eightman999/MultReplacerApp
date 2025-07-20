import Foundation
import SwiftUI

class I18n: ObservableObject {
    static let shared = I18n()
    @Published var lang: String = "ja" {
        didSet { load(lang) }
    }
    private var strings: [String: String] = [:]

    init() {
        load(lang)
    }

    func load(_ lang: String) {
        if let url = Bundle.main.url(forResource: lang, withExtension: "json", subdirectory: "lang"),
           let data = try? Data(contentsOf: url),
           let dict = try? JSONSerialization.jsonObject(with: data) as? [String: String] {
            strings = dict
        }
    }

    func tr(_ key: String) -> String {
        strings[key] ?? key
    }
}
