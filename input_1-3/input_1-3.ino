const int YELLOW_IN = 12;
const int YELLOW_OUT = 11;
const int GREEN_IN = 6;
const int GREEN_OUT = 7;
const int BLUE_IN = 2;
const int BLUE_OUT = 3;

void setup() {
  // put your setup code here, to run once:
  pinMode(YELLOW_IN, INPUT);
  pinMode(YELLOW_OUT, OUTPUT);
  pinMode(GREEN_IN, INPUT);
  pinMode(GREEN_OUT, OUTPUT);
  pinMode(BLUE_IN, INPUT_PULLUP);
  pinMode(BLUE_OUT, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int yellowIn = digitalRead(YELLOW_IN);
  digitalWrite(YELLOW_OUT, yellowIn);
  
  int greenIn = digitalRead(GREEN_IN);
  digitalWrite(GREEN_OUT, !greenIn);

  int blueIn = digitalRead(BLUE_IN);
  digitalWrite(BLUE_OUT, !blueIn);
}
