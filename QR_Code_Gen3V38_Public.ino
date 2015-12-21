/*
QR_Code_Gen3
(c)2014 William L. "Bud" Nail, Technological Services Company
This program and support functions fully implement the generation of several useful versions of the QR Code standard from a text string.

It does this in a relatively small memory foot print such as the Arduino Uno and still leaves room for additional application functions.
It can be easily modified to support additional QR codes with some limitations.

This program is licensed under GPL 2, 3 or LGPL (your choice) that work well for non-commercial and some limited commercial projects (IMHO). 
If you are interested in a simple low cost commercial or developer's license for embedding in your projects, 
see  www.technologicalservicescompany.com for contact information.

Donations are also accepted for further development. 

Regardless of the license, the code is free for review and experimentation by individuals and commercial or non-commercial enterprises.

History:
When this program development began, in early 2014, it was thought by most (from reviewing forums on the subject) that it would be next to impossible
to implement a dynamic QR code generator on a processor such as the Arduino UNO that has so little SRAM.

I had a specific project in mind that would use the Arduino UNO chip on a custom board with a small graphics display.
Researching, I discovered that the U8g (universal 8 bit graphics) library supported all of the several displays I was considering using.
I also found that David Hend had successfully created a dynamic QR code on a larger processor, when other people were only capable
of generating static codes. He referenced "Thonky's" excellent tutorial on programming QR code generators.

With those references I proceeded to build my own QR generator that would fit in a Arduino UNO or similar embedded processor's foot print.

So far, this program is capable of generating QR version 1Q, 2Q, 3L, and 3M code format. Then it displays 
that information as a QR code on the screen. 

Many thanks to David for giving me a starting point and a link to Thonky's tutorial (www.thonky.com) he used as the basis for his design.
Seeing his code and comparing it to the tutorial, helped greatly in understanding what is required to build a fuller implementation.
I basically replaced his code modules one by one while using his remaining code to test results against. If any of his code remains it 
is not intentional, though I continue to use some of the same function names.

Thanks also to Thonky.com for a great learning resource.

For those interested:

Here are design features: 
(1) A general purpose graphics library is being used instead of the TFT library David used, so that it can support more devices.
(2) This version is significantly altered to allow use in low memory devices such as the Arduino Uno 
    and devices with the same or similar chip sets. This is accomplished by moving constants to PROGMEM and downsizing variables.
(3) This version defaults to a 2x2 pixel module rather than the fixed 10x10 pixel module in David's work. 
(4) Completely new logic has been added to fully conform to the QR spec(as for as I know) for the versions implemented. 
    Instead of simply picking a mask and hoping it works (it didn't always), each mask is tried and scored according
    to the spec and the best mask is used.
(5) This version has been restructured so that new QR versions can be added without significantly rewriting the code.
(6) While currently a 2x2 pixel module is hard coded, much of the supporting functions are resolution independent.
(7) The code is designed to "fail gracefully".  That is to say if your try to send a message that is not legal according
    to the spec, it sends what it can, rather than crashing or generating an unreadable code. It shortens messages
    that are too long and substitutes illegal characters with legal characters.

Hardware Consideratons: 
The current version of the program has been targeted specifically to work with the DOG102 series of Displays. But,
since it uses the U8glib library for Universal 8 bit graphics, the program should be usable with most any graphic display.
Simply take one of the U8glib examples and pull the line that looks equivalent to:
//set up SPI for model display used and wiring
U8GLIB_DOGS102 u8g(13, 11, 10, 9);		// SPI Com: SCK = 13, MOSI = 11, CS = 10, A0(also referenced as CD) = 9
Replace this line with the one that matches your display.
Adjust the SPI communication ports to match the ones on your hardware.

To Do: (1) build the qr code in a buffer area so that it can be built repeatedly for each mask 0-7 - done
       (2) build the test to score the mask methods. - done
       (3) build the best scoring and Display - done
       (4) turn the process into a function so that any message string can be sent to it at any time.
       (5) make all methods work with QR code Versions 2 and 3 as well as 1 - done
       (6) auto covert lower case to upper case since trying to encode lowercase is a common mistake - done
       (7) change encoding to include allowed non alpha numeric - done
       (8) - maybe add numeric encoding vs only alphanumeric encoding - this will require changes throughout
       (9) The current version uses 2x2 pixels per QR code module. Make the size selectable.
       
Version History:
V3 added the option to choose other mask types - the specification requires you try all 7 and use the best.
   this version only provides that you can use any of the 7
V5 Fixed the bug in the ECC calculation that would give bad ECC.
V6 adds scalable templates for versions 1, 2 and 3
V7 adds a QRcode Buffer that lets you generate the code off screen for grading - currently only the template
v8 adds some text functions for debugging.
v9 clears out a lot of the old methods and comments, 
   added function to create format bits for all ECCs and Masks
   modified the informationBits function to place format bits for QR code versions 1-3, probably works up to version 6
   started on making the data bits in the informationBits function work for several QR versions
   modified and tested up1 portion so far.
v10 added improved version of placing the data and ecc into the qr matix up to where the transition from data to ecc occurs
    at this point it became apparent that the data and ecc need to be strung together first to avoid all the possible
    points where this transition occurs. not complete.
v11 ran into more memory problems so worked on shrinking SRAM usage, moved log table to progmem (flash memory)
    This freed up an additional 512 bytes of SRAM memory
v12 moved format information bits of Mosk 0-7 of ECC Q to PROGMEM also
    verified that info written to display is correct for each mask type of Q
    however - none of the other mask patterns seem to work
V13 transition from using array of bytes to represent binary arrays where there is one byte to bit to bit for bit
V14 clean up of old methods so that the portion of databits to ecc bits can change
    most masks tested. Even number of chars work, problem with last char of odd length
V15 fixed bug of last odd character cleaned out some debugging code. Now all lengths and all Masks work for 1Q type
V16 start fleshing out provision for  other ECC levels: L, M, and H, added information bit arrays for each
    Added grading of image logic - not tested.
V17 Tested and debugged grading system - results corresponded to thonky's QR generator
    Automatic selection of best code not implemented yet.
v18 Modified info bits to drow to buffer or screen
v20 This version fully supports Mask grading and automatically picks the best mask for the qr code and uses it.
v21 Cleaned up removed most old unused code. Added complete punctuation allowed. Auto convert lower case to upper
V22 Expanded arrays to accomodate QR Version 2 Q. Verified there was still enough SRAM to function.
    Removed the size dependencies from calculating the QR code data and ECC
    All that remains (I think) is to make changes to the drawing of the QR code for various versions
V23 implemented placing information bits for Version 2 and 3
V24 Now capable of generating Type 2 Q QR codes, though the mask scoring does not match up with Thonky's Generator
V26 Got rid of additional strings that grows when scaling up for Type 3
V28 Got rid of last bad string "dataString"
V29 Cleaned up and rewrote much of ECC to work with different number of ECC and Message code words
    Still has a bug - works for Type 2 but not Type 3
V30 Now Supports Versions 1Q,2Q, and 3L,
V31 Now Supports Versions 1Q,2Q,3L and 3M
V32 Cleaned out old comments - first publically released version
V33 Various optimizations
V34 Starting adding application code and memory got a bit tight, looking for more memory
V35 Eliminated an 80 character array and haved another, saving 120 bytes of sram, with improved math
    Eliminated the last dependency on String objects that really suck up the sram
V38_Public Removed GPS Logging code for public version

Developer Notes:
  Basic steps are:
     0. select the QR code type
     1. translate the message into data bits
     2. Calculate the Error Correction Codes using polynomial division
     3. Place the data and ECC bits within a template for evaluation
     3. Find the best mask to use for the QR code by trying them all and picking the best (grading)
     3. drawing the qr code on the screen using the mask selected above
     
  Key functions and data structures:
     qrBuff[] - An off screen buffer 32x32 bits is provided for testing QR codes before displaying them 
 
     "bitString" functions - read and write functions as if there was a string of bits data type (we pre-use qrBuff[] for temporary storage)
 
     setMod2(int m,int x,int y,int c) - where m is mode, x is column, y is row and c is color (0 white, 1 black)
       it draws a qr module to the screen (mode = 0) or to the buffer(mode = 1)
       this is the lowest drawing level that you might use to add new qr versions
       
      calculateQRCode() is where the message is translated into data bits - do this before the functions below
       this is where you would add new calculations for different QR code versions
       
      ecc() calculates the error correction code words for the data code words
 
      drawTemplate(int mode) places the standard template on the screen or in the buffer. mode = 0(screen), 1 (buffer)
      
      informationBits2(int mode) places the data bits in the qr code either on the screen or in the buffer. mode = 0(screen), 1 (buffer)
      
      formatBits() places the information about the QR code type and mask selected into the QRcode, always on screen. 
        should not be used in the buffer as part of the grading process.
        
  Limitations:
    The offscreen buffer is only designed to work up to version 3 which is 29 x 29 modules. Each module is represented by 1 bit.
    Working with more than the 32 bits present in an unsigned long int has its challenges.
    This version only supports codes that have a single block of ECC appended at the end. Not interleaved versions, starting with 3Q
*/


//start includes
#include <stdint.h>
//wln included pgmspace to move log table(s) and other constants to flash memory and save SRAM
#include <avr/pgmspace.h>
//wln include the universal graphics library
#include "U8glib.h"

//wln...
//set up SPI for model display used and wiring for Ardunino Uno style devices
//simply change this next line to match your wiring scheme
U8GLIB_DOGS102 u8g(13, 11, 10, 9);		// SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//...wln
//Set to true to enable serial debug information
boolean debug = false;

/*
Log/Anti-Log table used to switch between integer notation
and alpha notation when performing arithmetic to calculate
ecc bits. Arduino supports log functions but does not work
in the same way as this table does.
***wln changed table from 4 elements per row to 2, removing the duplicate index values
*** also changed to use a byte table from the orginal unsigned int to save memory. reduced 2020 bytes to 510 bytes of ram
*** Newer ultra low memory use version moves Log table from SRAM to flash. Separate Log and AntiLog tables used now.
*/
//wln Log table in Flash Memory
// to lookup values use form "y = pgm_read_byte_near(antiLogTableF + x);" where y^x
PROGMEM  prog_uchar antiLogTableF[]  = {
  1,
  2,
  4,
  8,
  16,
  32,
  64,
  128,
  29,
  58,
  116,
  232,
  205,
  135,
  19,
  38,
  76,
  152,
  45,
  90,
  180,
  117,
  234,
  201,
  143,
  3,
  6,
  12,
  24,
  48,
  96,
  192,
  157,
  39,
  78, 
  156,
  37,
  74,
  148,
  53,
  106,
  212,
  181,
  119,
  238,
  193,
  159,
  35,
  70,
  140,
  5,
  10,
  20,
  40,
  80,
  160,
  93,
  186,
  105,
  210,
  185,
  111,
  222,
  161,
  95,
  190,
  97,
  194,
  153,
  47,
  94,
  188,
  101,
  202,
  137,
  15,
  30,
  60,
  120,
  240,
  253,
  231,
  211,
  187,
  107,
  214,
  177,
  127,
  254,
  225,
  223,
  163,
  91,
  182,
  113,
  226,
  217,
  175,
  67,
  134,
  17,
  34,
  68,
  136,
  13,
  26,
  52,
  104,
  208,
  189,
  103,
  206,
  129,
  31,
  62,
  124,
  248,
  237,
  199,
  147,
  59,
  118,
  236,
  197,
  151,
  51,
  102,
  204,
  133,
  23,
  46,
  92,
  184,
  109,
  218,
  169,
  79,
  158,
  33,
  66,
  132,
  21,
  42,
  84,
  168,
  77,
  154,
  41,
  82,
  164,
  85,
  170,
  73,
  146,
  57,
  114,
  228,
  213,
  183,
  115,
  230,
  209,
  191,
  99,
  198,
  145,
  63,
  126,
  252,
  229,
  215,
  179,
  123,
  246,
  241,
  255,
  227,
  219,
  171,
  75,
  150,
  49,
  98,
  196,
  149,
  55,
  110,
  220,
  165,
  87,
  174,
  65,
  130,
  25,
  50,
  100,
  200,
  141,
  7,
  14,
  28,
  56,
  112,
  224,
  221,
  167,
  83,
  166,
  81,
  162,
  89,
  178,
  121,
  242,
  249,
  239,
  195,
  155,
  43,
  86,
  172,
  69,
  138,
  9,
  18,
  36,
  72,
  144,
  61,
  122,
  244,
  245,
  247,
  243,
  251,
  235,
  203,
  139,
  11,
  22,
  44,
  88,
  176,
  125,
  250,
  233,
  207,
  131,
  27,
  54,
  108,
  216,
  173,
  71,
  142,
  1,
};//end of antiLog table
// to lookup values use form "x = pgm_read_byte_near(LogTableF + y);" where y^x
//the first element is really empty but something had to be put there.
PROGMEM  prog_uchar LogTableF[]  = {
  0,
  0,
  1,
  25,
  2,
  50,
  26,
  198,
  3,
  223,
  51,
  238,
  27,
  104,
  199,
  75,
  4,
  100,
  224,
  14,
  52,
  141,
  239,
  129,
  28,
  193,
  105,
  248,
  200,
  8,
  76,
  113,
  5,
  138,
  101,  
  47,
  225,
  36,
  15,
  33,
  53,
  147,
  142,
  218,
  240,
  18,
  130,
  69,
  29,
  181,
  194,
  125,
  106,
  39,
  249,
  185,
  201,
  154,
  9,
  120,
  77,
  228,
  114,
  166,
  6,
  191,
  139,
  98,
  102,
  221,
  48,
  253,
  226,
  152,
  37,
  179,
  16,
  145,
  34,
  136,
  54,
  208,
  148,
  206,
  143,
  150,
  219,
  189,
  241,
  210,
  19,
  92,
  131,
  56,
  70,
  64,
  30,
  66,
  182,
  163,
  195,
  72,
  126,
  110,
  107,
  58,
  40,
  84,
  250,
  133,
  186,
  61,
  202,
  94,
  155,
  159,
  10,
  21,
  121,
  43,
  78,  
  212,
  229,
  172,
  115,
  243,
  167,
  87,
  7,
  112,  
  192,
  247,
  140,
  128,
  99,  
  13,  
  103,
  74,
  222,
  237,
  49,
  197,
  254,
  24,
  227,
  165,
  153,
  119,
  38,
  184,
  180,
  124,
  17,
  68,
  146,
  217,
  35,
  32,
  137,
  46,
  55,
  63,
  209,
  91,
  149,
  188,
  207,
  205,
  144,
  135,
  151,
  178,
  220,
  252,
  190,
  97,
  242,
  86,
  211,
  171,
  20,
  42,
  93,
  158,
  132,
  60,
  57,
  83,
  71,
  109,
  65,
  162,
  31,
  45,
  67,
  216,
  183,
  123,
  164,
  118,
  196,
  23,
  73,
  236,
  127,
  12,
  111,
  246,
  108,
  161,
  59,
  82,
  41,
  157,
  85,
  170,
  251,
  96,
  134,
  177,
  187,
  204,
  62,
  90,
  203,
  89,
  95,
  176,
  156,
  169,
  160,
  81,
  11,
  245,
  22,
  235,
  122,
  117,
  44,
  215,
  79, 
  174,
  213,
  233,
  230,
  231,
  173,
  232,
  116,
  214,
  244,
  234,
  168,
  80,
  88,
  175
};//end of Log table

//Store format bit patterns in Flash. i.e QM0 is Q type ECC Mask 0
// These could be changed to single unsigned int with a little work if Flash mem gets tight.
/* commented out ECC types you are not using to save memory, uncomment to use
// ECC type H
PROGMEM  prog_uchar HM0[]  = {0,0,1,0,1,1,0,1,0,0,0,1,0,0,1};//ECC Level H Mask 0
PROGMEM  prog_uchar HM1[]  = {0,0,1,0,0,1,1,1,0,1,1,1,1,1,0};//ECC Level H Mask 1
PROGMEM  prog_uchar HM2[]  = {0,0,1,1,1,0,0,1,1,1,0,0,1,1,1};//ECC Level H Mask 2
PROGMEM  prog_uchar HM3[]  = {0,0,1,1,0,0,1,1,1,0,1,0,0,0,0};//ECC Level H Mask 3
PROGMEM  prog_uchar HM4[]  = {0,0,0,0,1,1,1,0,1,1,0,0,0,1,0};//ECC Level H Mask 4
PROGMEM  prog_uchar HM5[]  = {0,0,0,0,0,1,0,0,1,0,1,0,1,0,1};//ECC Level H Mask 5,
PROGMEM  prog_uchar HM6[]  = {0,0,0,1,1,0,1,0,0,0,0,1,1,0,0};//ECC Level H Mask 6
PROGMEM  prog_uchar HM7[]  = {0,0,0,1,0,0,0,0,0,1,1,1,0,1,1};//ECC Level H Mask 7,

// ECC type Q finished and tested
PROGMEM  prog_uchar QM0[]  = {0,1,1,0,1,0,1,0,1,0,1,1,1,1,1};//ECC Level Q Mask 0
PROGMEM  prog_uchar QM1[]  = {0,1,1,0,0,0,0,0,1,1,0,1,0,0,0};//ECC Level Q Mask 1
PROGMEM  prog_uchar QM2[]  = {0,1,1,1,1,1,1,0,0,1,1,0,0,0,1};//ECC Level Q Mask 2
PROGMEM  prog_uchar QM3[]  = {0,1,1,1,0,1,0,0,0,0,0,0,1,1,0};//ECC Level Q Mask 3
PROGMEM  prog_uchar QM4[]  = {0,1,0,0,1,0,0,1,0,1,1,0,1,0,0};//ECC Level Q Mask 4
PROGMEM  prog_uchar QM5[]  = {0,1,0,0,0,0,1,1,0,0,0,0,0,1,1};//ECC Level Q Mask 5,
PROGMEM  prog_uchar QM6[]  = {0,1,0,1,1,1,0,1,1,0,1,1,0,1,0};//ECC Level Q Mask 6
PROGMEM  prog_uchar QM7[]  = {0,1,0,1,0,1,1,1,1,1,0,1,1,0,1};//ECC Level Q Mask 7,
// ECC type M
PROGMEM  prog_uchar MM0[]  = {1,0,1,0,1,0,0,0,0,0,1,0,0,1,0};//ECC Level M Mask 0
PROGMEM  prog_uchar MM1[]  = {1,0,1,0,0,0,1,0,0,1,0,0,1,0,1};//ECC Level M Mask 1
PROGMEM  prog_uchar MM2[]  = {1,0,1,1,1,1,0,0,1,1,1,1,1,0,0};//ECC Level M Mask 2
PROGMEM  prog_uchar MM3[]  = {1,0,1,1,0,1,1,0,1,0,0,1,0,1,1};//ECC Level M Mask 3
PROGMEM  prog_uchar MM4[]  = {1,0,0,0,1,0,1,1,1,1,1,1,0,0,1};//ECC Level M Mask 4
PROGMEM  prog_uchar MM5[]  = {1,0,0,0,0,0,0,1,1,0,0,1,1,1,0};//ECC Level M Mask 5,
PROGMEM  prog_uchar MM6[]  = {1,0,0,1,1,1,1,1,0,0,1,0,1,1,1};//ECC Level M Mask 6
PROGMEM  prog_uchar MM7[]  = {1,0,0,1,0,1,0,1,0,1,0,0,0,0,0};//ECC Level M Mask 7,
*/
// ECC type L 
PROGMEM  prog_uchar LM0[]  = {1,1,1,0,1,1,1,1,1,0,0,0,1,0,0};//ECC Level L Mask 0
PROGMEM  prog_uchar LM1[]  = {1,1,1,0,0,1,0,1,1,1,1,0,0,1,1};//ECC Level L Mask 1
PROGMEM  prog_uchar LM2[]  = {1,1,1,1,1,0,1,1,0,1,0,1,0,1,0};//ECC Level L Mask 2
PROGMEM  prog_uchar LM3[]  = {1,1,1,1,0,0,0,1,0,0,1,1,1,0,1};//ECC Level L Mask 3
PROGMEM  prog_uchar LM4[]  = {1,1,0,0,1,1,0,0,0,1,0,1,1,1,1};//ECC Level L Mask 4
PROGMEM  prog_uchar LM5[]  = {1,1,0,0,0,1,1,0,0,0,1,1,0,0,0};//ECC Level L Mask 5,
PROGMEM  prog_uchar LM6[]  = {1,1,0,1,1,0,0,0,1,0,0,0,0,0,1};//ECC Level L Mask 6
PROGMEM  prog_uchar LM7[]  = {1,1,0,1,0,0,1,0,1,1,1,0,1,1,0};//ECC Level L Mask 7,

int maskType = 0;//Place holde for the QR code mask type
// wln change for trying different masks
//int maskType = 0;//(row + column) mod 2 == 0
//int maskType = 1;//(row) mod 2 == 0
//int maskType = 2;//(column) mod 3 == 0
//int maskType = 3;//(row + column) mod 3 == 0
//int maskType = 4;//( floor(row / 2) + floor(column / 3) ) mod 2 == 0
//int maskType = 5;//((row * column) mod 2) + ((row * column) mod 3) == 0
//int maskType = 6;//( ((row * column) mod 2) + ((row * column) mod 3) ) mod 2 == 0
//int maskType = 7;//( ((row + column) mod 2) + ((row * column) mod 3) ) mod 2 == 0 (according to thonky this is correct not what is in QR specification Page 51


//place holder for Message in binary form - use smallest version for application to save SRAM
//byte BinaryMessComp[26]={};//big enough for Version 1. one bit per bit rather than byte for bit holds both Message and ECC, saves 174 bytes of SRAM
//byte BinaryMessComp[44]={};//big enough for Version 2. 
byte BinaryMessComp[70]={};//big enough for Version 3. 

//Array to hold Message in Integer format (used in poly arithmetic) - use smallest version for application to save SRAM
//unsigned int MessageI[13] = {};//big enough for Version 1 Q
//unsigned int MessageI[22] = {};//big enough for Version 2 Q
unsigned int MessageI[55] = {};//big enough for Version 3 L
//unsigned int MessageI[44] = {};//big enough for Version 3 M

//Array to hold Message in Alpha Notation (used in arithmetic) - use smallest version for application to save SRAM
//unsigned int MessageA[13] = {};//big enough for Version 1 Q
//unsigned int MessageA[22] = {};//big enough for Version 2 Q
unsigned int MessageA[55] = {};//big enough for Version 3 L
//unsigned int MessageA[44] = {};//big enough for Version 3 M

// QR code Version - These are set up below
// qrV = QR version 1, 2, or 3
// qrE =Error Correction Level, L, M, Q, or H

// for additional Generator Polynomials see http://www.thonky.com/qr-code-tutorial/generator-polynomial-tool/?degree=13
// These could be moved to PROGMEM to save SRAM
// If you want to add other Versions use the types provided as examples. You must also edit the following functions for new types:
//   encodeDataLength() : the length of the longest message in bytes allowed
//   getReqDataWords()  : the number of required data code words
//   getReqECCWords()   : the number of required Error Correction Code words
  

//**** CHange the QRversion by uncommenting ONE of the lines below. Comment out the one you are not using
//The line below is for version 1 Q where the ECC polynomial is:
//  x13 + α74x12 + α152x11 + α176x10 + α100x9 + α86x8 + α100x7 + α106x6 + α104x5 + α130x4 + α218x3 + α206x2 + α140x + α78
//int qrV = 1;char qrE = 'Q';unsigned int GeneratorA[14] = {0,74,152,176,100,86,100,106,104,130,218,206,140,78};//version 1Q

//The line below is for version 2 Q where the ECC polynomial is:
//  x22 + α210x21 + α171x20 + α247x19 + α242x18 + α93x17 + α230x16 + α14x15 + α109x14 + α221x13 + α53x12 + α200x11 + α74x10 + α8x9 + α172x8 + α98x7 + α80x6 + α219x5 + α134x4 + α160x3 + α105x2 + α165x + α231
//int qrV = 2;char qrE = 'Q';unsigned int GeneratorA[23] = {0,210,171,247,242,93,230,14,109,221,53,200,74,8,172,98,80,219,134,160,105,165,231};//version 2Q

//Version 3 L where the ECC polynomial is:
//x15 + α8x14 + α183x13 + α61x12 + α91x11 + α202x10 + α37x9 + α51x8 + α58x7 + α58x6 + α237x5 + α140x4 + α124x3 + α5x2 + α99x + α105
int qrV = 3;char qrE = 'L';unsigned int GeneratorA[16] = {0,8,183,61,91,202,37,51,58,58,237,140,124,5,99,105};//version 3L

//Version 3 M where the ECC polynomial is:
//x26 + α173x25 + α125x24 + α158x23 + α2x22 + α103x21 + α182x20 + α118x19 + α17x18 + α145x17 + α201x16 + α111x15 + α28x14 + α165x13 + α53x12 + α161x11 + α21x10 + α245x9 + α142x8 + α13x7 + α102x6 + α48x5 + α227x4 + α153x3 + α145x2 + α218x + α70
//int qrV = 3;char qrE = 'M';unsigned int GeneratorA[27] ={0,173,125,158,2,103,182,118,17,145,201,111,28,165,53,161,21,245,142,13,102,48,227,153,145,218,70};

//Holder for Result from arithmitic - use smallest version for application to save SRAM
//unsigned int Result[15] = {};//big enough for V 1 Q
//unsigned int Result[24] = {};//big enough for V 2 Q
unsigned int Result[56] = {};//big enough for V 3 L
//unsigned int Result[45] = {};//big enough for V 3 M

//Holder for Result from XOR operation (used in arithmetic) - use smallest version for application to save SRAM
//unsigned int xorResult[15] = {};//big enough for V 1 Q
//unsigned int xorResult[24] = {};//big enough for V 2 Q
unsigned int xorResult[56] = {};//big enough for V 3 L
//unsigned int xorResult[45] = {};//big enough for V 3 M

// QR image buffer for grading. supports up to version 3, 29 x29 modules
unsigned long qrBuff[29] = {};//sized for 1, 2 and 3. Does double duty as offscreen graphics buffer and bit string storage (V27 and later)
int qrBuffindex = 0;// holds pointer to the next available bit in the qrBuff when using it for bit string storage
// QR module width and height
byte qrS = 2;//2 means a 2x2 pix per module
int qrM = 21;//number of modules per side default version 1, changed automatically to correct size based on qrV
// QR module pixel offsets. moves the the QR code from the corner on the display - can be whatever you like but are overridden by centerFlag
//byte xOffs = 25; byte yOffs = 40;
byte xOffs = 3; byte yOffs = 22; 
int centerFlag = 3;// if not 0 overides xOffs and yOffs to center the code, 1 centers Horz, 2 centers Vert, 3 centers both directions

//char array we hope to convert (a C string) - just a place holder at this point
// Size the array for the maximum number of characters you intend to encode plus 1.
//char charsToEncode[78] = = "";
//must be uppercase and numeric
//various test strings
  //String messageToEncode = "HACKADAY";
  //String messageToEncode = "S211111S";
  //String messageToEncode = "123456789012345";
//char charsToEncode[10] = "JOHN 3:16";
char charsToEncode[78] = "http://www.technologicalservicescompany.com";
//char charsToEncode[78] = "ROMANS 6:13";
//String messageToEncode = "A123456789";//Mask 4 - works
 //String messageToEncode = "ABCDEFGHIJ";//Mask 0 - works
//String messageToEncode = "12345ABCDE";//Mask 5 - works
 //String  messageToEncode = "A1B2C3D4E5";//mask 0 - works
//String messageToEncode = "AZ123";//mask 2 - odd length test
//String messageToEncode = "aA1. ..$%*+_/:";// - odd char  test
//String messageToEncode = "0123456789ABCDEFGHI";// - too long message  test for Type 1
//char charsToEncode[78] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789ABCDEFGHIJKLMNOPQRSTUVWXY";//Type 3 test
//String messageToEncode = "AZ12";//mask 7
//char charsToEncode[5] = "AZ12";//mask 7
//char charsToEncode[78] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
int grade =0;//place holder for qrmask grade
//ECC words in binary

//char array length
int length = 77;//max length version 3L
//byte counter
int byteCounter = 0;
//Keeps track of data displayed
int counter = 0;
int led = 4;// used in my hardware for troubleshooting
int PPS;//pulse per second
char mess9[10]="John 3:16";//a place holder for 9 character messages to be displayed on screen
char mess3[4]="000";//a place holder for 3 character messages to be displayed on screen
byte button;//status of button

void setup(void)
{
  //Start Serial at 115200 baud Used for trouble shooting
 Serial.begin(115200);//saves 290 bytes if not using
  //Init LCD
  //wln
 // turn on LED
 //pinMode(led, OUTPUT);// used in my hardware for troubleshooting
 //digitalWrite(led,HIGH); // used in my hardware for troubleshooting
 pinMode(A3, INPUT);//use as input
 digitalWrite(A3,HIGH);//turn on internal pullup
 //setupGPS();//specific to my hardware, comment out for public QR_Cod_Gen3
 
 //------------------------- QR Stuff ---------------
  //Generate data used for QR.  This is where the real work is done
  calculateQRCode();//this is in the generateBits tab(file)
  
  // rotate screen - optional - tested
  u8g.setRot90();
  //u8g.setRot180();
  //u8g.setRot270();
  //set font size
  u8g.setFont(u8g_font_7x13);
  //set # modules/side and position based on version
  if(qrV==2){
    qrM=25;
  }
  if(qrV==3){
    qrM=29;
  }
  // set the offsets to center code if center flag is set
  if(centerFlag > 0){//overide offsets
    if(centerFlag & 1){//center horz
    xOffs = getCenterOffsetX();
    }
    if(centerFlag & 2){//center vert
    yOffs = getCenterOffsetY();
    }
  }
   maskType = findBestMask();//find the best mask for the current message 
}

void draw(void) {
  // graphic commands to redraw the complete screen should be placed here
 // text to draw 
  //u8g.setFont(u8g_font_baby);
  //u8g.setFont(u8g_font_7x13);
  u8g.setColorIndex(1);//black ink
  //char mess[]="John 3:16";
  u8g.drawStr( 0, 10, mess9);
  //u8g.setPrintPos(70, 20); // for debugging
  //u8g.print(maskType);// for debugging
  
 //Draw QR code template
  drawTemplate(0);//mode 0 is to the screen
  //Fill in data bits
  informationBits2(0);//mode 0 is to screen. fill in bits over the template
  
  formatBits();//drow format bits. this puts the Code Type and Mask Type into the code
  if(button){
    u8g.setColorIndex(1);//black ink
    u8g.drawStr( 0, 100, mess3);
  }
  
  // the following just blinks a 6x6 area to show that the program has not crashed
  // very useful as your program expands.
  /*
  if(PPS){
     u8g.setColorIndex(0);//white ink
     u8g.drawBox(0,0,6,6);// 
  }else{
    u8g.setColorIndex(1);//black ink
     u8g.drawBox(0,0,6,6);// 
  }
  */
 // u8g.drawStr( 0, 10, "John 3:16");
}

void loop()
{
  //mess9[0]='0' + millis()/1000 % 10;
  // proves we have not run out of memory if the draw() function is flashing an area of the screen
  if(millis() % 1000 < 500){
    if(PPS==LOW){
     // mess9[0]=' ';
    }
  PPS = HIGH;
  
  
  }else{
    if(PPS==HIGH){
     // mess9[0]='J';
    }
    PPS=LOW;
   
  }
  button = digitalRead(A3);//read the button
  
  //GPShandler();//specific to my hardware,comment out for public QR_Code_Gen3
  if(millis() % 1000 < 10){//up date display every second or so
   // picture loop as recommended by the U8glib examples
   //
    u8g.firstPage(); 
    do {
      draw();
    } while( u8g.nextPage() );
      {
      }
      delay(100);
  }
}
  
