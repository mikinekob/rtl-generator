# RTL Generator - AI アシスタント開発ガイドライン

## プロジェクト概要
- **プロジェクト名**: RTL Generator
- **目的**: C言語で Verilog RTL コードとテストベンチを自動生成するツール
- **言語**: C99
- **開発環境**: Windows PC + w64devkit + VS Code + GitHub Copilot
- **GitHub**: https://github.com/mikinekob/rtl-generator

---

## ディレクトリ構成

```
rtl-generator/
├── src/                    # ソースファイル
│   ├── main.c             # メインプログラム
│   ├── generator.c        # RTL/Testbench 生成関数
│   └── template.c         # テンプレート・引数処理
├── include/               # ヘッダファイル
│   └── rtl_generator.h   # 関数・構造体宣言
├── test/                  # テストファイル
├── examples/              # 使用例
├── docs/                  # ドキュメント
├── Makefile              # ビルド設定
├── .gitignore            # Git 無視ファイル
├── LICENSE               # MIT ライセンス
├── README.md             # プロジェクト説明
└── CLAUDE.md             # このファイル
```

---

## コードスタイルガイド

### 命名規則
- **関数**: `snake_case` （例: `generate_verilog_module`, `parse_arguments`）
- **構造体**: `snake_case` （例: `ModuleConfig`）
- **定数**: `UPPER_CASE` （例: `MAX_PORTS`, `MAX_WIDTH`）
- **変数**: `snake_case` （例: `module_name`, `input_count`）

### コンパイル設定
```bash
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -I./include
```

### コメント規則
```c
// 1行コメント: 実装詳細の説明

/*
 * 複数行コメント: 関数やセクションの説明
 * より詳細な説明
 */
```

### 関数テンプレート
```c
/**
 * 関数の説明（日本語）
 * 
 * @param param1 パラメータ1の説明
 * @param param2 パラメータ2の説明
 * @return 戻り値の説明
 */
void function_name(int param1, char *param2) {
    // 実装
}
```

---

## ビルドコマンド

```bash
# プロジェクトルートで実行

# フルビルド
make

# ビルド成果物を削除
make clean

# ビルド＆実行
make run
```

---

## 主要なデータ構造

### ModuleConfig 構造体
```c
typedef struct {
    char module_name[256];      // モジュール名（最大256文字）
    int input_count;            // 入力ポート数
    int output_count;           // 出力ポート数
    int input_width[16];        // 各入力ポートのビット幅
    int output_width[16];       // 各出力ポートのビット幅
} ModuleConfig;
```

---

## 実装済み関数

### 1. `generate_verilog_module()`
**ファイル**: `src/generator.c`
```c
void generate_verilog_module(ModuleConfig *config, FILE *output_file);
```
- Verilog RTL モジュールを生成
- config に基づいてポート宣言を出力
- ファイルに書き込み

### 2. `generate_testbench()`
**ファイル**: `src/generator.c`
```c
void generate_testbench(ModuleConfig *config, FILE *output_file);
```
- テストベンチテンプレートを生成
- クロック生成・リセット処理を含む
- モジュールインスタンス化

### 3. `print_help()`
**ファイル**: `src/generator.c`
```c
void print_help(void);
```
- ヘルプメッセージを表示
- 使用方法と引数一覧を出力

---

## 現在のタスク: Issue #1 - コマンドライン引数解析

### 問題説明
RTL Generator は��マンドライン引数を受け取って、生成コードをカスタマイズする必要があります。

現在は `default_module.v` のみ生成されていますが、ユーザーが指定した名前やポート設定で生成できるようにします。

### 入出力例

#### 実行例 1: 基本的な使用
```bash
.\bin\rtl-gen.exe --module adder --inputs 32,32 --outputs 33
```

**出力:**
```
Generated: adder.v
Generated: adder_tb.v
```

**生成ファイル:**
- `adder.v`: Verilog モジュール（名前が adder）
- `adder_tb.v`: テストベンチ

#### 実行例 2: ヘルプ表示
```bash
.\bin\rtl-gen.exe --help
```

**出力:**
```
RTL Generator - Verilog code generation tool

Usage: rtl-gen [options]

Options:
  --module <name>    Module name
  --inputs <widths>  Input port widths (comma-separated)
  --outputs <widths> Output port widths (comma-separated)
  --help             Show this help message
```

### 実装詳細

#### 実装対象
- **ファイル**: `src/template.c`
- **関数**: `void parse_arguments(int argc, char *argv[], ModuleConfig *config)`
- **呼び出し元**: `src/main.c` の `main()` 関数

#### 引数パースルール

| フラグ | 説明 | 例 |
|--------|------|-----|
| `--module <名前>` | モジュール名を設定 | `--module adder` |
| `--inputs <幅>` | 入力ポート幅（カンマ区切り） | `--inputs 32,32` |
| `--outputs <幅>` | 出力ポート幅（カンマ区切り） | `--outputs 33` |
| `--help` | ヘルプを表示＆終了 | `--help` |

#### バリデーションルール

1. **モジュール名**
   - 1～255 文字の英数字とアンダースコアのみ
   - 数字から始まってはいけない
   - 例: ✅ `adder`, `ALU_32`, `add_module` / ❌ `123abc`, `add-module`

2. **ポート幅**
   - 1～128 ビット
   - 正の整数のみ
   - 例: ✅ `8,16,32`, `1` / ❌ `0`, `-8`, `256`

3. **ポート数**
   - 最大 16 個の入力、16 個の出力
   - 最小 1 個

4. **エラー処理**
   - 無効な引数 → stderr に出力 → `exit(1)`
   - ヘルプ表示が正常終了 → `exit(0)`

#### 実装アルゴリズム
```c
void parse_arguments(int argc, char *argv[], ModuleConfig *config) {
    // 1. argc == 1 なら（引数なし）
    //    - ヘルプを表示
    //    - 終了

    // 2. argv をループ（i = 1 から argc-1 まで）
    //    a. "--help" の場合: print_help() と exit(0)
    //    b. "--module" の場合:
    //       - 次の引数を取得
    //       - バリデーション（英数字・アンダースコアのみ）
    //       - config->module_name に格納
    //    c. "--inputs" の場合:
    //       - 次の引数をカンマで分割
    //       - 各値をバリデーション（1-128）
    //       - config->input_width[] に格納
    //       - config->input_count を設定
    //    d. "--outputs" の場合:
    //       - "--inputs" と同様の処理

    // 3. エラー時:
    //    - fprintf(stderr, "エラーメッセージ");
    //    - exit(1);

    // 4. 正常終了時:
    //    - 何もしない（main() に戻る）
}
```

### テスト方法

#### テスト 1: ヘルプ表示
```bash
make clean && make
.\bin\rtl-gen.exe --help
```
**期待出力**: ヘルプメッセージが表示される

#### テスト 2: 有効な引数
```bash
.\bin\rtl-gen.exe --module adder --inputs 32,32 --outputs 33
```
**期待出力:**
```
Generated: adder.v
Generated: adder_tb.v
```
**ファイル確認:**
```bash
type adder.v
type adder_tb.v
```

#### テスト 3: 無効なモジュール名
```bash
.\bin\rtl-gen.exe --module "123-invalid"
```
**期待出力**: エラーメッセージが stderr に出力される

#### テスト 4: 無効なポート幅
```bash
.\bin\rtl-gen.exe --module test --inputs 256,32
```
**期待出力**: エラーメッセージが stderr に出力される

#### テスト 5: 引数なし
```bash
.\bin\rtl-gen.exe
```
**期待出力**: ヘルプメッセージが表示される

### 成功基準
- ✅ `--help` でヘルプが表示される
- ✅ `--module` でモジュール名が設定される
- ✅ `--inputs` でカンマ区切り幅がパースされる
- ✅ `--outputs` でカンマ区切り幅がパースされる
- ✅ 生成ファイル名がモジュール名で変わる
- ✅ バリデーション失敗時にエラーメッセージが出力される
- ✅ すべてのテストケースが成功する

---

## GitHub ワークフロー

### 1. フィーチャーブランチを作成
```bash
git checkout -b feature/issue-1-cli-parsing
```

### 2. コード実装
```bash
# VS Code で src/template.c を編集
# GitHub Copilot を活用
```

### 3. ビルド＆テスト
```bash
make clean && make
.\bin\rtl-gen.exe --help
.\bin\rtl-gen.exe --module adder --inputs 32,32 --outputs 33
```

### 4. コミット
```bash
git add .
git commit -m "feat: #1 コマンドライン引数パース機能を実装"
```

### 5. Push
```bash
git push origin feature/issue-1-cli-parsing
```

### 6. GitHub で Pull Request を作成
- タイトル: `feat: #1 コマンドライン引数パース機能`
- 説明: 実装内容を記載

### 7. マージ
- GitHub で `Merge pull request` をクリック

---

## 便利なコマンド

```bash
# リポジトリルートで実行

# ビルド＆テスト一括
make clean && make && .\bin\rtl-gen.exe

# 生成ファイルの確認
dir *.v

# Git ステータス確認
git status

# コミットログ確認
git log --oneline
```

---

## よくある問題と解決方法

### 問題 1: ビルドエラー
```
gcc: error: src/template.c: No such file or directory
```
**解決**: ファイルが正しく作成されているか確認
```bash
dir src
```

### 問題 2: 実行エラー
```
'.\bin\rtl-gen.exe' is not recognized
```
**解決**: ビルド完了を確認
```bash
make clean && make
```

### 問題 3: ポート幅パース失敗
**原因**: `strtok()` で間違ったセパレータを使用
**解決**: `strtok(str, ",")` でカンマをセパレータに指定

---

## 重要な注意事項

1. **バリデーションは必須**
   - 無効入力でプログラムがクラッシュしてはいけない
   - エラーメッセージは stderr に出力

2. **メモリ管理**
   - `strtok()` 使用時は元の文字列を保存（破壊される）
   - 必要に応じて `strcpy()` で複製

3. **テストは複数回実行**
   - 成功ケース、失敗ケース両方テスト
   - 境界値をテスト

4. **GitHub Copilot の活用**
   - 「カンマ区切り文字列をパースする C 関数」
   - 「文字列がアルファベットとアンダースコアのみか検証する関数」
   - など、AI に質問して効率化

---

## 次のステップ（Issue #2 以降）

### Issue #2: Verilog RTL テンプレート拡張
- 複数のポート宣言を動的に生成
- 信号幅に応じた宣言

### Issue #3: Testbench テンプレート拡張
- ポート信号の宣言
- テスト信号の初期化

### Issue #4: ドキュメント完成
- README に詳細な使用例を追加
- 仕様書を完成させる

---

## 参考情報

- **strtok() の使用例**: https://cplusplus.com/reference/cstring/strtok/
- **getopt_long() の使い方**: より高度な引数処理に使用可能
- **C99 標準**: https://en.cppreference.com/w/c/99

---

## Copilot への指示例

```
「CLAUDE.md に従って、Issue #1 のコマンドライン引数解析を C99 で実装してください。
 src/template.c の parse_arguments() 関数を完成させてください。」
```

または、より詳しく:

```
「以下の機能を実装してください:
 1. コマンドライン引数 --module, --inputs, --outputs, --help をパース
 2. 無効な入力値をバリデーション
 3. エラーはステップに出力して exit(1)
 4. ModuleConfig 構造体に値を格納」
```

---

## このドキュメントについて

- 最終更新: 2026/03/20
- プロジェクト状態: Issue #1 実装中
- 次回レビュー予定: Issue #1 完了後
