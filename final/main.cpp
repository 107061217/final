#include "mbed.h"

#include "bbcar.h"

 

#define duration 1.5

 

DigitalOut led3(LED3);

DigitalOut led2(LED2);

DigitalOut led1(LED1);

 

PwmOut pin9(D9), pin8(D8);

 

DigitalInOut pin10(D10);

DigitalIn pin3(D3);
Serial xbee(D12, D11); 
Serial pc(USBTX,USBRX); 
Serial uart(D1,D0); 
float data[10];
Ticker servo_ticker;

Ticker encoder_ticker;

Timer t;

Thread camera;

 

BBCar car(pin8, pin9, servo_ticker);

 

float dist;

parallax_encoder encoder0(pin3, encoder_ticker);

void straight(int speed, int len)

{

    encoder0.reset();

    car.goStraight(speed);

    while (encoder0.get_cm() < len)

        wait_ms(50);

    car.stop();
    xbee.printf("straight\n\r");
    wait(0.5);

}

 

void front_left()

{

    car.turn(60, 0.1); // turn left

    wait(duration + 0.08);

    car.stop();
    xbee.printf("left\n\r");
    wait(0.5);

}

void back_left()

{

    car.turn(-5.5, 10); // turn left

    wait(duration + 0.08);

    car.stop();
    xbee.printf("left\n\r");
    wait(0.5);

}

void back_right()

{

    car.turn(-68, 0.1); // turn right

    wait(duration + 0.08);

    car.stop();
    xbee.printf("right\n\r");
    wait(0.5);

}

void front_right()

{

    car.turn(6.0, 10); // turn right

    wait(duration + 0.08);

    car.stop();
    xbee.printf("right\n\r");

    wait(0.5);

}


int main()

{

    double pwm_table0[] = {-150, -120, -90, -60, -30, 0, 30, 60, 90, 120, 150};

    double speed_table0[] = {-10.445, -9.812, -9.647, -9.408, -5.900, 0.000, 5.900, 10.843, 11.880, 11.401, 12.199};

    double pwm_table1[] = {-150, -120, -90, -60, -30, 0, 30, 60, 90, 120, 150};

    double speed_table1[] = {-10.445, -9.812, -9.647, -9.408, -5.900, 0.000, 5.900, 10.843, 11.880, 11.401, 12.199};
    car.setCalibTable(11, pwm_table0, speed_table0, 11, pwm_table1, speed_table1);

    led1 = 1;

    led2 = 1;

    led3 = 1;

    xbee.printf("start!\n\r"); /
    parallax_ping  ping1(pin10);
    int i=0;
 
    // go straight to until find data matrix

    straight(80, 119);

    front_left();


    //straight(80, 53);
    led1 = 0;
    xbee.printf("mission 1\n\r");
    straight(80, 80);

    //photo
    back_right();

    straight(-80, 5);
    char s[21];
    sprintf(s,"image_classification"); 
    uart.puts(s); 
    wait(5);
    xbee.printf("taking picture\n\r");
    // reverse parking
    straight(80, 5);
    back_left();

    straight(-80, 4);

    back_right();

    straight(-80,15 );

    xbee.printf("parking\n\r");
    straight(80, 21);

    front_right();

    straight(80, 20);


    front_right();

    straight(80, 110);
    xbee.printf("mission 2\n\r");
    front_right();

 

 

    // set led

    led2 = 0;



    //straight(80, 10);
    straight(80,10);

    front_right();

    straight(80, 20);

    for(i=0;i<10;i++)
    {   
        car.turn(-30,0.5);
        data[i] = ping1;
        wait(0.02);
    }
    car.turn(30,0.5);
    wait(0.4);
    car.stop();


    straight(-80, 19);

    back_left();

    straight(80, 63);

    front_right();


    led3 = 0;


    straight(80, 150);
    
    for(i=0;i<10;i++)
    {
        xbee.printf("%f\n\r",data[i]);
        wait(0.1);
    }
    int flag = 0;


    for(i=0;i<10;i++)

    {

        xbee.printf("%f\n\r",data[i]);

        wait(0.1);
    }

    float a[50];

    for(i=0;i<10;i++)

    {

        a[i] = data[i+1] - data[i];

    }

    for(i=0;i<10;i++)

    {

        if(a[i] * a[i+1] == 0){

            flag = 0; 

        }

        else if(a[i] * a[i+1] > 0){

            flag = 1;

        }

        else if(a[i] * a[i+1] < 0){

            flag = 2;

        }

        else flag = 3;

    }


        if((a[10] <= a[25]) && (a[25] <= a[40])){

            flag = 0; 

        }

        else if((a[10] >= a[25]) && (a[25] <= a[40])){

            if (a[25] - a[10] > 3) flag = 1;

            else flag = 3;

        }

        else flag = 2;


    if(flag == 0){

        xbee.printf("right triangle");

    }

    else if(flag = 1){

        xbee.printf("triangle");

    }

    else if(flag == 2){

        xbee.printf("other");

    }

    else{

        xbee.printf("rectangle");

    }

   
}