#include "console.h"
#include "vga.h"
#include "read.h"
#include "syspw.h"
#include "panic.h"
#include "keyboard.h"
#include "string.h"
#include "libc.h"

#define MAX_BUFFER_SIZE 128
char input_buffer[MAX_BUFFER_SIZE];
int buffer_index = 0;
int clearint = 0;

void execute_command(const char* command) {
    // Pre-built commands
    if (strcmp(command, "") == 0) {
        // Do nothing for an empty command
    } else if (strcmp(command, "clear") == 0) {
        rows = 0;
        rows--;
        console_init(COLOR_WHITE, COLOR_BLACK);
    } else if (strcmp(command, "reboot") == 0) {
        rows = 0;
        syspw(0);
    } else if (strcmp(command, "shutdown") == 0) {
        rows = 0;
        syspw(1);
    } else if (strcmp(command, "halt") == 0) {
        rows = 0;
        syspw(2);
    } else if (strncmp(command, "exec ", 5) == 0) {
        const char* command = command + 5;
        printf("\n\n");
        execute_file(&rootfs, command, 1);
        rows+=2;
    } else if (strncmp(command, "cat ", 4) == 0) {
        const char* new_directory = command + 4;
        read_from_file(&rootfs, new_directory, buffer, sizeof(buffer), 0);
        printf("\n%s\n", buffer);
        rows+=25;
    } else if (strncmp(command, "uname", 4) == 0) {
        char* workingdir = current_directory;
        current_directory = "/proc";
        read_from_file(&rootfs, "version", buffer, sizeof(buffer), 0);
        printf("\n%s\n", buffer);
        rows+=2;
        current_directory = workingdir;
    } else if (strcmp(command, "ls") == 0) {
        printf("\n");
        printf("\n");
        list_files(&rootfs, 0);
        printf("\n");
        printf("\n");
        rows+=3;
    } else if (strncmp(command, "cd ", 3) == 0) {
        // Check if the command starts with "cd "
        const char* new_directory = command + 3;  // Get the characters after "cd "
        if (strcmp(new_directory, "..") == 0) {
            cd_parent_directory();
        } else {
            // Change to the specified directory
            change_directory(&rootfs, new_directory);
        }
    } else {
        // Default action for unrecognized commands
        printf("\nUnrecognized command.\n");
        rows+=3;
    }
}

void k_sh() {
    rows = 0;
    console_gotoxy(0, rows);
    printf("[%s (%s)]# ", username, current_directory);

    while (1) {
        unsigned char scancode = read_key();

        if (scancode == 0x48) {
            return;
        }

        char key = scancode_to_char(scancode);

        if (key != 0) {
            if (scancode == SCAN_CODE_KEY_BACKSPACE) {
                if (buffer_index > 0) {
                    buffer_index--;
                    console_ungetchar();
                }
            } else if (scancode == ENTER_KEY_SCANCODE) {
                input_buffer[buffer_index] = '\0';
                execute_command(input_buffer);
                buffer_index = 0;
                
                if (rows >= 24) {
                    printf("\n");
                    rows = 24;
                } else
                    rows++;
                console_gotoxy(0, rows);
                printf("[%s (%s)]# ", username, current_directory);
            } else {
                if (buffer_index < MAX_BUFFER_SIZE - 1) {
                    input_buffer[buffer_index++] = key;
                    printf("%c", key);
                }
            }
        }
    }
}
