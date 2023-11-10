#include "console.h"
#include "string.h"
#include "types.h"
#include "vga.h"

static uint16 *g_vga_buffer;
//index for video buffer array
static uint32 g_vga_index;
//fore & back color values
uint8 g_fore_color = COLOR_WHITE, g_back_color = COLOR_BLACK;
static uint16 g_temp_pages[MAXIMUM_PAGES][VGA_TOTAL_ITEMS];
uint32 g_current_temp_page = 0;

// clear video buffer array
void console_clear(VGA_COLOR_TYPE fore_color, VGA_COLOR_TYPE back_color) {
    uint32 i;

    for (i = 0; i < VGA_TOTAL_ITEMS; i++) {
        g_vga_buffer[i] = vga_item_entry(NULL, fore_color, back_color);
    }
    g_vga_index = 0;
    cursor_pos_x = 0;
    cursor_pos_y = 0;
    vga_set_cursor_pos(cursor_pos_x, cursor_pos_y);
}

//initialize console
void console_init(VGA_COLOR_TYPE fore_color, VGA_COLOR_TYPE back_color) {
    g_vga_buffer = (uint16 *)VGA_ADDRESS;
    g_fore_color = fore_color;
    g_back_color = back_color;
    cursor_pos_x = 0;
    cursor_pos_y = 0;
    console_clear(fore_color, back_color);
}

void console_scroll(int type) {
    uint32 i;
    if (type == SCROLL_UP) {
        // Check for newline character before rendering
        if (cursor_pos_y >= VGA_HEIGHT-1) {
            // Scroll the entire screen up by one row
            for (uint32 i = 0; i < VGA_TOTAL_ITEMS - VGA_WIDTH; i++) {
                g_vga_buffer[i] = g_vga_buffer[i + VGA_WIDTH];
            }
        }
    } else {
        // Check for newline character before rendering
        if (cursor_pos_y >= VGA_HEIGHT+1) {
            // Scroll the entire screen up by one row
            for (uint32 i = 0; i < VGA_TOTAL_ITEMS - VGA_WIDTH; i++) {
                g_vga_buffer[i] = g_vga_buffer[i + VGA_WIDTH];
            }
        }
    }
}

void scroll(int direction)
{
    if (direction == 0) // Scroll down
    {
        // Clear the first row
        uint16 clear_item = vga_item_entry(' ', g_fore_color, g_back_color);
        for (uint32 i = 0; i < VGA_WIDTH; i++) {
            g_vga_buffer[i] = clear_item;
        }

        for (uint32 i = VGA_WIDTH; i < VGA_TOTAL_ITEMS; i++) {
            g_vga_buffer[i - VGA_WIDTH] = g_vga_buffer[i];
        }
    }
    if (direction == 1) // Scroll up
    {
        // Scroll up
        for (uint32 i = 0; i > VGA_TOTAL_ITEMS + VGA_WIDTH; i--) {
            g_vga_buffer[i] = g_vga_buffer[i + VGA_WIDTH];
        }
    }
}


void console_newline() {
    // Check for newline character before rendering
    if (cursor_pos_y >= VGA_HEIGHT - 1) {
        // Scroll the entire screen up by one row
        for (uint32 i = 0; i < VGA_TOTAL_ITEMS - VGA_WIDTH; i++) {
            g_vga_buffer[i] = g_vga_buffer[i + VGA_WIDTH];
        }
        g_vga_index = (cursor_pos_y + 1) * VGA_WIDTH;
        // Update the cursor position
        cursor_pos_x = 0;
        cursor_pos_y = VGA_HEIGHT - 1;
        vga_set_cursor_pos(cursor_pos_x, cursor_pos_y);
    } else {
        // Update the cursor position
        g_vga_index = (cursor_pos_y + 1) * VGA_WIDTH;
        cursor_pos_x = 0;
        ++cursor_pos_y;
        vga_set_cursor_pos(cursor_pos_x, cursor_pos_y);
    }
}

//assign ascii character to video buffer
void console_putchar(char ch) {
    if (ch == '\t') {
        for(int i = 0; i < 4; i++) {
            g_vga_buffer[g_vga_index++] = vga_item_entry(' ', g_fore_color, g_back_color);
            vga_set_cursor_pos(cursor_pos_x++, cursor_pos_y);
        }
    } else if (ch == '\n') {
        console_newline();
    } else {
        if (ch > 0) {
            g_vga_buffer[g_vga_index++] = vga_item_entry(ch, g_fore_color, g_back_color);
            vga_set_cursor_pos(++cursor_pos_x, cursor_pos_y);
        }
    }
}

void console_putchar_dark(char ch) {
    if (ch == '\t') {
        for(int i = 0; i < 4; i++) {
            g_vga_buffer[g_vga_index++] = vga_item_entry(' ', COLOR_DARK_GREY, COLOR_BLACK);
            vga_set_cursor_pos(cursor_pos_x++, cursor_pos_y);
        }
    } else if (ch == '\n') {
        console_newline();
    } else {
        if (ch > 0) {
            g_vga_buffer[g_vga_index++] = vga_item_entry(ch, COLOR_DARK_GREY, COLOR_BLACK);
            vga_set_cursor_pos(++cursor_pos_x, cursor_pos_y);
        }
    }
}

void console_putchar_blue(char ch) {
    if (ch == '\t') {
        for(int i = 0; i < 4; i++) {
            g_vga_buffer[g_vga_index++] = vga_item_entry(' ', COLOR_BRIGHT_CYAN, COLOR_BLACK);
            vga_set_cursor_pos(cursor_pos_x++, cursor_pos_y);
        }
    } else if (ch == '\n') {
        console_newline();
    } else {
        if (ch > 0) {
            g_vga_buffer[g_vga_index++] = vga_item_entry(ch, COLOR_BRIGHT_CYAN, COLOR_BLACK);
            vga_set_cursor_pos(++cursor_pos_x, cursor_pos_y);
        }
    }
}


// revert back the printed character and add 0 to it
void console_ungetchar() {
    if(g_vga_index > 0) {
        g_vga_buffer[g_vga_index--] = vga_item_entry(0, g_fore_color, g_back_color);
        if(cursor_pos_x > 0) {
            vga_set_cursor_pos(cursor_pos_x--, cursor_pos_y);
        } else {
            cursor_pos_x = VGA_WIDTH;
            if (cursor_pos_y > 0)
                vga_set_cursor_pos(cursor_pos_x--, --cursor_pos_y);
            else
                cursor_pos_y = 0;
        }
    }

    // set last printed character to 0
    g_vga_buffer[g_vga_index] = vga_item_entry(0, g_fore_color, g_back_color);
}

// revert back the printed character until n characters
void console_ungetchar_bound(uint8 n) {
    if(((g_vga_index % VGA_WIDTH) > n) && (n > 0)) {
        g_vga_buffer[g_vga_index--] = vga_item_entry(0, g_fore_color, g_back_color);
        if(cursor_pos_x >= n) {
            vga_set_cursor_pos(cursor_pos_x--, cursor_pos_y);
        } else {
            cursor_pos_x = VGA_WIDTH;
            if (cursor_pos_y > 0)
                vga_set_cursor_pos(cursor_pos_x--, --cursor_pos_y);
            else
                cursor_pos_y = 0;
        }
    }

    // set last printed character to 0
    g_vga_buffer[g_vga_index] = vga_item_entry(0, g_fore_color, g_back_color);
}

void console_gotoxy(uint16 x, uint16 y) {
    g_vga_index = (80 * y) + x;
    cursor_pos_x = x;
    cursor_pos_y = y;
    vga_set_cursor_pos(cursor_pos_x, cursor_pos_y);
}

void console_gotox(uint16 x) {
    g_vga_index + x;
    cursor_pos_x = x;
    vga_set_cursor_pos(cursor_pos_x, cursor_pos_y);
}

//print string by calling print_char
void console_putstr(const char *str) {
    uint32 index = 0;
    while (str[index]) {
        if (str[index] == '\n')
            console_newline();
        else
            console_putchar(str[index]);
        index++;
    }
}

void printf(const char *format, ...) {
    char **arg = (char **)&format;
    int c;
    char buf[32];

    arg++;

    memset(buf, 0, sizeof(buf));
    while ((c = *format++) != 0) {
        if (c != '%')
            console_putchar(c);
        else {
            char *p, *p2;
            int pad0 = 0, pad = 0;

            c = *format++;
            if (c == '0') {
                pad0 = 1;
                c = *format++;
            }

            if (c >= '0' && c <= '9') {
                pad = c - '0';
                c = *format++;
            }

            switch (c) {
                case 'd':
                case 'u':
                case 'x':
                    itoa(buf, c, *((int *)arg++));
                    p = buf;
                    goto string;
                    break;

                case 's':
                    p = *arg++;
                    if (!p)
                        p = "(null)";

                string:
                    for (p2 = p; *p2; p2++)
                        ;
                    for (; p2 < p + pad; p2++)
                        console_putchar(pad0 ? '0' : ' ');
                    while (*p)
                        console_putchar(*p++);
                    break;

                default:
                    console_putchar(*((int *)arg++));
                    break;
            }
        }
    }
}

void printf_dark(const char *format, ...) {
    char **arg = (char **)&format;
    int c;
    char buf[32];

    arg++;

    memset(buf, 0, sizeof(buf));
    while ((c = *format++) != 0) {
        if (c != '%')
            console_putchar_dark(c);
        else {
            char *p, *p2;
            int pad0 = 0, pad = 0;

            c = *format++;
            if (c == '0') {
                pad0 = 1;
                c = *format++;
            }

            if (c >= '0' && c <= '9') {
                pad = c - '0';
                c = *format++;
            }

            switch (c) {
                case 'd':
                case 'u':
                case 'x':
                    itoa(buf, c, *((int *)arg++));
                    p = buf;
                    goto string;
                    break;

                case 's':
                    p = *arg++;
                    if (!p)
                        p = "(null)";

                string:
                    for (p2 = p; *p2; p2++)
                        ;
                    for (; p2 < p + pad; p2++)
                        console_putchar_dark(pad0 ? '0' : ' ');
                    while (*p)
                        console_putchar_dark(*p++);
                    break;

                default:
                    console_putchar_dark(*((int *)arg++));
                    break;
            }
        }
    }
}

void printf_blue(const char *format, ...) {
    char **arg = (char **)&format;
    int c;
    char buf[32];

    arg++;

    memset(buf, 0, sizeof(buf));
    while ((c = *format++) != 0) {
        if (c != '%')
            console_putchar_blue(c);
        else {
            char *p, *p2;
            int pad0 = 0, pad = 0;

            c = *format++;
            if (c == '0') {
                pad0 = 1;
                c = *format++;
            }

            if (c >= '0' && c <= '9') {
                pad = c - '0';
                c = *format++;
            }

            switch (c) {
                case 'd':
                case 'u':
                case 'x':
                    itoa(buf, c, *((int *)arg++));
                    p = buf;
                    goto string;
                    break;

                case 's':
                    p = *arg++;
                    if (!p)
                        p = "(null)";

                string:
                    for (p2 = p; *p2; p2++)
                        ;
                    for (; p2 < p + pad; p2++)
                        console_putchar_blue(pad0 ? '0' : ' ');
                    while (*p)
                        console_putchar_blue(*p++);
                    break;

                default:
                    console_putchar_blue(*((int *)arg++));
                    break;
            }
        }
    }
}
void printf_green(const char *format, ...) {
    char **arg = (char **)&format;
    int c;
    char buf[32];

    arg++;

    memset(buf, 0, sizeof(buf));
    while ((c = *format++) != 0) {
        if (c != '%')
            console_putchar_green(c);
        else {
            char *p, *p2;
            int pad0 = 0, pad = 0;

            c = *format++;
            if (c == '0') {
                pad0 = 1;
                c = *format++;
            }

            if (c >= '0' && c <= '9') {
                pad = c - '0';
                c = *format++;
            }

            switch (c) {
                case 'd':
                case 'u':
                case 'x':
                    itoa(buf, c, *((int *)arg++));
                    p = buf;
                    goto string;
                    break;

                case 's':
                    p = *arg++;
                    if (!p)
                        p = "(null)";

                string:
                    for (p2 = p; *p2; p2++)
                        ;
                    for (; p2 < p + pad; p2++)
                        console_putchar_green(pad0 ? '0' : ' ');
                    while (*p)
                        console_putchar_green(*p++);
                    break;

                default:
                    console_putchar_green(*((int *)arg++));
                    break;
            }
        }
    }
}

void console_putchar_green(char ch) {
    if (ch == '\t') {
        for(int i = 0; i < 4; i++) {
            g_vga_buffer[g_vga_index++] = vga_item_entry(' ', COLOR_BRIGHT_GREEN, COLOR_BLACK);
            vga_set_cursor_pos(cursor_pos_x++, cursor_pos_y);
        }
    } else if (ch == '\n') {
        console_newline();
    } else {
        if (ch > 0) {
            g_vga_buffer[g_vga_index++] = vga_item_entry(ch, COLOR_BRIGHT_GREEN, COLOR_BLACK);
            vga_set_cursor_pos(++cursor_pos_x, cursor_pos_y);
        }
    }
}