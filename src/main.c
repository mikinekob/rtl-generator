#include "../include/rtl_generator.h"

int main(int argc, char *argv[]) {
    ModuleConfig config;
    FILE *rtl_file, *tb_file;
    char rtl_filename[256], tb_filename[256];
    
    // デフォルト値を設定
    memset(&config, 0, sizeof(config));
    strcpy(config.module_name, "default_module");
    config.input_count = 1;
    config.output_count = 1;
    config.input_width[0] = 32;
    config.output_width[0] = 32;
    
    // コマンドライン引数を解析（Issue #1）
    // parse_arguments(argc, argv, &config);
    
    // RTL ファイルを生成
    sprintf(rtl_filename, "%s.v", config.module_name);
    rtl_file = fopen(rtl_filename, "w");
    if (rtl_file == NULL) {
        perror("Error opening RTL file");
        return 1;
    }
    
    generate_verilog_module(&config, rtl_file);
    fclose(rtl_file);
    
    // Testbench ファイルを生成
    sprintf(tb_filename, "%s_tb.v", config.module_name);
    tb_file = fopen(tb_filename, "w");
    if (tb_file == NULL) {
        perror("Error opening testbench file");
        return 1;
    }
    
    generate_testbench(&config, tb_file);
    fclose(tb_file);
    
    printf("Generated: %s\n", rtl_filename);
    printf("Generated: %s\n", tb_filename);
    
    return 0;
}