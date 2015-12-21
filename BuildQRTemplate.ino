
//scales for QR version 1, 2, 3
// alignment patterns not scaleable to larger sizes - fixed
void drawTemplate(int mode)
{
// 3 corner finder patterns
//wln...
int c =1; 
  // top left outer box using standard QR indexes
  for(int i=1;i<8;i++){//top
    //(x,y,c) where c = 0 is white, all other is black
      setMod2(mode,i,1,c); 
  }
  for(int i=1;i<8;i++){//bottom
       setMod2(mode,i,7,c);
  }
  for(int i=2;i<7;i++){//left
       setMod2(mode,1,i,c);
  }
  for(int i=2;i<7;i++){//right
        setMod2(mode,7,i,c);
  }
  // END top left outer box using standard QR indexes
 // top left outer box using standard QR indexes
 // top left inner box
  for(int i=3;i<6;i++){//top
    setMod2(mode,i,3,c);//(x,y,c) where c = 0 is white, all other is black
    setMod2(mode,i,4,c);
    setMod2(mode,i,5,c);
  }
  //END top left inner box
  
  // top right outer box using standard QR indexes
  for(int i=qrM-6;i<qrM+1;i++){//top
        setMod2(mode,i,1,c);
  }
  for(int i=qrM-6;i<qrM+1;i++){//bottom
        setMod2(mode,i,7,c);
  }
  for(int i=2;i<7;i++){//left
        setMod2(mode,qrM-6,i,c);
  }
  for(int i=2;i<7;i++){//right
        setMod2(mode,qrM,i,c);
  }
  // END top right outer box using standard QR indexes
 
 // top right inner box
  for(int i=qrM-4;i<qrM-1;i++){//top
        setMod2(mode,i,3,c);//(x,y,c) where c = 0 is white, all other is black
        setMod2(mode,i,4,c);
        setMod2(mode,i,5,c);
  }
  //END top left inner box
  
   // bot left outer box using standard QR indexes
  for(int i=1;i<8;i++){//top
        setMod2(mode,i,qrM-6,c);//(x,y,c) where c = 0 is white, all other is black
  }
  for(int i=1;i<8;i++){//bottom
        setMod2(mode,i,qrM,c);//(x,y,c) where c = 0 is white, all other is black
  }
  for(int i=qrM-5;i<qrM+1;i++){//left
        setMod2(mode,1,i,c);//(x,y,c) where c = 0 is white, all other is black
  }
  for(int i=qrM-5;i<qrM+1;i++){//right
        setMod2(mode,7,i,c);//(x,y,c) where c = 0 is white, all other is black
  }
  // END bot left outer box using standard QR indexes
 
 // bot left inner box
  for(int i=3;i<6;i++){//top
    setMod2(mode,i,qrM-4,c);//(x,y,c) where c = 0 is white, all other is black
    setMod2(mode,i,qrM-3,c);
    setMod2(mode,i,qrM-2,c);
  }
  //END bot left inner box
  
//Timing Pattern
 
 for(int i=9;i<qrM-7;i+=2){
        setMod2(mode,i,7,c);//horz
        setMod2(mode,7,i,c);//vert
  }

   //end Timing Pattern
   //alighnment pattern
   /* commenting this out saves 320 bytes but see qrV==3 version below for better method
   if(qrV==2){// qr code version is 2
      setMod2(mode,19,19,1);//center
      setMod2(mode,17,17,1);//left
      setMod2(mode,17,18,1);
      setMod2(mode,17,19,1);
      setMod2(mode,17,20,1);
      setMod2(mode,17,21,1);
      setMod2(mode,21,17,1);//right
      setMod2(mode,21,18,1);
      setMod2(mode,21,19,1);
      setMod2(mode,21,20,1);
      setMod2(mode,21,21,1);
      setMod2(mode,18,17,1);//top
      setMod2(mode,19,17,1);
      setMod2(mode,20,17,1);
      setMod2(mode,18,21,1);//bot
      setMod2(mode,19,21,1);
      setMod2(mode,20,21,1);
   }
   */
   if(qrV==3){// qr code version is 3
      setMod2(mode,23,23,1);//center
      int i = 21;int k = 25; 
      for(int j=i;j<26;j++){
        setMod2(mode,i,j,c);//left
        setMod2(mode,k,j,c);//right
        setMod2(mode,j,i,c);//top
        setMod2(mode,j,k,c);//bot
      }
   }
   //place dark bit
      setMod2(mode,9,qrM-7,1);//(x,y,c) where c = 0 is white, all other is black
}//end of drawTemplate

