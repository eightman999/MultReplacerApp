import SwiftUI

// 置換ルールを表す構造体
struct ReplacementItem: Identifiable, Hashable {
    let id = UUID()
    var before: String = ""
    var after: String = ""
}

struct ContentView: View {
    // MARK: - State Properties

    @EnvironmentObject private var i18n: I18n
    
    @State private var filePath: String = ""
    @State private var replacements: [ReplacementItem] = [ReplacementItem()]
    @State private var showingConfirmationDialog = false
    @State private var fileContent: String = ""
    @State private var newContent: String = ""
    @State private var showingErrorAlert = false
    @State private var errorMessage: String = ""

    // MARK: - Body
    
    var body: some View {
        VStack(alignment: .leading, spacing: 10) {
            
            // 1. ファイルパス選択エリア
            HStack {
                Text(i18n.tr("path_label"))
                TextField("", text: $filePath)
                    .textFieldStyle(RoundedBorderTextFieldStyle())
                Button(i18n.tr("browse")) {
                    selectFile()
                }
                Picker("", selection: $i18n.lang) {
                    Text("ja").tag("ja")
                    Text("en").tag("en")
                }
                .pickerStyle(MenuPickerStyle())
            }
            .padding([.horizontal, .top])
            
            // 2. 注意書き
            Text(i18n.tr("caution"))
                .foregroundColor(.red)
                .padding(.horizontal)

            // 3. 置換リスト
            VStack {
                HStack {
                    Text(i18n.tr("before")).frame(maxWidth: .infinity)
                    Text("").frame(width: 20)
                    Text(i18n.tr("after")).frame(maxWidth: .infinity)
                    Text("").frame(width: 50) // 削除ボタン用のスペース
                }
                .padding(.horizontal)
                
                ScrollView {
                    ForEach($replacements) { $item in
                        HStack {
                            TextField(i18n.tr("before"), text: $item.before)
                                .textFieldStyle(RoundedBorderTextFieldStyle())
                            
                            Image(systemName: "arrow.right")
                            
                            TextField(i18n.tr("after"), text: $item.after)
                                .textFieldStyle(RoundedBorderTextFieldStyle())
                            
                            Button(action: {
                                deleteReplacementRow(item: item)
                            }) {
                                Image(systemName: "trash")
                                    .foregroundColor(.red)
                            }
                            .buttonStyle(PlainButtonStyle())
                        }
                        .padding(.horizontal)
                    }
                }
            }

            // 4. 操作ボタン
            HStack {
                Button(i18n.tr("add")) {
                    addReplacementRow()
                }
                
                Spacer()
                
                Button(i18n.tr("execute")) {
                    executeReplacements()
                }
                .disabled(filePath.isEmpty)
            }
            .padding([.horizontal, .bottom])
            
        }
        .frame(minWidth: 800, minHeight: 500)
        .sheet(isPresented: $showingConfirmationDialog) {
            ConfirmationDialog(
                isPresented: $showingConfirmationDialog,
                newContent: newContent,
                onConfirm: saveChanges
            )
        }
        .alert(isPresented: $showingErrorAlert) {
            Alert(title: Text(i18n.tr("error")), message: Text(errorMessage), dismissButton: .default(Text("OK")))
        }
    }
    
    // MARK: - Functions
    
    /// ファイル選択ダイアログを表示し、選択されたファイルのパスを取得する
    private func selectFile() {
        let openPanel = NSOpenPanel()
        openPanel.canChooseFiles = true
        openPanel.canChooseDirectories = false
        openPanel.allowsMultipleSelection = false
        
        if openPanel.runModal() == .OK {
            if let url = openPanel.url {
                filePath = url.path
            }
        }
    }
    
    /// 置換リストに新しい行を追加する
    private func addReplacementRow() {
        replacements.append(ReplacementItem())
    }
    
    /// 指定された行を置換リストから削除する
    private func deleteReplacementRow(item: ReplacementItem) {
        if let index = replacements.firstIndex(where: { $0.id == item.id }) {
            // 最後の1行でない場合のみ削除
            if replacements.count > 1 {
                replacements.remove(at: index)
            }
        }
    }
    
    /// 置換処理を実行する
    private func executeReplacements() {
        guard !filePath.isEmpty, let url = URL(string: "file://\(filePath)") else {
            showError(i18n.tr("invalid_path"))
            return
        }
        
        do {
            // 1. ファイルを読み込む
            fileContent = try String(contentsOf: url, encoding: .utf8)
            
            // 2. 置換処理
            var currentContent = fileContent
            for item in replacements {
                if !item.before.isEmpty {
                    currentContent = currentContent.replacingOccurrences(of: item.before, with: item.after)
                }
            }
            newContent = currentContent
            
            // 3. 確認ダイアログを表示
            showingConfirmationDialog = true
            
        } catch {
            showError(i18n.tr("read_error") + " " + error.localizedDescription)
        }
    }
    
    /// 変更をファイルに保存する
    private func saveChanges() {
        guard let url = URL(string: "file://\(filePath)") else {
            showError(i18n.tr("invalid_path"))
            return
        }
        
        do {
            try newContent.write(to: url, atomically: true, encoding: .utf8)
            print("ファイルが正常に保存されました。")
        } catch {
            showError(i18n.tr("save_error") + " " + error.localizedDescription)
        }
    }
    
    /// エラーアラートを表示する
    private func showError(_ message: String) {
        errorMessage = message
        showingErrorAlert = true
    }
}

// MARK: - Confirmation Dialog

struct ConfirmationDialog: View {
    @Binding var isPresented: Bool
    let newContent: String
    let onConfirm: () -> Void
    
    var body: some View {
        VStack {
            Text(i18n.tr("preview_title"))
                .font(.title)
                .padding()
            
            ScrollView {
                Text(newContent)
                    .padding()
                    .frame(maxWidth: .infinity, maxHeight: .infinity, alignment: .topLeading)
                    .background(Color(NSColor.textBackgroundColor))
                    .border(Color.gray, width: 1)
            }
            .padding()
            
            HStack {
                Button(i18n.tr("cancel")) {
                    isPresented = false
                }
                .keyboardShortcut(.cancelAction)
                
                Spacer()
                
                Button(i18n.tr("execute_save")) {
                    onConfirm()
                    isPresented = false
                }
                .keyboardShortcut(.defaultAction)
            }
            .padding()
        }
        .frame(minWidth: 600, minHeight: 400)
    }
}


// MARK: - Preview

struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
        ContentView()
    }
}