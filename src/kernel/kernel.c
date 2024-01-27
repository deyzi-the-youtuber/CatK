#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "term.h"
#include "printk.h"
#include "panic.h"
#include "cpu.h"
#include "keyboard.h"

static char * sys_arch = "X86/intel";
static char * sys_term = "80x25";
static char * sys_sesh = "/bin/k_sh";
static char * sys_ver = "0.07";
static char * sys_home = "/home/root";
static char * sys_name = "Catkernel";

void bootart();

void kmain() 
{

	terminal_init();                                /* This starts the terminal, prints the "Cat... kernel!"" message, and */
	printk("%CCat... ", VGA_COLOR_CYAN);			/* shows the CatK boot logo.*/
	printk("%Ckernel!\n", VGA_COLOR_LIGHT_CYAN);
	bootart();
    // Print static vars
	printk("Arch: %s\n", sys_arch);
	printk("Term: %s\n", sys_term);
	printk("Sesh: %s\n", sys_sesh);
	printk("Vers: %s\n", sys_ver);
	printk("Home: %s\n", sys_home);
	printk("Name: %s\n", sys_name);
    printk("\n");
	cpuid_info();

    // Take in keyboard inputs (no shell started, default action)
    while (1) {
        // Read a key scancode
        unsigned char scancode = read_key();

        // Convert the scancode to a character
        char key = scancode_to_char(scancode);

        // Check if a valid character was returned
        if (key != 0) {
            // Print the character to the console
            if (scancode == SCAN_CODE_KEY_ENTER) {
                printk("");
            }
            printk("%c", key);
        }
    
    }
}


void bootart()
{
        printk("%C           __           __             \n",VGA_COLOR_WHITE);
        printk("%C          /  \\         /  \\        \n", VGA_COLOR_WHITE);
        printk("%C         / /\\ \\       / /\\ \\       \n", VGA_COLOR_LIGHT_CYAN);
        printk("%C        / /  \\ \\     / /  \\ \\      \n", VGA_COLOR_LIGHT_CYAN);
        printk("%C       / /      \\___/      \\ \\         _______   _____   _______  ___   _\n", VGA_COLOR_LIGHT_CYAN);
        printk("%C      /                       \\       |   ____| /  _  \\ |       ||   | | |\n", VGA_COLOR_LIGHT_CYAN);
        printk("%C     |        |      |         |      |  |     |  | |  ||_     _||   |_| |\n", VGA_COLOR_LIGHT_CYAN);
        printk("%C   ---        |      |         ---    |  |     |  |_|  |  |   |  |      _|\n", VGA_COLOR_LIGHT_CYAN);
        printk("%C     |                         |      |  |     |       |  |   |  |     |_ \n", VGA_COLOR_LIGHT_CYAN);
        printk("%C   ---  ", VGA_COLOR_LIGHT_CYAN);
        printk("%C//", VGA_COLOR_CYAN);
        printk("%C       ^       ", VGA_COLOR_LIGHT_CYAN);
        printk("%C//", VGA_COLOR_CYAN);
        printk("%C    ---    |  |____ |   _   |  |   |  |    _  |\n", VGA_COLOR_LIGHT_CYAN);
        printk("%C      \\         \\/\\/          /       |_______||__| |__|  |___|  |___| |_|\n", VGA_COLOR_LIGHT_CYAN);
        printk(" %C      \\                     /        Written from scratch by the team! :3 \n", VGA_COLOR_CYAN);
        printk("%C        \\___________________/      \n", VGA_COLOR_CYAN);
        printk("%C         ===================       \n", VGA_COLOR_LIGHT_RED);
        printk("%C        =========", VGA_COLOR_RED);
        printk("%C\\/", VGA_COLOR_LIGHT_BROWN);
        printk("%C==========      \n", VGA_COLOR_RED);
        printk("%C                /  \\               \n", VGA_COLOR_LIGHT_BROWN);
        printk("%C               |catk|              \n", VGA_COLOR_LIGHT_BROWN);
        printk("%C                \\__/               \n", VGA_COLOR_LIGHT_BROWN);
		printk("%C", VGA_COLOR_LIGHT_GREY);
}