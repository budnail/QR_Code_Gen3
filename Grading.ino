//wln 2014
//These functions grade the image in the Offscreen buffer according to the QR standard
// Descriptions of the grading method is straight from thonky.com
// This function draws the QR code in the buffer with each of the mask types and returns the winner
int findBestMask(){
  //temp debug
  //return 2;//force a certain mask for debug
  
  //draw each QR image without information bits to buffer and grade
  int mode = 1;//draw to buffer not screen
  int currentScore;//holder for the current Score
  int bestScore = 16000;// holder for the best score (lowest). initialize with a really high number
  int bestMask = 0;// holder of the winning Mask number
  for(int i=0;i<8;i++){// there are 8 types 0-7
    clearBuff();//clear the buffer
    maskType = i;//set the mask type this is a global
    drawTemplate(mode);//draw template in buffer. mode is set to 1
    informationBits2(mode);//draw info bits in buffer. mode is set to 1
    currentScore = QRgrade();//grade the code currently in the buffer
    if(currentScore < bestScore){
      bestScore = currentScore;//new best score
      bestMask = maskType;//record as the current winner
    }
  }
  // all masks have been tried. return the best
  return bestMask;
}

int QRgrade(){
  //performs all the tests and returns the total grade on the image currently in the buffer. lower is better
  int grade=QReval_1() + QReval_2() + QReval_3() + QReval_4();//perform 1st, 2nd, 3rd and 4th test
  //int grade=QReval_4();//debug to perform one test at a time
  if (grade<1){ grade=1;}//don't believe a grade of 0, return a 1 for trouble shooting
  return grade;//final score
}

int QReval_1(){
  /* For the first evaluation condition, check each row one-by-one. If there are five consecutive modules of the same color, 
     add 3 to the penalty. If there are more modules of the same color after the first five, add 1 for each additional module 
     of the same color. Afterward, check each column one-by-one, checking for the same condition. Add the horizontal and 
     vertical total to obtain penalty score #1.
     http://www.thonky.com/qr-code-tutorial/data-masking/#evaluation-condition-1
     
  // x selects the bit of the buffer
  // x = 0 represents the first column of the module
  // y selects the 32 bit the word of the buffer
  // bit 0 represents the top most module
  */
  int penaltyTotal=0;
  if(qrV == 1){
    qrM = 21;//number of modules per side
  }
  if(qrV == 2){
    qrM = 25;//number of modules per side
  }
  if(qrV == 3){
    qrM = 29;//number of modules per side
  }
  // row test: count # of consecutive modules in row of same color 
  for(int j=0;j<qrM;j++){// move top to bottom
   int currentColor = -1;//no color reset after each row
   int colorCount=0;//reset after each row
   for(int i=0;i<qrM;i++){// move left to right
    if(bitRead(qrBuff[i],j) == currentColor){
      colorCount++;//add 1 because it is consecutive
      if(colorCount == 5){
        penaltyTotal +=3;//add 3 for 5 in a row
      }
      if(colorCount > 5){
        penaltyTotal +=1;//add 1 for each additional in a row
      }
    }else{
      colorCount=0;//not consecutive reset counter
      currentColor=bitRead(qrBuff[i],j);//color change
    }
    
   }
  }//end of row test
  // column test: count # of consecutive modules in column of same color 
  for(int i=0;i<qrM;i++){// move left to right
   int currentColor = -1;//no color reset after each row
   int colorCount=0;//reset after each col
   for(int j=0;j<qrM;j++){// move top to bottom
    if(bitRead(qrBuff[i],j) == currentColor){
      colorCount++;//add 1 because it is consecutive
      if(colorCount == 5){
        penaltyTotal +=3;//add 3 for 5 in a row
      }
      if(colorCount > 5){
        penaltyTotal +=1;//add 1 for each additional in a row
      }
    }else{
      colorCount=0;//not consecutive reset counter
      currentColor=bitRead(qrBuff[i],j);//color change
    }
   }
  }//end of column test
  return penaltyTotal;//total penalty for this test
}// end of QReval_1

int QReval_2(){
/*For second evaluation condition, look for areas of the same color that are at least 2x2 modules or larger. 
  The QR code specification says that for a solid-color block of size m × n, the penalty score is 3 × (m - 1) × (n - 1). 
  However, the QR code specification does not specify how to calculate the penalty when there are multiple ways of 
  dividing up the solid-color blocks.

  Therefore, rather than looking for solid-color blocks larger than 2x2, simply add 3 to the penalty score for every 
  2x2 block of the same color in the QR code, making sure to count overlapping 2x2 blocks. For example, a 3x2 block 
  of the same color should be counted as two 2x2 blocks, one overlapping the other.
  http://www.thonky.com/qr-code-tutorial/data-masking/#evaluation-condition-2
  
  // x selects the bit of the buffer
  // x = 0 represents the first column of the module
  // y selects the 32 bit the word of the buffer
  // bit 0 represents the top most module
  */
  int penaltyTotal=0;
  if(qrV == 1){
    qrM = 21;//number of modules per side
  }
  if(qrV == 2){
    qrM = 25;//number of modules per side
  }
  if(qrV == 3){
    qrM = 29;//number of modules per side
  }
  // 2x2  test: count # of 2x2 modules of same color 
  for(int j=0;j<qrM-1;j++){// move top to bottom
   int currentColor = -1;//no color reset after each row
   for(int i=0;i<qrM-1;i++){// move left to right
   currentColor = bitRead(qrBuff[i],j);
    if(bitRead(qrBuff[i],j+1) == currentColor && bitRead(qrBuff[i+1],j) == currentColor && bitRead(qrBuff[i+1],j+1) == currentColor){
      //mod to right, below and diagonal same color
      penaltyTotal +=3;//add 3 for each 2x2
    }
   }
  }//end of 2x2 test
  return penaltyTotal;//total penalty for this test
}//end of QReval_2

int QReval_3(){
/*
The third penalty rule looks for patterns of dark-light-dark-dark-dark-light-dark that have four light modules on either side. 
In other words, it looks for any of the following two patterns:
  10111010000
OR
  00001011101
Each time this pattern is found, add 40 to the penalty score. 
*/

int penaltyTotal=0;
  if(qrV == 1){
    qrM = 21;//number of modules per side
  }
  if(qrV == 2){
    qrM = 25;//number of modules per side
  }
  if(qrV == 3){
    qrM = 29;//number of modules per side
  }
  // pattern  test: detect 10111010000(0x05D0 and patterns 000 0101 1101(0x005D)
  unsigned int pattern1 = 0x05D0;
  unsigned int pattern2 = 0x005D;
  unsigned int patternMask = 0x07FF;//last 11 bits
  //check for patterns in rows
  for(int j=0;j<qrM;j++){// move top to bottom
   int currentPattern = 0;//reset after each row
   for(int i=0;i<qrM-10;i++){// move left to right
   currentPattern = currentPattern + bitRead(qrBuff[i],j);
    if(currentPattern == pattern1 || currentPattern == pattern2 && i>10){//need all the current bits loaded before test
      penaltyTotal +=40;//add 40 for each match
    }
    currentPattern = currentPattern <<1;//shift bits 1 position
    currentPattern = currentPattern & patternMask;// mask to 11 bit pattern
   }
  }//end of pattern in rows test
  // patterns in column test 
  for(int i=0;i<qrM;i++){// move left to right
    int currentPattern = 0;//reset after each column 
   for(int j=0;j<qrM;j++){// move top to bottom
    currentPattern = currentPattern + bitRead(qrBuff[i],j);
    if(currentPattern == pattern1 || currentPattern == pattern2 && j>10){//need all the current bits loaded before test
      penaltyTotal +=40;//add 40 for each match
    }
    currentPattern = currentPattern <<1;//shift bits 1 position
    currentPattern = currentPattern & patternMask;// mask to 11 bit pattern
   }
  }//end of pattern in columns test
  return penaltyTotal;//total penalty for this test

}//end of QReval_3

int QReval_4(){
/*
This evaluation condition is based on the ratio of light modules to dark modules. 
To calculate this penalty rule, do the following steps:
 Count the total number of modules in the matrix.
 Count how many dark modules there are in the matrix.
 Calculate the percent of modules in the matrix that are dark: (darkmodules / totalmodules) * 100
 Determine the previous and next multiple of five of this percent. 
   For example, for 43 percent, the previous multiple of five is 40, and the next multiple of five is 45.
 Subtract 50 from each of these multiples of five and take the absolute value of the result. 
   For example, |40 - 50| = |-10| = 10 and |45 - 50| = |-5| = 5.
 Divide each of these by five. 
   For example, 10/5 = 2 and 5/5 = 1.
 Finally, take the smallest of the two numbers and multiply it by 10. 
   In the example above, the lower number is 1, so the result is 10. This is penalty score #4.
*/

  if(qrV == 1){
    qrM = 21;//number of modules per side
  }
  if(qrV == 2){
    qrM = 25;//number of modules per side
  }
  if(qrV == 3){
    qrM = 29;//number of modules per side
  }
 int numMods = qrM * qrM;// get number of all modules
 int darkCount=0;
 // count # of dark modules
  for(int j=0;j<qrM;j++){// move top to bottom
   for(int i=0;i<qrM;i++){// move left to right
    if(bitRead(qrBuff[i],j) == 1){
      darkCount++;//add 1 because it is dark
    }
   }
  }//end of dark count
  long int perCentDark = darkCount*100/numMods;//calculate percent dark meh
  int prev5 = perCentDark / 5 * 5;
  int next5 = prev5 + 5;
  int prev = abs(prev5 - 50)/5;
  int next = abs(next5 - 50)/5;
  if(prev < next){//return the smaller value
    return prev * 10;
  }else{
    return next * 10;
  } 
  //return perCentDark;
}// end of QReval_4
