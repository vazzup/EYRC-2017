/*
* TASK1B.c
*
* Created: 04/11/2017 22:58:28
*  Author: Avinash Dubey
*/


#include <avr/io.h>
#define F_CPU 14745600
#include <util/delay.h>
#include "buzzer.h"
#include "f5motors.h"
#include "f5lcd.h"
#include "f5llf.h"
#include "f5step.h"
#include "f5pid.h"
#include "f5path.h"

int init_num = 45;
int at_home = 1;
int orientation = FNORTH;
void port_init()
{
	buzzer_pin_config();
	lcd_port_config();
	motion_pin_config();
	adc_pin_config();
}

void init_devices (void)
{
	cli(); //Clears the global interrupts
	port_init();
	adc_init();
	timer5_init();
	sei(); //Enables the global interrupts
}

void ReachDestinationAvoidingNode(unsigned char Xd,unsigned char Yd,unsigned char Xn,unsigned char Yn)
{
	char s[60];
	int path_len, i, dest_node, col_num, row_num;
	row_num = (((int)Xd) - 1);
	col_num = ((int)(((char)Yd) - 'A'));
	dest_node = row_num * Y + col_num;
	init_devices(); // Initialise all ports for sensors
	lcd_set_4bit(); // Init for LCD
	lcd_init(); // Init for LCD
	// forward_one_step_pid();
	for(int i=0; i<60; i++) {
		s[i] = '\0';
	}
	// lcd_cursor(2, 1);
	// lcd_string("PATHCH");
	get_commands(init_num, dest_node, orientation, &orientation, &path_len, s, &at_home);
	/* for(i =0; i<path_len; i++) {
		lcd_cursor(2, 1);
		lcd_wr_char(s[i]);
		_delay_ms(1500);
	} */
	
	for(i = 0; i < path_len; i++) {
		switch(s[i]) {
			case 'F': {
				forward_one_step_pid();
				break;
			}
			case 'B': {
				break;
			}
			case 'R': {
				right_one_step();
				break;
			}
			case 'L': {
				left_one_step();
				break;
			}
			default: {
				break;
			}
		}
	}
	lcd_print(2, 8, init_num, 3);
	lcd_print(2, 12, dest_node, 3);
	lcd_print(1, 1, orientation, 3);
	lcd_print(1, 5, path_len, 3);
	_delay_ms(2000);
	init_num = dest_node;
}

//Do not make changes in main function
int main(void)
{
	ReachDestinationAvoidingNode(5,'D',6,'D');
	buzzer_on();
	_delay_ms(500);
	buzzer_off();
	ReachDestinationAvoidingNode(2,'F',2,'D');
	buzzer_on();
	_delay_ms(500);
	buzzer_off();
	ReachDestinationAvoidingNode(2,'A',2,'C');
	buzzer_on();
}