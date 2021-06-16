// led on if exceed 100 m
void led_on(long double dist) 
{
    if (dist >= 100.0)
        GPIO_PORTF_DATA_R = 0x02;

}

int main(){

initialize();    //Call the initialization of the GPIO of tiva

initalizeLCD();
/*Call the intialization commands, data length set, then clear the lcd screen 
of any previous data, return home command, and turning the display on + controlling the cursor*/ 

init_uart(); //uart ports initializations' 



    while(1){

GPIO_PORTF_DATA_R &= ~0x02;
dist = 0.0;
        
/*print on the lcd the message to print for the gps programe to start by using SW2*/
printOnLCD(Start0, 1); //press SW2 to, 1stROW
printOnLCD(Start1, 2);//Start 2ndROW

while((GPIO_PORTF_DATA_R & 0x10)){

if (!(GPIO_PORTF_DATA_R & 0x01)){

printOnLCD(Stop0, 1);
printOnLCD(Stop1, 2);

delay10Ns(10000000);     //delay 10 seconds
/*printing on lcd the word "Distance= " and the calculated distance right after it. AT 1stROW*/
printOnLCD(Distance, 1);
printDist(dist);

while((GPIO_PORTF_DATA_R & 0x10)){

/* o is a variable set to idintefy if the recived data from the gps are correct, after reseting the value of O to 0 and then re-check*/
        lat_long();
            if (o == 1)
            {
                Total_distance(long2,lat2) ;
            }
            o = 0 ;
      printDist(dist);
      led_on(dist); //red led on after dist value reachs 100meter
      }
      printOnLCD(Stop2, 2);
      delay10Ns(1000000);			//delay 10 seconds
      }

    }

  }

}
