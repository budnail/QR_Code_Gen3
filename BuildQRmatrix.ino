void clearBuff()
{//set all modules in qrBuff to white.
  for(int i=0;i<29;i++){
    qrBuff[i] = 0;
  }
}

void formatBits()
{//writes the format bits to the screen
  //setMod2(0,1,1,1);//(m,x,y,c) where m=0 for screen or m=1 for buffer(not used for format bits) x=1,y=1 is top left corner and c = 0 is white, all other is black
// wln - new multi format information strings

   int counter = 0;
   
    for(int i = qrM; i > qrM-7; i--){//format bits 14,13,..8 vert
     int modColor = readFormatBits(qrE, maskType, counter); //(char ECC, int Mask, nth)
     setMod2(0,9,i,modColor);//(mode,x,y,color)
     counter += 1;
   }
   
    for(int i = qrM-7; i < qrM+1; i++){// format bits 7...0 horz
     int modColor = readFormatBits(qrE, maskType, counter); //(char ECC, int Mask, nth)
     setMod2(0,i,9,modColor);//(mode,x,y,color)
     counter += 1;  
   }
   
//...wln
   //Clear counter to display information bits again
   counter = 0;     
   for(int i = 1; i < 7; i++){//format bits 14,13,...9 left horzontal
    int modColor = readFormatBits(qrE, maskType, counter); //(char ECC, int Mask, nth)
     counter += 1; 
   }
   int modColor = readFormatBits(qrE, maskType, counter); //(char ECC, int Mask, nth)
   setMod2(0,8,9,modColor);//(x,y,color)
   counter += 1; 
   modColor = readFormatBits(qrE, maskType, counter); //(char ECC, int Mask, nth)
   setMod2(0,9,9,modColor);//(x,y,color)
   counter += 1; 
   modColor = readFormatBits(qrE, maskType, counter); //(char ECC, int Mask, nth)
   setMod2(0,9,8,modColor);//(x,y,color)
   counter += 1; 
  
    for(int i = 6; i > 0; i--){
       int modColor = readFormatBits(qrE, maskType, counter); //(char ECC, int Mask, nth)
       setMod2(0,9,i,modColor);//(x,y,color)    
       counter += 1;      
   }

//end format & version 
}

//writes the encoded information bits to the screen  or buffer
void informationBits2(int mode)// where mode = 0 to Screen and 1 to Buffer
{   
   //clear counter for message bits
   counter = 0;
//Write Data Bits 
//up1 is moving up the first(in data bit order) column
//down1 is moving down the first time (2nd column)
//this is repeated

//up1 improved for qrV = 1, 2, or 3 (maybe higher)
  for(int y=qrM;y>9;y--){
    for(int x=qrM;x>qrM-2;x--){
      int mask = getMask2(x, y, maskType);
      int databit = getMessCompBit(counter);
      int c = databit^!mask;
      setMod2(mode,x,y,c);//(mode,x,y,databit XOR NOT(Mask)) 
      counter += 1;
    }
  }
  
//down1 improved for qrV = 1, 2, or 3 (maybe higher)
  for(int y=10;y<qrM+1;y++){
    for(int x=qrM-2;x>qrM-4;x--){
      int mask = getMask2(x, y, maskType);
      int databit = getMessCompBit(counter);
      int c = databit^!mask;
      setMod2(mode,x,y,c);//(mode,x,y,databit XOR NOT(Mask)) 
    counter += 1;
    }
  }
//Dog...
  //up2 improved for qrV = 1
  if(qrV==1){
    for(int y=qrM;y>9;y--){
      for(int x=qrM-4;x>qrM-6;x--){
        int mask = getMask2(x, y, maskType);      
        int databit = getMessCompBit(counter);
        int c = databit^!mask;
        setMod2(mode,x,y,c);//(mode,x,y,databit XOR NOT(Mask)) 
        counter += 1;
      }
    }
  }
  //up2 improved for qrV = 2, 3 
  if(qrV==2 || qrV==3){
    //Serial.print("databit: ");
    for(int y=qrM;y>qrM-4;y--){// from bottom to alignment pattern
      for(int x=qrM-4;x>qrM-6;x--){
        int mask = getMask2(x, y, maskType);
        int databit = getMessCompBit(counter);
        int c = databit^!mask;
        setMod2(mode,x,y,c);//(mode,x,y,databit XOR NOT(Mask)) 
        counter += 1;
      }
    }
    for(int y=qrM-9;y>9;y--){// from alignment pattern to finder pattern
      for(int x=qrM-4;x>qrM-6;x--){
        int mask = getMask2(x, y, maskType);
        int databit = getMessCompBit(counter);
        int c = databit^!mask;
        setMod2(mode,x,y,c);//(mode,x,y,databit XOR NOT(Mask)) 
        counter += 1;
      }
    }
  }
  
//down2 improved for qrV = 1
  if(qrV==1){
    for(int y=10;y<qrM+1;y++){
      for(int x=qrM-6;x>qrM-8;x--){
        int mask = getMask2(x, y, maskType);
        int databit = getMessCompBit(counter);
        int c = databit^!mask;
        setMod2(mode,x,y,c);//(mode,x,y,databit XOR NOT(Mask)) 
        counter += 1;
      }
    }
  }
  //down2 improved for qrV = 2, 3
    if(qrV==2 || qrV==3 ){
    for(int y=10;y<qrM-8;y++){// from bottom finder to alignment pattern
      for(int x=qrM-6;x>qrM-8;x--){
        int mask = getMask2(x, y, maskType);
        int databit = getMessCompBit(counter);
        int c = databit^!mask;
        setMod2(mode,x,y,c);//(mode,x,y,databit XOR NOT(Mask)) 
        counter += 1;
      }
    }
    for(int y=qrM-3;y<qrM+1;y++){// from alignment pattern to bottom
      for(int x=qrM-6;x>qrM-8;x--){
        int mask = getMask2(x, y, maskType);
        int databit = getMessCompBit(counter);
        int c = databit^!mask;
        setMod2(mode,x,y,c);//(mode,x,y,databit XOR NOT(Mask)) 
        counter += 1;
      }
    }
  }

//Dog...
//up3 improved
 if(qrV==1){
    //for(int y=qrM;y>qrM-4;y--){//up to end of data bits
    for(int y=qrM;y>7;y--){//up to horizontal timing pattern
      for(int x=qrM-8;x>qrM-10;x--){
        int mask = getMask2(x, y, maskType);
        int databit = getMessCompBit(counter);
        int c = databit^!mask;
        setMod2(mode,x,y,c);//(modex,y,databit XOR NOT(Mask)) 
        counter += 1;
      }
    }
  //continue up3
  for(int y=6;y>0;y--){//from horizontal timing pattern to top
      for(int x=qrM-8;x>qrM-10;x--){
        int mask = getMask2(x, y, maskType);
      int databit = getMessCompBit(counter);
      int c = databit^!mask;
      setMod2(mode,x,y,c);//(mode,x,y,databit XOR NOT(Mask)) 
      counter += 1;
      }
  }
 } //end up 3 for qrV= 1
 // up3 for qrV=2 and 3
 
  if(qrV==2 || qrV==3){
    for(int y=qrM;y>qrM-4;y--){// from bottom to alignment pattern
      for(int x=qrM-8;x>qrM-10;x--){
        int mask = getMask2(x, y, maskType);
        int databit = getMessCompBit(counter);
        int c = databit^!mask;
        setMod2(mode,x,y,c);//(mode,x,y,databit XOR NOT(Mask)) 
        counter += 1;
      }
    }
    //section here for along side of alignment pattern
    for(int y=qrM-4;y>qrM-9;y--){// from bottom to top of alignment pattern
        int x = qrM-9;
        int mask = getMask2(x, y, maskType);
        int databit = getMessCompBit(counter);
        int c = databit^!mask;
        setMod2(mode,x,y,c);//(mode,x,y,databit XOR NOT(Mask)) 
        counter += 1;
    }
    
    for(int y=qrM-9;y>7;y--){// from alignment pattern to finder pattern
      for(int x=qrM-8;x>qrM-10;x--){
        int mask = getMask2(x, y, maskType);
        int databit = getMessCompBit(counter);
        int c = databit^!mask;
        setMod2(mode,x,y,c);//(mode, x,y,databit XOR NOT(Mask)) 
        counter += 1;
      }
    }
    //continue up3
  for(int y=6;y>0;y--){//from horizontal timing pattern to top
      for(int x=qrM-8;x>qrM-10;x--){
        int mask = getMask2(x, y, maskType);
        int databit = getMessCompBit(counter);
        int c = databit^!mask;
        setMod2(mode,x,y,c);//(mode,x,y,databit XOR NOT(Mask)) 
        counter += 1;
      }
  }
  }
 
  //down3 improved for qrV = 1,2,3
  if(qrV==1 || qrV==2 || qrV==3 ){
    for(int y=1;y<7;y++){
      for(int x=qrM-10;x>qrM-12;x--){
        int mask = getMask2(x, y, maskType);
        int databit = getMessCompBit(counter);
        int c = databit^!mask;
        setMod2(mode,x,y,c);//(mode,x,y,databit XOR NOT(Mask)) 
        counter += 1;
      }
    }
  }
  
  //skip over timing pattern
   //down3 improved 
  if(qrV==1 || qrV==2 || qrV==3){
    for(int y=8;y<qrM+1;y++){
      for(int x=qrM-10;x>qrM-12;x--){
        int mask = getMask2(x, y, maskType);
        int databit = getMessCompBit(counter);
        int c = databit^!mask;
        setMod2(mode,x,y,c);//(mode,x,y,databit XOR NOT(Mask)) 
        counter += 1;
      }
    }
  }
  
  //up4 for
 if(qrV==2 || qrV==3){//up 4 for V2 and V3
    //for(int y=qrM;y>qrM-4;y--){//up to end of data bits
    for(int y=qrM;y>7;y--){//up to horizontal timing pattern
      for(int x=qrM-12;x>qrM-14;x--){
        int mask = getMask2(x, y, maskType);
        int databit = getMessCompBit(counter);
        int c = databit^!mask;
        setMod2(mode,x,y,c);//(mode,x,y,databit XOR NOT(Mask)) 
        counter += 1;
      }
    }
  for(int y=6;y>0;y--){//from horizontal timing pattern to top
      for(int x=qrM-12;x>qrM-14;x--){
        int mask = getMask2(x, y, maskType);
        int databit = getMessCompBit(counter);
        int c = databit^!mask;
        setMod2(mode,x,y,c);//(mode,x,y,databit XOR NOT(Mask)) 
        counter += 1;
      }
  }
 } //end up 4for qrV= 2,3
 //down4  for qrV = 2,3
  if(qrV==2 || qrV==3 ){
    for(int y=1;y<7;y++){
      for(int x=qrM-14;x>qrM-16;x--){
        int mask = getMask2(x, y, maskType);
        int databit = getMessCompBit(counter);
        int c = databit^!mask;
        setMod2(mode,x,y,c);//(mode,x,y,databit XOR NOT(Mask)) 
        counter += 1;
      }
    }
  }
  
  //skip over timing pattern
   //down4  
  if(qrV==2 || qrV==3){
    for(int y=8;y<qrM+1;y++){
      for(int x=qrM-14;x>qrM-16;x--){
        int mask = getMask2(x, y, maskType);
        int databit = getMessCompBit(counter);
        int c = databit^!mask;
        setMod2(mode,x,y,c);//(mode,x,y,databit XOR NOT(Mask)) 
        counter += 1;
      }
    }
  }
  //place holder for up and down for V3 (not V1 or V2)
   //up5 for
 if(qrV==3){//up 4 for V2 and V3
    //for(int y=qrM;y>qrM-4;y--){//up to end of data bits
    for(int y=qrM;y>7;y--){//up to horizontal timing pattern
      for(int x=qrM-16;x>qrM-18;x--){
        int mask = getMask2(x, y, maskType);
        int databit = getMessCompBit(counter);
        int c = databit^!mask;
        setMod2(mode,x,y,c);//(mode,x,y,databit XOR NOT(Mask)) 
        counter += 1;
      }
    }
  for(int y=6;y>0;y--){//from horizontal timing pattern to top
      for(int x=qrM-16;x>qrM-18;x--){
        int mask = getMask2(x, y, maskType);
        int databit = getMessCompBit(counter);
        int c = databit^!mask;
        setMod2(mode,x,y,c);//(,mode,x,y,databit XOR NOT(Mask)) 
        counter += 1;
      }
  }
 } //end up 5for qrV= 3
 //down5  for qrV = 3
  if(qrV==3 ){
    for(int y=1;y<7;y++){
      for(int x=qrM-18;x>qrM-20;x--){
        int mask = getMask2(x, y, maskType);
        int databit = getMessCompBit(counter);
        int c = databit^!mask;
        setMod2(mode,x,y,c);//(mode,x,y,databit XOR NOT(Mask)) 
        counter += 1;
      }
    }
  }
  
  //skip over timing pattern
   //down5  
  if(qrV==3){
    for(int y=8;y<qrM+1;y++){
      for(int x=qrM-18;x>qrM-20;x--){
        int mask = getMask2(x, y, maskType);
        int databit = getMessCompBit(counter);
        int c = databit^!mask;
        setMod2(mode,x,y,c);//(mode,x,y,databit XOR NOT(Mask)) 
        counter += 1;
      }
    }
  }
  //pick up with common here measure from left side
 //up 
 
 //all, v1,2,3
 for(int y=qrM-8;y>9;y--){//between info bits
      for(int x=9;x>7;x--){
      int mask = getMask2(x, y, maskType);
      int databit = getMessCompBit(counter);
      int c = databit^!mask;
      setMod2(mode,x,y,c);//(mode,x,y,databit XOR NOT(Mask)) 
      counter += 1;
      }
  } 
 

 
 //down
  // improved same for all
    for(int y=10;y<qrM-7;y++){
      for(int x=6;x>4;x--){
        int mask = getMask2(x, y, maskType);
        int databit = getMessCompBit(counter);
        int c = databit^!mask;
        setMod2(mode,x,y,c);//(mode,x,y,databit XOR NOT(Mask)) 
        counter += 1;
      }
    }
  
  
 //up  next all
 for(int y=qrM-8;y>9;y--){//between info bits
      for(int x=4;x>2;x--){
        int mask = getMask2(x, y, maskType);
        int databit = getMessCompBit(counter);
        int c = databit^!mask;
        setMod2(mode,x,y,c);//(mode,x,y,databit XOR NOT(Mask)) 
        counter += 1;
      }
  } 
 
  //down all last
  
    for(int y=10;y<qrM-7;y++){
      for(int x=2;x>0;x--){
        int mask = getMask2(x, y, maskType);
        int databit = getMessCompBit(counter);
        int c = databit^!mask;
        setMod2(mode,x,y,c);//(mode,x,y,databit XOR NOT(Mask)) 
        counter += 1;
      }
    }
  counter = 0;     
}//end of informationBits2


int getMask2(int x, int y, int maskType2){
  // for purposes of masking, rows and columns are numbered starting with 0
  x--;y--;//fix for row and col numbering
  
  if(maskType2 == 0){
      if ((y + x) % 2 == 0){//(row + column) mod 2 == 0
        return 0;
      }else{
      return 1;
      }
  }
   if(maskType2 == 1){//swapped for test
      if ((y) % 2 == 0){//(row) mod 2 == 0
        return 0;
      }else{
      return 1;
      }
  }
  if(maskType2 == 2){//(column) mod 3 == 0
      if ((x) % 3 == 0){
        return 0;
      }else{
      return 1;
      }
  }
  if(maskType2 == 3){//(row + column) mod 3 == 0
      if ((y + x) % 3 == 0){
        return 0;
      }else{
      return 1;
      }
  }
  //x++;y++;//debug mask 4 does not seem to work
  if(maskType2 == 4){//( floor(row / 2) + floor(column / 3) ) mod 2 == 0
      if ((int(y/2) + int(x/3)) % 2 == 0){
        return 0;
      }else{
      return 1;
      }
  }
  if(maskType2 == 5){//((row * column) mod 2) + ((row * column) mod 3) == 0
      if ((y * x) % 2 + (y * x) % 3 == 0){
        return 0;
      }else{
      return 1;
      }
  }
  if(maskType2 == 6){//( ((row * column) mod 2) + ((row * column) mod 3) ) mod 2 == 0
      if (((y * x) % 2 + (y * x) % 3) % 2 == 0){
        return 0;
      }else{
      return 1;
      }
  }
  if(maskType2 == 7){//( ((row + column) mod 2) + ((row * column) mod 3) ) mod 2 == 0
      if ((((y + x) % 2) + ((y * x) % 3)) % 2 == 0){
        return 0;
      }else{
      return 1;
      }
  }

}
// function to return the nth bit in the global BinaryMessComp[]
int getMessCompBit(int n)
{
  int byteIndex = n/8;// BinaryMessComp[n/8]
  byte bitIndex = n % 8;
  byte a = 1;
  byte bitMask = a<<bitIndex;
  if (BinaryMessComp[byteIndex] & bitMask){
    return 1;// bit was 1
  }else{
    return 0;//bit was 0
  } 
}
