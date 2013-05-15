/* Stredlip
   Control an RGB LED Strip
   via Serial Console, send HSV values followed by a newline
*/

  const byte dim_curve[] = {
    0,   1,   1,   2,   2,   2,   2,   2,   2,   3,   3,   3,   3,   3,   3,   3,
    3,   3,   3,   3,   3,   3,   3,   4,   4,   4,   4,   4,   4,   4,   4,   4,
    4,   4,   4,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   6,   6,   6,
    6,   6,   6,   6,   6,   7,   7,   7,   7,   7,   7,   7,   8,   8,   8,   8,
    8,   8,   9,   9,   9,   9,   9,   9,   10,  10,  10,  10,  10,  11,  11,  11,
    11,  11,  12,  12,  12,  12,  12,  13,  13,  13,  13,  14,  14,  14,  14,  15,
    15,  15,  16,  16,  16,  16,  17,  17,  17,  18,  18,  18,  19,  19,  19,  20,
    20,  20,  21,  21,  22,  22,  22,  23,  23,  24,  24,  25,  25,  25,  26,  26,
    27,  27,  28,  28,  29,  29,  30,  30,  31,  32,  32,  33,  33,  34,  35,  35,
    36,  36,  37,  38,  38,  39,  40,  40,  41,  42,  43,  43,  44,  45,  46,  47,
    48,  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,
    63,  64,  65,  66,  68,  69,  70,  71,  73,  74,  75,  76,  78,  79,  81,  82,
    83,  85,  86,  88,  90,  91,  93,  94,  96,  98,  99,  101, 103, 105, 107, 109,
    110, 112, 114, 116, 118, 121, 123, 125, 127, 129, 132, 134, 136, 139, 141, 144,
    146, 149, 151, 154, 157, 159, 162, 165, 168, 171, 174, 177, 180, 183, 186, 190,
    193, 196, 200, 203, 207, 211, 214, 218, 222, 226, 230, 234, 238, 242, 248, 255,
};

int rLedPin = 9;
int gLedPin = 10;
int bLedPin = 11;

int tdelay = 2;
int minsdiff = 50;
int minvdiff = 50;

byte r;
byte g;
byte b;

int h;
int s;
int v;
void setup()  { 
  // nothing happens in setup 
  s = v = r = g = b = 0;
  h = -1;
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  Serial.print("Send HSV values like \"H S V T\\n\" with T for delay\n\r");
  
} 

void loop()  { 
while (Serial.available() > 0) {
    char buff[255];
    int nh, nv, ns;
    memset( buff, 0, sizeof(buff));
    if( !Serial.readBytesUntil('\r', buff, 255) )
       break;
    else
    {
      if( sscanf(buff, "%d %d %d %d", &nh, &ns, &nv, &tdelay ) != 4 )
      {
        Serial.print("Send HSV values like \"H S V T\\n\" with T for delay\n\r");
        break;
      }

      Serial.print(nh);
      Serial.print(ns);
      Serial.print(nv);
      Serial.print(tdelay);
      break;
    }

//    // look for the next valid integer in the incoming serial stream:
//    int nh = Serial.parseInt();
//    // do it again:
//    int ns = Serial.parseInt();
//    // do it again:
//    int nv = Serial.parseInt();
//    
//    tdelay = Serial.parseInt();
    

    // look for the newline. That's the end of your
    // sentence:
    byte b = Serial.read();
    if (b == '\n' || b == '\r' ) {
      nh = constrain(nh, 0, 360);
      ns = constrain(ns, 0, 100);
      nv = constrain(nv, 0, 100);
      
      if( h != nh || (abs(ns - s) > minsdiff ) || (abs( nv - v ) > minvdiff ) )
      {
        h = nh;
        s = ns;
        v = nv;
        newColor();
      }
      else
      {
        while( s != ns || v != nv )
        {
          if( s < ns )
            s += 1;
          else if( s > ns )
            s -= 1;
          
          if( v < nv )
            v +=1;
          else if( v > nv )
            v -= 1;
            
          HSV_to_RGB(h, s, v, &r, &g, &b);
          analogWrite(rLedPin, dim_curve[r]);
          analogWrite(gLedPin, dim_curve[g]);
          analogWrite(bLedPin, dim_curve[b]);
          delay(tdelay*2);
        }
        HSV_to_RGB(h, s, v, &r, &g, &b);
        analogWrite(rLedPin, dim_curve[r]);
        analogWrite(gLedPin, dim_curve[g]);
        analogWrite(bLedPin, dim_curve[b]);
        delay(tdelay*2);
      }
      
    }
  }
}

void newColor( )
{
  while( r || g || b )
  {
    if( r )
      r -= 1;
    if( g )
      g -= 1;
    if( b )
      b -= 1;
    analogWrite(rLedPin, dim_curve[r]);
    analogWrite(gLedPin, dim_curve[g]);
    analogWrite(bLedPin, dim_curve[b]);
    delay(tdelay);
  }
  analogWrite(rLedPin, 0);
  analogWrite(gLedPin, 0);
  analogWrite(bLedPin, 0); 
  delay(tdelay);
  byte nr, ng, nb;
  HSV_to_RGB(h, s, v, &nr, &ng, &nb);
  while( r != nr || b != nb || g != ng )
  {
    if( r < nr )
      r += 1;
    if( g < ng )
      g += 1;
    if( b < nb )
      b += 1;
    analogWrite(rLedPin, dim_curve[r]);
    analogWrite(gLedPin, dim_curve[g]);
    analogWrite(bLedPin, dim_curve[b]);
    delay(tdelay);
  }
}

/*
  HSV_to_RGB originally by hdznrrd
  https://gist.github.com/hdznrrd/656996)
*/
void HSV_to_RGB(float h, float s, float v, byte *r, byte *g, byte *b)
{
int i,f,p,q,t;
h = max(0.0, min(360.0, h));
s = max(0.0, min(100.0, s));
v = max(0.0, min(100.0, v));
s /= 100;
v /= 100;
if(s == 0) {
// Achromatic (grey)
*r = *g = *b = round(v*255);
return;
}
 
h /= 60; // sector 0 to 5
i = floor(h);
f = h - i; // factorial part of h
p = v * (1 - s);
q = v * (1 - s * f);
t = v * (1 - s * (1 - f));
switch(i) {
case 0:
*r = round(255*v);
*g = round(255*t);
*b = round(255*p);
break;
case 1:
*r = round(255*q);
*g = round(255*v);
*b = round(255*p);
break;
case 2:
*r = round(255*p);
*g = round(255*v);
*b = round(255*t);
break;
case 3:
*r = round(255*p);
*g = round(255*q);
*b = round(255*v);
break;
case 4:
*r = round(255*t);
*g = round(255*p);
*b = round(255*v);
break;
default: // case 5:
*r = round(255*v);
*g = round(255*p);
*b = round(255*q);
}
}


