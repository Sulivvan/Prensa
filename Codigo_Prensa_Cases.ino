#include <LiquidCrystal.h>

//Entradas
int b1 = 6;
int b2 = 3;
int VAC = 4;
int Sensor = A3;
int b4_ok = A2;
// LCD
int rs = A0;
int rw = 12;
int en = 13;
int d4 = 11;
int d5 = 10;
int d6 = 9;
int d7 = 8;
//Tiempo de prensado
int b3_menu = A1;
int delay_down = 0;
int time_press = 0;
int long_press = 5000;
int normal_press = 100;

int last_menu = 0;
int vac_on = 0;
int salir_sel = 0;
int start = 0;
int start_vac = 0;
int but1_on = 0;
int but2_on = 0;
int sense_it = 0;
int config_menu=0;
int i=1;
LiquidCrystal lcd(rs, rw, en, d4, d5, d6, d7);

int p1 = LOW;
int p2 = LOW;
int s1 = LOW;
//Salidas
int valve1 = 7;
int vaccum = 5;

void setup() {
  //LCD init
  lcd.begin(16, 2);
  lcd.print("Prensa Control");
  lcd.setCursor(0, 1);
  lcd.print("Version 1.0");
  delay(3000);
  //Interrupcion
  //  attachInterrupt();
  //Salidas
  pinMode(valve1, OUTPUT);
  pinMode(vaccum, OUTPUT);
  //Entradas
  pinMode(b1, INPUT);
  pinMode(b2, INPUT);
  pinMode(VAC, INPUT);
  pinMode(Sensor, INPUT);
  pinMode(b3_menu, INPUT);
  pinMode(b4_ok, INPUT);
  lcd.clear();
  lcd.home();
  lcd.print("VAC=OFF");
  lcd.setCursor(0, 1);
  lcd.print("PRENSA=OFF");
  delay(50);
  Serial.begin(9600);
}

void Menu() {
  while (s1 = digitalRead(VAC)) {
    //start=0;
    time_press += 500;
    delay(100);
    //Serial.print(time_press);
  }
  if (time_press >= 4000) {
    start = 0;
    //lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("      Menu    ");
    lcd.setCursor(0, 1);
    lcd.print(">Config     ");
    lcd.setCursor(0, 1);
    delay(200);
    if (s1 = digitalRead(VAC)== HIGH ){
      config_menu=1;
      lcd.clear();
      delay(200);
      while(config_menu==1){
        lcd.home();
        lcd.print("Elegir tiempo");
        lcd.setCursor(0,1);
        lcd.print(i);
        if(p1=digitalRead(b1) == HIGH){
          i+=1;
          //lcd.print(i);
          delay(200);
        }
        else if(p2=digitalRead(b2)==HIGH && i>1){
          i-=1;
          //lcd.print(i);
          delay(200);
        }else if(s1 = digitalRead(VAC)==HIGH){
          i=i;
          config_menu=0;
          break;
        }
      }
    }
    if (p1 = digitalRead(b1) == HIGH) {
      last_menu = 1;
      while (last_menu == 1) {
        salir_sel = 1;
        //lcd.clear();
        lcd.home();
        lcd.print(" Config   ");
        lcd.setCursor(0, 1);
        lcd.print(">Salir ");
        delay(200);
        if (s1 = digitalRead(VAC) == HIGH && last_menu == 1 && salir_sel == 1) {
          time_press = 0;
          last_menu = 0;
          //lcd.clear();
          lcd.home();
          lcd.print("VAC=OFF    ");
          lcd.setCursor(0, 1);
          lcd.print("PRENSA=OFF    ");
          delay(50);
          break;
        }
        if (p1 = digitalRead(b1) == HIGH) {
          last_menu = 0;
          break;
        }
      }
    }
  } else if (time_press < 5000 && time_press > 0) {
    start = 1;
    time_press = 0;
    //lcd.clear();
  }
}

void loop() {
  Menu();
  if (start == 1) { // Cuando este activado el vacio que diga activado VAC=ON y abajo que diga Press=OFF y al activar que diga Press=ON , vacio no se puede desactivar mientras el piston este activado
    if (int s1 = digitalRead(VAC)) {
      start_vac = 1;
      lcd.home();
      lcd.print("VAC=ON   ");
      lcd.setCursor(0,1);
      while (start_vac == 1) {
        Menu();
        time_press = 0;
        //lcd.clear();
        lcd.home();
        lcd.print("VAC=ON         ");
        lcd.setCursor(0, 1);
        lcd.print("PRENSA=OFF ");
        delay(50);
        digitalWrite(vaccum, HIGH);
        vac_on = 1;
        p1 = digitalRead(b1);
        p2 = digitalRead(b2);
        s1 = digitalRead(VAC);
        if (p1 && p2 == HIGH) {
          start_vac = 2;
        }
        delay(20);
        while (s1 == HIGH) {
          digitalWrite(vaccum, LOW);
          //lcd.clear();
          lcd.home();
          lcd.print("VAC=OFF ");
          lcd.setCursor(0, 1);
          lcd.print("PRENSA=OFF ");
          delay(20);
          vac_on = 0;
          start = 0;
          start_vac = 0;
          break;
        } while (start_vac == 2) {
          p1 = digitalRead(b1);
          p2 = digitalRead(b2);
          s1 = digitalRead(VAC);
          while (s1 == HIGH) {
            digitalWrite(vaccum, LOW);
            //lcd.clear();
            lcd.home();
            lcd.print("VAC=OFF ");
            lcd.setCursor(0, 1);
            lcd.print("PRENSA=OFF ");
            delay(20);
            vac_on = 0;
            start = 0;
            start_vac = 0;
            break;
          }
          if (vac_on  == 1 && p1 == HIGH && p2 == HIGH) {
            digitalWrite(valve1, HIGH);
            //lcd.clear();
            lcd.home();
            lcd.print("VAC=ON       ");
            lcd.setCursor(0, 1);
            lcd.print("PRENSA=ON    ");
            delay(100);
            int s4 = digitalRead(Sensor);
            while (s4 == LOW && vac_on  == 1) {
              lcd.home();
              lcd.print("VAC=ON     ");
              lcd.setCursor(0, 1);
              lcd.print("PRENSA=ON     ");
              digitalWrite(valve1, HIGH);
              Serial.println(i);
              delay(i*1000);
              digitalWrite(valve1, LOW);
              //lcd.clear();
              break;
            }
          } else {
            //lcd.clear();
            lcd.home();
            lcd.print("VAC=ON    ");
            lcd.setCursor(0, 1);
            lcd.print("PRENSA=OFF    ");
            delay(100);
            digitalWrite(valve1, LOW);
          }
        }

      }
    }
  }
}



