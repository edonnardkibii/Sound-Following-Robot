float x=0;
float y=0;
int a=0;
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
x=analogRead(A0);
y=0.9391*y+0.0609*x;
a++;
Serial.println(y);
delay(300);


}
