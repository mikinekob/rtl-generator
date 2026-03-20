#ifndef RTL_GENERATOR_H
#define RTL_GENERATOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// モジュール設定構造体
typedef struct {
    char module_name[256];
    int input_count;
    int output_count;
    int input_width[16];
    int output_width[16];
} ModuleConfig;

// 関数宣言
void parse_arguments(int argc, char *argv[], ModuleConfig *config);
void generate_verilog_module(ModuleConfig *config, FILE *output_file);
void generate_testbench(ModuleConfig *config, FILE *output_file);
void print_help(void);

#endif // RTL_GENERATOR_H