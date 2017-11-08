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

#include "config.h"
#include "Kynetic_pins.h"

#include "motors.h"
#include "dataStreams.h"
#include "timers.h"
#include "motion.h"
#include "gCode.h"

#include "Machines\cartesian.h"
#include "Machines\coreXY.h"
#include "Machines\delta.h"

#include "kynetic.h"


void setup() {

   startSerial();

   startSD();

   startStepperTickISR();

   setPins();

   armMotors();
  
   startPollTimers();
}


void loop() {
   
   // Nested if-else priority scheme
   // * After any operation completes, higher priority operations are given the first opportunity to run
   // * All operations should run quickly so that higher priority operations are not delayed excessively
   
   if( motionControl.check() )  // Highest Priority
   {
      if( runProgram )
      {
         motorController();
      }
      else
      {
         machine.executeHome();
      }
   }
   else if( false && motionControl.precheck(10) ) // prevent executing other code if very close to next motion control operation
   {
      // do nothing
   }
   else if( false && motion.bufferVacancy() ) // Execute G code, feed blocks to the motion controller 
   {
      //executeCode();
   }
   else if( false && getNextProgramBlock ) // Read SD card and Parse G code
   {
      //readNextProgramLine();
   }
   else if ( buttonsAndUI.check() ) // check if any buttons are depressed and update Display
   {
      if( !digitalRead(SELECT_BUTTON_PIN) )
      {
         machine.startHome( true, true, true );
      }
   }
   else if( maintenance.check() ) // Lowest Priority
   {
      /*
      SERIAL_PORT.print( A_motor.getPositionMM() ); SERIAL_PORT.print( "\t" );
      SERIAL_PORT.print( B_motor.getPositionMM() ); SERIAL_PORT.print( "\t" );
      SERIAL_PORT.println( C_motor.getPositionMM() );
      */
      SERIAL_PORT.println( digitalRead(SELECT_BUTTON_PIN) );
      
   }   

}




