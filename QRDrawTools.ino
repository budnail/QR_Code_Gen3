//wln 2014
//uses u8g graphics library

//Set the state of each module directly on the display
//QR modules are numbered with top left = 1, 1
// Currently the x and y are transposed - on purpose

void setMod2(int m, int x, int y, int c){
  // m = 0 draw to screen, m=1 draw to buffer
  // x, y position with 1,1 being top left.
  // c = 0 white, c = 1 black
  //xOffs = 28;//debug
  //return;//debug
  if (m == 1){
    setBuf(x,y,c);//draw to the buffer
    }else{
    // actual draw to screen
    if (c == 0){
     u8g.setColorIndex(0);//white ink
     }else{
     u8g.setColorIndex(1);//black ink
    }
    u8g.drawBox((y-1)*qrS + xOffs,(qrM-x)*qrS + yOffs,qrS,qrS);//
  }
}

//Set the state of a module on the buffer
//QR modules are numbered with top left = 1, 1
void setBuf(int x, int y, int c){
  // x selects the bit of the buffer
  // x = 0 represents the first column of the module
  // y selects the 32 bit the word of the buffer
  // bit 0 represents the top most module
  //unsigned long pattern = 1 << x;// only the x bit is 1
  if (x>28 || x<0){x=0;}//dont allow overrange
  if (y>28 || y<0){y=0;}// dont allow overrange
  if (c == 0){
    //set the module to 0, white ink
    bitClear(qrBuff[y],x);
  }else{
   //set the module to 1, black ink
    bitSet(qrBuff[y],x);
  }
}

// simply places whatever is in the buffer on the screen
// used as a debug tool
void displayBuff(void){
  // i is the col, j is the row
  for(int i =1; i<= qrM; i++){
    for(int j =1; j<= qrM; j++){
   int c = bitRead(qrBuff[j],i);
   setMod2(0,i,j,c);
  }
  }
}

// simply places whatever is in the buffer on the screen
// Rotated
void displayBuffR(void){
  //rotate
  // i is the col, j is the row
  for(int i =1; i<= qrM; i++){
    for(int j =1; j<= qrM; j++){
   int c = bitRead(qrBuff[j],i);
   setMod2(0,i,qrM-j,c);
  }
  }
}

//test pattern for debugging
void testPattern(void){
  for(int i=1; i<22;i++){
    setBuf(i,i,1);//dia
    setBuf(21,i,1);//right
    setBuf(1,i,1);//left
    setBuf(i,1,1);//top
    setBuf(i,21,1);//bot
  }
  displayBuffR();
}

//get offset functions for centering QR code on the display
int getCenterOffsetX()
{
  int width = u8g.getWidth();
  int offSet = (width - qrM * qrS)/2;//qrM is the qr codes width in modules, qrS is the module width in pixels
  return offSet;
}
int getCenterOffsetY()
{
  int height = u8g.getHeight();
  int offSet = (height - qrM * qrS)/2;//qrM is the qr codes width in modules, qrS is the module width in pixels
  return offSet;
}
