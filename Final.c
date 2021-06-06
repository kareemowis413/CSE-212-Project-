# include "stdint.h"
# include "INITIALIZATION.h"
# include "math.h"

#define M_PI   3.14159265358979323846264338327950288
void SystemInit(){}
volatile long double dist ;
volatile long double long1 = 30.064540;
volatile long double lat1 = 31.278079;

char Start0[] = "Press SW1 to";
char Start1[] = "start";  
char Stop0[] =  "Press SW2 at";
char Stop1[] =  "any time to stop";
char Stop2[] =  "Press SW2 again";  
char Distance[] = "Distance=";


  
  // initialization  
void initialize (void){

/*SYSCTL_RCGCGPIO_R |= 0x23;
while(!(SYSCTL_PRGPIO_R & 0x23));
*/
SYSCTL_RCGCGPIO_R |= 0x01;
while((SYSCTL_PRGPIO_R & 0x01) == 0){};  
SYSCTL_RCGCGPIO_R |= 0x02;
while((SYSCTL_PRGPIO_R & 0x02) == 0){};
SYSCTL_RCGCGPIO_R |= 0x20;
while((SYSCTL_PRGPIO_R & 0x20) == 0){};

GPIO_PORTF_LOCK_R   = 0x4C4F434B ;
GPIO_PORTB_LOCK_R   = 0x4C4F434B ;
GPIO_PORTA_LOCK_R   = 0x4C4F434B ;



// port F  
GPIO_PORTF_CR_R     = 0x1F ; // to allow change pins f 0,1,2,3,4   
GPIO_PORTF_AMSEL_R  = 0 ; // disable analog for the bins     
GPIO_PORTF_PCTL_R   = 0 ;// GPIO for pins
GPIO_PORTF_AFSEL_R  = 0 ; // not allternative
GPIO_PORTF_DIR_R    = 0x0E ; //output for leds and input for switches 
GPIO_PORTF_DEN_R    = 0x1F ; // digital not analog
GPIO_PORTF_PUR_R    = 0x11; //PUR for switches
GPIO_PORTF_DATA_R   &= ~0x0E ; // initialize (turn off leds)

// port B
GPIO_PORTB_CR_R     = 0xFF ;    
GPIO_PORTB_AMSEL_R  = 0 ;     
GPIO_PORTB_PCTL_R   = 0 ;
GPIO_PORTB_AFSEL_R  = 0 ;
GPIO_PORTB_DIR_R    = 0xFF ; 
GPIO_PORTB_DEN_R    = 0xFF ;

// port A
GPIO_PORTA_CR_R     = 0xE0 ; // enable change on pin rs =5,rw = 6, EN =7   
GPIO_PORTA_AMSEL_R  = 0 ;     
GPIO_PORTA_PCTL_R   = 0 ;
GPIO_PORTA_AFSEL_R  = 0 ;
GPIO_PORTA_DIR_R    = 0xE0 ; 
GPIO_PORTA_DEN_R    = 0xE0 ;
GPIO_PORTA_DATA_R &= ~0xE0;

dist = 0.0;        //Global variable
}

/*
1-delay function
2-function prints and perform orders to lcd
3-LCD Commands to initalize LCD
*/

//delay function takes an integer as a parameter and produce a time delay equal to this integer multipled by 10 nanoseconds

void delay10Ns(int time)
{
    int i;
    for(i = 0; i < time; i++) 
    {}
}


/*That function prints and perform orders to lcd as it takes two characters parameters, char rs takes two values 0 to perform LCD commands and it is the same value of (connected to) pin 5 at port A, RS pin of LCD takes value 1 to perform LCD DATA write
  , char D0toD7 is the data pins for LCD and it is the same values of (connected to) pin 5 at port A*/
  
void LCD(char rs, char D0toD7)
    {
        delay10Ns(600);
        GPIO_PORTB_DATA_R = D0toD7;         //sets Port B pins to D0toD7 values
        delay10Ns(600);                     //6 millisecond
        if (!(rs))                          //LCD Command
        {GPIO_PORTA_DATA_R &= ~0x20;}       //check if rs value is 0
        else                                //LCD Data
        {
        GPIO_PORTA_DATA_R |= 0x20;         //check if rs value is 0
        }
        delay10Ns(600);                    //6 millisecond
        GPIO_PORTA_DATA_R |= 0x80;         //set enable to logic 1
        delay10Ns(600);                    //6 millisecond
        GPIO_PORTA_DATA_R &= ~0x80;        //set enable to logic 0
        delay10Ns(600);                    //6 millisecond
    }


/*LCD Commands to initalize LCD*/
void initalizeLCD()
  {
  LCD(0,  0x3C);                                //Function set
  LCD(0,  0x01);                                //Clear
  LCD(0,  0x02);                                //Return Home
  LCD(0,  0x0C);                                //0x0E//Display ON/control OFF
  }
  
    
void printOnLCD(char ch[], int position)
{
int i;
if (position == 1)              //LCD clear and line 1
  {
  LCD(0, 0x01);
  LCD(0, 0x02);
  }
  else if (position == 2)        //LCD Data to line 2
  {
  LCD(0, 0xC0);                 //position takes value beginning of 1 for first line , 2 for beginning of second line, x for for current position
  }
  
for( i = 0;ch[i]!='\0'; i++) LCD(1 ,ch[i]);
}

// calculate the distance
void Total_distance(long double long2,long double lat2 ){

volatile long double dlong = long2 * ( M_PI / 180.0 ) - long1 * ( M_PI / 180.0 );
volatile long double dlat = lat2 * ( M_PI / 180.0 ) - lat1 * ( M_PI / 180.0 );
volatile long double change = pow(sin(dlat / 2), 2) +
                          cos(lat1) * cos(lat2) *
                          pow(sin(dlong / 2), 2);
change = 2 * asin(sqrt(change));  
change = change * 6371000;
if (change >= 0.5){ 
dist = dist + change;
long1 = long2 ;
lat1 =  lat2 ;
}
}
// led on if exceed 100 m
void led_on(long double dist){
if (dist >=  100.0) 
  GPIO_PORTF_DATA_R = 0x02;
  
}

// display on LCD
void printDist(int dist){
int i;  
int tmp1 = dist;
int tmp2 = 0;

for(i = 5; i >= 0; i--) 
    {
        int num = tmp1 / pow(10,i);                //split last digit from number
        tmp1 = tmp1 - num * pow(10,i);
        tmp2 += num;
        if(tmp2){                              //ignoring zeroes at Most signfient digit
        LCD(1, num + '0');
        }
    }
    
LCD(1,  'm');
LCD(0, 0x89);
    
}

  //initialization first dummy point
long double long2 = 30.064540;
long double lat2 = 31.278079;

void dummy_Data_Generator()
{
	while (long2 < (30.064540+0.000200))
	{
	long2 += 0.000005;
	}
	while (lat2 < (31.278079+0.000400))
	{
  lat2 += 0.000005 ;
	}
	long2 += 0.000005;
  lat2 += 0.000005 ;
	
      Total_distance(long2,lat2);// put the values
}	

int main(){

initialize(); // call the initialization finction
initalizeLCD();
// long1 , lat1 from gps first time

	while(1){
  
GPIO_PORTF_DATA_R &= ~0x02;
dist = 0;
  
printOnLCD(Start0, 1);
printOnLCD(Start1, 2);

while((GPIO_PORTF_DATA_R & 0x10)){
  
if (!(GPIO_PORTF_DATA_R & 0x01)){

printOnLCD(Stop0, 1);
printOnLCD(Stop1, 2);
  
delay10Ns(10000000);
printOnLCD(Distance, 1);
printDist(dist);

while((GPIO_PORTF_DATA_R & 0x10)){
			
			dummy_Data_Generator();
      printDist(dist);
        
      led_on(dist);
      }
      printOnLCD(Stop2, 2);
      delay10Ns(1000000);      
      }
    
    }
  
  }

}
