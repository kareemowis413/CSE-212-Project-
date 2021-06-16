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
