#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "lcd_hd44780_avr.h"
#include <avr/eeprom.h>
//--------------------------------
void initRegisters(void);
uint8_t readSensors(void);
void right(void);
void left(void);
void abitLeft(void);
void abitRight(void);
void forward(void);
void lineFollow(void);
void turnRight(void);
void turnLeft(void);
void stopBot(void);
uint8_t readSensorsB(void);
void lineFollowForScan(void);
void lineCount(void);
void goForward (int8_t);
void moveBot(int8_t,int8_t );
void countloop();
int next_check(int x_co,int y_co);
int set_values(int x,int y);
void findPath(int gx,int gy, int cx,int cy);
void checkPath(int gx,int gy,int x1,int y1);
void moveByStack(int gx,int gy);
int isEmptyX();
int isFullX();
int popX();
void pushX(int data);
int isEmptyY();
int isFullY();
int popY();
void pushY(int data);
void fillArray(void);
uint8_t readSignal(void);
uint8_t readYellow(void);
uint8_t place =0;
uint8_t box =0;

//------------------------------------
uint8_t counter = 0;
uint8_t b_node =0;
uint8_t px = 0 ;
uint8_t nx = 3 ;
uint8_t py = 1;
uint8_t ny= 2;
uint8_t dir =0 ;
int8_t curx = 0;
int8_t cury =0;
int current_x=0;
int current_y=2;
int goal_x=0;
int goal_y=0;
int last_checkedx = 0;
int last_checkedy = 0;
int x=0;
int y=0;
int steps=0;
int MAXSIZE = 20;
int stackX[20];
int topX = -1;
int stackY[20];
int topY = -1;
int grid[9][5] =
{
	{40, 40, 40, 50,50}, // 0
	{50, 40, 50, 50,50},  // 1
	{40, 40, 40, 50,50}, // 2
	{40, 40, 40, 50,50},  // 3
	{40, 50, 40, 40,50},  // 4
	{40, 40, 40, 40,40},  // 5
	{40, 40, 50, 40,50},  // 6
	{40, 50, 40, 40,40},  // 7
	{40, 40, 40, 40,40}  // 8
	//   0  1  2  3
};

int grid1[9][5] =
{
	{40, 40, 40, 50,50}, // 0
	{50, 40, 50, 50,50}, // 1
	{40, 40, 40, 50,50},  // 2
	{40, 50, 40, 50,50},  // 3
	{40, 40, 40, 40,50},  // 4
	{40, 40, 40, 40,40},  // 5
	{40, 40, 50, 40,50},  // 6
	{40, 50, 40, 40,40},  // 7
	{40, 40, 40, 40,40}  // 8
	//   0  1  2  3
};
int visited[9][5] =
{
	{0, 0, 0, 1, 1},  // 0
	{0, 0, 0, 1, 1},  // 1
	{0, 0, 0, 1, 1},  // 2
	{0, 0, 0, 1, 1},  // 3
	{0, 0, 0, 0, 1},  // 0
	{0, 0, 0, 0, 0},  // 0
	{0, 0, 0, 0, 0},  // 1
	{0, 0, 0, 0, 0},  // 2
	{0, 0, 0, 0, 0}   // 3
	//   0  1  2  3
};

//------------------------------------
int main(void)
{
	uint8_t signal;
	initRegisters();
	_delay_ms(3000);
	
	while(1)
	{
		
		forward();
		_delay_ms(1000);
		right();
		_delay_ms(1000);
		left();
		_delay_ms(1000);
		
		/*if (eeprom_read_byte((uint8_t*)80)==0xFF)
		{
			lineFollowForScan();
		}
		else if (eeprom_read_byte((uint8_t*)80)==0x00)
		{
			fillArray();
			grid1[0][3]=50;
			grid1[1][3]=50;
			grid1[2][3]=50;
			grid1[3][3]=50;
			grid1[4][3]=50;
			for(int i=0;i<=8;i++)
			{
				for(int j=0; j<=4; j++)
				{
					grid[i][j]=grid1[i][j];
					visited[i][j]=0;
				}
			}
			
			LCDClear();
			LCDWriteStringXY(3,1,"Algorithm Start");
			_delay_ms(500);
			LCDClear();
			LCDWriteStringXY(3,1,"Values");
			_delay_ms(1000);
			LCDClear();
			LCDWriteIntXY(0,0,goal_x,2);
			LCDWriteIntXY(5,0,goal_y,2);
			//-----------------------------------
			countloop();
			findPath(goal_x,goal_y,current_x,current_y);
			moveByStack(goal_x,goal_y);
			//setDirection(nx);
			_delay_ms(3000);
			//----------------------------------------
			countloop();
			findPath(2,0,current_x,current_y);
			moveByStack(2,0);
			setDirection(nx);
			PORTC|=1<<7;
			_delay_ms(300);
			PORTC&=~(1<<7);
			_delay_ms(3000);
			
			//-----------------------------------------
			
			
			countloop();
			findPath(4,0,current_x,current_y);
			moveByStack(4,0);
			countloop();
			findPath(5,0,current_x,current_y);
			moveByStack(5,0);
			
			_delay_ms(3000);
			
			
			//-----------------------------------------
			
			countloop();
			findPath(7,0,current_x,current_y);
			moveByStack(7,0);
			_delay_ms(120000);
			/*countloop();
			findPath(8,0,current_x,current_y);
			moveByStack(8,0);
			while(1)
			{
				stopBot();
			}
			
		}*/
		
		
	}
}
void initRegisters()	
{
	DDRA=0x00;
	PORTA=0xFF;
	DDRC=0x80;
	DDRB|=0x0f;
	DDRC|=1<<7;
	PORTC&=~(1<<7);
	DDRD&=~(1<<1);
	PORTD|=1<<1;
	DDRD|=1<<7;
	PORTD|=1<<7;
	PORTC=0x7F;
	TCCR0 = 1<<WGM00 | 1<< WGM01 | 1<<COM01 |1<<CS02;
	OCR0=255;
	TCCR2=1<<WGM20 | 1<<WGM21 |1<<COM21 | 1<<CS21 |1<<CS22 ;
	OCR2 = 255 ;
	LCDInit(LS_NONE);
	LCDClear();
	LCDWriteStringXY(0,0,"Welcome To Bugs");
	LCDClear();
	LCDWriteStringXY(8,0,"BUGS");
	LCDWriteIntXY(8,1,b_node,2);
	LCDWriteStringXY(1,0,"count");
	LCDWriteIntXY(1,1,counter,2);
	dir=py;
}
//----------------------------------------------------------------------

//-----------------------------------------------------------------------------------
uint8_t readSensors()
{
	uint8_t sensor ;
	
	if (bit_is_clear(PINC,0)&&bit_is_set(PINC,1)&&bit_is_set(PINC,2))
	{
		sensor=3;
	}//0 1 1
	else if (bit_is_clear(PINC,1)&&bit_is_set(PINC,0)&&bit_is_set(PINC,2))
	{
		sensor=5;
	}// 1 0 1
	else if (bit_is_clear(PINC,2)&&bit_is_set(PINC,1)&&bit_is_set(PINC,0))
	{
		sensor=6;
	} // 1 1 0
	else if (bit_is_clear(PINC,0)&&bit_is_clear(PINC,1)&&bit_is_set(PINC,2))
	{
		
		sensor=1;
	}// 0 0 1
	else if (bit_is_clear(PINC,2)&&bit_is_clear(PINC,1)&&bit_is_set(PINC,0))
	{
		sensor=4;
	}// 1 0 0
	else if (bit_is_clear(PINC,0)&&bit_is_clear(PINC,1)&&bit_is_clear(PINC,2))
	{
		sensor=0;
	}// 0 0 0
	else if (bit_is_clear(PINC,0)&&bit_is_set(PINC,1)&&bit_is_clear(PINC,2))
	{
		sensor=2;
	}// 0 1 0
	else if (bit_is_set(PINC,0)&&bit_is_set(PINC,1)&&bit_is_set(PINC,2))
	{
		sensor=7;
	}//1 1 1

	return sensor;
}
uint8_t readSensorsB()
{
	uint8_t sensorb;
	if (bit_is_clear(PINC,3)&&bit_is_set(PINC,4)&&bit_is_clear(PINC,5))
	{
		sensorb=9;
	}
	else if (bit_is_clear(PINC,3)&&bit_is_set(PINC,4)&&bit_is_set(PINC,5))
	{
		sensorb=1;
	}
	else if (bit_is_clear(PINC,3)&&bit_is_clear(PINC,4)&&bit_is_clear(PINC,5))
	{
		sensorb= 11;
	}
	else if (bit_is_clear(PINC,3)&&bit_is_set(PINC,4)&&bit_is_clear(PINC,5)) //set
	{
		sensorb= 11;
	}
	return sensorb;
}
uint8_t readSignal()
{
	uint8_t signal=1;
	if (bit_is_clear(PINC,6))
	{
		signal=0;
		
	}else
	{
		signal=1;
	}
	
	return signal;
}
uint8_t readYellow()
{
	uint8_t yellow ;
	if (bit_is_clear(PIND,1))
	{
		yellow=1;
	}else
	{
		yellow=0;
	}
}
//---------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------
void lineBack()
{
	uint8_t sensorb=readSensorsB;
	if (sensorb==6)
	{
		
	}
	else if (sensorb==6)
	{
		
	}
	else if (sensorb==6)
	{
		
	}
	else if (sensorb==6)
	{
		
	}
	else if (sensorb==6)
	{
		
	}
	
}
void lineFollowForScan(void)
{
	uint8_t sensor;
	uint8_t sensorb;
	uint8_t signal;
	uint8_t yellow ;
	sensor=readSensors();
	sensorb=readSensorsB();
	signal=readSignal();
	yellow=readYellow();
	if (signal==0)
	{
		do
		{
			signal=readSignal();
			stopBot();
		} while (signal==0);
		box=1;
	}
	if (sensorb==9)
	{
		b_node++;
		place--;
		eeprom_write_byte ((uint8_t*) counter,50);
		place++;
		LCDWriteIntXY(8,1,b_node,2);
		_delay_ms(250);
	}
	else if (sensor==3)
	{
		right();
	}
	else if (sensor==6)
	{
		left();
	}
	else if (sensor==5)
	{
		forward();
	}else if (sensor==1)
	{
		right();
	}
	else if (sensor==4)
	{
		left();
	}
	else if (sensor==0||sensor==2)
	{
		if (sensor==0)
		{
			if (box==1)
			{
				eeprom_write_byte ((uint8_t*) counter,60);
				place++;
				box=0;
			}else
			{
				eeprom_write_byte ((uint8_t*) counter,40);
				place++;
				//check=1;
			}
		}
		counter++;
		LCDWriteIntXY(1,1,counter,2);
		stopBot();
		_delay_ms(100);
		forward();
		_delay_ms(100);
		if (counter==9)
		{
			do
			{
				lineFollow();
				sensorb=readSensorsB();
			} while (sensorb!=11);
			
			sensorb=readSensorsB();
			if (sensorb==9)
			{
				b_node++;
				place--;
				
				place++;
				LCDWriteIntXY(8,1,b_node,2);
				_delay_ms(250);
			}
			turnLeft();
			stopBot();
			_delay_ms(500);
		}else if (counter==10)
		{
			do
			{
				lineFollow();
				sensorb=readSensorsB();
			} while (sensorb!=11);
			
			sensorb=readSensorsB();
			if (sensorb==9)
			{
				b_node++;
				place--;
				
				place++;
				LCDWriteIntXY(8,1,b_node,2);
				_delay_ms(250);
			}
			turnLeft();
			stopBot();
			_delay_ms(500);
		}else if (counter==18)
		{
			do
			{
				lineFollow();
				sensorb=readSensorsB();
			} while (sensorb!=11);
			
			sensorb=readSensorsB();
			if (sensorb==9)
			{
				b_node++;
				place--;
				
				place++;
				LCDWriteIntXY(8,1,b_node,2);
				_delay_ms(250);
			}
			turnRight();
			stopBot();
			_delay_ms(500);
		}else if (counter==19)
		{
			do
			{
				lineFollow();
				sensorb=readSensorsB();
			} while (sensorb!=11);
			
			sensorb=readSensorsB();
			if (sensorb==9)
			{
				b_node++;
				place--;
				
				place++;
				LCDWriteIntXY(8,1,b_node,2);
				_delay_ms(250);
			}
			turnRight();
			stopBot();
			_delay_ms(500);
		}else if (counter==24)
		{
			do
			{
				lineFollow();
				sensorb=readSensorsB();
			} while (sensorb!=11);
			
			sensorb=readSensorsB();
			if (sensorb==9)
			{
				b_node++;
				place--;
				
				place++;
				LCDWriteIntXY(8,1,b_node,2);
				_delay_ms(250);
			}
			turnRight();
			stopBot();
			_delay_ms(500);
		}else if (counter==25)
		{
			do
			{
				lineFollow();
				sensorb=readSensorsB();
			} while (sensorb!=11);
			
			sensorb=readSensorsB();
			if (sensorb==9)
			{
				b_node++;
				place--;
				
				place++;
				LCDWriteIntXY(8,1,b_node,2);
				_delay_ms(250);
			}
			turnLeft();
			stopBot();
			_delay_ms(500);
		}else if (counter==27)
		{
			do
			{
				lineFollow();
				sensorb=readSensorsB();
			} while (sensorb!=11);
			
			sensorb=readSensorsB();
			if (sensorb==9)
			{
				b_node++;
				place--;
				
				place++;
				LCDWriteIntXY(8,1,b_node,2);
				_delay_ms(250);
			}
			turnLeft();
			stopBot();
			_delay_ms(500);
			
		}else if (counter==28)
		{
			do
			{
				lineFollow();
				sensorb=readSensorsB();
			} while (sensorb!=11);
			
			sensorb=readSensorsB();
			if (sensorb==9)
			{
				b_node++;
				place--;
				
				LCDWriteIntXY(8,1,b_node,2);
				_delay_ms(250);
			}
			turnRight();
			stopBot();
			_delay_ms(500);
		}else if (counter==29)
		{
			do
			{
				lineFollow();
				sensorb=readSensorsB();
			} while (sensorb!=11);
			sensorb=readSensorsB();
			if (sensorb==9)
			{
				b_node++;
				place--;
				
				place++;
				LCDWriteIntXY(8,1,b_node,2);
				_delay_ms(250);
			}
			turnLeft();
			stopBot();
			_delay_ms(500);
		}
		else if (counter==30)
		{
			do
			{
				lineFollow();
				sensorb=readSensorsB();
			} while (sensorb!=11);
			
			sensorb=readSensorsB();
			if (sensorb==9)
			{
				b_node++;
				place--;
				
				place++;
				LCDWriteIntXY(8,1,b_node,2);
				_delay_ms(250);
			}
			turnLeft();
			stopBot();
			_delay_ms(500);
		}
		else if (counter==32)
		{
			do
			{
				lineFollow();
				sensorb=readSensorsB();
			} while (sensorb!=11);
			sensorb=readSensorsB();
			if (sensorb==9)
			{
				b_node++;
				place--;
			
				place++;
				LCDWriteIntXY(8,1,b_node,2);
				_delay_ms(250);
			}
			turnRight();
			stopBot();
			_delay_ms(500);
		}
		else if (counter==33)
		{
			do
			{
				lineFollow();
				sensorb=readSensorsB();
			} while (sensorb!=11);
			sensorb=readSensorsB();
			if (sensorb==9)
			{
				b_node++;
				place--;
				
				place++;
				LCDWriteIntXY(8,1,b_node,2);
				_delay_ms(250);
			}
			turnRight();
			stopBot();
			_delay_ms(500);
		}
		else if (counter==35)
		{
			eeprom_write_byte ((uint8_t*) 80,0x00);
			while (1)
			{
				stopBot();
			}
			
			
		}
		
		
	}
}
void right(void)
{
	OCR0=0;
	OCR2=255;
	PORTB&=~(1<<2|1<<4);
	PORTB|=0x03;
}
void left(void)
{
	OCR0=255;
	OCR2=0;
	PORTB&=~(1<<2|1<<4);
	PORTB|=0x03;
}
void forward(void)
{
	OCR0=180;
	OCR2=180;
	PORTB&=~(1<<2|1<<4);
	PORTB|=0x03;
}
void forwardSlow(void)
{
	OCR0=120;
	OCR2=120;
	PORTB&=~(1<<2|1<<4);
	PORTB|=0x03;
}
void abitRight(void)
{
	OCR0=200;
	OCR2=255;
	PORTB&=~(1<<2|1<<4);
	PORTB|=0x03;
}
void abitLeft(void)
{
	OCR0=255;
	OCR2=200;
	PORTB&=~(1<<2|1<<4);
	PORTB|=0x03;
}
void turnRight()
{
	uint8_t sensor ;
	OCR0=140; //140
	OCR2=140; //140
	PORTB&=~(1<<1|1<<4);
	PORTB|=0b00000101;
	_delay_ms(120); //120
	do
	{
		sensor=readSensors();
	} while (sensor!=3);
	stopBot();
	_delay_ms(400);
	if (dir==py)
	{
		dir=nx;
	}
	else if (dir==px)
	{
		dir=py;
	}
	else if (dir==ny)
	{
		dir=px;
	}
	else if (dir==nx)
	{
		dir=ny;
	}
}
void turnLeft()
{
	uint8_t sensor ;
	OCR0=140; //140
	OCR2=140;
	PORTB&=~(1<<0|1<<2);
	PORTB|=0x12;
	_delay_ms(120); //120
	do
	{
		sensor=readSensors();
	} while (sensor!=6);
	stopBot();
	_delay_ms(400);
	if (dir==py)
	{
		dir=px;
	}
	else if (dir==px)
	{
		dir=ny;
	}
	else if (dir==ny)
	{
		dir=nx;
	}
	else if (dir==nx)
	{
		dir=py;
	}
}
void turnRightR()
{
	uint8_t sensor ;
	OCR0=255; //140
	OCR2=255; //140
	PORTB&=~(1<<1|1<<4);
	PORTB|=0b00000101;
	_delay_ms(250);
	do
	{
		sensor=readSensors();
	} while (sensor!=3);
	stopBot();
	_delay_ms(400);
	if (dir==py)
	{
		dir=nx;
	}
	else if (dir==px)
	{
		dir=py;
	}
	else if (dir==ny)
	{
		dir=px;
	}
	else if (dir==nx)
	{
		dir=ny;
	}
}
void turnLeftR()
{
	uint8_t sensor ;
	OCR0=255;
	OCR2=255;
	PORTB&=~(1<<0|1<<2);
	PORTB|=0x12;
	_delay_ms(250);
	do
	{
		sensor=readSensors();
	} while (sensor!=6);
	stopBot();
	_delay_ms(400);
	if (dir==py)
	{
		dir=px;
	}
	else if (dir==px)
	{
		dir=ny;
	}
	else if (dir==ny)
	{
		dir=nx;
	}
	else if (dir==nx)
	{
		dir=py;
	}
}
void stopBot()
{
	OCR0=0;
	OCR2=0;
	PORTB|=0x03;
}
//new functions
void lineFollow()
{
	int8_t sensor;
	uint8_t sensorb;
	uint8_t signal;
	sensor=readSensors();
	sensorb=readSensorsB();
	signal=readSignal();
	if (signal==0)
	{
		do
		{
			signal=readSignal();
			stopBot();
		} while (signal==0);
	}
	if (sensor==3)
	{
		right();
	}
	else if (sensor==6)
	{
		left();
	}
	else if (sensor==5)
	{
		forward();
	}else if (sensor==1)
	{
		right();
	}
	else if (sensor==4)
	{
		left();
	}
	else if (sensor==0)
	{

		if (dir==py)
		{
			cury++;
			LCDWriteIntXY(0,1,cury,2);
			LCDWriteIntXY(10,1,curx,2);
			_delay_ms(400);
		}
		else if (dir==px)
		{
			curx++;
			LCDWriteIntXY(0,1,cury,2);
			LCDWriteIntXY(10,1,curx,2);
			_delay_ms(400);
		}
		else if (dir==nx)
		{
			curx--;
			LCDWriteIntXY(0,1,cury,2);
			LCDWriteIntXY(10,1,curx,2);
			_delay_ms(400);
		}
		else if (dir==ny)
		{
			cury--;
			LCDWriteIntXY(0,1,cury,2);
			LCDWriteIntXY(10,1,curx,2);
			_delay_ms(200);
		}
	}
}
void lineCount()
{
	uint8_t sensor =readSensors();
	if (sensor==0)
	{
		if (dir==py)
		{
			cury++;
			_delay_ms(400);
		}
		else if (dir==px)
		{
			curx++;
			_delay_ms(400);
		}
		else if (dir==nx)
		{
			curx--;
			_delay_ms(400);
		}
		else if (dir==ny)
		{
			cury--;
			_delay_ms(400);
		}
	}
}
void goForward (int8_t g)
{
	uint8_t sensorB ;
	if (dir==px||dir==nx)
	{
		do
		{
			lineFollow();
		} while (curx!=g);
		stopBot();
		_delay_ms(200);
		do
		{
			sensorB=readSensorsB();
			lineFollow();
		} while (sensorB!=11);
		stopBot();
		_delay_ms(200);
	}
	if (dir==py||dir==ny)
	{
		do
		{
			lineFollow();
		} while (cury!=g);
		stopBot();
		_delay_ms(200);
		do
		{
			sensorB=readSensorsB();
			lineFollow();
		} while (sensorB!=11);
		stopBot();
		_delay_ms(200);
	}
}
void setDirection(uint8_t direction)
{
	if (direction==py)
	{
		if (dir==px)
		{
			turnRight();
			stopBot();
		}
		if (dir==nx)
		{
			turnLeft();
			stopBot();
		}
		if (dir==ny)
		{
			turnLeft();
			stopBot();
			turnLeft();
		}
		if (dir==py)
		{
			stopBot();
		}
	}
	if (direction==px)
	{
		if (dir==py)
		{
			turnLeft();
			stopBot();
		}
		if (dir==nx)
		{
			turnLeft();
			stopBot();
			turnLeft();
		}
		if (dir==ny)
		{
			turnRight();
			stopBot();
		}
		if (dir==px)
		{
			stopBot();
		}
	}
	if (direction==ny)
	{
		if (dir==px)
		{
			turnLeft();
			stopBot();
		}
		if (dir==nx)
		{
			turnRight();
			stopBot();
		}
		if (dir==ny)
		{
			stopBot();
		}
		if (dir==py)
		{
			turnRight();
			stopBot();
			turnRight();
		}
	}
	if (direction==nx)
	{
		if (dir==px)
		{
			turnLeft();
			stopBot();
			turnLeft();
		}
		if (dir==nx)
		{
			stopBot();
		}
		if (dir==ny)
		{
			turnLeft();
			stopBot();
		}
		if (dir==py)
		{
			turnRight();
			stopBot();
		}
	}
}
void moveBot(int8_t x1,int8_t y1)
{
	int8_t gx ,gy;
	if (x1>0)
	{
		setDirection(px);
		gx=curx+x1;
		goForward(gx);
	}
	if (x1<0)
	{
		setDirection(nx);
		gx=curx+x1;
		goForward(gx);
	}
	if (x1==0)
	{
		stopBot();
	}
	if (y1>0)
	{
		setDirection(py);
		gy=cury+y1;
		goForward(gy);
	}
	if (y1<0)
	{
		setDirection(ny);
		gy=cury+y1;
		goForward(gy);
	}
	if (y1==0)
	{
		stopBot();
	}
}
void fillArray()
{
	LCDClear();
	LCDWriteStringXY(0,0,"X");
	LCDWriteStringXY(5,0,"Y");
	LCDWriteStringXY(10,0,"V");
	uint8_t readcount=0;
	uint8_t byteRead;
	uint8_t i,j;
	/*
	for (i=0;i<=7;i++)
	{
	for (j=0;j<=3;j++)
	{
	byteRead = eeprom_read_byte((uint8_t*)readcount);
	LCDWriteIntXY(0,1,i,3);
	LCDWriteIntXY(5,1,j,3);
	LCDWriteIntXY(10,1,byteRead,3);
	_delay_ms(500);
	readcount++;
	grid1[i][j]=byteRead;
	}
	}*/
	for (i=0;i<=8;i++)
	{
		byteRead = eeprom_read_byte((uint8_t*)readcount);
		LCDWriteIntXY(0,1,i,3);
		LCDWriteIntXY(5,1,0,3);
		LCDWriteIntXY(10,1,byteRead,3);
		//_delay_ms(1000);
		grid1[i][0]=byteRead;
		if (grid1[i][0]==60)
		{
			goal_x=i;
			goal_y=0;
		}
		readcount++;
		if (grid1[i][0]==0xff)
		{
			grid1[i][0]=50;
		}
	}
	
	for (i=8;i>=0;i--)
	{
		byteRead = eeprom_read_byte((uint8_t*)readcount);
		LCDWriteIntXY(0,1,i,3);
		LCDWriteIntXY(5,1,1,3);
		LCDWriteIntXY(10,1,byteRead,3);
		//	_delay_ms(1000);
		grid1[i][1]=byteRead;
		if (grid1[i][1]==60)
		{
			goal_x=i;
			goal_y=1;
		}
		if (grid1[i][1]==0xff)
		{
			grid1[i][1]=50;
		}
		readcount++;
		if (i==0)
		{
			break;
		}
	}
	
	//**************************************************************************
	for (i=0;i<=5;i++)
	{
		byteRead = eeprom_read_byte((uint8_t*)readcount);
		LCDWriteIntXY(0,1,i,3);
		LCDWriteIntXY(5,1,2,3);
		LCDWriteIntXY(10,1,byteRead,3);
		//_delay_ms(1000);
		grid1[i][2]=byteRead;
		if (grid1[i][2]==60)
		{
			goal_x=i;
			goal_y=2;
		}
		
		readcount++;
		if (grid1[i][2]==0xff)
		{
			grid1[i][2]=50;
		}
	}
	grid1[6][2]=50;
	readcount+=3;
	for (i=7;i<=8;i++)
	{
		
		byteRead = eeprom_read_byte((uint8_t*)readcount);
		LCDWriteIntXY(0,1,i,3);
		LCDWriteIntXY(5,1,2,3);
		LCDWriteIntXY(10,1,byteRead,3);
		//_delay_ms(1000);
		grid1[i][2]=byteRead;
		if (grid1[i][2]==60)
		{
			goal_x=i;
			goal_y=2;
		}
		if (grid1[i][2]==0xff)
		{
			grid1[i][2]=50;
		}
		
		readcount++;
	}
	
	//*****************************************************************
	for (i=8;i>=6;i--)
	{
		byteRead = eeprom_read_byte((uint8_t*)readcount);
		LCDWriteIntXY(0,1,i,3);
		LCDWriteIntXY(5,1,3,3);
		LCDWriteIntXY(10,1,byteRead,3);
		//_delay_ms(1000);
		grid1[i][3]=byteRead;
		if (grid1[i][3]==60)
		{
			goal_x=i;
			goal_y=3;
		}
		readcount++;
		if (grid1[i][3]==0xff)
		{
			grid1[i][3]=50;
		}
		if (i==5)
		{
			break;
		}
	}
	
	
	for (i=6;i<=8;i++)
	{
		byteRead = eeprom_read_byte((uint8_t*)readcount);
		LCDWriteIntXY(0,1,i,3);
		LCDWriteIntXY(5,1,3,3);
		LCDWriteIntXY(10,1,byteRead,3);
		//_delay_ms(1000);
		grid1[i][4]=byteRead;
		if (grid1[i][4]==60)
		{
			goal_x=i;
			goal_y=4;
		}
		if (grid1[i][4]==0xff)
		{
			grid1[i][4]=50;
		}
		readcount++;
		
	}
	/*readcount+=12;
	
	*/
	for (i=0;i>=5;i--)
	{
		//byteRead = eeprom_read_byte((uint8_t*)readcount);
		//LCDWriteIntXY(0,1,i,3);
		//LCDWriteIntXY(5,1,3,3);
		//LCDWriteIntXY(10,1,byteRead,3);
		//_delay_ms(1000);
		grid1[i][3]=50;
		readcount++;
		if (i==0)
		{
			break;
		}
	}
	
}
//--------------algo---------------------------------------
int next_check(int x_co,int y_co)
{
	if(0<=x_co<=4&&0<=y_co<=8&&grid[x_co][y_co]==40||grid[x_co][y_co]==60)
	{
		return 1;
	}

	else  if(0<=x_co<=4&&0<=y_co<=8&&grid[x_co][y_co]==0)
	{
		return 2;
	}
}

int set_values(int x,int y)
{
	int i=x;
	int j=y;
	grid[current_x][current_y]=0;
	visited[current_x][current_y]=1;
	

	if(next_check(i,j+1)==1&&j+1<=4)
	{
		if(visited[i][j+1]==0)
		{
			if(grid[i][j]!=50)
			{
				if (grid[i][j]!=40)
				{
					grid[i][j + 1] = grid[i][j] + 1;
					visited[i][j + 1] = 1;
				}
				else
				{
					visited[i][j] = 0;
				}

			}

		}
	}

	if(next_check(i,j+1)==2)
	{
		if(grid[i][j+1]==0)
		{
			grid[i][j+1]=50;
			visited[i][j+1]=1;
		}

	}

	if(next_check(i,j-1)==1&&j-1>=0)
	{
		if(visited[i][j-1]==0)
		{
			if(grid[i][j]!=50)
			{

				if (grid[i][j]!=40)
				{
					grid[i][j - 1] = grid[i][j] + 1;
					visited[i][j - 1] = 1;
				}
				else
				{
					visited[i][j ] = 0;
				}
			}
		}
	}
	if(next_check(i,j-1)==2)
	{
		if(grid[i][j+1]==0)
		{
			grid[i][j-1]=50;
			visited[i][j-1]=1;
		}
	}

	if(next_check(i+1,j)==1&&i+1<=8)
	{
		if(visited[i+1][j]==0)
		{
			if(grid[i][j]!=50)
			{

				if (grid[i][j]!=40)
				{
					grid[i+1][j ] = grid[i][j] + 1;
					visited[i+1][j] = 1;
				}
				else
				{
					visited[i][j ] = 0;
				}
			}
		}
	}
	if(next_check(i+1,j)==2)
	{
		if(grid[i][j+1]==0)
		{
			grid[i+1][j]=50;
			visited[i+1][j]=1;
		}
	}

	if(next_check(i-1,j)==1&&i-1>=0)
	{
		if(visited[i-1][j]==0)
		{
			if(grid[i][j]!=50)
			{

				if (grid[i][j]!=40)
				{
					grid[i-1][j ] = grid[i][j] + 1;
					visited[i-1][j ] = 1;
				}
				else
				{
					visited[i][j ] = 0;
				}
			}

		}
	}
	if(next_check(i-1,j)==2)
	{
		if(grid[i][j+1]==0)
		{
			grid[i-1][j]=50;
			visited[i-1][j]=1;
		}
	}
}

void countloop()
{


	//--------------positive------------------

	for(int i=0; i<=4; i++)
	{
		if(current_x<=8)
		{
			set_values(current_x,i);
		}
	}
	for(int i=4; i>=0; i--)
	{
		if(current_x<=8)
		{
			set_values(current_x,i);
		}
	}



	for(int i=4; i>=0; i--)
	{
		if(current_x+1<=8)
		{
			set_values(current_x+1,i);
		}
	}

	for(int i=0; i<=4; i++)
	{
		if(current_x+1<=8)
		{
			set_values(current_x+1,i);
		}
	}



	for(int i=0; i<=4; i++)
	{
		if(current_x+2<=8)
		{
			set_values(current_x+2,i);
		}
	}
	for(int i=4; i>=0; i--)
	{
		if(current_x+2<=8)
		{
			set_values(current_x+2,i);
		}
	}


	for(int i=0; i<=4; i++)
	{
		if(current_x+3<=8)
		{
			set_values(current_x+3,i);
		}
	}
	for(int i=4; i>=0; i--)
	{
		if(current_x+3<=8)
		{
			set_values(current_x+3,i);
		}
	}


	for(int i=0; i<=4; i++)
	{
		if(current_x+4<=8)
		{
			set_values(current_x+4,i);
		}
	}

	for(int i=4; i>=0; i--)
	{
		if(current_x+4<=8)
		{
			set_values(current_x+4,i);
		}
	}

	for(int i=4; i>=0; i--)
	{
		if(current_x+5<=8)
		{
			set_values(current_x+5,i);
		}
	}

	for(int i=0; i<=4; i++)
	{
		if(current_x+5<=8)
		{
			set_values(current_x+5,i);
		}
	}

	for(int i=0; i<=4; i++)
	{
		if(current_x+6<=8)
		{
			set_values(current_x+6,i);
		}
	}
	for(int i=4; i>=0; i--)
	{
		if(current_x+6<=8)
		{
			set_values(current_x+6,i);
		}
	}


	for(int i=4; i>=0; i--)
	{
		if(current_x+7<=8)
		{
			set_values(current_x+7,i);
		}
	}
	for(int i=0; i<=4; i++)
	{
		if(current_x+7<=8)
		{
			set_values(current_x+7,i);
		}
	}
	
	for(int i=4; i>=0; i--)
	{
		if(current_x+8<=8)
		{
			set_values(current_x+8,i);
		}
	}
	for(int i=0; i<=4; i++)
	{
		if(current_x+8<=8)
		{
			set_values(current_x+8,i);
		}
	}

	//-----------------------------------------------
	//---------------------negative------------------

	for(int i=1; i<=4; i++)
	{
		if(current_x>=0)
		{
			set_values(current_x,i);
		}
	}

	for(int i=1; i>=4; i--)
	{
		if(current_x>=0)
		{
			set_values(current_x,i);
		}
	}
	for(int i=4; i>=1; i--)
	{
		if(current_x-1>=0)
		{
			set_values(current_x-1,i);
		}
	}

	for(int i=1; i<=4; i++)
	{
		if(current_x-1>=0)
		{
			set_values(current_x-1,i);
		}
	}

	for(int i=0; i<=4; i++)
	{
		if(current_x-2>=0)
		{
			set_values(current_x-2,i);
		}
	}
	for(int i=4; i>=0; i--)
	{
		if(current_x-2>=0)
		{
			set_values(current_x-2,i);
		}
	}


	for(int i=0; i<=4; i++)
	{
		if(current_x-3>=0)
		{
			set_values(current_x-3,i);
		}
	}

	for(int i=4; i>=0; i--)
	{
		if(current_x-3>=0)
		{
			set_values(current_x-3,i);
		}
	}

	for(int i=0; i<=4; i++)
	{
		if(current_x-4>=0)
		{
			set_values(current_x-4,i);
		}
	}

	for(int i=4; i>=0; i--)
	{
		if(current_x-4>=0)
		{
			set_values(current_x-4,i);
		}
	}

	for(int i=0; i<=4; i++)
	{
		if(current_x-5>=0)
		{
			set_values(current_x-5,i);
		}
	}
	for(int i=4; i>=0; i--)
	{
		if(current_x-5>=0)
		{
			set_values(current_x-5,i);
		}
	}

	for(int i=0; i<=4; i++)
	{

		if(current_x-6>=0)
		{
			set_values(current_x-6,i);
		}

	}

	for(int i=4; i>=0; i--)
	{
		if(current_x-6>=0)
		{
			set_values(current_x-6,i);
		}

	}


	for(int i=0; i<=4; i++)
	{
		if(current_x-7>=0)
		{
			set_values(current_x-7,i);
		}
	}
	for(int i=4; i>=0; i--)
	{
		if(current_x-7>=0)
		{
			set_values(current_x-7,i);
		}
	}
	
	
	for(int i=0; i<=4; i++)
	{
		if(current_x-8>=0)
		{
			set_values(current_x-8,i);
		}
	}
	for(int i=4; i>=0; i--)
	{
		if(current_x-8>=0)
		{
			set_values(current_x-8,i);
		}
	}
}

//------------------path finder----------------------
void findPath(int gx,int gy, int cx,int cy)
{
	steps=grid[gx][gy] - grid[cx][cy];
	last_checkedx=gx;
	last_checkedy=gy;
	for(int i=0; i<=steps; i++)
	{
		checkPath(gx,gy,last_checkedx,last_checkedy);
	}

}

void checkPath(int gx,int gy,int x1, int y1)
{
	if(x1==gx&&y1==gy)
	{
		pushX(x1);
		pushY(y1);
	}

	if (grid[x1][y1 + 1] < grid[x1][y1]&&y1 + 1 <= 4)
	{

		pushX(x1);
		pushY(y1+1);
		last_checkedx = x1;
		last_checkedy = y1 + 1;

	}
	else if (grid[x1][y1 - 1] < grid[x1][y1]&&y1 - 1 >= 0)
	{

		pushX(x1);
		pushY(y1-1);
		last_checkedx = x1;
		last_checkedy = y1 - 1;

	}
	else if (grid[x1 + 1][y1] < grid[x1][y1]&&x1 + 1 <= 7)
	{
		pushX(x1+1);
		pushY(y1);
		last_checkedx = x1 + 1;
		last_checkedy = y1;
	}
	else if (grid[x1 - 1][y1] < grid[x1][y1]&&x1 - 1>=0)
	{
		pushX(x1-1);
		pushY(y1);
		last_checkedx = x1 - 1;
		last_checkedy = y1;
	}

}


//----------------stack----------------------

int isEmptyX()
{

	if(topX == -1)
	return 1;
	else
	return 0;
}

int isFullX()
{

	if(topX == MAXSIZE)
	return 1;
	else
	return 0;
}

int popX()
{
	int data;

	if(!isEmptyX())
	{
		data = stackX[topX];
		topX = topX - 1;
		return data;
	}

}

void pushX(int data)
{
	if(!isFullX())
	{
		topX = topX + 1;
		stackX[topX] = data;
	}
}
int isEmptyY()
{
	if(topY == -1)
	return 1;
	else
	return 0;
}
int isFullY()
{

	if(topY == MAXSIZE)
	return 1;
	else
	return 0;
}
int popY()
{
	int data;
	if(!isEmptyY())
	{
		data = stackY[topY];
		topY = topY - 1;
		return data;
	}
}
void pushY(int data)
{
	if(!isFullY())
	{
		topY = topY + 1;
		stackY[topY] = data;
	}
}
void moveByStack(int gx,int gy)
{
	int finalx=0;
	int finaly=0;
	for(int i=0; i<=steps; i++)
	{
		
		x=popX();
		y=popY();
		if (x==0&&y==2)
		{
			finalx=1;
			finaly=0;
		}
		else
		{
			finalx=x-current_x;
			finaly=y-current_y;
		}
		moveBot(finaly,finalx);
		current_x=x;
		current_y=y;
		stopBot();
		_delay_ms(800);
	}
	for(int i=0;i<=8;i++)
	{
		for(int j=0; j<=4; j++)
		{
			grid[i][j]=grid1[i][j];
			visited[i][j]=0;
		}
	}
	

}
