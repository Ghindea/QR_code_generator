#pragma once
#include "config.h" // config file

// input string maxlen for mem alloc depending on version
#if version < 15    
#define MAXLEN 500
#elif version >= 15 && version < 30 
#define MAXLEN 1800
#else
#define MAXLEN 3000
#endif

// path to loadable file depending on data type
#if data_type == 1  
#define mode_path "utils/type/numeric_mode_capacity.txt"
#elif data_type == 2
#define mode_path "utils/type/alphanumeric_mode_capacity.txt"
#elif data_type == 3
#define mode_path "utils/type/byte_mode_capacity.txt"
#else
#define mode_path "utils/type/kanji_mode_capacity.txt"
#endif

// color format to display
#define COLOR_RED       "\x1b[31m"
#define COLOR_RESET     "\x1b[0m"
#define UNDERLINE       "\e[4m"
#define BOLD            "\e[1m"
#define COLOR_OFF       "\e[m"


//dg