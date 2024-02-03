#ifndef SERIAL_H
#define SERIAL_H

#define COM_PORT_1 0x3f8 // Communication Port 1

int init_serial();
void write_serial(const char * str);
int is_transmit_empty();
void pserial(const char * str);

#endif