/*
 * led.c
 *
 *  Created on: Nov 15, 2024
 *      Author: ProX
 */

#include "led.h"

#define NUM_BOX	5

/*
 * @brief:	Struct of circle led in traffic light
 */
typedef struct {
	uint16_t 	center_x;
	uint16_t 	center_y;
	uint16_t 	COLOR_FILL;
	uint16_t 	radius;
	uint8_t 	is_fill;
} circle;

/*
 * @brief:	Struct of number to show traffic time
 */
typedef struct {
	uint16_t 	x;
	uint16_t 	y;
	uint16_t 	num;
	uint8_t 	len;
	uint16_t 	fc;
	uint16_t 	bc;
	uint8_t 	sizey;
} number;

/*
 * @brief:	Struct of traffic light follow West and East
 */
struct {
	uint16_t x;
	uint16_t y;
	uint16_t height;
	uint16_t width;
	uint16_t COLOR_FILL;

	circle red;
	circle yellow;
	circle green;

	number num;
} traffic_horizontal;

/*
 * @brief:	Struct of traffic light follow North and South
 */
struct {
	uint16_t x;
	uint16_t y;
	uint16_t height;
	uint16_t width;
	uint16_t COLOR_FILL;

	circle red;
	circle yellow;
	circle green;

	number num;
} traffic_vertical;

/*
 * @brief:	Struct to create a box to seperate lcd_screen
 */
struct {
	uint16_t X;
	uint16_t Y;
	uint16_t height;
	uint16_t width;
	uint16_t COLOR_EDGE;
	uint16_t COLOR_FILL;
	uint8_t is_fill;
} BOX[NUM_BOX];

//draw a rectangle-box
void box_rec(uint8_t box_idx, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color_edge, uint16_t color_fill, uint8_t is_fill) {
	BOX[box_idx].X = x;
	BOX[box_idx].Y = y;
	BOX[box_idx].height = height;
	BOX[box_idx].width = width;
	BOX[box_idx].COLOR_EDGE = color_edge;
	BOX[box_idx].COLOR_FILL = color_fill;
	BOX[box_idx].is_fill = is_fill;

	lcd_DrawRectangle(
		BOX[box_idx].X, BOX[box_idx].Y,
		BOX[box_idx].X + BOX[box_idx].width, BOX[box_idx].Y + BOX[box_idx].height,
		BOX[box_idx].COLOR_EDGE
	);

	if (BOX[box_idx].is_fill) {
		lcd_Fill(
			BOX[box_idx].X, BOX[box_idx].Y,
			BOX[box_idx].X + BOX[box_idx].width,
			BOX[box_idx].Y + BOX[box_idx].height, BOX[box_idx].COLOR_FILL
		);
	}
}

/*
 * @brief:	function to init traffic light
 * @para:	none
 * @retval:	none
 */
void init_traffic_light(void) {
/* ============================ START horizontal ============================ */
	traffic_NS.x 					= 40;
	traffic_NS.y 					= 60;
	traffic_NS.width 			= 120;
	traffic_NS.height 		= 40;
	traffic_NS.COLOR_FILL = BLACK;

	//draw a black box
	lcd_DrawRectangle(
		traffic_NS.x, traffic_NS.y,
		traffic_NS.x + traffic_NS.width, traffic_NS.y + traffic_NS.height,
		traffic_NS.COLOR_FILL
	);
	lcd_Fill(
		traffic_NS.x, traffic_NS.y,
		traffic_NS.x + traffic_NS.width, traffic_NS.y + traffic_NS.height,
		traffic_NS.COLOR_FILL
	);

	//calculate center and radius of each light inside above black box
	traffic_NS.red.center_x 		= traffic_NS.x + traffic_NS.width / 6;
	traffic_NS.yellow.center_x 	= traffic_NS.x + 3 * traffic_NS.width / 6;
	traffic_NS.green.center_x 	= traffic_NS.x + 5 * traffic_NS.width / 6;

	traffic_NS.red.center_y 		= traffic_NS.y + traffic_NS.height / 2;
	traffic_NS.yellow.center_y 	= traffic_NS.red.center_y;
	traffic_NS.green.center_y 	= traffic_NS.red.center_y;

	traffic_NS.red.radius 			= (traffic_NS.height / 2)  * 90 / 100 ;
	traffic_NS.yellow.radius 		= traffic_NS.red.radius;
	traffic_NS.green.radius 		= traffic_NS.red.radius;

	traffic_NS.red.is_fill = 1;

	traffic_NS.red.COLOR_FILL 		= (traffic_NS.red.is_fill) ? RED : BLACK;
	traffic_NS.yellow.COLOR_FILL 	= (traffic_NS.yellow.is_fill) ? YELLOW : BLACK;
	traffic_NS.green.COLOR_FILL 	= (traffic_NS.green.is_fill) ? GREEN : BLACK;

	lcd_DrawCircle(
		traffic_NS.red.center_x, traffic_NS.red.center_y, 
		traffic_NS.red.COLOR_FILL, traffic_NS.red.radius, traffic_NS.red.is_fill
	);
	lcd_DrawCircle(
		traffic_NS.yellow.center_x, traffic_NS.yellow.center_y, 
		traffic_NS.yellow.COLOR_FILL, traffic_NS.yellow.radius, traffic_NS.yellow.is_fill
	);
	lcd_DrawCircle(
		traffic_NS.green.center_x, traffic_NS.green.center_y, 
		traffic_NS.green.COLOR_FILL, traffic_NS.green.radius, traffic_NS.green.is_fill
	);

	//init num
	traffic_NS.num.len 		= 2;
	traffic_NS.num.fc 		= BLACK;
	traffic_NS.num.bc 		= WHITE;
	traffic_NS.num.sizey 	= 32;
	traffic_NS.num.x 			= traffic_NS.x - traffic_NS.num.len * traffic_NS.num.sizey / 2;
	traffic_NS.num.y 			= traffic_NS.y;
	traffic_NS.num.num 		= 10;
	lcd_ShowIntNum(
		traffic_NS.num.x, traffic_NS.num.y, 
		traffic_NS.num.num, traffic_NS.num.len, 
		traffic_NS.num.fc, traffic_NS.num.bc, 
		traffic_NS.num.sizey
	);
/* ============================= END horizontal ============================= */


/* ============================= START vertical ============================= */
	traffic_vertical.x 					= 180;
	traffic_vertical.y 					= 120;
	traffic_vertical.width 			= traffic_horizontal.height;
	traffic_vertical.height 		= traffic_horizontal.width;
	traffic_vertical.COLOR_FILL = BLACK;

	// draw a black box
	lcd_DrawRectangle(
		traffic_vertical.x, traffic_vertical.y,
		traffic_vertical.x + traffic_vertical.width, traffic_vertical.y + traffic_vertical.height, 
		traffic_vertical.COLOR_FILL
	);
	lcd_Fill(
		traffic_vertical.x, traffic_vertical.y, 
		traffic_vertical.x + traffic_vertical.width, traffic_vertical.y + traffic_vertical.height, 
		traffic_vertical.COLOR_FILL
	);

	// calculate center and radius of each light inside above black box
	traffic_vertical.red.center_y 		= traffic_vertical.y + traffic_vertical.height / 6;
	traffic_vertical.yellow.center_y 	= traffic_vertical.y + 3 * traffic_vertical.height / 6;
	traffic_vertical.green.center_y 	= traffic_vertical.y + 5 * traffic_vertical.height / 6;

	traffic_vertical.red.center_x 		= traffic_vertical.x + traffic_vertical.width / 2;
	traffic_vertical.yellow.center_x 	= traffic_vertical.red.center_x;
	traffic_vertical.green.center_x 	= traffic_vertical.red.center_x;

	traffic_vertical.red.radius 			= (traffic_vertical.width / 2)  * 90 / 100 ;
	traffic_vertical.yellow.radius 		= traffic_vertical.red.radius;
	traffic_vertical.green.radius 		= traffic_vertical.red.radius;

	traffic_vertical.red.is_fill = 1;

	traffic_vertical.red.COLOR_FILL 		= (traffic_vertical.red.is_fill) ? RED : BLACK;
	traffic_vertical.yellow.COLOR_FILL 	= (traffic_vertical.yellow.is_fill) ? YELLOW : BLACK;
	traffic_vertical.green.COLOR_FILL 	= (traffic_vertical.green.is_fill) ? GREEN : BLACK;

	lcd_DrawCircle(traffic_vertical.red.center_x, traffic_vertical.red.center_y, traffic_vertical.red.COLOR_FILL, traffic_vertical.red.radius, traffic_vertical.red.is_fill);
	lcd_DrawCircle(traffic_vertical.yellow.center_x, traffic_vertical.yellow.center_y, traffic_vertical.yellow.COLOR_FILL, traffic_vertical.yellow.radius, traffic_vertical.yellow.is_fill);
	lcd_DrawCircle(traffic_vertical.green.center_x, traffic_vertical.green.center_y, traffic_vertical.green.COLOR_FILL, traffic_vertical.green.radius, traffic_vertical.green.is_fill);

	// init num
	traffic_vertical.num.len 		= 2;
	traffic_vertical.num.fc 		= BLACK;
	traffic_vertical.num.bc 		= WHITE;
	traffic_vertical.num.sizey 	= 32;
	traffic_vertical.num.x 			= traffic_vertical.x;
	traffic_vertical.num.y 			= traffic_vertical.y - traffic_vertical.num.sizey;
	traffic_vertical.num.num 		= 99;
	lcd_ShowIntNum(traffic_vertical.num.x, traffic_vertical.num.y, traffic_vertical.num.num, traffic_vertical.num.len, traffic_vertical.num.fc, traffic_vertical.num.bc, traffic_vertical.num.sizey);
/* ============================== END vertical ============================== */
}

/*
 * @brief: 	display traffic light function
 * @para:	i - id of traffic light(0: WE, 1: NS)
 * 			red, yellow, green - state of red, yellow and green led (1: on, 0: off)
 * @retval:	none*/
void control_traffic_light(uint8_t i, uint8_t red, uint8_t yellow, uint8_t green){
	if(i == 0){
		//draw red
		if(!red){
			traffic_horizontal.red.COLOR_FILL = BLACK;
		}
		else{
			traffic_horizontal.red.COLOR_FILL = RED;
		}
		if(!yellow){
			traffic_horizontal.yellow.COLOR_FILL = BLACK;
		}
		else{
			traffic_horizontal.yellow.COLOR_FILL = YELLOW;
		}
		if(!green){
			traffic_horizontal.green.COLOR_FILL = BLACK;
		}
		else{
			traffic_horizontal.green.COLOR_FILL = GREEN;
		}
		lcd_DrawCircle(traffic_horizontal.red.center_x, traffic_horizontal.red.center_y, traffic_horizontal.red.COLOR_FILL, traffic_horizontal.red.radius, 1);
		//draw yellow
		lcd_DrawCircle(traffic_horizontal.yellow.center_x, traffic_horizontal.yellow.center_y, traffic_horizontal.yellow.COLOR_FILL, traffic_horizontal.yellow.radius, 1);
		//draw green
		lcd_DrawCircle(traffic_horizontal.green.center_x, traffic_horizontal.green.center_y, traffic_horizontal.green.COLOR_FILL, traffic_horizontal.green.radius, 1);
	}
	else{
		if(!red){
			traffic_vertical.red.COLOR_FILL = BLACK;
		}
		else{
			traffic_vertical.red.COLOR_FILL = RED;
		}
		if(!yellow){
			traffic_vertical.yellow.COLOR_FILL = BLACK;
		}
		else{
			traffic_vertical.yellow.COLOR_FILL = YELLOW;
		}
		if(!green){
			traffic_vertical.green.COLOR_FILL = BLACK;
		}
		else{
			traffic_vertical.green.COLOR_FILL = GREEN;
		}
		//draw red
		lcd_DrawCircle(traffic_vertical.red.center_x, traffic_vertical.red.center_y, traffic_vertical.red.COLOR_FILL, traffic_vertical.red.radius, 1);
		//draw yellow
		lcd_DrawCircle(traffic_vertical.yellow.center_x, traffic_vertical.yellow.center_y, traffic_vertical.yellow.COLOR_FILL, traffic_vertical.yellow.radius, 1);
		//draw green
		lcd_DrawCircle(traffic_vertical.green.center_x, traffic_vertical.green.center_y, traffic_vertical.green.COLOR_FILL, traffic_vertical.green.radius, 1);
	}
}

/*
 * @brief: 	update traffic light time value and show mode function
 * @para:	val1 is value of WE time, val2 is value of NS time
 * 			mode is mode of traffic system described in the request
 * @retval:	none*/
void update_led_buf(unsigned val1, unsigned val2, unsigned mode){
	if(mode == 4 || mode == 3 || mode == 2){
		lcd_ShowIntNum(140, 280, mode, 1, RED, WHITE, 32);
	}
	else{
		lcd_ShowIntNum(140, 280, mode, 1, RED, WHITE, 32);
	}
	traffic_horizontal.num.num = val1;
	traffic_vertical.num.num = val2;
}

/*
 * @brief: 	display number function
 * @para:	noe
 * @retval:	none*/
void LCD_DisplayNum(){
	lcd_ShowStr(60, 280, "Mode:", RED, WHITE, 32, 0);
	lcd_ShowIntNum(traffic_horizontal.num.x, traffic_horizontal.num.y, traffic_horizontal.num.num, traffic_horizontal.num.len, traffic_horizontal.num.fc, traffic_horizontal.num.bc, traffic_horizontal.num.sizey);
	lcd_ShowIntNum(traffic_vertical.num.x, traffic_vertical.num.y, traffic_vertical.num.num, traffic_vertical.num.len, traffic_vertical.num.fc, traffic_vertical.num.bc, traffic_vertical.num.sizey);
}
