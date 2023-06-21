const short led_pin{ 13 }, toggle_pin{ 8 };
unsigned long start{}, elapsed{};
constexpr unsigned long mins_to_millis_conversion_factor{ static_cast<unsigned long>(60) * 1000 };
constexpr long double millis_to_mins_conversion_factor{ 1.00/(static_cast<long double>(60) * 1000) };
//-------------------------------------------------------------------------------
// set time limit here

const short timeLimitMins{ 1 };
//-------------------------------------------------------------------------------
// function declarations

short constexpr millis_to_minutes(const auto);
unsigned constexpr long minutes_to_millis(const auto);
void toggle_switch(auto);

//-------------------------------------------------------------------------------
void setup() {
  Serial.begin(9600);
  pinMode(led_pin, OUTPUT);
  pinMode(toggle_pin, OUTPUT);
  // Serial.print("mins_to_millis_conversion_factor");
  // Serial.println(mins_to_millis_conversion_factor);
  // Serial.print("millis_to_mins_conversion_factor");
  // Serial.println(millis_to_mins_conversion_factor);
}

void loop() {
  
  // turn on switch
  toggle_switch(true);

  // initialize
  start = millis();
  Serial.print("start: ");
  Serial.println(start);
  elapsed = start;
  // wait until time elapsed is past the set limit
  const auto timeLimitMillis = minutes_to_millis(timeLimitMins);
  Serial.print("timeLimitMillis: ");
  Serial.println(timeLimitMillis);
  
  // start timer
  while(elapsed < timeLimitMillis) {
    elapsed = millis() - start;
    Serial.print("elapsed: ");
    Serial.println(elapsed);
    delay(1000);
  }
  // turn off switch & do nothing
  toggle_switch(false);
  while(true) {}

}

//-------------------------------------------------------------------------------
// function definitions

short constexpr millis_to_minutes(const auto millisVal) {
  const auto result{ millisVal * millis_to_mins_conversion_factor};
  return round(result);
}

unsigned long constexpr minutes_to_millis(const auto minsVal) {
  const auto result{ minsVal * mins_to_millis_conversion_factor };
  return result;
}

void toggle_switch(auto val) {
  if(val) {
    digitalWrite(led_pin, HIGH);
    digitalWrite(toggle_pin, HIGH);
  }
  else {
    digitalWrite(led_pin, LOW);
    digitalWrite(toggle_pin, LOW);
  }
  return;
}