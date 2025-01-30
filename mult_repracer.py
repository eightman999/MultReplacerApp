import tkinter as tk
from tkinter import filedialog, messagebox, ttk

from tools.line_tools import multi_replace

class MultReplacerApp:

    def __init__(self, root):
        self.root = root
        version = self.get_version()
        self.root.title(f"置き換え君 v{version}")
        self.root.geometry("1280x720")  # Set default window size to 720p

        self.file_path = tk.StringVar()

        # File path selection
        self.file_frame = tk.Frame(root)
        self.file_frame.grid(row=0, column=0, padx=10, pady=5, sticky="ew")

        self.file_label = tk.Label(self.file_frame, text="パス:")
        self.file_label.grid(row=0, column=0)

        self.file_entry = tk.Entry(self.file_frame, textvariable=self.file_path, width=100)
        self.file_entry.grid(row=0, column=1, padx=5, sticky="ew")

        self.browse_button = tk.Button(self.file_frame, text="参照", command=self.browse_file)
        self.browse_button.grid(row=0, column=2)

        # Caution label
        self.caution_label = tk.Label(root,
                                      text="注意：英数変換キーなどを押すと、一文字としてカウントされることがあります！！",
                                      fg="red")
        self.caution_label.grid(row=1, column=0, padx=10, pady=5, sticky="w")

        # Replacements Frame
        self.replacements_frame = tk.Frame(root)
        self.replacements_frame.grid(row=2, column=0, padx=10, pady=10, sticky="nsew")
        self.replacements_frame.grid_rowconfigure(0, weight=1)
        self.replacements_frame.grid_columnconfigure(0, weight=1)

        # Scrollable area
        self.scroll_canvas = tk.Canvas(self.replacements_frame, width=1280, height=670)
        self.scroll_canvas.grid(row=0, column=0, sticky="nsew")

        self.scrollbar = ttk.Scrollbar(self.replacements_frame, orient="vertical", command=self.scroll_canvas.yview)
        self.scrollbar.grid(row=0, column=1, sticky="ns")
        self.scroll_canvas.configure(yscrollcommand=self.scrollbar.set)

        # Content frame inside the canvas
        self.scrollable_frame = tk.Frame(self.scroll_canvas)
        self.scrollable_frame.bind(
            "<Configure>",
            lambda e: self.scroll_canvas.configure(scrollregion=self.scroll_canvas.bbox("all"))
        )
        self.scroll_canvas.create_window((0, 0), window=self.scrollable_frame, anchor="nw")

        # Placeholder for replacement rows
        self.replacements = []

        # Add initial replacement row
        self.add_replacement_row()

        # Add button
        self.add_button = tk.Button(root, text="追加", command=self.add_replacement_row)
        self.add_button.grid(row=3, column=0, pady=5, sticky="w", padx=10)

        # Execute button
        self.execute_button = tk.Button(root, text="実行", command=self.execute_replacements)
        self.execute_button.grid(row=3, column=1, pady=5, sticky="e", padx=10)

        # Configure grid for resizing
        root.grid_rowconfigure(2, weight=1)
        root.grid_columnconfigure(0, weight=1)
    def get_version(self):
        """バージョン情報をファイルから読み取る"""
        try:
            with open("version.txt", "r", encoding="utf-8") as file:
                return file.read().strip()  # 改行や余分な空白を削除した文字列を返す
        except FileNotFoundError:
            return ""  # ファイルが存在しない場合のデフォルトバージョン
        except Exception as e:
            return f"Error ({e})"  # 他の予期しないエラーをデバッグ用に返す
    def browse_file(self):
        from tkinter import filedialog
        file_path = filedialog.askopenfilename()
        if file_path:
            self.file_path.set(file_path)

    def add_replacement_row(self):
        row_frame = tk.Frame(self.scrollable_frame)
        row_frame.pack(fill=tk.X, pady=2)

        delete_button = tk.Button(row_frame, text="削除", command=lambda: self.delete_replacement_row(row_frame))
        delete_button.pack(side=tk.LEFT, padx=5)

        before_text = tk.Entry(row_frame, width=50)
        before_text.pack(side=tk.LEFT, padx=5, fill=tk.X, expand=True)

        arrow_label = tk.Label(row_frame, text="→")
        arrow_label.pack(side=tk.LEFT, padx=5)

        after_text = tk.Entry(row_frame, width=50)
        after_text.pack(side=tk.LEFT, padx=5, fill=tk.X, expand=True)

        self.replacements.append((before_text, after_text))

    def delete_replacement_row(self, row_frame):
        row_frame.destroy()
        self.replacements = [(before, after) for before, after in self.replacements if before.winfo_exists()]

    def execute_replacements(self):
        file_path = self.file_path.get()
        if not file_path:
            return

        with open(file_path, 'r', encoding='utf-8') as file:
            content = file.read()

        replacements_dict = {before_text.get(): after_text.get() for before_text, after_text in self.replacements if before_text.get()}
        print(replacements_dict)
        new_content = multi_replace(content, replacements_dict)
        print(new_content)

        # Show confirmation dialog
        confirm_dialog = tk.Toplevel(self.root)
        confirm_dialog.title("確認")
        confirm_dialog.geometry("800x600")  # Set default size for the dialog

        text_widget = tk.Text(confirm_dialog, wrap=tk.WORD)
        text_widget.insert(tk.END, new_content)
        text_widget.config(state=tk.DISABLED)
        text_widget.pack(pady=10, fill=tk.BOTH, expand=True)

        button_frame = tk.Frame(confirm_dialog)
        button_frame.pack(pady=5, fill=tk.X)

        cancel_button = tk.Button(button_frame, text="キャンセル", command=confirm_dialog.destroy)
        cancel_button.pack(side=tk.LEFT, padx=5)

        execute_button = tk.Button(button_frame, text="実行", command=lambda: self.save_replacements(file_path, new_content, confirm_dialog))
        execute_button.pack(side=tk.LEFT, padx=5)

    def save_replacements(self, file_path, new_content, dialog):
        with open(file_path, 'w', encoding='utf-8') as file:
            file.write(new_content)
        dialog.destroy()
        messagebox.showinfo("完了", "置換が完了しました。")

if __name__ == "__main__":

    root = tk.Tk()
    app = MultReplacerApp(root)
    root.mainloop()