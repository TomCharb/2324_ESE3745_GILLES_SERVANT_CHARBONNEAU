/**
 *@file shell.h
 *@brief Shell permettant de controller le moteur
 *@date Oct 1, 2023
 *@author Nicolas
 */

#ifndef INC_MYLIBS_SHELL_H_
#define INC_MYLIBS_SHELL_H_

#define UART_RX_BUFFER_SIZE 1
#define UART_TX_BUFFER_SIZE 100
#define CMD_BUFFER_SIZE 64
#define MAX_ARGS 9
#define ASCII_LF 0x0A			// LF = line feed, saut de ligne
#define ASCII_CR 0x0D			// CR = carriage return, retour chariot
#define ASCII_BACK 0x08			// BACK = Backspace

void Shell_Init(void);
void Shell_Loop(void);

#endif /* INC_MYLIBS_SHELL_H_ */
