#ifndef STEP_EY

#define STEP_EY

#include "f5motors.h"
#include "f5lcd.h"
#include "f5pid.h"

void read_and_print_sensor(int *left_s, int *center_s, int *right_s) {
		Left_white_line = ADC_Conversion(3);	//Getting data of Left WL Sensor
		Center_white_line = ADC_Conversion(2);	//Getting data of Center WL Sensor
		Right_white_line = ADC_Conversion(1);	//Getting data of Right WL Sensor
		lcd_print (1, 1, Left_white_line, 3);
		lcd_print (1, 5, Center_white_line, 3);
		lcd_print (1, 9, Right_white_line, 3);
		*left_s = Left_white_line;
		*right_s = Right_white_line;
		*center_s = Center_white_line;
}

void forward_one_step(void)
{
	int left_s, right_s, center_s;
	left_s = ADC_Conversion(3);	//Getting data of Left WL Sensor
	center_s = ADC_Conversion(2);	//Getting data of Center WL Sensor
	right_s = ADC_Conversion(1);	//Getting data of Right WL Sensor
	print_sensor(1, 1, 1);
	print_sensor(1, 5, 2);
	print_sensor(1, 9, 3);
	while(!(center_s>THRESH && left_s>THRESH && right_s>THRESH)) {
		left_s = ADC_Conversion(3);	//Getting data of Left WL Sensor
		center_s = ADC_Conversion(2);	//Getting data of Center WL Sensor
		right_s = ADC_Conversion(1);	//Getting data of Right WL Sensor
		print_sensor(1, 1, 1);
		print_sensor(1, 5, 2);
		print_sensor(1, 9, 3);
		int flag=0;
		if(center_s>THRESH)
		{
			flag=1;
			forward();
			velocity(HIGH_S,HIGH_S);
		}

		if((left_s<THRESH) && (flag==0))
		{
			flag=1;
			forward();
			velocity(MID_S,LOW_S);
		}

		if((right_s<THRESH) && (flag==0))
		{
			flag=1;
			forward();
			velocity(LOW_S,MID_S);
		}
	}
	// _delay_ms(100);
	stop();
	_delay_ms(LOW_S);
}

void left_one_step() {
	int left_s, center_s, right_s;
	lcd_cursor(2, 0);
	lcd_string("BEGINLEFT");
	velocity(HIGH_S, HIGH_S);
	forward();
	_delay_ms(750);
	stop();
	velocity(HIGH_S - (4 * KP), HIGH_S);
	do {
		left();
		left_s = ADC_Conversion(3);	//Getting data of Left WL Sensor
		center_s = ADC_Conversion(2);	//Getting data of Center WL Sensor
		right_s = ADC_Conversion(1);	//Getting data of Right WL Sensor
		print_sensor(1,1,3);	//Prints value of White Line Sensor1
		print_sensor(1,5,2);	//Prints Value of White Line Sensor2
		print_sensor(1,9,1);	//Prints Value of White Line Sensor3
	} while(!(left_s < THRESH && center_s < THRESH && right_s < THRESH));
	stop();
	_delay_ms(100);
	do {
		left();
		left_s = ADC_Conversion(3);	//Getting data of Left WL Sensor
		center_s = ADC_Conversion(2);	//Getting data of Center WL Sensor
		right_s = ADC_Conversion(1);	//Getting data of Right WL Sensor
		print_sensor(1,1,3);	//Prints value of White Line Sensor1
		print_sensor(1,5,2);	//Prints Value of White Line Sensor2
		print_sensor(1,9,1);	//Prints Value of White Line Sensor3
	} while(!(left_s> THRESH || center_s > THRESH || right_s > THRESH));
	stop();
	_delay_ms(100);
	forward_one_step_pid();
}

void right_one_step() {
	int left_s, center_s, right_s;
	lcd_cursor(2, 0);
	lcd_string("BEGINRIGHT");
	velocity(HIGH_S, HIGH_S);
	forward();
	_delay_ms(750);
	stop();
	velocity(HIGH_S, HIGH_S - (4 * KP));
	do {
		right();
		left_s = ADC_Conversion(3);	//Getting data of Left WL Sensor
		center_s = ADC_Conversion(2);	//Getting data of Center WL Sensor
		right_s = ADC_Conversion(1);	//Getting data of Right WL Sensor
		print_sensor(1,1,3);	//Prints value of White Line Sensor1
		print_sensor(1,5,2);	//Prints Value of White Line Sensor2
		print_sensor(1,9,1);	//Prints Value of White Line Sensor3
	} while(!(left_s < THRESH && center_s < THRESH && right_s < THRESH));
	stop();
	_delay_ms(100);
	do {
		right();
		left_s = ADC_Conversion(3);	//Getting data of Left WL Sensor
		center_s = ADC_Conversion(2);	//Getting data of Center WL Sensor
		right_s = ADC_Conversion(1);	//Getting data of Right WL Sensor
		print_sensor(1,1,3);	//Prints value of White Line Sensor1
		print_sensor(1,5,2);	//Prints Value of White Line Sensor2
		print_sensor(1,9,1);	//Prints Value of White Line Sensor3
	} while(!(left_s> THRESH || center_s > THRESH || right_s > THRESH));
	stop();
	_delay_ms(100);
	forward_one_step_pid();
}
#endif