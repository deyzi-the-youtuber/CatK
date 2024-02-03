#include <io_ports.h>
#include <serial.h>
#include <types.h>
#include <console.h>

int init_serial()
{
    k_printf("init_serial() on COM1\n");
    outportb(COM_PORT_1 + 1, 0x00);    // Disable all interrupts
    outportb(COM_PORT_1 + 3, 0x80);    // Enable DLAB (set baud rate divisor)
    outportb(COM_PORT_1 + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
    outportb(COM_PORT_1 + 1, 0x00);    //                  (hi byte)
    outportb(COM_PORT_1 + 3, 0x03);    // 8 bits, no parity, one stop bit
    outportb(COM_PORT_1 + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
    outportb(COM_PORT_1 + 4, 0x0B);    // IRQs enabled, RTS/DSR set
    outportb(COM_PORT_1 + 4, 0x1E);    // Set in loopback mode, test the serial chip
    outportb(COM_PORT_1 + 0, 0xAE);    // Test serial chip (send byte 0xAE and check if serial returns same byte)
 
    // Check if serial is faulty (i.e: not same byte as sent)
    if(inportb(COM_PORT_1 + 0) != 0xAE)
    {
        return 1;
    }
 
    // If serial is not faulty set it in normal operation mode
    // (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
    outportb(COM_PORT_1 + 4, 0x0F);
    return 0;
}

int is_transmit_empty()
{
   return inportb(COM_PORT_1 + 5) & 0x20;
}

void write_serial(const char * str)
{
   while (*str != '\0')
   {
      while (is_transmit_empty() == 0);

      if (*str == '\n')
      {
         // Move to the beginning of the next line while staying at the same X position
         outportb(COM_PORT_1, '\r');  // Carriage return
         while (is_transmit_empty() == 0);
      }
      outportb(COM_PORT_1, *str);
      str++;
   }
}

void pserial(const char * str)
{ // Use const char* for the string parameter
    write_serial(str);
    write_serial("\n");
}