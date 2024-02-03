#include <fs.h>
#include <config.h>
#include <cpu.h>
#include <console.h>
#include <string.h>
#include <serial.h>

void vfs_init()
{
    // Initialize the file table
    for (size i = 0; i < MAX_FILES; ++i) {
        rootfs.file_table[i].filename[0] = '\0';  // Empty filename indicates an unused entry
    }

    k_printf("Creating '/' structure\n");
    create_folder(&rootfs, "bin", "/");
    create_folder(&rootfs, "boot", "/");
    create_folder(&rootfs, "cdrom", "/");
    create_folder(&rootfs, "dev", "/");
    create_folder(&rootfs, "etc", "/");
    create_folder(&rootfs, "home", "/");
    create_folder(&rootfs, "lib", "/");
    create_folder(&rootfs, "media", "/");
    create_folder(&rootfs, "mnt", "/");
    create_folder(&rootfs, "mount", "/");
    create_folder(&rootfs, "proc", "/");
    create_folder(&rootfs, "run", "/");
    create_folder(&rootfs, "sbin", "/");
    create_folder(&rootfs, "sys", "/");
    create_folder(&rootfs, "tmp", "/");
    create_folder(&rootfs, "usr", "/");
    create_folder(&rootfs, "var", "/");

    create_folder(&rootfs, "ksh", "/etc");
    create_folder(&rootfs, "bin", "/usr");
    create_folder(&rootfs, "local", "/usr");
    create_folder(&rootfs, "share", "/usr");

    current_directory = "/etc";
    write_to_file(&rootfs, "launchp", "");
    add_data_to_file(&rootfs, "launchp", "sh\n");
    k_printf("Dumping kernel values to /proc\n");
    pserial("Dumping kernel values to /proc");
    current_directory = "/proc";
    write_to_file(&rootfs, "buff", buffer);
    write_to_file(&rootfs, "buff-second", buffer2);
    k_printf("Dumping apps to /bin\n");
    pserial("Dumping apps to /bin");
    current_directory = "/bin";
    write_to_file(&rootfs, "print", "type:App\nprint");
    write_to_file(&rootfs, "clear", "type:App\nclear");
    write_to_file(&rootfs, "delay", "type:App\ndelay");
    current_directory = "/sbin";
    write_to_file(&rootfs, "sh", "type:App\nsh");
    k_printf("Created /sbin/sh\n");
    write_to_file(&rootfs, "shutdown", "type:App\nshutdown");
    k_printf("Created /sbin/shutdown\n");
    write_to_file(&rootfs, "reboot", "type:App\nreboot");
    k_printf("Created /sbin/reboot\n");
    k_printf("Setting hostname to defaults from 'defaulthostname'\n");
    current_directory = "/etc";
    write_to_file(&rootfs, "hostname", defaulthostname);
    read_from_file(&rootfs, "hostname", buffer, sizeof(buffer), 1);
    k_printf("Setting hostname to defaults from 'defaulthostname'\n");
    current_directory = "/etc";
    write_to_file(&rootfs, "hostname", defaulthostname);
    read_from_file(&rootfs, "hostname", buffer, sizeof(buffer), 1);
    strcpy(host_name, buffer);
    k_printf("Copying kernel values to proc\n");
    current_directory = "/proc";
    write_to_file(&rootfs, "arch", arch);
    write_to_file(&rootfs, "version", vername);
    add_data_to_file(&rootfs, "version", versionnumber);
    write_to_file(&rootfs, "versionnum", versionnumber);
    write_to_file(&rootfs, "ostype", "Catkernel");
    write_to_file(&rootfs, "cpu", brand);
    write_to_file(&rootfs, "vga", "80x25");
    current_directory = "/";
    write_to_file(&rootfs, "desk", "type:App\ngraphics_init");
}