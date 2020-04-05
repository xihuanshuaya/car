#include <Servo.h>
#include <IRremote.h>  // 使用IRRemote函数库
//定义五中运动状态
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
int rightMotor2 = 13;
int irReceiverPin = 2;  // 红外接收器的 OUTPUT 引脚接在 PIN2 
IRrecv irrecv(irReceiverPin); // 设置irReceiverPin定义的端口为红外信号接收端口


void setup() {
  // put your setup code here, to run once:
  //设置控制电机的引脚为输出状态
  pinMode(leftMotor1, OUTPUT);
  pinMode(leftMotor2, OUTPUT);
  pinMode(rightMotor1, OUTPUT);
  pinMode(rightMotor2, OUTPUT);
  Serial.begin(9600);
  irrecv.enableIRIn(); // 初始化红外接收器

}

void loop() {
  // put your main code here, to run repeatedly:

  decode_results results;    // 定义results变量为红外结果存放位置
  if   (irrecv.decode(&results)) {   // 解码成功，把数据放入results.value变量中
    Serial.println(results.value, HEX);//以16进制换行输出接收代码
    // 0 ,stop
    if   (results.value==0xFF6897)
    {
      Serial.println("auto");
      stop_car();
      forward( ) ;
      delay(2000);
      stop_car();
      delay(100);
      
      backward() ;
      delay(2000);
      stop_car();
      delay(100);
      
      turn_left();
      delay(500);
      stop_car();
      delay(100);
      
      turn_right();
      delay(500);
      stop_car();
      delay(100);
      
      turn_around();
      delay(3200);
      stop_car();
      
    }//1前进
    else if((results.value==0xFF30CF))
    {
      forward( ) ;
      Serial.println("go");
      delay(2000);
      stop_car();
    }//2后退
    else if(results.value==0xFF18E7)
    {
      backward( ) ;
      Serial.println("go back");
      delay(2000);
      stop_car();
    }//3 turn left
    else if(results.value==0xFF7A85)     
    {
      turn_left( );
      Serial.println("turn left");
      delay(320);
      stop_car();
    }//4 turn right
    else if(results.value==0xFF10EF) 
    {
      turn_right( );
      Serial.println("turn right");
      delay(320);
      stop_car();
    } //turn around
    else if((results.value==0xFF38C7))    
    {
      turn_around( );
      Serial.println("turn around");
      delay(3157);
      stop_car();
    }
    else{
      Serial.println("unknown command, stop");
      stop_car();
    }

    irrecv.resume();    // 继续等待接收下一组信号
    delay(500);
  }

}
//运动控制函数
void motorRun(int cmd)
{
  switch(cmd){
  case FORWARD:
    digitalWrite(leftMotor1, LOW);
    digitalWrite(leftMotor2, HIGH);
    digitalWrite(rightMotor1, LOW);
    digitalWrite(rightMotor2, HIGH);
    break;
  case BACKWARD:
    digitalWrite(leftMotor1, HIGH);
    digitalWrite(leftMotor2, LOW);
    digitalWrite(rightMotor1, HIGH);
    digitalWrite(rightMotor2, LOW);
    break;
  case TURNLEFT:
    digitalWrite(leftMotor1, HIGH);
    digitalWrite(leftMotor2, LOW);
    digitalWrite(rightMotor1, LOW);
    digitalWrite(rightMotor2, HIGH);
    break;
  case TURNRIGHT:
    digitalWrite(leftMotor1, LOW);
    digitalWrite(leftMotor2, HIGH);
    digitalWrite(rightMotor1, HIGH);
    digitalWrite(rightMotor2, LOW);
    break;
  default:
    digitalWrite(leftMotor1, LOW);
    digitalWrite(leftMotor2, LOW);
    digitalWrite(rightMotor1, LOW);
    digitalWrite(rightMotor2, LOW);
  }
}

void forward(){
  motorRun(FORWARD);
}

void stop_car(){
  motorRun(STOP);
}

void backward(){
  motorRun(BACKWARD);
}

void turn_left(){
  motorRun(TURNLEFT);
}

void turn_right(){
  motorRun(TURNRIGHT);
}

void turn_around(){
  motorRun(TURNRIGHT);
}













