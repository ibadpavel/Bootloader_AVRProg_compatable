/*****************************************************************************
*
* AVRPROG compatible boot-loader
* Version  : 0.85 (Dec. 2008)
* Compiler : avr-gcc 4.1.2 / avr-libc 1.4.6
* size     : depends on features and startup ( minmal features < 512 words)
* by       : Martin Thomas, Kaiserslautern, Germany
*            eversmith@heizung-thomas.de
*            Additional code and improvements contributed by:
*           - Uwe Bonnes
*           - Bjoern Riemer
*           - Olaf Rempel
*
* License  : Copyright (c) 2006-2008 M. Thomas, U. Bonnes, O. Rempel
*            Free to use. You have to mention the copyright
*            owners in source-code and documentation of derived
*            work. No warranty! (Yes, you can insert the BSD
*            license here)
*
* Tested with ATmega8, ATmega16, ATmega162, ATmega32, ATmega324P,
*             ATmega644, ATmega644P, ATmega128, AT90CAN128
*
* - Initial versions have been based on the Butterfly bootloader-code
*   by Atmel Corporation (Authors: BBrandal, PKastnes, ARodland, LHM)
*
****************************************************************************
*
*  See the makefile and readme.txt for information on how to adapt 
*  the linker-settings to the selected Boot Size (BOOTSIZE=xxxx) and 
*  the MCU-type. Other configurations futher down in this file.
*
*  With BOOT_SIMPLE, minimal features and discarded int-vectors
*  this bootloader has should fit into a a 512 word (1024, 0x400 bytes) 
*  bootloader-section. 
*
****************************************************************************/
// ATMega8
// (BOOTSIZE=128)
// MT_BOOTLOADER_ADRESS = 0x1F00
// (BOOTSIZE=256)
// MT_BOOTLOADER_ADRESS = 0x1E00
// (BOOTSIZE=512)
// MT_BOOTLOADER_ADRESS = 0x1C00
// (BOOTSIZE=1024)
// MT_BOOTLOADER_ADRESS = 0x1800

// Тактовая частота контроллера (кварца)
#ifndef F_CPU
#define F_CPU 12000000
//#define F_CPU 8000000
#endif

//	Скорость UART
//#define BAUDRATE 9600
#define BAUDRATE 19200

// передача через RS485
//#define USE_RS485
//#define RS485_DDR	DDRD
//#define RS485_PORT	PORTD
//#define RS485_PNUM	2


/* Режим двойной скорости UART (бит U2C)*/
//#define UART_DOUBLESPEED

/* Используется второй UART на mega128 / can128 / mega162 / mega324p / mega644p */
//#define UART_USE_SECOND

/* Тип устройства:
   Для AVRProg выбирать BOOT 
   Это корректное значение для bootloader.
   avrdude может определить только part-code для ISP */
#define DEVTYPE     DEVTYPE_BOOT
//#define DEVTYPE     DEVTYPE_ISP

// Выбор порта для кнопки входа в загрузчик
// Чтобы войти в загрузчик надо чтобы при запуске эта кнопка замыкала пин на землю
#define BLPORT		PORTD
#define BLDDR		DDRD
#define BLPIN		PIND
#define BLPNUM		PIND2

// Выбор порта для индикатора работы загрузчика
// Светодиод горит - мы в загрузчике
#define ENABLE_BOOT_LED
#define BIPORT		PORTB
#define BIDDR		DDRB
#define BIPIN		PINB
#define BIPNUM		PINB2

// Выключить Собачий таймер на время загрузчика
#define DISABLE_WDT_AT_STARTUP

/*	Watchdog-reset is issued at exit 
	define the timeout-value here (see avr-libc manual)*/
#define EXIT_WDT_TIME   WDTO_250MS

/*
 * Выбор режима загрузчика
 * SIMPLE-Mode - Загрузчик стартует когда нажата его кнопка
 *   переход к основной программе осуществляется после сброса 
 *   (кнопка должна быть отжата) либо по команде от программатора
 *   При этом режиме вывод на кнопку конфигурируется как вход-с подтягом,
 *   но при выходе из загрузчика все выставляется по умолчанию
 * POWERSAVE-Mode - Startup is separated in two loops
 *   which makes power-saving a little easier if no firmware
 *   is on the chip. Needs more memory
 * BOOTICE-Mode - для зашивки  JTAGICE файла upgrade.ebn в Мегу16.
 *   что превращает ее в JTAG отладчик. Разумеется нужно добавить весь необходимый
 *   обвяз на кристалл для этого. И частота должна быть везде прописана как 7372800
 *   в F_CPU Для совместимости с родной прошивкой JTAG ICE
 * WAIT-mode Bootloader ожидает команды на вход, если ее не было в течении промежутка времени
 *   (который настраивается) то проихсодит переход к основной программе.
 */
#define START_SIMPLE
//#define START_WAIT
//#define START_POWERSAVE
//#define START_BOOTICE

/* Команда для входа в загрузчик в START_WAIT */
//#define START_WAIT_UARTCHAR 'S'

/* Выдержка для START_WAIT mode ( t = WAIT_TIME * 10ms ) */
//#define WAIT_VALUE 400

/*
 * enable/disable readout of fuse and lock-bits
 * (AVRPROG has to detect the AVR correctly by device-code
 * to show the correct information).
 */
//#define ENABLEREADFUSELOCK

/* enable/disable write of lock-bits
 * WARNING: lock-bits can not be reseted by bootloader (as far as I know)
 * Only protection no unprotection, "chip erase" from bootloader only
 * clears the flash but does no real "chip erase" (this is not possible
 * with a bootloader as far as I know)
 * Keep this undefined!
 */
//#define WRITELOCKBITS

/*
 * define the following if the bootloader should not output
 * itself at flash read (will fake an empty boot-section)
 */
//#define READ_PROTECT_BOOTLOADER 

#define VERSION_HIGH '0'
#define VERSION_LOW  '8'

#define GET_LOCK_BITS           0x0001
#define GET_LOW_FUSE_BITS       0x0000
#define GET_HIGH_FUSE_BITS      0x0003
#define GET_EXTENDED_FUSE_BITS  0x0002

/* Расчет делителя частоты для USART*/
#ifdef UART_DOUBLESPEED

	#define UART_CALC_BAUDRATE(baudRate) ((uint32_t)((F_CPU) + ((uint32_t)baudRate * 4UL)) / ((uint32_t)(baudRate) * 8UL) - 1)

#else

	#define UART_CALC_BAUDRATE(baudRate) ((uint32_t)((F_CPU) + ((uint32_t)baudRate * 8UL)) / ((uint32_t)(baudRate) * 16UL) - 1)

#endif


#include <stdint.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/boot.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "Bits.h"

#include "chipdef.h"
 
uint8_t gBuffer[SPM_PAGESIZE];

static void sendchar(uint8_t data)
{
	while (!(UART_STATUS & (1<<UART_TXREADY)));
	UART_DATA = data;
}

static uint8_t recvchar(void)
{
	while (!(UART_STATUS & (1<<UART_RXREADY)));
	return UART_DATA;
}

static inline void eraseFlash(void)
{
	// erase only main section (bootloader protection)
	uint32_t addr = 0;
	while (APP_END > addr) 
	{
		boot_page_erase(addr);		// Perform page erase
		boot_spm_busy_wait();		// Wait until the memory is erased.
		addr += SPM_PAGESIZE;
	}
	boot_rww_enable();
}

static inline void recvBuffer(pagebuf_t size)
{
	pagebuf_t cnt;
	uint8_t *tmp = gBuffer;

	for (cnt = 0; cnt < sizeof(gBuffer); cnt++) 
	{
		*tmp++ = (cnt < size) ? recvchar() : 0xFF;
	}
}

static inline uint16_t writeFlashPage(uint16_t waddr, pagebuf_t size)
{
	uint32_t pagestart = (uint32_t)waddr<<1;
	uint32_t baddr = pagestart;
	uint16_t data;
	uint8_t *tmp = gBuffer;

	do 
	{
		data = *tmp++;
		data |= *tmp++ << 8;
		boot_page_fill(baddr, data);	// call asm routine.

		baddr += 2;			// Select next word in memory
		size -= 2;			// Reduce number of bytes to write by two
	} 
	while (size);				// Loop until all bytes written

	boot_page_write(pagestart);
	boot_spm_busy_wait();
	boot_rww_enable();		// Re-enable the RWW section

	return baddr>>1;
}

// static inline uint16_t writeEEpromPage(uint16_t address, pagebuf_t size)
// {
// 	uint8_t *tmp = gBuffer;
// 
// 	do 
// 	{
// 		eeprom_write_byte( (uint8_t*)address, *tmp++ );
// 		address++;			// Select next byte
// 		size--;				// Decreas number of bytes to write
// 	}
// 	while (size);				// Loop until all bytes written
// 
// 	// eeprom_busy_wait();
// 
// 	return address;
// }
// 
// static inline uint16_t readFlashPage(uint16_t waddr, pagebuf_t size)
// {
// 	uint32_t baddr = (uint32_t)waddr<<1;
// 	uint16_t data;
// 
// 	do 
// 	{		
// 		sendchar(data);			// send LSB
// 		sendchar((data >> 8));		// send MSB
// 
// 		baddr += 2;			// Select next word in memory
// 		size -= 2;			// Subtract two bytes from number of bytes to read
// 	} 
// 	while (size);				// Repeat until block has been read
// 	
// 	return baddr>>1;
// }
// 
// static inline uint16_t readEEpromPage(uint16_t address, pagebuf_t size)
// {
// 	do 
// 	{
// 		sendchar( eeprom_read_byte( (uint8_t*)address ) );
// 		address++;
// 		size--;				// Decrease number of bytes to read
// 	} 
// 	while (size);				// Repeat until block has been read
// 
// 	return address;
// }

static void (*jump_to_app)(void) = 0x0000;

int main(void)
{
	uint16_t address = 0;
	uint8_t device = 0, val;

#ifdef ENABLE_BOOT_LED	// LED ON
	BIPORT |= (1<<BIPNUM);	
	BIDDR  |= (1<<BIPNUM);
#endif

#ifdef DISABLE_WDT_AT_STARTUP
	#ifdef WDT_OFF_SPECIAL
		#warning "using target specific watchdog_off"
		bootloader_wdt_off();
	#else
		cli();
		wdt_reset();
		wdt_disable();
	#endif
#endif

	BLDDR  &= ~(1<<BLPNUM);		// set as Input
	BLPORT |= (1<<BLPNUM);		// Enable pullup

	// Set baud rate
	UART_BAUD_HIGH = (UART_CALC_BAUDRATE(BAUDRATE)>>8) & 0xFF;
	UART_BAUD_LOW = (UART_CALC_BAUDRATE(BAUDRATE) & 0xFF);

	UART_CTRL = UART_CTRL_DATA;
	UART_CTRL2 = UART_CTRL2_DATA;
	
	sbi(DDRD, 1);	// установить ножку TXD на выход

	for(;;) 
	{
		val = recvchar();
		// Autoincrement?
		if (val == 'a') {
			sendchar('Y');			// Autoincrement is quicker
		}
		//write address
		else if (val == 'A') {
			address = recvchar();		//read address 8 MSB
			address = (address<<8) | recvchar();
			sendchar('\r');
		}
		// Buffer load support
		else if (val == 'b') {
			sendchar('Y');								// Report buffer load supported
			sendchar((sizeof(gBuffer) >> 8) & 0xFF);	// Report buffer size in bytes
			sendchar(sizeof(gBuffer) & 0xFF);
		}
		// Start buffer load
		else if (val == 'B') 
		{
			pagebuf_t size;
			size = recvchar() << 8;						// Load high byte of buffersize
			size |= recvchar();							// Load low byte of buffersize
			val = recvchar();							// Load memory type ('E' or 'F')
			recvBuffer(size);

			if (device == DEVTYPE) {
				if (val == 'F') {
					address = writeFlashPage(address, size);
				} 
				//else if (val == 'E') {
				//	address = writeEEpromPage(address, size);
				//}
				sendchar('\r');
			} else {
				sendchar(0);
			}
		}
		// Block read
// 		else if (val == 'g') 
// 		{
// 			pagebuf_t size;
// 			size = recvchar() << 8;				// Load high byte of buffersize
// 			size |= recvchar();				// Load low byte of buffersize
// 			val = recvchar();				// Get memtype
// 
// 			if (val == 'F') {
// 				address = readFlashPage(address, size);
// 			} 
// 			else if (val == 'E')  {
// 				address = readEEpromPage(address, size);
// 			}
//  		}
		// Chip erase 
		else if (val == 'e') 
		{
			if (device == DEVTYPE) {
				eraseFlash();
			}
			sendchar('\r');	
		}
		// Exit upgrade
		else if (val == 'E') 
		{
			wdt_enable(EXIT_WDT_TIME); // Enable Watchdog Timer to give reset
			sendchar('\r');
		}
		// Enter programming mode
		// Leave programming mode
		else if (val == 'P' || val == 'L') 
		{
			sendchar('\r');
		}
		// return programmer type
		else if (val == 'p') 
		{
			sendchar('S');		// always serial programmer
		}
		// Return device type
		else if (val == 't') {
			sendchar(DEVTYPE);
			sendchar(0);
		}
		// clear and set LED ignored
		else if ((val == 'x') || (val == 'y')) {
			recvchar();
			sendchar('\r');
		}
		// set device
		else if (val == 'T') {
			device = recvchar();
			sendchar('\r');
		}
		// Return Signature Bytes (it seems that
		// AVRProg expects the "Atmel-byte" 0x1E last
		// but shows it first in the dialog-window)
		else if (val == 's') 
		{
			sendchar(SIG_BYTE3);
			sendchar(SIG_BYTE2);
			sendchar(SIG_BYTE1);
		}
	} 
	return 0;
}
