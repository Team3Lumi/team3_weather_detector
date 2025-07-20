#define BUTTON_PIN_SW4 6  // Chân GPIO của SW4
#define LED_PIN 34        // Chân GPIO của LED

unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

void setup() {
  pinMode(BUTTON_PIN_SW4, INPUT_PULLUP);  // Cấu hình SW4 với pull-up
  pinMode(LED_PIN, OUTPUT);  // Cấu hình chân LED làm output
  Serial.begin(115200);
}

void loop() {
  int buttonState = digitalRead(BUTTON_PIN_SW4);  // Đọc trạng thái nút SW4
  if (buttonState == LOW) {  // Kiểm tra nếu nút nhấn được nhấn xuống
    unsigned long currentTime = millis();
    if (currentTime - lastDebounceTime > debounceDelay) {
      digitalWrite(LED_PIN, HIGH);  // Bật LED khi nút nhấn
      Serial.println("SW4 pressed! LED ON");
      lastDebounceTime = currentTime;
    }
  } else {
    digitalWrite(LED_PIN, LOW);  // Tắt LED khi không nhấn
  }
}