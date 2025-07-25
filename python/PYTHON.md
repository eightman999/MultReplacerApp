# Python実装について

## 概要
このディレクトリには、「まるち置き換え君」のPython実装が含まれています。

## ファイル構成
- `mult_repracer.py` - メインアプリケーション
- `まるち置き換え君.spec` - PyInstaller用の設定ファイル

## 主な機能
`MultReplacerApp`クラスによって実装されたGUIアプリケーション：

### 基本機能
- **ファイル選択**: 置換対象のファイルをファイルダイアログで選択
- **複数置換**: 複数の置換ルールを同時に適用
- **プレビュー**: 置換結果を確認してから実際の置換を実行
- **スクロール対応**: 大量の置換ルールに対応したスクロール可能なUI

### 自動アップデート機能
- GitHubリポジトリから最新バージョンをチェック
- 新しいバージョンが利用可能な場合、自動でダウンロード・更新
- プラットフォーム別対応（macOS: .dmg, Windows: .exe）

### UI構成
- **ファイルパス入力**: 対象ファイルのパス表示・選択
- **置換ルール**: 「変更前」→「変更後」の置換ルールを複数設定可能
- **動的行追加**: 「追加」ボタンで置換ルールを追加
- **削除機能**: 各置換ルールの削除が可能
- **確認ダイアログ**: 置換実行前に結果をプレビュー

## 技術仕様
- **フレームワーク**: tkinter (Python標準GUI)
- **文字エンコーディング**: UTF-8
- **外部ライブラリ**: requests (GitHub API通信用)
- **カスタムモジュール**: `tools.line_tools.multi_replace`

## 実行方法
```bash
python mult_repracer.py
```

## PyInstaller設定
`まるち置き換え君.spec`ファイルを使用してスタンドアロンアプリケーションとしてビルド可能。