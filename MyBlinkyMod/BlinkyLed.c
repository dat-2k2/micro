/*------------------------------------------------------------
 * Name: Blinky.c
 * Purpose: LED PG7 Flasher for MCBSTM32F200
 *----------------------------------------------------------*/
/*------------------------------------------------------------
 Main function
 *----------------------------------------------------------*/
 #define RCCAHB1_ENR (*(unsigned long*)0x40023830)
#define RCCAHB1_PORTG 0x40 
#define GPIOG_MODER (*(unsigned long*)0x40021800) 			
#define GPIOH_MODER (*(unsigned long*)0x40021C00) 
 #define GPIOI_MODER (*(unsigned long*)0x40022000) 
	
#define GPIOG_ODR *(unsigned long*)(0x40021814)
#define GPIOH_ODR *(unsigned long*)(0x40021C14)
#define GPIOI_ODR *(unsigned long*)(0x40022014)
unsigned long int j;
void delay(){
	 for( j=0; j<200000 ;j++ ){} //Delay
}
int main ()
{
//-------Declaration of type of variables---------------------
int i; //counter for get ready delay
 //counter for blinky delay
//-------Initialization of variables--------------------------
	 i=0;
//-------Main cycle of algorithm------------------------------

 RCCAHB1_ENR |= 0x40; //Enable port G clocking
	 for(i=0; i<4; i++){} //small delay for GPIOG get ready
	RCCAHB1_ENR |= 0x80;//Enable port H clocking
		  for(i=0; i<4; i++){} //small delay for GPIOH get ready
	RCCAHB1_ENR |= 0x100;//Enable port I clocking
 for(i=0; i<4; i++){} //small delay for GPIOI get ready

 GPIOG_MODER = (GPIOG_MODER &
~(1ul<<15)) | (1ul<<14); //Set PG7 as General purpose output
	  GPIOG_MODER = (GPIOG_MODER &
~(1ul<<13)) | (1ul<<12); //Set PG6 as General purpose output
	  GPIOG_MODER = (GPIOG_MODER &
~(1ul<<17)) | (1ul<<16); //Set PG8 as General purpose output
 GPIOH_MODER = (GPIOH_MODER &
~(1ul<<5)) | (1ul<<4); //Set PH2 as General purpose output
	  GPIOH_MODER = (GPIOH_MODER &
~(1ul<<7)) | (1ul<<6); //Set PH3 as General purpose output
	  GPIOH_MODER = (GPIOH_MODER &
~(1ul<<13)) | (1ul<<12); //Set PH6 as General purpose output
	  GPIOH_MODER = (GPIOH_MODER &
~(1ul<<15)) | (1ul<<14); //Set PH7 as General purpose output
	  GPIOI_MODER = (GPIOI_MODER &
~(1ul<<21)) | (1ul<<20); //Set PI10 as General purpose output
for(i=0; i<4; i++){}
while(1)
 {
//PH3
	  GPIOH_ODR |= (1ul<<3); //Turn LED ON!
delay();
//PH6
	 	  GPIOH_ODR |= (1ul<<6); //Turn LED ON!
delay();
	 //PH7
	 	  GPIOH_ODR |= (1ul<<7); //Turn LED ON!
delay();
	 	//PI10
	 	 	  GPIOI_ODR |= (1ul<<10); //Turn LED ON!
delay();
	 //PG6
	  GPIOG_ODR |= (1ul<<6); //Turn LED ON!
delay();
	 //PG7
 GPIOG_ODR |= (1ul<<7); //Turn LED ON!
delay();
	 	 //PG7
 GPIOG_ODR |= (1ul<<7); //Turn LED ON!
delay();
	 	 //PG8
 GPIOG_ODR |= (1ul<<8); //Turn LED ON!
delay();
	 	 //PH2
 GPIOH_ODR |= (1ul<<2); //Turn LED ON!
delay();


//PH3
	  GPIOH_ODR &= ~(1ul<<3); //Turn LED OFF!
delay();
//PH6
	 	  GPIOH_ODR &= ~(1ul<<6); //Turn LED OFF!
delay();
	 //PH7
	 	  GPIOH_ODR &= ~(1ul<<7); //Turn LED OFF!
delay();
	 	//PI10
	 	 	  GPIOI_ODR &= ~(1ul<<10); //Turn LED OFF!
delay();
	 //PG6
	  GPIOG_ODR &= ~(1ul<<6); //Turn LED OFF!
delay();
	 //PG7
 GPIOG_ODR &= ~(1ul<<7); //Turn LED OFF!
delay();
	 	 //PG7
 GPIOG_ODR &= ~(1ul<<7); //Turn LED OFF!
delay();
	 	 //PG8
 GPIOG_ODR &= ~(1ul<<8); //Turn LED OFF!
delay();
	 	 //PH2
 GPIOH_ODR &= ~(1ul<<2); //Turn LED OFF!
delay();
 }
}