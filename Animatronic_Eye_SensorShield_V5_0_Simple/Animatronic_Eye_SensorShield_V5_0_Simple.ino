// Animatronic Eye Control with Arduino UNO + Sensor Shield V5.0
// 动画眼球控制代码 - 简化快捷命令版本（1-8 数字命令）
// 命令对应常见眼球视线方向和眨眼动作

#include <Servo.h>

// 舵机对象
Servo servoXAxis;       // 眼球左右转
Servo servoYAxis;       // 眼球上下转
Servo servoEyelidTL;    // 上眼睑左
Servo servoEyelidTR;    // 上眼睑右
Servo servoEyelidBL;    // 下眼睑左
Servo servoEyelidBR;    // 下眼睑右

// 引脚定义
#define PIN_X_AXIS 9
#define PIN_Y_AXIS 10
#define PIN_EYELID_TL 11
#define PIN_EYELID_TR 6
#define PIN_EYELID_BL 5
#define PIN_EYELID_BR 3

// 舵机当前位置
int xPos = 90, yPos = 90, eyelidOpen = 180;

void setup() {
  Serial.begin(9600);
  delay(500);
  
  Serial.println("\n╔════════════════════════════════════╗");
  Serial.println("║  Animatronic Eye Control System    ║");
  Serial.println("║  Arduino UNO + Sensor Shield V5.0  ║");
  Serial.println("╚════════════════════════════════════╝");
  
  // 初始化舵机
  servoXAxis.attach(PIN_X_AXIS);
  servoYAxis.attach(PIN_Y_AXIS);
  servoEyelidTL.attach(PIN_EYELID_TL);
  servoEyelidTR.attach(PIN_EYELID_TR);
  servoEyelidBL.attach(PIN_EYELID_BL);
  servoEyelidBR.attach(PIN_EYELID_BR);
  
  delay(200);
  centerEyes();
  delay(500);
  
  Serial.println("\n✓ System Ready\n");
  printCommands();
}

void loop() {
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    input.trim();
    
    if (input.length() > 0) {
      int cmd = input.toInt();
      executeCommand(cmd);
    }
  }
  delay(10);
}

void printCommands() {
  Serial.println("━━━ 快捷命令（输入数字 1-8）━━━");
  Serial.println("1 → 正视前方（直视）");
  Serial.println("2 → 向右看");
  Serial.println("3 → 向左看");
  Serial.println("4 → 向上看");
  Serial.println("5 → 向下看");
  Serial.println("6 → 眨眼一次");
  Serial.println("7 → 眼睛睁大");
  Serial.println("8 → 眼睛闭合");
  Serial.println("9 → 随机扫视");
  Serial.println("0 → 显示此菜单");
  Serial.println("━━━━━━━━━━━━━━━━━━━");
}

void executeCommand(int cmd) {
  switch(cmd) {
    case 1:
      Serial.println("→ 正视前方");
      setEyePosition(90, 90, 180);
      break;
      
    case 2:
      Serial.println("→ 向右看");
      setEyePosition(150, 90, 180);
      break;
      
    case 3:
      Serial.println("→ 向左看");
      setEyePosition(30, 90, 180);
      break;
      
    case 4:
      Serial.println("→ 向上看");
      setEyePosition(90, 150, 180);
      break;
      
    case 5:
      Serial.println("→ 向下看");
      setEyePosition(90, 30, 180);
      break;
      
    case 6:
      Serial.println("→ 眨眼一次");
      blink();
      break;
      
    case 7:
      Serial.println("→ 眼睛睁大");
      setEyePosition(90, 90, 180);
      delay(200);
      break;
      
    case 8:
      Serial.println("→ 眼睛闭合");
      setEyePosition(90, 90, 0);
      delay(300);
      setEyePosition(90, 90, 180);  // 闭眼后自动睁开
      break;
      
    case 9:
      Serial.println("→ 随机扫视");
      randomGaze();
      break;
      
    case 0:
      printCommands();
      break;
      
    default:
      Serial.println("❌ 未知命令，请输入 0-9");
      break;
  }
}

// 设置眼球和眼睑位置
void setEyePosition(int x, int y, int eyelid) {
  xPos = constrain(x, 0, 180);
  yPos = constrain(y, 0, 180);
  eyelidOpen = constrain(eyelid, 0, 180);
  
  servoXAxis.write(xPos);
  servoYAxis.write(yPos);
  updateEyelids(eyelidOpen);
}

// 更新眼睑（上眼睑向上，下眼睑向下，形成张开状态）
void updateEyelids(int openAmount) {
  int topPos = map(openAmount, 0, 180, 0, 180);
  int bottomPos = map(openAmount, 0, 180, 180, 0);
  
  servoEyelidTL.write(topPos);
  servoEyelidTR.write(topPos);
  servoEyelidBL.write(bottomPos);
  servoEyelidBR.write(bottomPos);
}

// 回到中间位置，眼睛张开
void centerEyes() {
  setEyePosition(90, 90, 180);
}

// 眨眼动作
void blink() {
  int speed = 150;  // 眨眼速度（毫秒）
  
  // 关闭眼睑
  for(int i = 180; i >= 0; i -= 20) {
    updateEyelids(i);
    delay(speed / 10);
  }
  
  delay(200);  // 闭眼保持时间
  
  // 睁开眼睑
  for(int i = 0; i <= 180; i += 20) {
    updateEyelids(i);
    delay(speed / 10);
  }
}

// 随机扫视（模拟自然的眼球运动）
void randomGaze() {
  for(int sweep = 0; sweep < 5; sweep++) {
    int randX = random(30, 150);
    int randY = random(30, 150);
    Serial.print("  • 看向 (");
    Serial.print(randX);
    Serial.print(", ");
    Serial.print(randY);
    Serial.println(")");
    
    setEyePosition(randX, randY, 180);
    delay(800);
  }
  
  // 扫视结束回到正视
  Serial.println("  • 恢复正视");
  setEyePosition(90, 90, 180);
}
