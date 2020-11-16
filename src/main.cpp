// библиотека для работы с ардуино
#include <Arduino.h>

// константы портов и времени
#define PIN_POT A0
#define BREAK_TIME 100
#define NUMBER_UPDATE 9600
#define PORT_BUTTON_FORWARD 8
#define PORT_BUTTON_BACK 9
#define TIME_DELAY_BUTTON 100

// переменные состояния кнопок
bool stateButtonForward, stateButtonBack;
bool oldStateButtonForward, oldStateButtonBack;

// таймеры для кнопок
unsigned long timerButtonForward;
unsigned long timerButtonBack;
 
// переменные для гашетки и штурвала
float angle; 
int stateTriggerSwitch = 0; // "0" - остановка, "1" - малый вперёд, "2" - полный вперёд, "-1" - малый назад, "-2" - полный назад

// цикл настройки
void setup()
{
  // частота обновления
  Serial.begin(NUMBER_UPDATE);

  // пин с переменным резистором является входом, чтение аналоговое
  pinMode(PIN_POT, INPUT);

  // пины для кнопок
  pinMode(PORT_BUTTON_FORWARD, OUTPUT);
  pinMode(PORT_BUTTON_BACK, OUTPUT);
}
 
// цикл контроллера
void loop(){
  // cчитывание в переменную rotat напряжения с переменного резистора:
  // микроконтроллер будет выдавать числа от 0 до 1023
  // пропорциональны положению поворота вала
  // конвертируем в 1,5 оборота, знак означает направление поворота
  angle = (analogRead(PIN_POT) - 512)/682.0;
 
  // записываем состояния кнопок
  stateButtonBack = digitalRead(PORT_BUTTON_BACK);
  stateButtonForward = digitalRead(PORT_BUTTON_FORWARD);
  
  // при нажатии нvа кнопку назад
  if ((stateButtonBack)*(!oldStateButtonBack)*(millis()-timerButtonBack >= TIME_DELAY_BUTTON)){
    timerButtonBack = millis();

    // уменьшаем положение гащетки
    if (stateTriggerSwitch > -2){
      stateTriggerSwitch = stateTriggerSwitch - 1;
    }
  }
  // переприсываивание состояний
  oldStateButtonBack = stateButtonBack;

   // при нажатии нvа кнопку вперед
  if ((stateButtonForward)*(!oldStateButtonForward)*(millis()-timerButtonForward >= TIME_DELAY_BUTTON)){
    timerButtonForward = millis();

        // увелививаем положение гащетки
    if (stateTriggerSwitch < 2){
      stateTriggerSwitch = stateTriggerSwitch + 1;
    }
  }
  // переприсываивание состояний
  oldStateButtonForward = stateButtonForward;

  // печатаем в серил порт угол и состояние гашетки
  Serial.println(angle);
  Serial.println(stateTriggerSwitch);

  // делаем задержку (чтобы с какой-то переодичностью считывать данные)
  delay(BREAK_TIME);
}