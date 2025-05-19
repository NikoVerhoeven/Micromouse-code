const int IRpin1 = 2;  //IR links
const int IRpin2 = 3;  //IR vooruit
const int IRpin3 = 4;  //IR rechts
const int ResetKnopPin = 5;
const int MotorPin11 = 6;
const int MotorPin12 = 7;
const int MotorPin21 = 8;
const int MotorPin22 = 9;
const int MotorPin1EN = 10;
const int MotorPin2EN = 11;
bool finished = false;

int WallsTop[16] = { 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int WallsRight[16] = { 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1 };
int WallsDown[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1 };
int WallsLeft[16] = { 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0 };

int FloodArray[16] =  { 0, 0, 0, 0, 
                        0, 0, 0, 0, 
                        0, 0, 0, 0, 
                        0, 0, 0, 1 };

int position = 0;  //linksboven
int direction = 2;  // 1 = boven, 2 = rechts, 3 = onder, 4 = links

void FloodFill(int FloodArray[16], int WallsTop[16], int WallsRight[16], int WallsDown[16], int WallsLeft[16]) {
  for (int i = 0; i < 15; i++) {
    FloodArray[i] = 0;
  }
  FloodArray[15] = 1;
  for (int i = 1; i < 17; i++) {
    for (int k = 0; k < 16; k++) {
      if (FloodArray[k] == i) {
        if (WallsTop[k] == 0) {
          if (FloodArray[k - 4] == 0) {
            FloodArray[k - 4] = i + 1;
          }
        }
        if (WallsRight[k] == 0) {
          if (FloodArray[k + 1] == 0) {
            FloodArray[k + 1] = i + 1;
          }
        }
        if (WallsDown[k] == 0) {
          if (FloodArray[k + 4] == 0) {
            FloodArray[k + 4] = i + 1;
          }
        }
        if (WallsLeft[k] == 0) {
          if (FloodArray[k - 1] == 0) {
            FloodArray[k - 1] = i + 1;
          }
        }
      }
    }
  }
}

void FindNextStep(int FloodArray[16], int WallsTop[16], int WallsRight[16], int WallsDown[16], int WallsLeft[16], int &position, int &direction) {
  int StartDistance = FloodArray[position];
  if (!WallsRight[position] && FloodArray[position + 1] < StartDistance) {
    //turn to right direction (right)
    int a = ((2 - direction) % 4);
    int b = ((direction - 2) % 4);
    if (a < 0) {
      a += 4;
    }
    if (b < 0) {
      b += 4;
    }
    if (a <= b) {
      TurnRight(a, direction);
    } else {
      TurnLeft(b, direction);
    }
    Forward();
    position += 1;
    //move forward

  } else if (!WallsDown[position] && FloodArray[position + 4] < StartDistance) {
    //turn to right direction (bottom)
    int a = ((3 - direction) % 4);
    int b = ((direction - 3) % 4);
    if (a < 0) {
      a +=4;
    }
    if (b < 0) {
      b +=4;
    }
    if (a <= b) {
      TurnRight(a, direction);
    } else {
      TurnLeft(b, direction);
    }
    Forward();
    position += 4;
    //move forward

  } else if (!WallsLeft[position] && FloodArray[position - 1] < StartDistance) {
    //turn to right direction (left)
    int a = ((4 - direction) % 4);
    int b = ((direction - 4) % 4);
    if (a < 0) {
      a += 4;
    }
    if (b < 0) {
      b +=4;
    }
    if (a <= b) {
      TurnRight(a, direction);
    } else {
      TurnLeft(b, direction);
    }
    Forward();
    position -= 1;
    //move forward
  } else if (!WallsTop[position] && FloodArray[position - 4] < StartDistance) {
    //turn to right direction (top)
    int a = ((1 - direction) % 4);
    int b = ((direction - 1) % 4);
    if (a < 0) {
      a +=4;
    }
    if (b < 0) {
      b +=4;
    }
    if (a <= b) {
      TurnRight(a, direction);
    } else {
      TurnLeft(b, direction);
    }
    Forward();
    position -= 4;
    //move forward
  }
}

void TurnRight(int turns, int &direction) {
  Serial.println("Right");
  Serial.println(turns);
  for (int i = 0; i < turns; i++) {
    digitalWrite(MotorPin11, LOW);
    digitalWrite(MotorPin12, HIGH);
    digitalWrite(MotorPin21, HIGH);
    digitalWrite(MotorPin22, LOW);
    delay(400);
    digitalWrite(MotorPin11, LOW);
    digitalWrite(MotorPin12, LOW);
    digitalWrite(MotorPin21, LOW);
    digitalWrite(MotorPin22, LOW);
    direction++;
    if (direction > 4) {
      direction = 1;
    }
  }
}

void TurnLeft(int turns, int &direction) {
  Serial.println("Left");
  Serial.println(turns);
  for (int i = 0; i < turns; i++) {
    digitalWrite(MotorPin11, HIGH);
    digitalWrite(MotorPin12, LOW);
    digitalWrite(MotorPin21, LOW);
    digitalWrite(MotorPin22, HIGH);
    
    delay(400);
    digitalWrite(MotorPin11, LOW);
    digitalWrite(MotorPin12, LOW);
    digitalWrite(MotorPin21, LOW);
    digitalWrite(MotorPin22, LOW);
    direction--;
    if (direction < 1) {
      direction = 4;
    }
  }
}


void Forward() {
  digitalWrite(MotorPin11, HIGH);
  digitalWrite(MotorPin12, LOW);

  digitalWrite(MotorPin21, HIGH);
  digitalWrite(MotorPin22, LOW);
  delay(470);
  digitalWrite(MotorPin11, LOW);
  digitalWrite(MotorPin12, LOW);
  digitalWrite(MotorPin21, LOW);
  digitalWrite(MotorPin22, LOW);
}

void setup() {
  pinMode(ResetKnopPin, INPUT);
  pinMode(IRpin1, INPUT);
  pinMode(IRpin2, INPUT);
  pinMode(IRpin3, INPUT);
  pinMode(MotorPin11, OUTPUT);
  pinMode(MotorPin12, OUTPUT);
  pinMode(MotorPin21, OUTPUT);
  pinMode(MotorPin22, OUTPUT);
  pinMode(MotorPin1EN, OUTPUT);
  pinMode(MotorPin2EN, OUTPUT);
  Serial.begin(9600);
  delay(20000);
  Serial.println("begin");

  analogWrite(MotorPin1EN, 200);
  analogWrite(MotorPin2EN, 200);

  digitalWrite(MotorPin11, LOW);
  digitalWrite(MotorPin12, HIGH);
  digitalWrite(MotorPin21, LOW);
  digitalWrite(MotorPin22, HIGH);
  delay(50);
  digitalWrite(MotorPin11, LOW);
  digitalWrite(MotorPin12, LOW);
  digitalWrite(MotorPin21, LOW);
  digitalWrite(MotorPin22, LOW);
  delay(500);
}

void loop() {
  Serial.println("checking walls");
  if (digitalRead(IRpin1) == LOW) {
    switch (direction) {
      case 1:
        if (WallsLeft[position] == 0) {
          WallsLeft[position] = 1;
          WallsRight[position - 1] = 1;
        }
        break;
      case 2:
        if (WallsTop[position] == 0) {
          WallsTop[position] = 1;
          WallsDown[position - 4] = 1;
        }
        break;
      case 3:
        if (WallsRight[position] == 0) {
          WallsRight[position] = 1;
          WallsLeft[position + 1] = 1;
        }
        break;
      case 4:
        if (WallsDown[position] == 0) {
          WallsDown[position] = 1;
          WallsTop[position + 4] = 1;
        }
        break;
    }
  }
  if (digitalRead(IRpin2) == LOW) {
    switch (direction) {
      case 1:
        if (WallsTop[position] == 0) {
          WallsTop[position] = 1;
          WallsDown[position - 4] = 1;
        }
        break;
      case 2:
        if (WallsRight[position] == 0) {
          WallsRight[position] = 1;
          WallsLeft[position + 1] = 1;
        }
        break;
      case 3:
        if (WallsDown[position] == 0) {
          WallsDown[position] = 1;
          WallsTop[position + 4] = 1;
        }
        break;
      case 4:
        if (WallsLeft[position] == 0) {
          WallsLeft[position] = 1;
          WallsRight[position - 1] = 1;
        }
        break;
    }
  }
  if (digitalRead(IRpin3) == LOW) {
    switch (direction) {
      case 1:
        if (WallsRight[position] == 0) {
          WallsRight[position] = 1;
          WallsLeft[position + 1] = 1;
        }
        break;
      case 2:
        if (WallsDown[position] == 0) {
          WallsDown[position] = 1;
          WallsTop[position + 4] = 1;
        }
        break;
      case 3:
        if (WallsLeft[position] == 0) {
          WallsLeft[position] = 1;
          WallsRight[position - 1] = 1;
        }
        break;
      case 4:
        if (WallsTop[position] == 0) {
          WallsTop[position] = 1;
          WallsDown[position - 4] = 1;
        }
        break;
    }
  }
  Serial.println("floodfill: ");

  FloodFill(FloodArray, WallsTop, WallsRight, WallsDown, WallsLeft);

  for (int i = 0; i < 16; i++) {
    Serial.print(FloodArray[i]);
  }
  Serial.println(" ");

  delay(2000);
  FindNextStep(FloodArray, WallsTop, WallsRight, WallsDown, WallsLeft, position, direction);

  Serial.println(position);
  Serial.println(direction);

  if (digitalRead(ResetKnopPin) == HIGH && digitalRead(IRpin1) == LOW && digitalRead(IRpin2) == LOW && digitalRead(IRpin3) == HIGH) {
    position = 0;
    direction = 2;
    delay(20000);
  }

  if (position == 15 || digitalRead(ResetKnopPin) == HIGH) {
    finished = true;
  }

  while (finished) {
    if (digitalRead(ResetKnopPin) == HIGH && digitalRead(IRpin1) == LOW && digitalRead(IRpin2) == LOW && digitalRead(IRpin3) == HIGH) {
      position = 0;
      direction = 2;
      delay(20000);
    }
    else if (digitalRead(ResetKnopPin) == HIGH && digitalRead(IRpin1) == HIGH) {
      for (int i = 0; i < 16; i++) {
        if (i == 0 || i == 1 || i == 2 || i == 3) {
          WallsTop[i] = 1;
        } else {
          WallsTop[i] = 0;
        }
        if (i == 3 || i == 7 || i == 11 || i == 15) {
          WallsRight[i] = 1;
        } else {
          WallsRight[i] = 0;
        }
        if (i == 12 || i == 13 || i == 14 || i == 15) {
          WallsDown[i] = 1;
        } else {
          WallsDown[i] = 0;
        }
        if (i == 0 || i == 4 || i == 8 || i == 12) {
          WallsLeft[i] = 1;
        } else {
          WallsLeft[i] = 0;
        }
      }
      position = 0;
      direction = 2;
      finished = false;
      delay(20000);
    }
  }
}
