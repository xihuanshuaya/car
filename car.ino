#include <Servo.h>
#include <SoftwareSerial.h>
//#include <TaskScheduler.h>
//定义运动状态
#define STOP      0
#define FORWARD   1
#define BACKWARD  2
#define TURNLEFT  3
#define TURNRIGHT 4
#define TURNAROUND 5

//定义需要用到的引脚
int leftMotor1 = 7;  
int leftMotor2 = 8;
int rightMotor1 = 12;
int rightMotor2 = 11;

int leftPWM = 5;
int rightPWM = 6;
int speedLevel = 500;
int led = 13;
Servo myservo;
int hc05RX = 9;
int hc05TX = 10;
SoftwareSerial BT(hc05RX, hc05TX);  //soft serial

//超声波传感器
int trigerPin = 4;
int echoPin = 3;

long distance = 1000;
long previousMills = 0;

int stopPin = 13;


void setup() {
  // put your setup code here, to run once:
  //设置控制电机的引脚为输出状态
  pinMode(leftMotor1, OUTPUT);
  pinMode(leftMotor2, OUTPUT);
  pinMode(rightMotor1, OUTPUT);
  pinMode(rightMotor2, OUTPUT);
  pinMode(leftPWM, OUTPUT);
  pinMode(rightPWM, OUTPUT);
  Serial.begin(9600);                       


  BT.begin(9600);
  BT.println("Hello from Arduino");

  analogWrite(leftPWM, speedLevel);
  analogWrite(rightPWM, speedLevel);
  //D1中断，执行停车操作
  pinMode(stopPin, OUTPUT);
  digitalWrite(stopPin, LOW);
  attachInterrupt(0, stop_car2, RISING);
}

void loop() {
  // put your main code here, to run repeatedly:
  //超声波测距''
    measure_distance();


  //use bluetooth to control
  if (BT.available()) // if text arrived in from BT serial...
  {
    char a = (BT.read());

    if (a == '?')
    {
      help_command();

    }

    if (a == 'a')
    {
      auto_drive();
    }

    if (a == 'g')
    {
      go_ahead();
    }

    if (a == 'b')
    {
      Serial.println("back mode");
      BT.println("back mode");
      backward( ) ;
//      delay(2000);
//      stop_car();
    }

    if (a == 'l')
    {
      Serial.println("turn left mode");
      BT.println("turn left mode");
      turn_left( );
      delay(100);
      stop_car();
    }

    if (a == 'r')
    {
      Serial.println("turn right mode");
      BT.println("turn right mode");
      turn_right( );
      delay(100);
      stop_car();
    }

    if (a == 'i')
    {
      Serial.println("speed up");
      BT.println("speed up");
      ledFlash();
      if (speedLevel < 200 ) {
        speedLevel += 50;
      }
      analogWrite(leftPWM, speedLevel);
      analogWrite(rightPWM, speedLevel);
    }

    if (a == 'd')
    {
      Serial.println("speed down");
      BT.println("speed down");
      ledFlash();
      if (speedLevel > 50 ) {
        speedLevel -= 50;
      }
      analogWrite(leftPWM, speedLevel);
      analogWrite(rightPWM, speedLevel);
    }

    if (a == 's')
    {
      Serial.println("stop mode");
      BT.println("stop mode");
      stop_car();
    }

    if (a == '0')
    {
      Serial.println("turn around");
      BT.println("turn around");
      turn_around( );
      delay(3157);
      stop_car();
    }

  }




}
//运动控制函数
void motorRun(int cmd)
{
  switch (cmd) {
    case FORWARD:
      ledFlash();
      digitalWrite(leftMotor1, LOW);
      digitalWrite(leftMotor2, HIGH);
      digitalWrite(rightMotor1, LOW);
      digitalWrite(rightMotor2, HIGH);
      break;
    case BACKWARD:
      ledFlash();
      digitalWrite(leftMotor1, HIGH);
      digitalWrite(leftMotor2, LOW);
      digitalWrite(rightMotor1, HIGH);
      digitalWrite(rightMotor2, LOW);
      break;
    case TURNLEFT:
      ledFlash();
      digitalWrite(leftMotor1, HIGH);
      digitalWrite(leftMotor2, LOW);
      digitalWrite(rightMotor1, LOW);
      digitalWrite(rightMotor2, HIGH);
      break;
    case TURNRIGHT:
      ledFlash();
      digitalWrite(leftMotor1, LOW);
      digitalWrite(leftMotor2, HIGH);
      digitalWrite(rightMotor1, HIGH);
      digitalWrite(rightMotor2, LOW);
      break;
    default:
      ledFlash();
      digitalWrite(leftMotor1, LOW);
      digitalWrite(leftMotor2, LOW);
      digitalWrite(rightMotor1, LOW);
      digitalWrite(rightMotor2, LOW);
  }
}

void forward() {
  motorRun(FORWARD);
}

void stop_car() {
  motorRun(STOP);
}

void stop_car2() {
  Serial.println("触发中断！!!");
  motorRun(STOP);
}


void backward() {
  motorRun(BACKWARD);
}

void turn_left() {
  motorRun(TURNLEFT);
}

void turn_right() {
  motorRun(TURNRIGHT);
}

void turn_around() {
  motorRun(TURNRIGHT);
}

void ledFlash() {
  digitalWrite(led, HIGH);
  delay(1);
  digitalWrite(led, LOW);
  delay(1);
}

void help_command() {
  Serial.println("command:");
  BT.println("Send 'a' to auto mode");
  BT.println("Send 'g' to go");
  BT.println("Send 'b' to back");
  BT.println("Send 'l' to turn left");
  BT.println("Send 'r' to turn right");
  BT.println("Send 'i' to speed up");
  BT.println("Send 'd' to speed down");
  BT.println("Send 's' to stop");
  BT.println("Send '0' to turn around");
}

void auto_drive() {
  forward();
  delay(2000);
  backward();
  delay(2000);
  turn_left();
  delay(2000);
  turn_right();
  delay(2000);
  stop_car();
}

void go_ahead() {
  Serial.println("go mode");
  BT.println("go mode");
  forward( ) ;
  Serial.println("go");
  delay(2000);
  stop_car();
}

void measure_distance() {
  Serial.println("start measure distance");
  pinMode(trigerPin, OUTPUT);
  long duration;
  digitalWrite(trigerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigerPin, LOW);
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
  Serial.print(duration);
  Serial.println(" us");

  long distance = duration * 340 / (2 * 1000);
  Serial.println(distance);
  Serial.println(" mm");

// stop car when distance is less than 500 mm
  if (distance < 500 && distance != 0 ) {
    Serial.println("检测到障碍物，刹车！");
    Serial.println(distance);
    BT.println("检测到障碍物，刹车！");
    BT.println(distance); 
    //触发硬件中断，(stopPin连接到D0中断，数字编号2 pin) 
    digitalWrite(stopPin, HIGH);
  }
  else {
    digitalWrite(stopPin, LOW);
  }

}
