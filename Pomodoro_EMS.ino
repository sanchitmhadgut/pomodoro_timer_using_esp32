#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

#include <Wire.h>
#include <TM1650.h>
#include <ErriezDS3231.h>

#include "button_scan.h"

// Create RTC object
ErriezDS3231 rtc;


// object
TM1650 d;

// semaphore
SemaphoreHandle_t displaySemaphore;


// Pins
static const uint8_t LED_PIN = 2;

// variables define

// Timer variables
hw_timer_t *timer = NULL;
volatile uint32_t isrCounter = 0;  // Counts every 1 ms
volatile bool timerflg = false;  // Flag to signal 10 ms interval

char rtcBuff[] = "0000";
char btnBuff[] = "0000";

volatile bool rtcflg = false;
volatile bool btnflg = false;
 

// protyping
uint8_t Button_Scan(void);

// define two tasks for Blink & AnalogRead
void displaySeg(void *pvParameters);
void ButtonScan(void *pvParameters);
void RtcTime(void *pvParameters);

void IRAM_ATTR onTimer() {
    isrCounter++;  // Increment counter every 1 ms
    if (isrCounter % 1000 == 0) {  // Check if 1000 ms have passed
        timerflg = true;  // Set flag to execute function
        isrCounter = 0;  // Reset counter
    }
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Wire.begin();
  Wire.setClock(100000);
  d.init();

  d.displayOff();
  d.setBrightness(TM1650_MIN_BRIGHT);
  d.displayOn();
  delay(100);

  pinMode(buttonUp, INPUT);
  pinMode(buttonDwn, INPUT);
  pinMode(buttonBck, INPUT);
  pinMode(buttonEntr, INPUT);

  // Initialize hardware timer
  timer = timerBegin(0, 80, true);              // Timer 0, prescaler 80 (1 us tick), count up
  timerAttachInterrupt(timer, &onTimer, true);  // Attach ISR
  timerAlarmWrite(timer, 1000, true);           // Set alarm to 1000 ticks (1 ms)
  timerAlarmEnable(timer);                      // Enable the timer alarm

   // Set date/time: 12:34:56 31 December 2020 Sunday
// if (!rtc.setTime(18, 50, 00)) {
//   Serial.println(F("Set date/time failed"));
//   return;
// }


rtc.setSquareWave(SquareWaveDisable);

  // Create binary semaphore
  displaySemaphore = xSemaphoreCreateBinary();  


  // Now set up two tasks to run independently.
  xTaskCreatePinnedToCore(
    displaySeg
    , "displaySeg"  // A name just for humans
    , 4096  // This stack size can be checked & adjusted by reading the Stack Highwater
    , NULL
    , 2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    , NULL
    , ARDUINO_RUNNING_CORE);

  xTaskCreatePinnedToCore(
    ButtonScan
    , "ButtonScan"  // A name just for humans
    , 1024  // This stack size can be checked & adjusted by reading the Stack Highwater
    , NULL
    , 3  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    , NULL
    , ARDUINO_RUNNING_CORE);

  xTaskCreatePinnedToCore(
    RtcTime
    , "RtcTime"  // A name just for humans
    , 2048  // This stack size can be checked & adjusted by reading the Stack Highwater
    , NULL
    , 1  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    , NULL
    , ARDUINO_RUNNING_CORE);  

    // Release semaphore to initialize display task
    xSemaphoreGive(displaySemaphore); 

    d.setDot(1,false);

}


void loop() {
  // put your main code here, to run repeatedly:
}

void displaySeg(void *pvParameters)  // This is a task.
{
  (void)pvParameters;
  bool showrtc = true;
  for (;;) {

    // Wait for semaphore signal
    if (xSemaphoreTake(displaySemaphore, portMAX_DELAY) == pdTRUE) {

      if (timerflg == true) {
        timerflg = false;
        // Debug output
        if (showrtc && rtcflg) {

          d.displayString(rtcBuff);
          d.setDot(1, true);
          d.setBrightnessGradually(TM1650_MAX_BRIGHT);
          Serial.print("RTC Updated: ");
          Serial.println(rtcBuff);
          rtcflg = false;

        } else if (!showrtc && btnflg) {
          d.displayString(btnBuff);
          d.setDot(1, false);
          d.setBrightnessGradually(TM1650_MAX_BRIGHT);
          Serial.print("Button Updated: ");
          Serial.println(btnBuff);
          btnflg = false;
        }
        //Alernate
        showrtc = !showrtc;
      }
    }
  }

  // Check remaining stack size

  // Serial.print("displaySeg Free Stack: ");
  // Serial.println(uxTaskGetStackHighWaterMark(NULL));

  // vTaskDelay(500/ portTICK_PERIOD_MS);
}

void ButtonScan(void *pvParameters) {
  (void)pvParameters;
  for (;;) {
    int button_status = Button_Scan();

    
    Serial.println(button_status);
    snprintf(btnBuff, sizeof(btnBuff), "%04d", button_status);
    btnflg = true;

    vTaskDelay(200/ portTICK_PERIOD_MS);

    // Signal display task
    xSemaphoreGive(displaySemaphore);

    
  }
}

void RtcTime(void *pvParameters){
  (void)pvParameters;
  for (;;) {
    uint8_t hour;
    uint8_t min;
    uint8_t sec;

    // Read date/time
  if (rtc.getTime(&hour, &min, &sec)) {
    snprintf(rtcBuff, sizeof(rtcBuff), "%02d%02d", hour,min);
    rtcflg = true;

    // Signal display task
    xSemaphoreGive(displaySemaphore);

  }
  else{
    Serial.println(F("Read date/time failed"));
  }

  // Serial.println(min); //////////************
    vTaskDelay(1000/ portTICK_PERIOD_MS);
  }
}
