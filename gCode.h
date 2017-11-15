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


struct gCode_state_machine_t
{
   byte G[17] = {0, 0, 17, 90, 0, 94, 21, 40, 49, 80, 98, 50, 54, 0, 0, 64, 69}; // G code groups are 0 - 16
   int M;
   
   float A, B, C, D, E, F, H, I, J, K, L, N, P, Q, R, S, T, U, V, W, X, Y, Z; // G, M, O intentionally omitted 

   float startX, startY, startZ;

   bool newAxisMove, newExtruderMove, newMcode;
 
} gCode;


void gCodeSetPosition(float x, float y, float z, float e)
{
   gCode.X = x;
   gCode.Y = y;
   gCode.Z = z;
   gCode.E = e;
}


char getNextChar()
{
   if( file.available() )
   {
      char ch;
      file.read(&ch, 1);
      //Serial.print(" ");Serial.print(int(ch));
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
      float diff = abs( gCode.X - number );
      if( diff > 0.0009f )
      {
         gCode.newAxisMove = true;
         if( gCode.G[3] == 90 )
         {
            gCode.X = number; // absolute
         }
         else if( gCode.G[3] == 91 )
         {
            gCode.X = gCode.startX + number; // incremental
         }
      }
      return;
   }

   if( letter == 'Y' )
   {
      float diff = abs( gCode.Y - number );
      if( diff > 0.0009f )
      {
         gCode.newAxisMove = true;
         if( gCode.G[3] == 90 )
         {
            gCode.Y = number; // absolute
         }
         else if( gCode.G[3] == 91 )
         {
            gCode.Y = gCode.startY + number; // incremental
         }
      }
      return;
   }

   if( letter == 'E' )
   {
      float diff = abs( gCode.E - number );
      if( diff > 0.0009f )
      {
         gCode.newExtruderMove = true;
         gCode.E = number;
      }
      return;
   }

   if( letter == 'F' )
   {
      gCode.F = number;
      return;
   }

   if( letter == 'Z' )
   {
      float diff = abs( gCode.Z - number );
      if( diff > 0.0009f )
      {
         gCode.newAxisMove = true;
         if( gCode.G[3] == 90 )
         {
            gCode.Z = number; // absolute
         }
         else if( gCode.G[3] == 91 )
         {
            gCode.Z = gCode.startZ + number; // incremental
         }
      }
      return;
   }

   if( letter == 'G' )
   {
      byte num = byte(number);

      switch( num )
      {
         case 4 :    // Group 0
         case 9 :
         case 28:
         case 29:
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
      }
      return;
   }

   if( letter == 'M' )
   {
      int num = int(number);

      gCode.M = num;
      gCode.newMcode = true;
      return;
   }

   if( letter == 'P' )
   {
      gCode.P = number;
      return;
   }
   
   if( letter == 'I' )
   {
      gCode.I = number;
      return;
   }
   
   if( letter == 'J' )
   {
      gCode.J = number;
      return;
   }
   
   if( letter == 'K' )
   {
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
               if( negative ) number *= -1.0f;
               setState( letter, number );
            }
         }
      }
   }

   if( ch == 0 ) return false; // return false if at end of file
   
   return true;
}


void addMovementBlock()
{
   float arcCenterX, arcCenterY;

   switch (gCode.G[1])
   {
      case 0:
         motion.addLinear_Block(0, gCode.X, gCode.Y, gCode.Z, MAX_VELOCITY);
         //Serial.print("   G0 ");
         break;

      case 1:
         motion.addLinear_Block(1, gCode.X, gCode.Y, gCode.Z, gCode.F);
         //Serial.print("   G1 ");
         break;

      case 2:
      case 3:
         arcCenterX = gCode.startX + gCode.I;
         arcCenterY = gCode.startY + gCode.J;
         motion.addArc_Block(gCode.G[1], gCode.X, gCode.Y, gCode.F, arcCenterX, arcCenterY);
         break;
   }
   //Serial.print(gCode.X);Serial.print(" ");
   //Serial.print(gCode.Y);Serial.print(" ");
   //Serial.println(gCode.Z);

   gCode.newAxisMove = false;

}

void addExtruderMove()
{
   gCode.newExtruderMove = false;
}


void executeCode()
{

   if( gCode.newAxisMove && gCode.newExtruderMove ) // extrude while moving
   {
      addMovementBlock();
      addExtruderMove();
   }
   else if ( gCode.newAxisMove ) // move only
   {
      addMovementBlock();
      //Serial.println("!");
   }
   else if ( gCode.newExtruderMove ) // extrude only
   {
      addExtruderMove();
   }

   if( gCode.newMcode )
   {
      switch(gCode.M)
      {
         case 0:     // Stop
            break;
         
         case 1:     // Optional Stop
            break;
         
         case 2:     // Program End
            break;
         
         case 3:     // Spindle Start CW
            break;
         
         case 4:     // Spindle Start CCW
            break;

         case 5:     // Spindle Stop
            break;

         case 6:     // Tool Change
            break;
         
         case 7:     // Shower Coolant
            break;

         case 8:     // Coolant On
            break;

         case 9:     // Coolant Off
            break;

         case 30:    // Program end, reset
            break;

         default:
            break;

      }
      gCode.newMcode = false;
   }



   // Non movement commands      
   

}



