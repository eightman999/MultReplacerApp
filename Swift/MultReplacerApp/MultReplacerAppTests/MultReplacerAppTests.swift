import XCTest
@testable import MultReplacerApp

class MultReplacerAppLogicTests: XCTestCase {

    // MARK: - Test Cases
    
    /// 単一の置換が正しく行われることをテスト
    func testSingleReplacement() {
        let content = "Hello, world!"
        let replacements = [ReplacementItem(before: "world", after: "Swift")]
        let newContent = performReplacements(content: content, replacements: replacements)
        XCTAssertEqual(newContent, "Hello, Swift!")
    }
    
    /// 複数の置換が正しく行われることをテスト
    func testMultipleReplacements() {
        let content = "The quick brown fox jumps over the lazy dog."
        let replacements = [
            ReplacementItem(before: "quick", after: "fast"),
            ReplacementItem(before: "brown", after: "red"),
            ReplacementItem(before: "dog", after: "cat")
        ]
        let newContent = performReplacements(content: content, replacements: replacements)
        XCTAssertEqual(newContent, "The fast red fox jumps over the lazy cat.")
    }
    
    /// 置換対象が見つからない場合に、何も変更されないことをテスト
    func testNoMatchingReplacement() {
        let content = "Hello, world!"
        let replacements = [ReplacementItem(before: "galaxy", after: "universe")]
        let newContent = performReplacements(content: content, replacements: replacements)
        XCTAssertEqual(newContent, "Hello, world!")
    }
    
    /// 空の文字列で置換できることをテスト
    func testReplacementWithEmptyString() {
        let content = "Hello, beautiful world!"
        let replacements = [ReplacementItem(before: " beautiful", after: "")]
        let newContent = performReplacements(content: content, replacements: replacements)
        XCTAssertEqual(newContent, "Hello, world!")
    }
    
    /// 大文字と小文字が区別されることをテスト
    func testCaseSensitiveReplacement() {
        let content = "Hello, World!"
        let replacements = [ReplacementItem(before: "world", after: "Swift")]
        let newContent = performReplacements(content: content, replacements: replacements)
        XCTAssertEqual(newContent, "Hello, World!", "'world' should not match 'World'")
    }
    
    /// 複数の同じ置換対象がすべて置換されることをテスト
    func testMultipleOccurrencesOfSameTarget() {
        let content = "A, B, C, A, D, A"
        let replacements = [ReplacementItem(before: "A", after: "Z")]
        let newContent = performReplacements(content: content, replacements: replacements)
        XCTAssertEqual(newContent, "Z, B, C, Z, D, Z")
    }
    
    // MARK: - Helper Function
    
    /// 置換処理を実行するヘルパー関数
    private func performReplacements(content: String, replacements: [ReplacementItem]) -> String {
        var currentContent = content
        for item in replacements {
            if !item.before.isEmpty {
                currentContent = currentContent.replacingOccurrences(of: item.before, with: item.after)
            }
        }
        return currentContent
    }
}