#include "../include/rtl_generator.h"

void parse_arguments(int argc, char *argv[], ModuleConfig *config) {
    // TODO: Issue #1 で実装予定
    // コマンドライン引数をパースして config に格納
    
    if (argc < 2) {
        print_help();
        exit(1);
    }
    
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--help") == 0) {
            print_help();
            exit(0);
        }
        // その他のオプションパース処理
    }
}