#ifndef ANSI_CODES_H
#define ANSI_CODES_H

// Reset and Text Formatting Codes
#define RESET               "\033[0m"
#define BOLD                "\033[1m"
#define DIM                 "\033[2m"
#define ITALIC              "\033[3m"
#define UNDERLINE           "\033[4m"
#define BLINK               "\033[5m"
#define REVERSE             "\033[7m"
#define HIDDEN              "\033[8m"
#define STRIKETHROUGH       "\033[9m"

#define BOLD_OFF            "\033[22m"
#define UNDERLINE_OFF       "\033[24m"
#define BLINK_OFF           "\033[25m"
#define REVERSE_OFF         "\033[27m"
#define VISIBLE             "\033[28m"
#define STRIKETHROUGH_OFF   "\033[29m"

// Foreground Colors
#define BLACK               "\033[30m"
#define RED                 "\033[31m"
#define GREEN               "\033[32m"
#define YELLOW              "\033[33m"
#define BLUE                "\033[34m"
#define MAGENTA             "\033[35m"
#define CYAN                "\033[36m"
#define WHITE               "\033[37m"

// Bright Foreground Colors
#define BRIGHT_BLACK        "\033[90m"
#define BRIGHT_RED          "\033[91m"
#define BRIGHT_GREEN        "\033[92m"
#define BRIGHT_YELLOW       "\033[93m"
#define BRIGHT_BLUE         "\033[94m"
#define BRIGHT_MAGENTA      "\033[95m"
#define BRIGHT_CYAN         "\033[96m"
#define BRIGHT_WHITE        "\033[97m"

// Background Colors
#define BG_BLACK            "\033[40m"
#define BG_RED              "\033[41m"
#define BG_GREEN            "\033[42m"
#define BG_YELLOW           "\033[43m"
#define BG_BLUE             "\033[44m"
#define BG_MAGENTA          "\033[45m"
#define BG_CYAN             "\033[46m"
#define BG_WHITE            "\033[47m"

// Bright Background Colors
#define BG_BRIGHT_BLACK     "\033[100m"
#define BG_BRIGHT_RED       "\033[101m"
#define BG_BRIGHT_GREEN     "\033[102m"
#define BG_BRIGHT_YELLOW    "\033[103m"
#define BG_BRIGHT_BLUE      "\033[104m"
#define BG_BRIGHT_MAGENTA   "\033[105m"
#define BG_BRIGHT_CYAN      "\033[106m"
#define BG_BRIGHT_WHITE     "\033[107m"

// 256-Color Foreground (Range 0-255)
#define FG_256_COLOR(n)     "\033[38;5;" #n "m"

// 256-Color Background (Range 0-255)
#define BG_256_COLOR(n)     "\033[48;5;" #n "m"

// Cursor Control
#define MOVE_CURSOR(x, y)   "\033[" #x ";" #y "H"
#define SAVE_CURSOR          "\033[s"
#define RESTORE_CURSOR       "\033[u"
#define MOVE_UP(n)           "\033[" #n "A"
#define MOVE_DOWN(n)         "\033[" #n "B"
#define MOVE_FORWARD(n)      "\033[" #n "C"
#define MOVE_BACKWARD(n)     "\033[" #n "D"
#define CLEAR_SCREEN         "\033[2J"
#define CLEAR_LINE           "\033[K"
#define CLEAR_FROM_CURSOR    "\033[J"
#define CLEAR_TO_CURSOR      "\033[1K"
#define CLEAR_TO_END_LINE    "\033[K"

// Miscellaneous
#define HIDE_CURSOR          "\033[?25l"
#define SHOW_CURSOR          "\033[?25h"

#define TITLE_COLOR "\033[1;33m"   // Bold Yellow
#define SUCCESS_COLOR "\033[1;32m" // Bright Green
#define ERROR_COLOR "\033[1;31m"   // Bright Red

#endif // ANSI_CODES_H

