import os
import shutil
import subprocess
import sys
import tkinter as tk
from tkinter import messagebox, ttk

import json
import requests
from tools.line_tools import multi_replace


LANG = 'ja'
_translations = {}


def load_language(lang):
    global LANG, _translations
    LANG = lang
    lang_file = os.path.join(os.path.dirname(__file__), 'lang', f'{lang}.json')
    try:
        with open(lang_file, 'r', encoding='utf-8') as f:
            _translations = json.load(f)
    except FileNotFoundError:
        _translations = {}


def tr(key):
    return _translations.get(key, key)


class MultReplacerApp:

    def __init__(self, root):
        self.root = root
        version = self.get_version()
        self.current_version = self.get_current_version()
        load_language(LANG)
        self.root.title(tr("title").format(version=version))
        self.root.geometry("1280x720")  # Set default window size to 720p
        # GitHubリポジトリ情報
        self.repo_owner = "eightman999"
        self.repo_name = "MultReplacerApp"

        # アップデート処理を実行
        self.check_and_update()
        self.file_path = tk.StringVar()

        # File path selection
        self.file_frame = tk.Frame(root)
        self.file_frame.grid(row=0, column=0, padx=10, pady=5, sticky="ew")

        # Language selector
        self.lang_var = tk.StringVar(value=LANG)
        lang_menu = ttk.Combobox(self.file_frame, width=5, textvariable=self.lang_var, values=["ja", "en"], state="readonly")
        lang_menu.grid(row=0, column=3, padx=5)
        lang_menu.bind("<<ComboboxSelected>>", self.on_lang_changed)

        self.file_label = tk.Label(self.file_frame, text=tr("path_label"))
        self.file_label.grid(row=0, column=0)

        self.file_entry = tk.Entry(self.file_frame, textvariable=self.file_path, width=100)
        self.file_entry.grid(row=0, column=1, padx=5, sticky="ew")

        self.browse_button = tk.Button(self.file_frame, text=tr("browse"), command=self.browse_file)
        self.browse_button.grid(row=0, column=2)

        # Caution label
        self.caution_label = tk.Label(root,
                                      text=tr("caution"),
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
        self.add_button = tk.Button(root, text=tr("add"), command=self.add_replacement_row)
        self.add_button.grid(row=3, column=0, pady=5, sticky="w", padx=10)

        # Execute button
        self.execute_button = tk.Button(root, text=tr("execute"), command=self.execute_replacements)
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
            return "x0.0.0"  # ファイルが存在しない場合のデフォルトバージョン
        except Exception as e:
            return f"Error ({e})"  # 他の予期しないエラーをデバッグ用に返す
    def get_current_version(self):
        """現在のバージョン情報を取得"""
        try:
            with open("version.txt", "r", encoding="utf-8") as f:
                return f.read().strip()
        except FileNotFoundError:
            return "v0.0.0"  # デフォルトバージョン（バージョンファイルがない場合）

    def get_latest_version(self):
        """GitHubリポジトリから最新リリース情報を取得"""
        url = f"https://api.github.com/repos/{self.repo_owner}/{self.repo_name}/releases/latest"
        response = requests.get(url)

        if response.status_code == 200:
            data = response.json()
            return data["tag_name"], data["assets"]  # 最新リリースバージョンとアセット情報を返す
        else:
            raise Exception(f"GitHub APIエラー: {response.status_code}")

    def download_new_version(self, download_url, output_file):
        """新しいバージョンのファイルをダウンロード"""
        response = requests.get(download_url, stream=True)
        with open(output_file, "wb") as f:
            shutil.copyfileobj(response.raw, f)
        print(f"新しいバージョンをダウンロードしました: {output_file}")

    def replace_and_restart(self, new_file, is_mac=False):
        """現在のアプリケーションを新しい実行ファイルで置き換え、再起動"""
        current_executable = sys.executable  # 現在の実行ファイル
        backup_file = current_executable + ".bak"

        # 実行ファイルのバックアップ
        shutil.move(current_executable, backup_file)
        print("元の実行ファイルをバックアップしました")

        # 新しいファイルを現在の実行ファイルとして置き換え
        shutil.move(new_file, current_executable)
        print("新しい実行ファイルで置き換えました")

        # 新しいファイルで再起動
        if is_mac:
            subprocess.run(["open", current_executable])
        else:
            subprocess.Popen(current_executable, shell=True)

        sys.exit()  # 現在のプロセスを終了

    def check_and_update(self):
        """バージョンを比較して必要ならアップデート"""
        print(f"現在のバージョン: {self.current_version}")

        try:
            latest_version, assets = self.get_latest_version()
            print(f"最新のバージョン: {latest_version}")

            # バージョンの比較
            if self.current_version == latest_version:
                print("すでに最新のバージョンです。")
                return

            # プラットフォームごとに適切なアセットを選ぶ
            is_mac = sys.platform == "darwin"
            asset_name = "app.dmg" if is_mac else "app.exe"  # 更新対象のファイル
            download_url = None

            for asset in assets:
                if asset_name in asset["name"]:
                    download_url = asset["browser_download_url"]
                    break

            if not download_url:
                print("ダウンロード可能なアセットが見つかりませんでした")
                return

            # 新しいバージョンをダウンロード
            new_file = os.path.join(os.getcwd(), asset_name)
            self.download_new_version(download_url, new_file)

            # 実行ファイルを置き換えて再起動
            self.replace_and_restart(new_file, is_mac=is_mac)

        except Exception as e:
            print(f"アップデート中にエラーが発生しました: {e}")

    def browse_file(self):
        from tkinter import filedialog
        file_path = filedialog.askopenfilename()
        if file_path:
            self.file_path.set(file_path)

    def add_replacement_row(self):
        row_frame = tk.Frame(self.scrollable_frame)
        row_frame.pack(fill=tk.X, pady=2)

        delete_button = tk.Button(row_frame, text=tr("delete"), command=lambda: self.delete_replacement_row(row_frame))
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
        confirm_dialog.title(tr("confirm_title"))
        confirm_dialog.geometry("800x600")  # Set default size for the dialog

        text_widget = tk.Text(confirm_dialog, wrap=tk.WORD)
        text_widget.insert(tk.END, new_content)
        text_widget.config(state=tk.DISABLED)
        text_widget.pack(pady=10, fill=tk.BOTH, expand=True)

        button_frame = tk.Frame(confirm_dialog)
        button_frame.pack(pady=5, fill=tk.X)

        cancel_button = tk.Button(button_frame, text=tr("cancel"), command=confirm_dialog.destroy)
        cancel_button.pack(side=tk.LEFT, padx=5)

        execute_button = tk.Button(button_frame, text=tr("execute"), command=lambda: self.save_replacements(file_path, new_content, confirm_dialog))
        execute_button.pack(side=tk.LEFT, padx=5)

    def save_replacements(self, file_path, new_content, dialog):
        with open(file_path, 'w', encoding='utf-8') as file:
            file.write(new_content)
        dialog.destroy()
        messagebox.showinfo(tr("completed"), tr("replace_done"))

    def on_lang_changed(self, event=None):
        load_language(self.lang_var.get())
        version = self.get_version()
        self.root.title(tr("title").format(version=version))
        self.file_label.config(text=tr("path_label"))
        self.browse_button.config(text=tr("browse"))
        self.caution_label.config(text=tr("caution"))
        self.add_button.config(text=tr("add"))
        self.execute_button.config(text=tr("execute"))

if __name__ == "__main__":

    root = tk.Tk()
    load_language(LANG)
    app = MultReplacerApp(root)
    root.mainloop()