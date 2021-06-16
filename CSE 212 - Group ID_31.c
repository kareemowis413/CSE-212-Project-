# include "stdint.h"
# include "math.h"
# include "stdio.h"
# include "string.h"
# include "stdlib.h"
# include "TM4C123GH6PM.h"


void SystemInit(){}

 volatile long double lat2 = 0;
 volatile long double long2 = 0;
 volatile long double dist = 0;
 volatile long double long1 = 0;
 volatile long double lat1 = 0;
 volatile long double long1temp = 0;
 volatile long double lat1temp = 0;
 volatile long double theta = 0;
 volatile long double lastdist = 0;
 volatile long double dist1 = 0;
 volatile long double dist2 = 0;
 volatile long double PI = 3.14159265358979323846264338327950288;

char Start0[] = "Press SW2 to";
char Start1[] = "start";
char Stop0[] =  "Press SW1 at";
char Stop1[] =  "any time to stop";
char Stop2[] =  "Press SW1 again";
char Distance[] = "Distance=";



  // initialization
void initialize (void){

/*SYSCTL_RCGCGPIO_R |= 0x33;
while(!(SYSCTL_PRGPIO_R & 0x33));
*/

SYSCTL_RCGCGPIO_R |= 0x01;                // activate port A
while((SYSCTL_PRGPIO_R & 0x01) == 0){};
SYSCTL_RCGCGPIO_R |= 0x02;                // activate port B
while((SYSCTL_PRGPIO_R & 0x02) == 0){};
SYSCTL_RCGCGPIO_R |= 0x20;                // activate port F
while((SYSCTL_PRGPIO_R & 0x20) == 0){};
SYSCTL_RCGCGPIO_R |= 0x10;                // activate port E
while((SYSCTL_PRGPIO_R & 0x10) == 0){};
SYSCTL_RCGCUART_R |= 0x20;                // start clock for UART5
while((SYSCTL_RCGCUART_R & 0x20) != 0x20){};

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

}
void init_uart(void)
{
    GPIO_PORTE_LOCK_R = 0x4C4F434B;
    GPIO_PORTE_CR_R     |= 0x30;
        GPIO_PORTE_AFSEL_R  |= 0x30 ;
        GPIO_PORTE_AMSEL_R  &= ~ 0x30 ;
        GPIO_PORTE_PCTL_R   |= 0x00110000 ;
        GPIO_PORTE_DEN_R    |= 0x30 ;

        UART5_CTL_R &= ~0x301;      // disable UART5
        UART5_IBRD_R = 104;         // baudrate(integer part) = 16000000/(16*9600)= 104.1666667 , integer = 104
        UART5_FBRD_R = 11;          // baudrate(fraction part) = round(0.1667*64)= 11
        UART5_LCRH_R = 0x00000070;  // enable FIFO ,no parity, one stop, 8-bit data
        UART5_CTL_R |= 0x301;       // enable UART5
}

char read_uart(void)
{
    while((UART5_FR_R&0x0010)!=0){}
            return((char)UART5_DR_R&0xFF);
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
        delay10Ns(300);
        GPIO_PORTB_DATA_R = D0toD7;         //sets Port B pins to D0toD7 values
        delay10Ns(300);                     //3 millisecond
        if (!(rs))                          //LCD Command
        {GPIO_PORTA_DATA_R &= ~0x20;}       //check if rs value is 0
        else                                //LCD Data
        {
        GPIO_PORTA_DATA_R |= 0x20;         //check if rs value is 0
        }
        delay10Ns(300);                    //3 millisecond
        GPIO_PORTA_DATA_R |= 0x80;         //set enable to logic 1
        delay10Ns(300);                    //3 millisecond
        GPIO_PORTA_DATA_R &= ~0x80;        //set enable to logic 0
        delay10Ns(300);                    //3 millisecond
    }


/*LCD Commands to initalize LCD*/
void initalizeLCD()
  {
  LCD(0,  0x3C);                                //Function set
  LCD(0,  0x01);                                //Clear
  LCD(0,  0x02);                                //Return Home
  LCD(0,  0x0C);                                ///Display ON/control OFF(cursor off / Blinking off)
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
long double Rad(long double degree)
{
    long double one_deg = (PI) / 180;
    return (one_deg * degree);
}
int first = 1 ;

void Total_distance(long double long2, long double lat2) {

    volatile long double dlong = 0;
    volatile long double dlat = 0;
    volatile long double change1 = 0;
    volatile long double change2 = 0;

    if (first==1){
        lat1 = lat2;
        long1 = long2;
        first = 0;
        long1temp = long1;
        lat1temp = lat1;
        }

    dlong = Rad(long2) - Rad(long1temp) ;
    dlat = Rad(lat2) - Rad(lat1temp) ;
    change1 = pow(sin(dlat / 2), 2) + cos(Rad(lat1temp)) * cos(Rad(lat2)) * pow(sin(dlong / 2), 2);

    dlong = Rad(long2)  - Rad(long1) ;
    dlat = Rad(lat2)  - Rad(lat1) ;
    change2 = pow(sin(dlat / 2), 2) + cos(Rad(lat1)) * cos(Rad(lat2)) * pow(sin(dlong / 2), 2); ;

    theta = atan2(cos(Rad(lat1)) * sin(Rad(lat2)) - sin(Rad(lat1)) * cos(Rad(lat2)) * cos(dlong), sin(dlong) * cos(Rad(lat2)));
    theta = abs(theta) * 180/PI;

    change1 = 2 * asin(sqrt(change1));
    change1 = change1 * 6371000;

    change2 = 2 * asin(sqrt(change2));
    change2 = change2 * 6371000;

    if ((change1 > 2) && (change1 < 500)) {
        long1temp = long2;
        lat1temp = lat2;
    }
    if ((change2 * cos(Rad(theta)) + change2 * sin(Rad(theta)) > lastdist) && (change1 > 2) && (change1 < 500)) {
        dist = change2 * cos(Rad(theta)) + change2 * sin(Rad(theta));
        lastdist = dist;
    }
}

// led on if exceed 100 m
void led_on(long double dist){
if (dist >=  100.0)
  GPIO_PORTF_DATA_R = 0x02;

}

// display on LCD
void printDist(int distance){
int i;
int tmp1 = distance;
int tmp2 = 0;

for(i = 10; i >= 0; i--)
    {
        int num = tmp1 / pow(10,i);                //split last digit from number
        tmp1 = tmp1 - num * pow(10,i);
        tmp2 += num;
        if(tmp2){                              //ignoring zeroes at Most signfient digit
        LCD(1, num + 0x30);
        }
    }

if(dist < 1)
{LCD(1,  '0');}
LCD(1,  'm');
LCD(0, 0x89);

}

int o = 0; // global variabe
void lat_long(){
    char gps[80];
    int i ;
        const char s[2] = ",";
      char* token;
        int j ;

    while(1)
    {   i = 0;
        gps[i]=read_uart();

        if(gps[i] == '$')
        {
            gps[++i]=read_uart();

            if(gps[i] == 'G')
            {
                gps[++i]=read_uart();

                if(gps[i] == 'P')
                {
                    gps[++i]=read_uart();

                    if(gps[i] == 'R')
                    {
                        gps[++i]=read_uart();

                        if(gps[i] == 'M')
                        {
                            gps[++i]=read_uart();

                            if(gps[i] == 'C')
                            {

                                break;
                            }
                        }
                    }
                }
            }
        }
    }
    do {
        gps[++i]=read_uart();

    } while(gps[i] == '$' || i <80);

    //char gps[80] = "$GPRMC,200751.00,A,4047.32510,N,02929.63031,E,9.879,105.80,301117,,,A*6C";
    if ( gps[17]=='A') {
            o = 1;
        token = strtok(gps, s);
        for (j = 0; j < 12; j++) {
            token = strtok(NULL, s);

            if (j == 2) {
                lat2 = floor(atof(token) / 100) + (atof(token) / 100 - floor(atof(token) / 100)) / 0.6;
            }
            if (j == 4) {
                long2 = floor(atof(token) / 100) + (atof(token) / 100 - floor(atof(token) / 100)) / 0.6;
            }   
        }
    }
}

int main(){

initialize(); // call the initialization finction
initalizeLCD();
init_uart();



    while(1){

GPIO_PORTF_DATA_R &= ~0x02;
dist = 0.0;

printOnLCD(Start0, 1);
printOnLCD(Start1, 2);

while((GPIO_PORTF_DATA_R & 0x10)){

if (!(GPIO_PORTF_DATA_R & 0x01)){

printOnLCD(Stop0, 1);
printOnLCD(Stop1, 2);

delay10Ns(10000000);     //delay 10 seconds
printOnLCD(Distance, 1);
printDist(dist);

while((GPIO_PORTF_DATA_R & 0x10)){


        lat_long();
            if (o == 1)
            {
                Total_distance(long2,lat2) ;
            }
            o = 0 ;
      printDist(dist);
      led_on(dist);
      }
      printOnLCD(Stop2, 2);
      delay10Ns(1000000);			//delay 10 seconds
      }

    }

  }

}
