#ifndef PID_EY

#define PID_EY

#include "f5motors.h"
#include "f5lcd.h"

#define THRESH 0x20
#define HIGH_S 120
#define MID_S 130
#define LOW_S 50
#define KP 5
#define KI 0

int weights[8] = {0, // 0 0 0 
	-4, // 0 0 1
	0, // 0 1 0
	-2, // 0 1 1 
	4, // 1 0 0 
	0, // 1 0 1
	2, // 1 1 0
	0 // 1 1 1 
};
int error = 0;
int integral = 0;
int proportional = 0;

void forward_one_step_pid(void)
{
	int left_s, right_s, center_s;
	int weight_index, l_bool, c_bool, r_bool;
	left_s = ADC_Conversion(3);	//Getting data of Left WL Sensor
	center_s = ADC_Conversion(2);	//Getting data of Center WL Sensor
	right_s = ADC_Conversion(1);	//Getting data of Right WL Sensor
	l_bool = ((left_s < THRESH)?0:1);
	c_bool = ((center_s < THRESH)?0:1);
	r_bool = ((right_s < THRESH)?0:1);
	weight_index = (((0 | (l_bool << 2)) | (c_bool <<  1)) | r_bool);
	proportional = weights[weight_index];
	velocity(HIGH_S - (KP * proportional), HIGH_S + (KP * proportional));
	while((center_s>THRESH && left_s>THRESH && right_s>THRESH)) {
		left_s = ADC_Conversion(3);	//Getting data of Left WL Sensor
		center_s = ADC_Conversion(2);	//Getting data of Center WL Sensor
		right_s = ADC_Conversion(1);	//Getting data of Right WL Sensor
		print_sensor(1, 1, 1);
		print_sensor(1, 5, 2);
		print_sensor(1, 9, 3);
		l_bool = ((left_s < THRESH)?0:1);
		c_bool = ((center_s < THRESH)?0:1);
		r_bool = ((right_s < THRESH)?0:1);
		weight_index = (((0 | (l_bool << 2)) | (c_bool <<  1)) | r_bool);
		proportional = weights[weight_index];
		velocity(HIGH_S - (KP * proportional), HIGH_S + (KP * proportional));
		forward();
		_delay_ms(100);
		stop();
	}
	stop();
	_delay_ms(300);
	while(!(center_s>THRESH && left_s>THRESH && right_s>THRESH)) {
		left_s = ADC_Conversion(3);	//Getting data of Left WL Sensor
		center_s = ADC_Conversion(2);	//Getting data of Center WL Sensor
		right_s = ADC_Conversion(1);	//Getting data of Right WL Sensor
		print_sensor(1, 1, 1);
		print_sensor(1, 5, 2);
		print_sensor(1, 9, 3);
		l_bool = (left_s < THRESH)?0:1;
		c_bool = (center_s < THRESH)?0:1;
		r_bool = (right_s < THRESH)?0:1;
		weight_index = (((0 | (l_bool << 2)) | (c_bool <<  1)) | r_bool);
		proportional = weights[weight_index];
		velocity(HIGH_S - (KP * proportional), HIGH_S + (KP * proportional));
		forward();
	}
	stop();
	_delay_ms(500);
}
#endif