//===========================
// LCD declare
//===========================
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2); // (주소,열,행)

int needClearFlag=  0;
int Clear_compare= 0; //Lcd Clear 적재적소에 할 수 있도록 기준 설정


//===========================
// Bluetooth declare
//===========================
#include <SoftwareSerial.h>
#define TX1 8
#define RX1 9
#define TX2 10
#define RX2 11
SoftwareSerial ble1(TX1,RX1);
SoftwareSerial ble2(TX2,RX2);
int Blue_flag = 0;
String myStr = "";


//===========================
// Bus reserve declare
//===========================
#define pin1 4  
#define pin2 5 
#define pin3 6

int start_flag = 2;
int mode = 0;

unsigned long pre_time = 0; // 채터링 방지
unsigned long cur_time = 0; // 채터링 방지

void setup() {
//===========================
// LCD setup
//===========================
  lcd.init(); // 초기화 해주고
  lcd.backlight(); // 백라이트 켜주고

  Serial.begin(9600); // ble값을 보기 위해 추가.
  ble1.begin(9600);
  ble2.begin(9600);

//===========================
// Bus Button setup
//===========================
  pinMode(pin1,INPUT);
  pinMode(pin2,INPUT);
  pinMode(pin3,INPUT);
}

//===========================
// LCD Function
//===========================
void Check_clear(void){
      if(needClearFlag == 1){
        lcd.clear(); 
        needClearFlag = -1;
      }
}
void lcd_normal(void){
      Check_clear();
      lcd.setCursor(1,0); lcd.print("Bus reserve Sys");
      lcd.setCursor(0,1); lcd.print("       ^_^      "); 
}
void lcd_check(void){
      Check_clear();
      lcd.setCursor(1,0); 
      lcd.print("To : Hwagok.   ");
      lcd.setCursor(1,1);
      lcd.print("Do you want?");
}

void lcd_bus_select(void){
      Check_clear();
      lcd.setCursor(1,0); 
      lcd.print("choose Bus");
      lcd.setCursor(0,1);
      lcd.print("1:650,2:70,3:63");
}

void lcd_sending(void){
      Check_clear();
      lcd.setCursor(0,0); 
      lcd.print(" Send your Bus");
      lcd.setCursor(0,1);
      lcd.print("   ..........   ");
}
int pushPin(void){
   if(digitalRead(pin1) == LOW || digitalRead(pin2) == LOW || digitalRead(pin3) == LOW) {
        lcd.clear();
        delay(500);
        
        return 1;

   }
   else return 0;
}

char bus_stop[10] = "Uzangsan";


void loop() {

   if(mode == 1){
      lcd_check();
      start_flag = 0;
      if(digitalRead(pin1) == LOW){
        mode = 2;
        needClearFlag = 1;
        delay(500);
      }
      else if(digitalRead(pin2) == LOW) {
        mode = 0;
        needClearFlag = 1;
        delay(500);
      }
   }
   else if(mode == 2){
      lcd_bus_select();
      int bus_num = -1;
      if(digitalRead(pin1) == LOW){
         bus_num = 650;
      }
      else if(digitalRead(pin2) == LOW) {
         bus_num = 70;
      }
      else if(digitalRead(pin3) == LOW) {
         bus_num = 63;
      }
      if(bus_num > 0){
        char message[50];
        lcd_sending();
        sprintf(message,"%s:hwagok:%dL",bus_stop,bus_num);
        Serial.write(message);
        mode = 0;   
        start_flag = 0;
        delay(5000);
      }
   }
   else if(pushPin())    start_flag = 1;
   else{
    lcd_normal();
   }
   
   if(start_flag == 1){
     mode = 1;
   }

}
