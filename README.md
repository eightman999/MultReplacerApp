# MultReplacerApp

テキストファイル内の複数の文字列を一度に置換するためのシンプルなデスクトップアプリケーションです。

このリポジトリには、同じ機能を持つアプリケーションが複数のプログラミング言語・フレームワークで実装されています。

## 機能

-   置換対象のテキストファイルを指定します。
-   置換したい文字列のペア（置換前と置換後）を複数設定できます。
-   置換ルールは動的に追加・削除できます。
-   「実行」ボタンを押すと、置換後のプレビューが表示されます。
-   プレビューで問題がなければ、実際にファイルを上書き保存します。

## 各言語・フレームワーク版

-   [**Python (Tkinter)**](./python/)
-   [**Java (Swing)**](./Java/)
-   [**Swift (SwiftUI)**](./Swift/)
-   [**C++ (Qt)**](./C++Qt/)
-   [**Web (HTML/CSS/JavaScript)**](./web/)

## Java版のビルドと実行

1.  **コンパイル:**
    `Java/`ディレクトリに移動し、以下のコマンドでJavaファイルをコンパイルします。
    ```bash
    javac MultReplacerApp.java ReplacementItem.java ConfirmationDialog.java
    ```
2.  **実行:**
    コンパイル後、以下のコマンドでアプリケーションを実行します。
    ```bash
    java MultReplacerApp
    ```

## Swift/SwiftUI版のビルドと実行

1.  **Xcodeを開く:**
    `Swift/MultReplacerApp/MultReplacerApp.xcodeproj` ファイルをXcodeで開きます。

2.  **ビルドと実行:**
    Xcodeのツールバーにある「▶」ボタンをクリックするか、`Cmd + R` キーを押してアプリケーションをビルドし、実行します。

## ライセンス

このプロジェクトは [MIT License](./LICENSE) の下で公開されています。