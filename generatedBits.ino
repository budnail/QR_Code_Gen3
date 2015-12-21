void calculateQRCode()
//void calculateQRCode(messageToEncode*)
{
  //Initialize varaibles
  int bitStringClear();//Clears the bitString
  
  //length = 0;
  byteCounter = 0;

  int reqDataWords = getReqDataWords();// the number of data words required for the current QR version 1Q,2Q,or 3Q
  int reqECCwords = getReqECCWords();// the number of ECC words required for the current QR version 1Q,2Q,or 3Q

  //Set Datamode bits to 1 for alphanumic
  setDataModeBits(1);//good for QR Versions 1,2,3 maybe higher
  
  //Encode length of data
  encodeDataLength(); //good for QR Versions 1,2,3 maybe higher
  //Serial.println("");Serial.println("length");Serial.println(length);
  //Endcode Data in binary
  encodeData(); //good for QR Versions 1,2,3 maybe higher
  
  //Terminate the bits
  terminateBits2();//good for all versions
  /*
   //Debug
   Serial.println("charsToEncode:");
  for(int i=0;i<78;i++){
    Serial.print(charsToEncode[i]);
  }
  Serial.println("");
   
   //Debug
   Serial.println("bitString:");
  for(int i=0;i<bitStringLength();i++){
    Serial.print(bitStringBitB(i));
  }
  Serial.println("");
  */
  //update the byteCounter
  byteCounter += bitStringLength()/8;

  //Add words at end of string if less than 104 bit's or 13 8-bit words
  addWord2();// good for QR Version 1Q, 2Q, 3Q 
  
  //Convert each 8-bit word from binary to decimal
  decimal3();//good for QR versions 1,2,3
  
  //Convert & store decimal Message to Alpha Notation
  messageToAlpha();//good for QR Versions 1Q,2Q
  
  //Generate ECC words
  ecc();//good for QR Versions 1Q,2Q, 3L, 3M
  
  // wln replacement for section 1A
  bitString2Message();// moves bitString into BinaryMessComp
  
    //Serial.println("ECC:");
    for(int h = 0; h < reqECCwords; h++){    
    //Convert to binary and append to eccBinary
        byte temp=0;
        for (int i=0;i<8;i++){//revers bit order
          bitWrite(temp,7-i,bitRead(Result[h],i));
        }
        BinaryMessComp[h+reqDataWords]=temp;
    //Serial.print(temp);Serial.print(" ");
    }
}

void setDataModeBits(int datamode)
{
  bitStringAppendB(0);bitStringAppendB(0);//both modes begin with "00"
 switch(datamode)
 {
   //numeric - not implemented
  case 0:
    bitStringAppendB(0);bitStringAppendB(1);//append "01"
    break;
  case 1:
    //alphanumeric
    bitStringAppendB(1);bitStringAppendB(0);//append "10"
    break; 
 }
}

void encodeDataLength()
{
  // 9 bit version only for Alpha numeric, other types use different lengths
  length = 78;
  for(int i=0; i< length; i++){// find the null signifying the end of the char array
  if(charsToEncode[i] == 0){
    length = i;//
  }
  }
  if (length > 16 && qrV == 1 && qrE =='Q'){length = 16;}// longer strings are not allowed - cut off excess
  if (length > 29 && qrV == 2 && qrE =='Q'){length = 29;}// longer strings are not allowed - cut off excess
  if (length > 77 && qrV == 3 && qrE =='L'){length = 77;}// longer strings are not allowed - cut off excess
  if (length > 61 && qrV == 3 && qrE =='M'){length = 61;}// longer strings are not allowed - cut off excess
  for(int i = 8; i > -1; i--){
    bitStringAppendB(bitRead(length, i));//new bitString method
  }
}

void encodeData()
{
  //int numArray[80];// large enough for message 80 long
  int totalArray[40];// large enough for message 80 long
  int numBits = 0;
    
  //If the length of the message is even 
    if (length % 2 == 0){
      //Serial.println("");Serial.print("totalArrayEven: ");
    for (int i = 0; i < length; i+=2){
      //First Character
      int part1 = charToQRval(charsToEncode[i]);
      //Second Character
      int part2 = charToQRval(charsToEncode[i+1]);
      //Perform the necessay arthmitic & store the result in totalArray
      //Later each value in the array will be converted to its binary version
      totalArray[i/2] = (45 * part1) + part2;
     // Serial.print(totalArray[i/2]);Serial.print("/");Serial.print(part1);Serial.print("/");Serial.print(part2);Serial.print(",");

    }
    //take the lower 11 bits of totalArray &  append  to bitString
    for(int h = 0; h < length/2; h++){
      for(int j = 11-1; j > -1; j--){
        bitStringAppendB(bitRead(totalArray[h], j));//new bitString
      }
    }
  }//end of even length
  
  //If the message length is odd
    if(length % 2 ==1){
    //Remove the last character that way it can 
    //Serial.println("");Serial.print("totalArrayOdd: ");
    for (int i = 0; i < length-1; i+=2){
      //First Character
      int part1 = charToQRval(charsToEncode[i]);
      //Second Character
      int part2 = charToQRval(charsToEncode[i+1]);
      //Perform the necessay arthmitic & store the result in totalArray
      totalArray[i/2] = (45 * part1) + part2;
     // Serial.print(totalArray[i/2]);Serial.print("/");Serial.print(part1);Serial.print("/");Serial.print(part2);Serial.print(",");
    }
  //last character  for odd length
  totalArray[length/2] = charToQRval(charsToEncode[length-1]);// convert char to a QR int value
 // Serial.print(totalArray[length/2]);Serial.print(",");
    //take the lower 11 bits of totalArray & append  to bitString
      for(int h = 0; h < (length-1)/2; h++){
      for(int j = 11-1; j > -1; j--){
        bitStringAppendB(bitRead(totalArray[h], j));//new bitString
      }
    }
    //take the lower 6 bits of the last char and append  to bitString
   for(int j = 6-1; j > -1; j--){
     bitStringAppendB(bitRead(totalArray[length/2], j));//new bitString
   }
  } //end of odd 
}//encodeData

void terminateBits2()//version to us bitString
{
  // don't exceed the number of data bits required or use more than 4 terminator bits
 int totalLength = bitStringLength();
 //get the number of required data code words
 int reqDataWords = getReqDataWords();
 int reqDataBits = reqDataWords * 8;
 
 if(reqDataBits - totalLength == 1){
   bitStringAppendB(0);
 }
 if(reqDataBits - totalLength == 2){
   bitStringAppendB(0);bitStringAppendB(0);
 }
 if(reqDataBits - totalLength == 3){
   bitStringAppendB(0);bitStringAppendB(0);bitStringAppendB(0);
 }
 if(reqDataBits - totalLength > 3){
   //max number of terminator bits
   bitStringAppendB(0);bitStringAppendB(0);bitStringAppendB(0);bitStringAppendB(0);
 }
 //Add More 0s to Make the Length a Multiple of 8 
 int remainingBits = 8 - (bitStringLength() % 8);
 if (remainingBits < 8){
   for(int i=0;i<remainingBits;i++){
     bitStringAppendB(0);
   }
 }
 
}

void addWord2() // pads special bytes at the end to equal the required byte length
{
  //get the number of required data code words
 int reqDataWords = getReqDataWords();
 int byteDifference = reqDataWords - byteCounter;
 for(int i = 0; i < byteDifference; i++){
   if(i % 2 == 0){//even
     //"11101100";
     bitStringAppendB(1);bitStringAppendB(1);bitStringAppendB(1);bitStringAppendB(0);
     bitStringAppendB(1);bitStringAppendB(1);bitStringAppendB(0);bitStringAppendB(0);
     byteCounter += 1;
   }else{//odd
     //"00010001"; 
     bitStringAppendB(0);bitStringAppendB(0);bitStringAppendB(0);bitStringAppendB(1);
     bitStringAppendB(0);bitStringAppendB(0);bitStringAppendB(0);bitStringAppendB(1);
     byteCounter += 1;
   }
 }
}
//wln - the following could be changed to use bitWrite function to simplify
int binaryToDecimal(String bits) // WLN takes a string made of 8, 0's and 1's, and converts to a number 
{
  char charBuffer[8];
  int numArray[8] = {};
  for(int i = 0; i < 8; i++){
    charBuffer[i] = bits.charAt(i);
    numArray[i] = charBuffer[i] - '0';
  }
  return (numArray[0] * pow(2, 7)) + (numArray[1] * pow(2, 6)) + (numArray[2] * pow(2, 5)) + (numArray[3] * pow(2, 4)) + (numArray[4] * pow(2, 3)) +(numArray[5] * pow(2, 2)) + (numArray[6] * pow(2, 1)) + (numArray[7] * pow(2, 0)); 
}

void messageToAlpha()
{
  //Serial.println("Message:");
  int alphaLength = getReqDataWords();
 
// Serial.println("MessageI");//debug
  for(int i = 0; i < alphaLength; i++){
    int x = MessageI[i];
    MessageA[i] = pgm_read_byte_near(LogTableF + x); // newer ultra low SRAM method
    //Serial.print(MessageI[i]); Serial.print(",");// used to verify at http://www.thonky.com/qr-code-tutorial/show-division-steps
  }
 // Serial.println("");
}

void ecc()
{ 
  // Good for QR Versions 1Q, 2Q, 3M, 3L
 int polyLengthE = getReqECCWords() +1; //#poly terms for GeneratorA. one more than the number of ecc code words (lead term is 0)
 int polyLengthM = getReqDataWords(); // #poly terms for message poly. the number of Message code words 
 //The first step is to multiply the generator polynomial by the lead term of the message polynomial.
 //We do this by taking the Alpha terms and adding 
 for(int i = 0; i < polyLengthE; i++){
   Result[i] = MessageA[0] + GeneratorA[i];
 }
 
 //Modulo Result
 for(int i = 0; i < polyLengthE; i++){
   if(Result[i] > 255){ 
     Result[i] = Result[i] % 255;
   }
 }
 
 //Convert result from Alpha Notation to Integer Notation
 for(int i = 0; i < polyLengthE; i++){
   int x = Result[i];
   Result[i] = pgm_read_byte_near(antiLogTableF + x);// newer ultra low memory method
 }
 //Step 1b: XOR the result with the message polynomial
 //XOR Polynomial
 for(int i = 0; i < polyLengthM; i++){
   int x = MessageI[i];
   int y = Result[i];
   if(i>=polyLengthE){y=0;}
   Result[i] = x ^ y;
 }
 //we keep repeating so that we have done this the number of times required.
 for(int p=0;p<polyLengthM-1;p++){//repeat for the number of message polynomials (except for first one which is done)
  // Serial.println("");Serial.print("Result(");Serial.print(p+1);Serial.print("): ");
   //remove the leading 0 term
   for(int i = 0; i < polyLengthM; i++){
        Result[i] = Result[i+1]; //terms are shifted and the last term should automatically be 0
        xorResult[i] = Result[i];//save a copy here
   //     Serial.print(Result[i]);Serial.print(",");
    }
    //The first step is to multiply the generator polynomial by the lead term of the message polynomial. 
    //Do this by adding the Alpha of the lead term to the generator alphas
    int leadExpA = pgm_read_byte_near(LogTableF + xorResult[0]);
    for(int i = 0; i < polyLengthE; i++){
      Result[i] = leadExpA + GeneratorA[i];
    }
    //Modulo Result
    for(int i = 0; i < polyLengthE; i++){
      if(Result[i] > 255){ 
        Result[i] = Result[i] % 255;
      }
    }   
    //Convert result from Alpha Notation to Integer Notation
    for(int i = 0; i < polyLengthE; i++){
      int x = Result[i];
      Result[i] = pgm_read_byte_near(antiLogTableF + x);// newer ultra low memory method
    }
    //XOR with the current Polynomial
    for(int i = 0; i < polyLengthM; i++){
      int x = xorResult[i];
      int y = Result[i];
      if(i>=polyLengthE){y=0;} 
      Result[i] = x ^ y;
    }
  } //end of loop 
  
 // Serial.println("");Serial.print("Result: ");
  //remove leading term which is now zero
  for(int i = 0; i < polyLengthM; i++){
    Result[i] = Result[i+1]; 
  //  Serial.print(Result[i]);Serial.print(",");
 }
 //Result[] contains the error correction code words
}//end ecc

int readFormatBits(char ECC, int Mask, int nth)
{
  //returns the value of the nth bit
  //return 0;//debug
  /* Comment out the ones you are not using to save memory
   if(ECC =='H'){
     if(Mask == 0){
      return pgm_read_byte_near(HM0 + nth);  
    }
    if(Mask == 1){
      return pgm_read_byte_near(HM1 + nth);
    }
    if(Mask == 2){
      return pgm_read_byte_near(HM2 + nth);
    }
    if(Mask == 3){
     return pgm_read_byte_near(HM3 + nth);
    }
    if(Mask == 4){
      return pgm_read_byte_near(HM4 + nth);
    }
    if(Mask == 5){
      return pgm_read_byte_near(HM5 + nth);
    }
    if(Mask == 6){
      return pgm_read_byte_near(HM6 + nth);
    }
    if(Mask == 7){
      return pgm_read_byte_near(HM7 + nth);
    }
  }//end if ECC=='H'
  
   if(ECC =='Q'){
    if(Mask == 0){
      return pgm_read_byte_near(QM0 + nth);  
    }
    if(Mask == 1){
      return pgm_read_byte_near(QM1 + nth);
    }
    if(Mask == 2){
      return pgm_read_byte_near(QM2 + nth);
    }
    if(Mask == 3){
     return pgm_read_byte_near(QM3 + nth);
    }
    if(Mask == 4){
      return pgm_read_byte_near(QM4 + nth);
    }
    if(Mask == 5){
      return pgm_read_byte_near(QM5 + nth);
    }
    if(Mask == 6){
      return pgm_read_byte_near(QM6 + nth);
    }
    if(Mask == 7){
      return pgm_read_byte_near(QM7 + nth);
    }
  }//end if ECC=='Q'
  
  if(ECC =='M'){
    if(Mask == 0){
      return pgm_read_byte_near(MM0 + nth);  
    }
    if(Mask == 1){
      return pgm_read_byte_near(MM1 + nth);
    }
    if(Mask == 2){
      return pgm_read_byte_near(MM2 + nth);
    }
    if(Mask == 3){
     return pgm_read_byte_near(MM3 + nth);
    }
    if(Mask == 4){
      return pgm_read_byte_near(MM4 + nth);
    }
    if(Mask == 5){
      return pgm_read_byte_near(MM5 + nth);
    }
    if(Mask == 6){
      return pgm_read_byte_near(MM6 + nth);
    }
    if(Mask == 7){
      return pgm_read_byte_near(MM7 + nth);
    }
  }//end if ECC=='M'
  */
if(ECC =='L'){
  if(Mask == 0){
      return pgm_read_byte_near(LM0 + nth);  
    }
    if(Mask == 1){
      return pgm_read_byte_near(LM1 + nth);
    }
    if(Mask == 2){
      return pgm_read_byte_near(LM2 + nth);
    }
    if(Mask == 3){
     return pgm_read_byte_near(LM3 + nth);
    }
    if(Mask == 4){
      return pgm_read_byte_near(LM4 + nth);
    }
    if(Mask == 5){
      return pgm_read_byte_near(LM5 + nth);
    }
    if(Mask == 6){
      return pgm_read_byte_near(LM6 + nth);
    }
    if(Mask == 7){
      return pgm_read_byte_near(LM7 + nth);
    }
  }//end if ECC=='L'
  
}//end of function


// implements ASCII to QR code conversion
int charToQRval(char a){
  if (a >= '0' && a<= '9'){//numeric
    return a - '0';
  }
  if( a >= 'A' && a <= 'Z'){//alpha upper case
    return a - 'A' +10;
  }
  if( a >= 'a' && a <= 'z'){//alpha lower case. Lower case is not supported in alphanumeric mode, 
    return a - 'a' +10;//but since this is a common error, force it upper case
  }
  //punctuation codes
  if( a == ' '){//space
    return 36;
  }
  if( a == '$'){//dollar
    return 37;
  }
  if( a == '%'){//percent
    return 38;
  }
  if( a == '*'){//star
    return 39;
  }
  if( a == '+'){//plus
    return 40;
  }
  if( a == '-'){//dash
    return 41;
  }
  if( a == '.'){//period
    return 42;
  }
  if( a == '/'){//forward slash
    return 43;
  }
  if( a == ':'){//colon
    return 44;
  }
  // for all other ascii not allowed, return the code for a '.' (period)
  return 41;//period - this could be -1 for an error but this simplifies the code allows graceful failure
}

//returns the number of required data code words for each QR Version supported
int getReqDataWords(){
  if(qrV==1 and qrE=='Q'){
    return 13;
  }
  if(qrV==2 and qrE=='Q'){
    return 22;
  }
  if(qrV==3 and qrE=='L'){
    return 55;
  }
  if(qrV==3 and qrE=='M'){
    return 44;
  }
}
//returns the number of required ECC code words for each QR Version supported
int getReqECCWords(){
  if(qrV==1 and qrE=='Q'){
    return 13;
  }
  if(qrV==2 and qrE=='Q'){
    return 22;
  }
  if(qrV==3 and qrE=='L'){
    return 15;
  }
  if(qrV==3 and qrE=='M'){
    return 26;
  }
}


//wln bitString Functions
// bitString is a holder for a string of bits that shares the use of qrBuff a 29x32 bit array
int bitStringClear()
{// has the effect of emptying  the bitString
  qrBuffindex=0;
}

int bitStringAppendC(char a)
{//appends a bit for char a = '0' or '1'
//returns the current number of bits in the bit string
int subBitIndex = qrBuffindex % 32;
  int wordIndex = qrBuffindex/32;
  if(a=='0'){
    bitWrite(qrBuff[wordIndex], subBitIndex, 0);
    qrBuffindex++;
    return qrBuffindex;
  }
  if(a=='1'){
    bitWrite(qrBuff[wordIndex], subBitIndex, 1);
    qrBuffindex++;
    return qrBuffindex;
  }
  //if a was not '0' or '1' return error
  return -1;
}

int bitStringAppendB(int a)
{//appends a bit for a = 0 or 1
//returns the current number of bits in the bit string
int subBitIndex = qrBuffindex % 32;
int wordIndex = qrBuffindex/32;
//Serial.print(qrBuffindex); Serial.print("|");Serial.print(wordIndex);Serial.print("|");Serial.print(qrBuff[wordIndex]);
  if(a==0){
    bitWrite(qrBuff[wordIndex], subBitIndex, 0);
    //Serial.print("/"); Serial.print(bitStringBitB(qrBuffindex));Serial.print("/"); Serial.print(a); Serial.print(",");
    qrBuffindex++;
    return qrBuffindex;
  }
  if(a==1){
    bitWrite(qrBuff[wordIndex], subBitIndex, 1);
   // Serial.print("/"); Serial.print(bitStringBitB(qrBuffindex));Serial.print("/"); Serial.print(a); Serial.print(",");
    qrBuffindex++;
    return qrBuffindex;
  }
  //if a was not 0 or 1 return error
  return -1;
}

int bitStringLength()
{//returns the length and incidentally the location of the next free bit
  return qrBuffindex;
}

int bitStringBitB(int n)
{//returns the bit addressed by n as a number 0 or 1
int subBitIndex = n % 32;
int wordIndex = n/32;
//unsigned long wordTemp=qrBuff[wordIndex];
return bitRead(qrBuff[wordIndex],subBitIndex);
}

char bitStringBitC(int n)
{//returns the bit addressed by n as a character '0' or '1'
int subBitIndex = n % 32;
int wordIndex = n/32;
return bitRead(qrBuff[wordIndex],subBitIndex) + '0';
}

void bitString2Message()
{// moves the bitString into BinaryMessComp[]
//Serial.print("BinaryMessComp: ");
  for(int i=0;i<qrBuffindex;i+=8){
    int wordIndex = i/32;
    int subBitIndex = i % 32;// will be 0,8,16, or 24
    unsigned long temp = qrBuff[wordIndex];
    unsigned long temp1 = temp >> subBitIndex;//shift right to put desired byte in lower byte
    temp1 = temp1 & 0xFF; //mask off the lower 8 bits
    BinaryMessComp[i/8] = byte(temp1);// get only the lower byte
    //Serial.print(BinaryMessComp[i/8]);Serial.print(",");
  }
  //Serial.println(" ");
}

void decimal3()
{// moves the bitString into MessageI[]
//Serial.println("");Serial.print("Decimal3: ");
  for(int i=0;i<qrBuffindex;i+=8){
    int wordIndex = i/32;
    int subBitIndex = i % 32;// will be 0,8,16, or 24
    unsigned long temp = qrBuff[wordIndex];
    unsigned long temp1 = temp >> subBitIndex;//shift right to put desired byte in lower byte
    temp1 = temp1 & 0xFF; //mask off the lower 8 bits
   // Serial.print(temp1); Serial.print(",");
        for (int i=0;i<8;i++){//revers bit order
          //temp = bitRead(Result[h],i)<<1;
          bitWrite(temp,7-i,bitRead(temp1,i));
        }
    MessageI[i/8] = byte(temp);// get only the lower byte
    //Serial.print(MessageI[i/8]); Serial.print(",");
  }
}
