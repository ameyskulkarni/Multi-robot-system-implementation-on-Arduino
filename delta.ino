//GAMMA

int motor_left[] = {6,7};
int motor_right[] = {8,9};
int coordinates[40][2];
int rightir = 2;
int centreir = 3;
int leftir = 4;
int flag = 0;
int inByte;
int k,r;
int i=0,curr_i,curr_j,next_i,next_j,curr_h,next_h;
int enc_r = 0,enc_l = 0;
int bitcount =0;
int data[50];
float s;
float theta,rdist,ldist;
int r_speed=40, l_speed=40;

#define TILE_COUNT 20
#define TURN_COUNT 5
#define BOT_WIDTH 0.16
#define W_RADIUS 0.034


void brake()
{

  analogWrite(motor_right[0], 0); 
  analogWrite(motor_right[1], 0); 



  analogWrite(motor_left[0], 0); 
  analogWrite(motor_left[1], 0);  
}
void backward(){ 

  //LEFT wheel reverse
  analogWrite(motor_left[0], 0); 
  analogWrite(motor_left[1], 40); 

  //RIGHT wheel reverse
  analogWrite(motor_right[0], 40); 
  analogWrite(motor_right[1], 0);
}

void forward(){ 

  //LEFT wheel forward
  analogWrite(motor_left[0], l_speed); 
  analogWrite(motor_left[1], 0); 

  //RIGHT wheel forward
  analogWrite(motor_right[0], 0); 
  analogWrite(motor_right[1], r_speed); 
}    

void spin_left()
{

  //RIGHT wheel reverse
  analogWrite(motor_right[0], 40); 
  analogWrite(motor_right[1], 0);

  //LEFT wheel forward
  analogWrite(motor_left[0], 40); 
  analogWrite(motor_left[1], 0); 

}

void spin_right()
{
  //RIGHT wheel forward
  analogWrite(motor_right[0], 0); 
  analogWrite(motor_right[1], 40); 


  //LEFT wheel reverse
  analogWrite(motor_left[0], 0); 
  analogWrite(motor_left[1], 40); 

}
void encr()
{
 enc_r++; 
}

void encl()
{
  enc_l++;
}


int correctHeading(int ci,int cj,int ni,int nj,int ch)
{ int head = 0;
brake();
  if(ni>ci && nj==cj)
  {
    head = 3;
  }
  if(ni<ci && nj==cj)
  {
    head = 1;
  }
  if(ni==ci && nj>cj)
  {
    head = 2;
  }
  if(ni==ci && nj<cj)
  {
    head = 4;
  }

Serial.print("Next head");
Serial.println(head);

  if(head!=ch){
    int diff = abs(head - ch);
    
    if(head>ch)
    {
     //turn clock wise till diff = 0
     do{
      Serial.print("ch changing");
      Serial.println(ch);
      turn_c();
      delay(100);
      ch+=1;
     }while(head>ch);
    }
    else
    {
       //turn anti - clock wise till diff = 0
        do{
          Serial.print("ch changing");
      Serial.println(ch);
      turn_a();
      delay(100);
      ch-=1;
     }while(head<ch);
    }

 
  }
  
brake();
return head;
}


void turn_c()
{theta = 0;
  do{
    spin_right();
    delay(20);
    /*rdist = enc_r * 0.785 * W_RADIUS;
    ldist = enc_l * 0.785 * W_RADIUS;
    theta += (rdist - ldist)/BOT_WIDTH;
    Serial.println(theta);*/
  }while(enc_r<TURN_COUNT && enc_l<TURN_COUNT);
}

void turn_a()
{theta = 0;
do{
    spin_left();
    delay(20);
    /*rdist = enc_r * 0.785 * W_RADIUS;
    ldist = enc_l * 0.785 * W_RADIUS;
    theta += (rdist - ldist)/BOT_WIDTH;
    Serial.println(theta);*/
  }while(enc_r<TURN_COUNT && enc_l<TURN_COUNT);
}



void takeStep()
{
  do{
    forward();
    delay(20);
    if (enc_r>enc_l)
    {
      l_speed=50;
      r_speed=40;
    }
    else if (enc_l>enc_r)
    {
      l_speed=40;
      r_speed=50;
    }
    else
    {
      l_speed=40;
      r_speed=40;
    }
    
    
  }while((enc_r+enc_l)/2<TILE_COUNT);

brake();

}




void setup() {
Serial.begin(9600);
Serial2.begin(9600);
 pinMode(rightir, INPUT);
  pinMode(leftir, INPUT);
  pinMode(centreir, INPUT);
  


attachInterrupt(digitalPinToInterrupt(18), encr, FALLING);
attachInterrupt(digitalPinToInterrupt(19), encl, FALLING);
  //Setup Motors
 for(i = 0; i < 2; i++){
 pinMode(motor_left[i], OUTPUT);
 pinMode(motor_right[i], OUTPUT);
}

//Set initial heading and coordinate
curr_i = 1;
curr_j = 1;
curr_h = 1;


}



void loop() {
   if (Serial2.available()) {
    int inByte = Serial2.read();
    i++;
    Serial.println(inByte);
    data[i] = inByte-48;
    if(inByte == 120)
   { bitcount++;
   Serial.println("BITCOUNTINCREMENT");
   
  }
   
    if(bitcount == 2 && inByte==120)
    { Serial.println("bitcount2");

      int t = 0;

      for (k = 0;data[k]!=72;k++);
      for(r = k+1;data[r]!=72;r++);
      //Traverse through array and assign coordinates
       for(int m = r-1; m>k;m-=2)
       {
         coordinates[t][1] = data[m];
         coordinates[t][0] = data[m-1];
         Serial.println("row");
         Serial.println(coordinates[t][0]);
         Serial.println(coordinates[t][1]);
         
          t++;
       }      
      //Run another loop to execute step wise command.


      for(int j =0; j<t-1;j++)
      {
       enc_r = 0;
       enc_l = 0;
       curr_h =  correctHeading(coordinates[j][0],coordinates[j][1],coordinates[j+1][0],coordinates[j+1][1],curr_h);
       Serial.print("Moved to heading");
       Serial.println(curr_h);
       delay(500);
       enc_r = 0;
       enc_l = 0;
       Serial.println("Taking step");
       takeStep(); 
       Serial.println("Took");
       
      bitcount = 0;
      }
      
    }
      }

}

