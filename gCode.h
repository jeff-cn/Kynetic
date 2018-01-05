/*
   Kynetic CNC Control Software
   Copyright (C) 2017 Phillip Schmidt

      This program is free software: you can redistribute it and/or modify
      it under the terms of the GNU General Public License as published by
      the Free Software Foundation, either version 3 of the License, or
      (at your option) any later version.
      
      This program is distributed in the hope that it will be useful,
      but WITHOUT ANY WARRANTY; without even the implied warranty of
      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
      GNU General Public License for more details.
      
      You should have received a copy of the GNU General Public License
      along with this program.  If not, see <http://www.gnu.org/licenses/>
*/


#include "gCodeStructure.h"
#include "gCodeOperations.h"


char getNextChar()
{
   if( file.available() )
   {
      char ch;
      file.read(&ch, 1);
      //Serial.print(" ");Serial.print(int(ch));
      //Serial.print(ch);
      return ch;
   }
   return 0;
}


void setState( char letter, float number )
{

   // list most common letters first to avoid uneeded compares
   //Serial.print(letter);Serial.println(number);

   if( letter == 'X' )
   {
      if( gCode.G[6] == 20 ) number *= 25.4f; // convert to mm

      if( gCode.G[0] == 92 )  // set position
      {
         gCode.X = number;
         return;
      }

      if( gCode.G[3] == 90 ) // absolute
      {
         if( abs( gCode.X - number ) > 0.0009f )
         {
            gCode.X = number;
            gCode.newAxisMove = true;
         }
      }
      else if( gCode.G[3] == 91 ) // incremental
      {
         if( abs(number) > 0.0009f )
         {
            gCode.X += number; 
            gCode.newAxisMove = true;
         }
      }
      return;
   }

   if( letter == 'Y' )
   {
      if( gCode.G[6] == 20 ) number *= 25.4f; // convert to mm
      
      if( gCode.G[0] == 92 )  // set position
      {
         gCode.Y = number;
         return;
      }

      if( gCode.G[3] == 90 ) // absolute
      {
         if( abs(gCode.Y - number) > 0.0009f )
         {
            gCode.Y = number;
            gCode.newAxisMove = true;
         }
      }
      else if( gCode.G[3] == 91 ) // incremental
      {
         if( abs(number) > 0.0009f )
         {
            gCode.Y += number; 
            gCode.newAxisMove = true;
         }
      }
      return;
   }

   if( letter == 'E' )
   {
      if( gCode.G[6] == 20 ) number *= 25.4f; // convert to mm
      
      if( gCode.G[0] == 92 )  // set position
      {
         gCode.E = number;
         return;
      }

      if( gCode.extrudeAbsoluteMode ) // absolute
      {
         if( abs(gCode.E - number) > 0.0009f )
         {
            gCode.E = number;
            gCode.newExtruderMove = true;
         }
      }
      else  // incremental
      {
         if( abs(number) > 0.0009f )
         {
            gCode.E += number; 
            gCode.newExtruderMove = true;
         }
      }
      return;
   }

   if( letter == 'F' )
   {
      if( gCode.G[6] == 21 ) // metric
      {
         gCode.F = number * ( 1.0f / 60.0f); // convert: mm/min ==> mm/s
      } 
      else if( gCode.G[6] == 20 ) // imperial
      {
         gCode.F = number * (25.4f / 60.0f); // convert: in/min ==> mm/s
      }
      return;
   }

   if( letter == 'Z' )
   {
      if( gCode.G[6] == 20 ) number *= 25.4f; // convert to mm

      if( gCode.G[0] == 92 )  // set position
      {
         gCode.Z = number;
         return;
      }

      if( gCode.G[3] == 90 ) // absolute
      {
         if( abs(gCode.Z - number) > 0.0009f )
         {
            gCode.Z = number;
            gCode.newAxisMove = true;
         }
      }
      else if( gCode.G[3] == 91 ) // incremental
      {
         if( abs(number) > 0.0009f )
         {
            gCode.Z += number; 
            gCode.newAxisMove = true;
         }
      }
      return;
   }

   if( letter == 'G' )
   {
      byte num = byte(number);

      switch( num )
      {
         case 4 :    // Group 0 -- dwell, exact stop, Home
         case 9 :
         case 28:
         case 29:
         case 92:    // set position
            KORE.delayedExecute = true;
            gCode.G[0] = num;
            break;

         case 0 :    // Group 1
         case 1 :
         case 2 :
         case 3 :
            gCode.G[1] = num;
            break;

         case 17:    // Group 2
         case 18:
         case 19:
            gCode.G[2] = num;
            break;        

         case 90:    // Group 3
         case 91:
            gCode.G[3] = num;
            break;

                  // No Group 4

         case 93:    // Group 5
         case 94:
         case 95:
            gCode.G[5] = num;
            break;

         case 20:    // Group 6
         case 21:
            gCode.G[6] = num;
            break;

         case 40:    // Group 7
         case 41:
         case 42:
            gCode.G[7] = num;
            break;

         default:
            break;
      }
      return;
   }

   if( letter == 'M' )
   {
      int num = int(number);

      gCode.M = num;
      gCode.newMcode = true;
      KORE.delayedExecute = true;
      return;
   }

   if( letter == 'P' )
   {
      gCode.P = number;
      return;
   }
   
   if( letter == 'I' )
   {
      gCode.newAxisMove = true;
      gCode.I = number;
      return;
   }
   
   if( letter == 'J' )
   {
      gCode.newAxisMove = true;
      gCode.J = number;
      return;
   }
   
   if( letter == 'K' )
   {
      gCode.newAxisMove = true;
      gCode.K = number;
      return;
   }

   if( letter == 'A' )
   {
      gCode.A = number;
      return;
   }
   
   if( letter == 'B' )
   {
      gCode.B = number;
      return;
   }
   
   if( letter == 'C' )
   {
      gCode.C = number;
      return;
   }

   if( letter == 'U' )
   {
      gCode.U = number;
      return;
   }
   
   if( letter == 'V' )
   {
      gCode.V = number;
      return;
   }
   
   if( letter == 'W' )
   {
      gCode.W = number;
      return;
   }
  
   if( letter == 'D' )
   {
      gCode.D = number;
      return;
   }
   
   if( letter == 'H' )
   {
      gCode.H = number;
      return;
   }

   if( letter == 'L' )
   {
      gCode.L = number;
      return;
   }

   if( letter == 'N' )
   {
      gCode.N = number;
      return;
   }
   
   if( letter == 'Q' )
   {
      gCode.Q = number;
      return;
   }
   
   if( letter == 'R' )
   {
      gCode.R = number;
      return;
   }
  
   if( letter == 'S' )
   {
      gCode.S = number;
      return;
   }
   
   if( letter == 'T' )
   {
      gCode.T = number;
      return;
   }

}


bool readNextProgramLine()
{
   bool endOfBlockFound = false;
   char ch = getNextChar();

   gCode.startX = gCode.X; // save current location
   gCode.startY = gCode.Y;
   gCode.startZ = gCode.Z;

   while( ch != 10 && ch != 0 )  // iterate to end of the line
   {
      if( endOfBlockFound ) // ignore all characters after the EOB until CR
      {
         ch = getNextChar(); // throw away
         //Serial.print(".");
      }
      else
      {
         if( ch == ';' || ch == '(' )
         {
            endOfBlockFound = true;
            ch = getNextChar();
         }
         else
         {
            if( ch > 96 && ch < 123 ) ch -= 32; // to upper

            if( ch > 64 && ch < 91 ) // Get Letter (ignore all else)
            {
               bool negative = false;
               bool validNum = false;
               float decimal = 1.0f; // arbitrary number above 0.1
               float number  = 0.0f;
               char letter   = ch;

               ch = getNextChar();

               if(ch == '-')  // check for negative (only valid if it is the first char after the letter)
               {
                  negative = true;
                  ch = getNextChar();
               }

               while( (ch > 47 && ch < 58) || ch == '.' || ch == ',' ) // Get Number (or decimal point )
               {
                  if( ch == '.' || ch == ',' )
                  {
                     if( decimal > 0.101f ) 
                     {
                        decimal = 0.1f; // only change if a decimal point has not been seen previously
                     }
                     else
                     {
                        // ( in the future multiple decimal points should probably generate an alarm, for now just ignore )
                     }
                  }
                  else
                  {
                     validNum = true; // at least one digit after the letter
                     if( decimal > 0.101f )
                     {
                        number = number * 10.0f + float( ch - 48 );  // int component
                     }
                     else
                     {
                        number += float( ch - 48 ) * decimal; // decimal component
                        decimal *= 0.1f;
                     }
                  }

                  ch = getNextChar();
               }

               if( validNum )
               {
                  if( negative ) number *= -1.0f;
                  setState( letter, number );
               }
               else
               {
                  ch = getNextChar(); // if number is invalid, keep moving forward
               } 
            }
            else
            {
               ch = getNextChar(); // if letter is invalid, keep moving forward
            }
         }
      }
   } // end of main while loop

   if( ch == 0 ) return false; // return false if at end of file
   
   return true;
}


void executeCodeNow()
{
   Group3();  // Abs/Inc selection

   Group5();  // Feed Mode

   Group6();  // Inch/Metric

   Group7();  // Cutter comp

   Group8();  // Tool length comp

   Group10();  // Return plane select

   Group11();  // Scaling

   Group12();  // Work coordinate

   Group15();  // Exact stop mode

   Group16();  // Rotation

   movementOperations();  // group 1 -- placed last to allow modification by other G codes on the same line

}


void executeCodeDelayed()
{
   Group0();  // dwell, non-modal commands

   Group2();  // Plane Selection

   Group9();  // Canned cycles
    
   mCodes();  // miscellaneous, delayed execute

   KORE.delayedExecute = false;
}



