/* TODO: 角速度のデータも採る
 *  時間も採る
 *  割り込みを使う
*/

#include "AD5328_1.h"
#include "MPU9250.h"

int cs1 = 10;
int cs2 = 9;
AD5328 ad5328_1(cs1);
AD5328 ad5328_2(cs2);

MPU9250 mpu9250;

float pres_MPa = 0.0; // 0~0.8MPA
int valve_port_0 = 0;   // 0~15
int valve_port_1 = 1;
int valve_port_2 = 2;
char show_start;

void Valve(float pres_MPa, int valve_port);

void setup() {
  Serial.begin(9600);
  ad5328_1.begin();
  ad5328_2.begin();
  Wire.begin();
  mpu9250.setup(0x68);
}

void loop() {
  if(mpu9250.update()){
    Serial.print("X : ");
    Serial.print(mpu9250.getAccX());
    Serial.print("  ,  ");
    Serial.print("Y : ");
    Serial.print(mpu9250.getAccY());
    Serial.print("  ,  ");
    Serial.print("Z : ");
    Serial.println(mpu9250.getAccZ());
  }
  
  if (Serial.available()) {
    show_start = Serial.read();
    if (show_start == '1') {
      pres_MPa = 0.6;

      Valve(pres_MPa, valve_port_0);
      Serial.print(valve_port_0);
      Serial.print(" -> ");
      Serial.println(pres_MPa);

      Valve(pres_MPa, valve_port_1);
      Serial.print(valve_port_1);
      Serial.print(" -> ");
      Serial.println(pres_MPa);

      Valve(pres_MPa, valve_port_2);
      Serial.print(valve_port_2);
      Serial.print(" -> ");
      Serial.println(pres_MPa);
/*
      delay(2000);

      pres_MPa = 0;
      Valve(pres_MPa, valve_port_0);
      Serial.print(valve_port_0);
      Serial.print(" -> ");
      Serial.println(pres_MPa);

      Valve(pres_MPa, valve_port_1);
      Serial.print(valve_port_1);
      Serial.print(" -> ");
      Serial.println(pres_MPa);

      Valve(pres_MPa, valve_port_2);
      Serial.print(valve_port_2);
      Serial.print(" -> ");
      Serial.println(pres_MPa);*/
    }
    else if (show_start == '0') {
      pres_MPa = 0;
      Valve(pres_MPa, valve_port_0);
      Serial.print(valve_port_0);
      Serial.print(" -> ");
      Serial.println(pres_MPa);

      Valve(pres_MPa, valve_port_1);
      Serial.print(valve_port_1);
      Serial.print(" -> ");
      Serial.println(pres_MPa);

      Valve(pres_MPa, valve_port_2);
      Serial.print(valve_port_2);
      Serial.print(" -> ");
      Serial.println(pres_MPa);
    }
  }
}

void Valve(float pres_MPa, int valve_port) {
  float pres_bar    = 0.0;
  float V_amplifier = 0.0;
  float V_DAC       = 0.0;
  float V_DAC_bit   = 0.0;

  pres_bar = pres_MPa * 10.0;
  V_amplifier = pres_bar / 8.0 * 10.0;
  V_DAC = V_amplifier / 2.0;        // Gain = 1 + R1 / R2, R1 = R2 = 10kΩ
  V_DAC_bit = V_DAC * 4096.0 / 5.0; // V_DAC = V_ref * V_DAC_bit / 2^N , V_ref = 5V, N = 12bit(分解能)

  switch (valve_port) {
    case 0:
      ad5328_1.write(DAC_A, V_DAC_bit);
      break;

    case 1:
      ad5328_1.write(DAC_B, V_DAC_bit);
      break;

    case 2:
      ad5328_1.write(DAC_C, V_DAC_bit);
      break;

    case 3:
      ad5328_1.write(DAC_D, V_DAC_bit);
      break;

    case 4:
      ad5328_1.write(DAC_E, V_DAC_bit);
      break;

    case 5:
      ad5328_1.write(DAC_F, V_DAC_bit);
      break;

    case 6:
      ad5328_1.write(DAC_G, V_DAC_bit);
      break;

    case 7:
      ad5328_1.write(DAC_H, V_DAC_bit);
      break;

    case 8:
      ad5328_2.write(DAC_A, V_DAC_bit);
      break;

    case 9:
      ad5328_2.write(DAC_B, V_DAC_bit);
      break;

    case 10:
      ad5328_2.write(DAC_C, V_DAC_bit);
      break;

    case 11:
      ad5328_2.write(DAC_D, V_DAC_bit);
      break;

    case 12:
      ad5328_2.write(DAC_E, V_DAC_bit);
      break;

    case 13:
      ad5328_2.write(DAC_F, V_DAC_bit);
      break;

    case 14:
      ad5328_2.write(DAC_G, V_DAC_bit);
      break;

    case 15:
      ad5328_2.write(DAC_H, V_DAC_bit);
      break;
  }
}
