int main(){

initialize(); // call the initialization finction
initalizeLCD();

// long1 , lat1 from gps first time


    while(1){

GPIO_PORTF_DATA_R &= ~0x02;
dist = 5;

printOnLCD(Start0, 1);
printOnLCD(Start1, 2);

while((GPIO_PORTF_DATA_R & 0x10)){

if (!(GPIO_PORTF_DATA_R & 0x01)){

printOnLCD(Stop0, 1);
printOnLCD(Stop1, 2);

delay10Ns(10000000);
printOnLCD(Distance, 1);
printDist(dist);

while(1){

        init_uart();
        lat_long();
            if (o == 1)
            {
                LCD(0,  0x01);                                //Clear
                LCD(0,  0x02);
                //printDist(long2*100000000);
                //delay10Ns(10000000);
                dist = dist + Total_distance(long2,lat2) ;
                long1 = long2 ;
                lat1 =  lat2 ;
            }
            o = 0 ;
      printDist(dist);
      led_on(dist);
      }
      printOnLCD(Stop2, 2);
      delay10Ns(1000000);
      }

    }

  }

}