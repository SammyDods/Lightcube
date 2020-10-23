/*
 _     _____ _____  _   _ _____   _____ _   _______ _____   _____ _   _ _____ _   _ _____ 
| |   |_   _|  __ \| | | |_   _| /  __ \ | | | ___ \  ___| |_   _| | | |_   _| \ | |  __ \
| |     | | | |  \/| |_| | | |   | /  \/ | | | |_/ / |__     | | | |_| | | | |  \| | |  \/
| |     | | | | __ |  _  | | |   | |   | | | | ___ \  __|    | | |  _  | | | | . ` | | __ 
| |_____| |_| |_\ \| | | | | |   | \__/\ |_| | |_/ / |___    | | | | | |_| |_| |\  | |_\ \
\_____/\___/ \____/\_| |_/ \_/    \____/\___/\____/\____/    \_/ \_| |_/\___/\_| \_/\____/
 * Sammy Dods Computer Engineering
 * 2018/10/0
*/

int posts[] = {13, 9, 5, 14, 12, 8, 4, 15, 11, 7, 3, 16, 10, 6, 2, 17};      //mapped
//int posts[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, A0, A1, A2, A3};                  // Anode pins in order they represent as posts
int ledNums[] = {0, 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768}; //Binary corrosponding to pin number

//Instiate array that holds location for all 64 LEDs
bool ledArray[6][17] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

int numArray[] = {0,0,0,0,0}; //Holds binary code for each layer

int delay3 = 0; //Declare Delay Variables
int delay4 = 10;

void setup() {
  
  Serial.begin(9600); //Set Data rate for serial transmission.
  for (int next = 0; next < 16; next++) // sets anodes as outputs
  {
    pinMode(posts[next], OUTPUT);
  }
  pinMode(A4, OUTPUT);  //sets cathodes as outputs
  pinMode(A5, OUTPUT);
  
}

void loop() {//Main Loop

  seq1();
  rain();
  sparkle();

}

void layer(int layerNum){//Turns on layer per argument.
  switch (layerNum){
      case 1:
          digitalWrite(A4, LOW);
          digitalWrite(A5, LOW);
          break;
      case 2:
          digitalWrite(A4, LOW);
          digitalWrite(A5, HIGH);
          break;
      case 3:
          digitalWrite(A4, HIGH);
          digitalWrite(A5, LOW);
          break;
      case 4: 
          digitalWrite(A4, HIGH);
          digitalWrite(A5, HIGH);
          break;
      default:
          break;
  }
}

void LightITUp(unsigned int commandCode) {//Turns on leds per commandCode
  for (int x = 0; x < 16; x++) {
    digitalWrite (posts[x], LOW);
    
  }
  for (int x = 0; x < 16; x++) {
    if (bitRead(commandCode, x) == 1) {
      digitalWrite (posts[x], HIGH);
    }
  }

  delayMicroseconds(500);  //After a short delay turn off leds

  for (int x = 0; x < 16; x++) {
    digitalWrite (posts[x], LOW);
  }
}

void lightUpCube(){ //Lights up any indexes in ledArray that are ==1

   //Calculates binary values based off of what leds should turn on.
   for (int x = 4; x >=1; x--){
    numArray[x] = 0;
    
    for (int y = 1; y <=16; y++){//Checks what leds are turned on and converts to binary code.
      if (ledArray[x][y] == 1){
        numArray[x] += ledNums[y];
        
        }
      }
    }
    
   for (int x = 0; x <= 5; x++){//Lights up cube x times.
    
    for (int y = 4; y >=1; y--){ //Light up each layer with corisponding binary code.
      
      layer(y); 
      LightITUp(numArray[y]);
      //Serial.println(numArray[y]);
      delay(1);
   
    }
  }
}

void clearLedArray(){ //Sets value of all indexes in ledArray to 0
for (int y = 0; y <=6; y++){

   for (int x = 0; x <= 17; x++){
  
    ledArray[y][x] = 0;
   }
}
}

int newDrop(){//Finds an led in the top layer that is turned off
  int newNum = random(1,17);
  
  while (ledArray[4][newNum] != 0){
    newNum = random(1,17);
    
  }
  return newNum;
  
}

void rain(){ //LEDs light up randomly and appear to stack on one another

  int nextDrop = 0;

  for (int x = 1; x <=64; x++){
  nextDrop = newDrop(); 
  
  for (int plane=4; plane >=1; plane--){

    if (ledArray[plane][nextDrop] == 0){//If the light is empty, light up led and turn off led above.
      ledArray[plane][nextDrop] = 1;
      ledArray[plane+1][nextDrop] = 0;
      lightUpCube();

      }
    }
  }
clearLedArray();//Reset for next call

}

void randomLedOff(int &newX, int &newY){ //Finds a random led that is turned off, takes reference arguments for led# and led layer
    newX = random(1,17);
    newY = random(1,5);
    
  while (ledArray[newY][newX] != 0){
    newX = random(1,17);
    newY = random(1,5);
    
  }
}

void randomLedOn(int &newX, int &newY){//Finds a random led that is turned on, takes reference arguments for led# and led layer
    newX = random(1,17);
    newY = random(1,5);

  while (ledArray[newY][newX] != 1){
    newX = random(1,17);
    newY = random(1,5);

  }
}

void sparkle(){//Turns all Leds on in random order then off.
  int nextDrop = 0;
  int lightX=0;
  int lightY=0;

  for (int x = 1; x <=64; x++){//Turns 64 leds on randomly
  randomLedOff(lightX,lightY);
  
    if (ledArray[lightY][lightX] == 0){
      ledArray[lightY][lightX] = 1;
      lightUpCube();
      
    }
  }
  for (int x = 1; x <=64; x++){//Turns 64 leds off randomly
  randomLedOn(lightX,lightY);
  
    if (ledArray[lightY][lightX] == 1){
      ledArray[lightY][lightX] = 0;
      lightUpCube();
      
    }
}
  
clearLedArray();//Reset for next call
}

void lightUpFour(int layerOne, int LayerTwo, int LayerThree, int LayerFour){//Lights up all layers for delay4 amount of time. Takes binary code as argmuments.
   for (int x =0; x <= delay4; x++){
     layer(1);
     LightITUp(layerOne);
     delay(delay3);
     LightITUp(0);
     layer(2);
     LightITUp(LayerTwo);
     delay(delay3);
     LightITUp(0);
     layer(3);
     LightITUp(LayerThree);
     delay(delay3);
     LightITUp(0);
     layer(4);
     LightITUp(LayerFour);
     delay(delay3);
     LightITUp(0);
  }
}

void seq1() {//Lights up a pattern in two directions.
  //Loops 4 times
  for (int x = 0; x <= 4; x++){   
    lightUpFour(51219,33079,4991,14334);
    lightUpFour(33079,4991,14334,32748);
    lightUpFour(4991,14334,32748,65224);
    lightUpFour(14334,32748,65224,60545);
    lightUpFour(32748,65224,60545,51219);
    lightUpFour(65224,60545,51219,33079);
    lightUpFour(60545,51219,33079,4991);
    }
    //Seccond Pattern
  for (int x = 0; x <= 4; x++){
    lightUpFour(12684,6350,36079,52983);
    lightUpFour(6350,36079,52983,61299);
    lightUpFour(36079,52983,61299,63281);
    lightUpFour(52983,61299,63281,29464);
    lightUpFour(61299,63281,29464,12684);
    lightUpFour(63281,29464,12684,6350);
    lightUpFour(29464,12684,6350,36079);
  }
}
