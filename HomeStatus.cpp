#include <iostream>
#include <mraa/gpio.h>
#include <mraa/aio.h>
#include <time.h>
#include <upm/lcm1602.h>
#include <mraa/pwm.h>


// Instantiate an Lcm1602 class
upm::Lcm1602 *lcd = new upm::Lcm1602(6, 0x20);




void date(){
char buf_d[10]="";
struct tm today;
time_t t;
time(&t);
today = *localtime(&t);
sprintf(buf_d, "Date:%d %d/%d", today.tm_year+1900, today.tm_mon+1,
today.tm_mday);
lcd-> setCursor(1,0);
lcd-> write(buf_d);
}

void temp() {
float value;
float celcius;
char buf[10]="";
mraa_aio_context TEMP;
TEMP = mraa_aio_init(2);
value = mraa_aio_read(TEMP);
celcius = (value*500)/1024;
sprintf(buf, "TEMP:%.1f C",
celcius);
lcd->setCursor(1,0);
lcd->write(buf);
sleep(1);
}





int main() {
mraa_gpio_context light;
mraa_gpio_context button;
mraa_gpio_context upbutton;
mraa_gpio_context buzzer;
mraa_gpio_context relay;
mraa_aio_context menu_select;
mraa_pwm_context servo; 

upbutton = mraa_gpio_init(2);
servo = mraa_pwm_init(3);           
buzzer = mraa_gpio_init(11);
button = mraa_gpio_init(8);  
light = mraa_gpio_init(12);  
menu_select = mraa_aio_init(1);
relay = mraa_gpio_init(9);
float duty;
mraa_gpio_dir(button, MRAA_GPIO_IN);
mraa_gpio_context touch;
touch = mraa_gpio_init(7);
mraa_gpio_dir(touch, MRAA_GPIO_IN);
mraa_gpio_dir(buzzer,MRAA_GPIO_OUT);
mraa_gpio_dir(light,MRAA_GPIO_OUT);          
mraa_gpio_dir(relay,MRAA_GPIO_OUT);                                        
while(1){
	int btstate = mraa_gpio_read(button);
	lcd->setCursor(0,0);
	lcd->write("****HOME Care******");
	sleep(1);
		while (mraa_aio_read(menu_select) > 100){
			if ( mraa_aio_read(menu_select) <300){
				lcd->setCursor(0,0);	
				lcd->write("**Temperature**");
				temp();	
			}
			if( mraa_aio_read(menu_select) >300 && mraa_aio_read(menu_select) <500){
			lcd->setCursor(0,0);
			lcd->write("Turn on Light   ");
			lcd->setCursor(1,0);
			lcd->write("1.YES,2.NO      ");
			sleep(2);
				if(mraa_gpio_read(upbutton) == 1 ){
				mraa_gpio_write(light,1);

				lcd->setCursor(0,0);
				lcd->write("light : yes");	
				sleep(2);
				}
				if (mraa_gpio_read(button) ==1){
				mraa_gpio_write(light,0);
				lcd->setCursor(0,0);                                                                               
				lcd->write("light : OFF");
				sleep(2);
				}
			}
	if( mraa_aio_read(menu_select) >500 && mraa_aio_read(menu_select) <800)
	{



        lcd->setCursor(0,0);
        lcd->write("MANAGE ALL POWER  ");
        lcd->setCursor(1,0);
        lcd->write("1.ON,2.OFF      ");
        sleep(2);
		if(mraa_gpio_read(upbutton) == 1 ){
        		mraa_gpio_write(relay,1);
        		lcd->setCursor(0,0);
        		lcd->write("POWER : ON");
        		sleep(2);
		}
		if(mraa_gpio_read(button) == 1 ){
        		mraa_gpio_write(relay,0);
        		lcd->setCursor(0,0);
        		lcd->write("POWER : OFF");
        		sleep(2);
		}

	}
if( mraa_aio_read(menu_select) >800){
	lcd->setCursor(0,0);
lcd->write("***Time***");
date();
}


}

if(mraa_gpio_read(button) == 1){
		mraa_gpio_write(buzzer,1);		
		sleep(1);
		mraa_gpio_write(buzzer,0);
		lcd->setCursor(0,0);
		lcd->write("Who are you    ");	
		sleep(3);		
		
		if(mraa_gpio_read(touch)){
		lcd->setCursor(1,0);
		lcd->write("authorize      ");
		sleep(2);
                mraa_pwm_period_ms(servo, 20);
                mraa_pwm_enable(servo, 1);


		for(duty = 0.03 ; duty < 0.12 ; duty += 0.01){
		mraa_pwm_write(servo, duty);
		usleep(10000);
		}
		sleep(2);
		for(duty = 0.11 ; duty>0.03 ; duty -= 0.01){
		mraa_pwm_write(servo, duty);
		usleep(10000);
                }
	
	}	

}
}
}	
