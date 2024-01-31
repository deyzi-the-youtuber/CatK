#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "include/config.h"
#include "include/fsnode.h"
#include "term.h"
#include "printk.h"
#include "panic.h"
#include "cpu.h"
#include "keyboard.h"
#include "k_sh.h"
#include "vfs.h"
#include "config.h"
#include "fsnode.h"
#include "pc.h"
#include "ramfs.h"
#include "kernel.h"
#include "multiboot.h"
#include "memory.h"

void bootart();

void kmain(unsigned long magic, unsigned long addr) 
{    
	terminal_init();                                /* This starts the terminal, prints the "Cat... kernel!"" message, and */
	printk("%CCat... ", VGA_COLOR_CYAN);			/* shows the CatK boot logo.*/
	printk("%Ckernel!\n", VGA_COLOR_LIGHT_CYAN);
	bootart();
    for (int i = 0; i < 80; ++i) {
    for (int i = 0; i < 1000; ++i) 
    {for (int j = 0; j < 1900; ++j) 
    {}}printk("#");}

    // Print static vars
	printk("Arch: %s\n", sys_arch);
	printk("Term: %s\n", sys_term);
	printk("Sesh: %s\n", sys_sesh);
	printk("Vers: %s\n", sys_ver);
	printk("Home: %s\n", sys_home);
	printk("Name: %s\n", sys_name);
	printk("Mntp: %s\n", sys_mountpoint);
    printk("\n");
    bootloader_info(magic, addr);
	cpuid_info();
    vfs_init();
    memory_init();
    createramfs();
    k_sh();
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
        printk("\n\n\nCatK(mascot) was created by Rodmatronics\n", VGA_COLOR_LIGHT_GREY);
}