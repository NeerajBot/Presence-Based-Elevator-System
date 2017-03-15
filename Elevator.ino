/*This particular program is designed for the iNTELLIFT,which use atmega8515 microcintroller with arduino config. 
  led display pin : disp : 0=A,1=B
  call out up : cloup, call out down : clodn
  call in : clin
  pins for out : clodn3: 2     cloup2: 3
                 clodn2: 4     cloup1: 5
                 clodn1: 6     cloup0: 7
  pins for in : clin3: 24    clin2: 25
                clin1: 26    clin0: 27
  motor pins : mot : 12, 13
  level : lvl3: 14    lvl2: 15
          lvl1: 10    lvl0: 11
  crtlvl: current level
  alarm: 28
  door: 29
  eleapp:34
  rst: reset pins : rst3: 30    rst2: 31
                    rst1: 32    rst0: 33
*/
//initializing pins
int disp[] = {1, 0};
int mot[] = {12, 13};
int clodn[] = {7, 6, 4, 2};
int cloup[] = {7, 5, 3, 2};
int clin[] = {27, 26, 25, 24};
int lvl[] = {14, 15, 10, 11};
int reset[] = {33, 32, 31, 30};
int alarm = 28;
int door = 29;
int eleapp = 34;
int calllvl;
int crtlvl;
int x;
//addressing i/o
void setup() {
  pinMode(alarm, INPUT);
  pinMode(door, OUTPUT);
  pinMode(eleapp, OUTPUT);
  for (int i = 0; i < 2; i++) {
    pinMode(disp[i], OUTPUT);
    pinMode(mot[i], OUTPUT);
  }
  for (int i = 0; i < 4; i++) {
    pinMode(clodn[i], INPUT);
    pinMode(cloup[i], INPUT);
    pinMode(clin[i], INPUT);
    pinMode(lvl[i], INPUT);
    pinMode(reset[i], OUTPUT);
  }
  rst();
}
//display function
void displ() {
  if (digitalRead(lvl[3]) == HIGH) {
    digitalWrite (disp[0], LOW);
    digitalWrite (disp[1], LOW);
    crtlvl = 0;
  }
  else if (digitalRead(lvl[2]) == HIGH) {
    digitalWrite (disp[0], LOW);
    digitalWrite (disp[1], HIGH);
    crtlvl = 1;
  }
  else if (digitalRead(lvl[1]) == HIGH) {
    digitalWrite (disp[0], HIGH);
    digitalWrite (disp[1], LOW);
    crtlvl = 2;
  }
  else if (digitalRead(lvl[0]) == HIGH) {
    digitalWrite (disp[0], HIGH);
    digitalWrite (disp[1], HIGH);
    crtlvl = 3;
  }
}
//reset function
void rst() {
  digitalWrite(reset[0], HIGH);
  digitalWrite(reset[1], HIGH);
  digitalWrite(reset[2], HIGH);
  digitalWrite(reset[3], HIGH);
}
//door function
void dr() {
  digitalWrite(door, HIGH);
  delay(5);
  digitalWrite(door, LOW);
}
//motor run up function
void motup() {
  digitalWrite(mot[0], LOW);
  digitalWrite(mot[1], HIGH);
  rst();
}
//motor run down function
void motdn() {
  digitalWrite(mot[0], HIGH);
  digitalWrite(mot[1], LOW);
  rst();
}
//motor stop 1 function
void motst1() {
  digitalWrite(mot[0], LOW);
  digitalWrite(mot[1], LOW);
  for (int i = 0; i < 1500; i++) {
    digitalWrite(reset[calllvl], LOW);
    dr();
  }
  rst();
}
//motor stop 2 function
void motst2() {
  digitalWrite(mot[0], LOW);
  digitalWrite(mot[1], LOW);
  digitalWrite(reset[calllvl], LOW);
  rst();
}
//up move function
void up() {
  displ();
  calllvl = crtlvl + 1;
  while (calllvl != crtlvl) {
    displ();
    motup();
    if (crtlvl == calllvl) {
      motst2();
    }
  }
}
//down move function
void down() {
  displ();
  calllvl = crtlvl - 1;
  while (calllvl != crtlvl) {
    displ();
    motdn();
    if (crtlvl == calllvl) {
      motst2();
    }
  }
}
//movement function
void movt() {
  if (calllvl > crtlvl) {
    while (calllvl != crtlvl) {
      displ();
      motup();
      if (crtlvl == calllvl) {
        motst1();
      }
      else if (digitalRead(alarm) == HIGH) {
        up();
        motst1();
        break;
      }
      else if (digitalRead(clin[calllvl]) == LOW && digitalRead(clodn[calllvl]) == LOW && digitalRead(cloup[calllvl]) == LOW) {
        if (digitalRead(clin[calllvl + 1]) == HIGH || digitalRead(cloup[calllvl + 1]) == HIGH) {
          x = 0;
        }
        else if (digitalRead(clin[calllvl + 2]) == HIGH || digitalRead(cloup[calllvl + 2]) == HIGH) {
          x = 1;
        }
        else {
          up();
        }
        break;
      }
    }
  }
  else if (calllvl < crtlvl) {
    while (calllvl != crtlvl) {
      displ();
      motdn();
      if (crtlvl == calllvl) {
        motst1();
      }
      else if (digitalRead(alarm) == HIGH) {
        down();
        motst2();
        break;
      }
      else if (digitalRead(clin[calllvl]) == LOW && digitalRead(clodn[calllvl]) == LOW && digitalRead(cloup[calllvl]) == LOW) {
        if (digitalRead(clin[calllvl - 1]) == HIGH || digitalRead(clodn[calllvl - 1]) == HIGH) {
          x = 0;
        }
        else if (digitalRead(clin[calllvl - 2]) == HIGH || digitalRead(clodn[calllvl - 2]) == HIGH) {
          x = 1;
        }
        else {
          down();
        }
        break;
      }
    }
  }
}
//main loop program
void loop() {
  displ();
  for (int i = 0; i < 4; i++) {
    if (digitalRead(clodn[i]) == HIGH || digitalRead(clin[i]) == HIGH) {
      if (digitalRead(clin[i]) == HIGH) {
        digitalWrite(eleapp, HIGH);
        displ();
        calllvl = i;
        if (calllvl == crtlvl) {
          motst1();
        }
        else {
          movt();
          digitalRead(clin[i]);
        }
        digitalWrite(eleapp, LOW);
      }
      else if (digitalRead(clodn[i]) == HIGH) {
        delay(500);
        if (digitalRead(clodn[i]) == HIGH) {
          digitalWrite(eleapp, HIGH);
          displ();
          calllvl = i;
          if (calllvl == crtlvl) {
            motst1();
          }
          else {
            movt();
            digitalRead(clodn[i]);
          }
          digitalWrite(eleapp, LOW);
        }
      }
    }
  }
  for (int i = 3; i > -1; i--) {
    if (digitalRead(cloup[i]) == HIGH || digitalRead(clin[i]) == HIGH) {
      if (digitalRead(clin[i]) == HIGH) {
        digitalWrite(eleapp, HIGH);
        displ();
        calllvl = i;
        if (calllvl == crtlvl) {
          motst1();
        }
        else {
          movt();
          digitalRead(clin[i]);
        }
        digitalWrite(eleapp, LOW);
      }
      else if (digitalRead(cloup[i]) == HIGH) {
        delay(500);
        if (digitalRead(cloup[i]) == HIGH) {
          digitalWrite(eleapp, HIGH);
          displ();
          calllvl = i;
          if (calllvl == crtlvl) {
            motst1();
          }
          else {
            movt();
            digitalRead(cloup[i]);
          }
          digitalWrite(eleapp, LOW);
        }
      }
    }
  }
}
