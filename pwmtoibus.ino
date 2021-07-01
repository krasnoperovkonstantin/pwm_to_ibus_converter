char point, i;
unsigned int vtime;
int buf[7], checksum;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  PORTC = 0b00111111;
  PCICR = 0b00000010;
  PCMSK1 = 0b00111111;
  Serial.begin(115200);
  //timer1
  TCCR1A=0b00000000;
  TCCR1B=0b00000010;   //   cpu/8
  TCCR1C=0b00000000;
}

void loop() {
  // put your main code here, to run repeatedly:
}

ISR(PCINT1_vect){
  vtime = TCNT1;
  TCNT1=0;
  vtime=vtime/2;
  if (vtime>200) 
  {
    digitalWrite(LED_BUILTIN, HIGH); 
    if (vtime>5000) 
    {
      point=0;
    }
    else
    {
      if (point<=5) 
      {
      buf[point]=vtime;
      }
      if (point==5)
      {
        checksum=0xffff;
        Serial.write(0x20);
        Serial.write(0x40);
        checksum=checksum-0x60;
        for (i=0;i<=5;i++)
        {
          Serial.write(buf[i]&0xff);
          Serial.write(buf[i]>>8);
          checksum=checksum-(buf[i]&0xff)-(buf[i]>>8);
        }
        for (i=0;i<=7;i++)
        {
          Serial.write(0xDC);
          Serial.write(0x05);
          checksum=checksum-0xDC-0x05;
        } 
        Serial.write(checksum&0xff);
        Serial.write(checksum>>8);
      } 
      point++; 
    }
  }
  digitalWrite(LED_BUILTIN, 0); 
}
