#include "PID.h"

PID pid0;
PID pid1;
PID pid2;
PID pid3;

void setup() {
  pinMode(13, OUTPUT);
  //  ele();
  pid0.init(4.7, 0.06, 0);
  pid1.init(3.4, 0.04, 0);
  pid2.init(6.2, 0.05, 0);
  pid3.init(0, 0, 0);
  Serial.begin(115200);
  Serial1.begin(100000, SERIAL_8E1);
}

void loop() {
  int u[4] = {};
  msg.id = 0x200;
  msg.len = 8;
  for (int i = 0; i < 4; i++) {
    u[i] = 0;
  }
  if (((data[5] & 0xC0) >> 6) == 1) {
    if (millis() - tim < 500) {
      for (int i = 0; i < 4; i++) {
        u[i] = 0;
      }
      //      u[0] = (millis() - tim) * 4;
      //      u[1] = (millis() - tim) * 4;
      //      u[2] = (millis() - tim) * 4;
    }
    else if (millis() - tim < 5500) {
      float vx, vy, vt;
      //      u[0] = 2000;
      //      u[1] = 2000;
      //      u[2] = 2000;
      vx = 0;//目標最大rpmは5000
      vy = 200;
      vt = 0;
      u[0] = (int)((vy + vt) / 0.27557830294);
      u[1] = (int)((sin120 * vx - 0.5 * vy + vt) / 0.27557830294);
      u[2] = (int)((-sin120 * vx - 0.5 * vy + vt) / 0.27557830294);
    }
    else if (millis() - tim < 2000) {
      for (int i = 0; i < 4; i++) {
        u[i] = 0;
      }
      //      u[0] = 8000 - (millis() - tim) * 4;
      //      u[1] = 8000 - (millis() - tim) * 4;
      //      u[2] = 8000 - (millis() - tim) * 4;
    }
    else {
      u[0] = 0;
      u[1] = 0;
      u[2] = 0;
    }
  }
  else if (((data[5] & 0xC0) >> 6) == 2) {
    tim = millis();
    u[0] = 0;
    u[1] = 0;
    u[2] = 0;
  }
  else if (((data[5] & 0xC0) >> 6) == 3) {
    tim = millis();
    float vx, vy, vt;
    vx = map(testch[3], 364, 1684, -5000, 5000);//目標最大rpmは5000
    vy = map(testch[2], 364, 1684, -5000, 5000);
    vt = map(testch[0] - 6, 364, 1684, -2500, 2500);
    u[0] = (int)((vy + vt) / 0.27557830294);
    u[1] = (int)((sin120 * vx - 0.5 * vy + vt) / 0.27557830294);
    u[2] = (int)((-sin120 * vx - 0.5 * vy + vt) / 0.27557830294);
    //    u[0] = map(testch[3], 364, 1684, -1000, 1000);//目標最大rpmは5000
    //    u[1] = map(testch[0], 364, 1684, -1000, 1000) - 12;
    //    u[2] = map(testch[2], 364, 1684, -1000, 1000);
    Serial.print(vx);
    Serial.print(",");
    Serial.print(vy);
    Serial.print(",");
    Serial.print(vt);
    Serial.print(",");
    Serial.print(pid0.debug());
    Serial.print(",");
    Serial.print(pid1.debug());
    Serial.print(",");
    Serial.print(pid2.debug());
    Serial.println("");
  }
  //  Serial.print(u[1]);
  u[0] = pid0.pid_out(u[0]);
  u[1] = pid1.pid_out(u[1]);
  u[2] = pid2.pid_out(u[2]);
  for (int i = 0; i < msg.len; i++) {
    msg.buf[i * 2] = u[i] >> 8;
    msg.buf[i * 2 + 1] = u[i] & 0xFF;
  }
  for (int i = 0; i < 4; i++)u[i] = 0;
  //  Serial.print(",");
  //  Serial.print(pid0.debug());
  //  Serial.print(",");
  //  Serial.print(pid1.debug());
  //  Serial.print(",");
  //  Serial.print(pid2.debug());
  //  Serial.println("");
  delay(10);
}
