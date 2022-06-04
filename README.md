QR_Code_Gen3
============

QR Code Generator for Arduino Uno Version 1, 2 and 3. Full Spec Implementation with Small Memory Footprint

QR_Code_Gen3
(c)2014 William L. "Bud" Nail, Technological Services Company
This program and support functions fully implement the generation of several useful versions of the QR Code standard from a text string.

It does this in a relatively small memory foot print such as the Arduino Uno and still leaves room for additional application functions.
It can be easily modified to support additional QR codes with some limitations.

This program is licensed under GPL 2, 3, LGPL or MIT (your choice) that work well for non-commercial and commercial projects (IMHO). 
If you are interested in a simple low cost commercial or developer's license for embedding in your projects, and these do not meet your needs 
see  www.technologicalservicescompany.com for contact information.

Donations are also accepted for further development. 

Regardless of the license, the code is free for review and experimentation by individuals and commercial or non-commercial enterprises.

To use:
Install [U8glib library for Universal 8 bit graphics](https://github.com/olikraus/u8glib) into your Arduino IDE.
QR_Code_Gen3... is the main code file.  The others are supporting files. After downloading the zip file and extracting the files, rename the folder containing the files to "QR_Code_Gen3V38_Public" (the current name of the main file without the ".ino"). To save memory, only one QR version is selected at a time for a project. Code lines that are not used for the currently selected QR Code type (Version 3 L as of this writing) are commented out.  Uncomment lines marked for other versions if you wish to use them.


History:
When this program development began, in early 2014, it was thought by most (from reviewing forums on the subject) that it would be next to impossible to implement a dynamic QR code generator on a processor such as the Arduino UNO that has so little SRAM.

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
since it uses the [U8glib library for Universal 8 bit graphics](https://github.com/olikraus/u8glib), the program should be usable with most any graphic display.
Simply take one of the U8glib examples and pull the line that looks equivalent to:
```
//set up SPI for model display used and wiring
U8GLIB_DOGS102 u8g(13, 11, 10, 9);		// SPI Com: SCK = 13, MOSI = 11, CS = 10, A0(also referenced as CD) = 9
```
Replace this line with the one that matches your display.
Adjust the SPI communication ports to match the ones on your hardware.

To Do: 
       
       (1) build the qr code in a buffer area so that it can be built repeatedly for each mask 0-7 - done

       (2) build the test to score the mask methods. - done

       (3) build the best scoring and Display - done

       (4) turn the process into a function so that any message string can be sent to it at any time.

       (5) make all methods work with QR code Versions 2 and 3 as well as 1 - done

       (6) auto covert lower case to upper case since trying to encode lowercase is a common mistake - done

       (7) change encoding to include allowed non alpha numeric - done

       (8) - maybe add numeric encoding vs only alphanumeric encoding - this will require changes throughout

       (9) The current version uses 2x2 pixels per QR code module. Make the size selectable.
       
Version History:

    V3 added the option to choose other mask types - the specification requires you try all 7 and use the best. this version only provides that you can use any of the 7
    V5 Fixed the bug in the ECC calculation that would give bad ECC.
    V6 adds scalable templates for versions 1, 2 and 3
    V7 adds a QRcode Buffer that lets you generate the code off screen for grading - currently only the template
    v8 adds some text functions for debugging.
    v9 clears out a lot of the old methods and comments, added function to create format bits for all ECCs and Masks
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
    V38 Removed dependencies on the String class which saved lots of memory.

Developer Notes:
  Basic steps are:
    
     0. select the QR code type
     1. translate the message into data bits
     2. Calculate the Error Correction Codes using polynomial division
     3. Place the data and ECC bits within a template for evaluation
     4. Find the best mask to use for the QR code by trying them all and picking the best (grading)
     5. drawing the qr code on the screen using the mask selected above
     
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
