# RTL Generator

C言語を使用して Verilog RTL コードを自動生成するツール

## 概要
自動的に Verilog モジュールとテストベンチを生成し、
ハードウェア設計の効率化を実現します。

## 機能
- Verilog RTL モジュール生成
- Testbench テンプレート生成
- カスタマイズ可能なパラメータ設定
- コマンドライン引数対応

## 使用言語
- C言語

## プロジェクト構成
```
rtl-generator/
├── src/              # ソースコード
├── include/          # ヘッダファイル
├── test/             # テストコード
├── examples/         # 使用例
├── docs/             # ドキュメント
├── Makefile
└── .gitignore
```

## 開発予定
- 2026年3月～6月：設計・準備
- 2026年7月：PC 到着後、開発開始予定

## 使用例
```bash
# 基本的な使用方法
$ rtl-gen --module adder --inputs 32,32 --output 33

# 出力: adder.v, adder_tb.v が生成される
```

## ライセンス
MIT


## Development Roadmap

### Week 1: 環境構築 (July 2026)
- VS Code + C言語開発環境セットアップ
- GitHub Copilot インストール
- リポジトリをクローン
- Makefile テスト

### Week 2-3: コア機能実装 (July 2026)
- Issue #1: コマンドライン引数処理
- Issue #2: Verilog RTL 生成
- Issue #3: Testbench 生成

### Week 4+: 完成・拡張 (August 2026+)
- Issue #4: ドキュメント完成
- テスト・デバッグ
- GitHub Copilot で効率化

