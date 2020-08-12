int number1 = 0;
int number2 = 0;
bool third = false;
char myOperator;
byte least;
byte most;
byte rem;
boolean data1 = false;
boolean data2 = false;
boolean data3 = false;
boolean exception = false;
boolean welcome = false;
int a = 12;
int b = 11;
int c = 2;
int d = 3;
int e = 4;
int f = 6;
int g = 7;
#define digit1 10
#define digit2 8
#define digit3 13

void setup() { 
  pinMode(a,OUTPUT);
  pinMode(b,OUTPUT);
  pinMode(c,OUTPUT);
  pinMode(d,OUTPUT);
  pinMode(e,OUTPUT);
  pinMode(f,OUTPUT);
  pinMode(g,OUTPUT);   
  pinMode(digit1,OUTPUT);
  pinMode(digit2,OUTPUT);
  pinMode(digit3,OUTPUT);
  Serial.begin(9600);
}

void loop() {
  digitalWrite(digit3,HIGH);
  digitalWrite(digit1,HIGH);
  digitalWrite(digit2,HIGH);
  if (welcome == false){
    Serial.println("                ******** Welcome to the AVR-assembled calculator ********");
    Serial.println("");
    Serial.println(" + : addition");
    Serial.println(" - : subtraction");
    Serial.println(" * : multiplication");
    Serial.println(" / : division");
    Serial.println(" g : greatest common divisor");
    Serial.println(" l : least common factor");
    Serial.println(" % : remainder");
    Serial.println("___________________________________________________________________________________________");
    welcome = true;
  }
  readNumber1();
  readMyOperator();
  readNumber2();
  showNewData();
  SHOW(least);
}

void readNumber1() {
 if (Serial.available() > 0 && data1 == false) {
    number1 = Serial.parseInt();
    data1 = true;
    third = false;
    int num1 = number1;
    Serial.print(num1);
 }
}

void readMyOperator() {
 if (Serial.available() > 0 && data1 == true && data2 == false) {
    myOperator = Serial.read();

    Serial.print(" ");
    Serial.print(myOperator);
    Serial.print(" ");
    data2 = true;
 }
}

void readNumber2() {
 if (Serial.available() > 0 && data1 == true && data2 == true && data3 == false) {
    number2 = Serial.parseInt();
    data3 = true;
    Serial.print(number2);
 }
}

void showNewData() {
 if (data3 == true) {
    if  (myOperator == '+')
    {
      add_8_s(least, number1, number2);
      if( least > 127)
      {
        if( number1 >= 0 && number2 >= 0)
        {
          least = 99;
        }
        else
        {
          least = 256 - least;
          if(least > 99)
            least = 99;          
          third = true;
        }
        
      }
    }
    else if(myOperator == '-')
    {
      sub_8_s(least, number1, number2);
      if( least > 127)
      {
        least = 256 - least;
        third = true;
      }
    }
    else if(myOperator == '*')
    {
      if( number1 < 0 && number2 <0)
      {
        number1 *= -1;
        number2 *= -1;
      }
      else if(number1 < 0 && number2 >= 0)
      {
        number1 *= -1;
        third = true;
      }
      else if(number2 < 0 && number1 >= 0)
      {
        number2 *= -1;
        third = true;
      }
      mpy_8_s(least, number1, number2);
    }
    else if(myOperator == '/')
    {
      if(number2 != 0){
        if(number1 >= 0 && number2 < 0)
        {
          number2 *= -1;
          div_8_u(least, rem, number1, number2);
         // least = 256 - least;
          third = true;        
        }
        else if(number1 < 0 && number2 < 0)
        {
          number1 *= -1;
          number2 *= -1;
          div_8_u(least, rem, number1, number2);
          if( rem != 0)
          {
            rem = number2 - rem;
            least = least + 1;
          }
        }
        if(number1 < 0 && number2 >= 0)
        {
          number1 *= -1;
          div_8_u(least, rem, number1, number2);
          if( rem != 0)
          {
            rem = number2 - rem;
            least = least + 1;
          }
          third = true;        
        }
        else 
          div_8_u(least, rem, number1, number2);
      }
      else{
        Serial.println("     Error : divided by zero");
        exception = true;
      }
    }
    


    else if(myOperator == '%')
    {
      if (number2 != 0){
      if(number1 >= 0 && number2 < 0)
      {
        number2 *= -1;
        div_8_u(least, rem, number1, number2);
       // least = 256 - least;
        third = true;        
      }
      else if(number1 < 0 && number2 < 0)
      {
        number1 *= -1;
        number2 *= -1;
        div_8_u(least, rem, number1, number2);
        if( rem != 0)
        {
          rem = number2 - rem;
          least = least + 1;
        }
      }
      if(number1 < 0 && number2 >= 0)
      {
        number1 *= -1;
        div_8_u(least, rem, number1, number2);
        if( rem != 0)
        {
          rem = number2 - rem;
          least = least + 1;
        }
      }
      else 
        div_8_u(least, rem, number1, number2);
      least = rem;
      }
      else{
        Serial.println("     Error : divided by zero");
        exception = true;
      }
    }

    
    else if(myOperator == 'g')
        if(number1 < 0 || number2 < 0)
        {
          Serial.println("     Error : invalid input. Please enter new numbers.");
          exception = true;
        }
        else{
          gcd_8_u(least, number1, number2);
        }
    else if(myOperator == 'l')
      lcf_8_u(least, number1, number2);
    else{
      Serial.println("     Error : invalid operator. Please enter new operator. ");
      exception = true;
    }
    if(exception == false){
      Serial.print(" = ");
      if(third == true)
        Serial.print("-");
      if( least > 99 && least <= 127)
        least = 99;   
      Serial.println(least);
   }else{
    exception = false;
   }
   data1 = false;
   data2 = false;
   data3 = false;
  }
}
void SHOW(int n){
  bool off_first = false;
  if (n > 99) n = 99;
  if (n < -99) n = -99;
  if(n < 0) n *= -1;
  char first = n%10;
  n /= 10;
  char second = n%10;
  if(second == 0){
    second = first;
    off_first = true;
  }
  if (off_first == false){
  digitalWrite(digit1,HIGH);
  digitalWrite(digit3,LOW);
  Seven_seg(first,0);
  delay(20);
  }
  digitalWrite(digit3,HIGH);
  digitalWrite(digit2,LOW);
  Seven_seg(second,0);
  delay(20);  
  digitalWrite(digit2,HIGH);
  digitalWrite(digit1,LOW);
  Seven_seg(10,third);
  delay(20);
}

void Seven_seg(char n,bool sign){
    digitalWrite(a,LOW);
    digitalWrite(b,LOW);
    digitalWrite(c,LOW);
    digitalWrite(d,LOW);
    digitalWrite(e,LOW);
    digitalWrite(f,LOW);
    digitalWrite(g,LOW); 
    if(sign == false && n != 10){
      if(n != 1 && n != 4 )//Turn seg 'a' on
           digitalWrite(a,HIGH);
      if(n != 5 && n != 6) //Turn seg 'b' on
           digitalWrite(b,HIGH);
      if(n != 2)  //Turn seg 'c' on
           digitalWrite(c,HIGH);
      if(n != 1 && n != 4 && n != 7 )//Turn seg 'd' on
           digitalWrite(d,HIGH); 
      if(n == 2 || n == 6 || n == 8 || n == 0)//Turn seg 'e' on 
         digitalWrite(e,HIGH);
      if(n != 1 && n != 2 && n != 3 && n != 7)//Turn seg 'f' on
         digitalWrite(f,HIGH);
      if(n != 1 && n != 0 && n != 7)//Turn seg 'g' on
         digitalWrite(g,HIGH);
    }
    else if(n == 10 && sign == 1){
      digitalWrite(g,HIGH);
    }
}

void sub_8_s( byte &result, byte operand1, byte operand2)// subtract 8 bit signed 
{
 asm volatile ( 
  "ldi %0, 0 \n\t" 
  "sub %1, %2 \n\t"
  "mov %0, %1 \n\t" 
  : "+r" (result), "+r" (operand1),  "+r" (operand2)
   );
}
void add_8_s( byte &result, byte operand1, byte operand2) // addition 8 bit signed
{
 asm volatile ( 
  "ldi %0, 0 \n\t" 
  "add %1, %2 \n\t"
  "mov %0, %1 \n\t" 
  : "+r" (result), "+r" (operand1),  "+r" (operand2)
   );
}
void mpy_8_s( byte &result, byte multiplicand, byte multiplier) // multiplication 8 bit signed
{
 asm volatile ( 
  "ldi %0, 0 \n\t" 
  "mul %1, %2 \n\t"
  "mov %0, R0 \n\t" 
  : "+r" (result), "+r" (multiplicand),  "+r" (multiplier)
   );
}
void div_8_u(byte &result,byte &remainder,byte dividend, byte divisor) 
{
    byte lc = 0;
    asm volatile ( 
  "div8u:  sub %1,%1 \n\t" /*clear remainder and carry*/ 
  " ldi %4,9 \n\t" /*init loop counter*/ 
  "d8u_1: rol %2 \n\t" /*shift left dividend*/
  " dec %4 \n\t" /*decrement counter*/ 
  " brne  d8u_2 \n\t" /*if done*/ 
  " rjmp d8u_4 \n\t" /*    return*/ 
  "d8u_2: rol %1 \n\t" /*shift dividend into remainder*/ 
  " sub %1,%3 \n\t" /*remainder = remainder - divisor*/ 
  " brcc  d8u_3 \n\t" /*if result negative*/ 
  " add %1,%3 \n\t" /*    restore remainder*/ 
  " clc \n\t" /*    clear carry to be shifted into result*/ 
  " rjmp  d8u_1 \n\t" /*else*/ 
  "d8u_3: sec \n\t" /*    set carry to be shifted into result*/ 
  " rjmp  d8u_1 \n\t" 
  "d8u_4: mov %0,%2 \n\t" 
  " mov %1,%1 \n\t" 
  : "+r" (result), "+r" (remainder) , "+r" (dividend),  "+r" (divisor), "+r" (lc));  
}
void gcd_8_u(byte &result, byte number1, byte number2){
  if(number1 == 0 || number2 == 0)
  {
    result = 0;
    return;
  }
  int a = number1, b = number2;
  int c = a, d = b;
  asm volatile(
    "loop: mov %2, %0 \n\t"
    "mov %3, %1 \n\t"
    "sub  %2, %3 \n\t"
    "breq  done \n\t"
    "brge  positive \n\t"
    "sub  %1, %0 \n\t"
    "jmp loop \n\t"
    "positive:  mov %0, %2 \n\t"
    "jmp loop \n\t"
    "done: mov %4,%0 \n\t"
    :"+r" (a), "+r" (b), "+r" (c), "+r" (d), "+r" (result)
    );  
}
void lcf_8_u(byte &result, byte number1, byte number2){
  mpy_8_s(result, number1, number2);
  byte mul = result;
  gcd_8_u(result, number1, number2);
  byte gcd = result;
  result = mul / gcd;
}

