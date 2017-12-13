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


#include "slowPWM.h"
#include "heaterPID.h"

heaterPID extruder1_PID( HEATER_MANAGER_HZ, EXTRUDER1_PID );
slowPWM   extruder1_PWM( MIN_HEATER_PERIOD );

heaterPID bed_PID( HEATER_MANAGER_HZ, BED_HEATER_PID );
slowPWM   bed_PWM( MIN_HEATER_PERIOD );

float Extruder1Temp;

void heaterPWM()  // set heater PWM based off of PID results
{
   Extruder1Temp = getExtruder1Temp();
   extruder1_PWM.set( extruder1_PID.in( 00, Extruder1Temp ) );
   bed_PWM.set( bed_PID.in( 128, 128 ) );
}


void heaterOperator()  // operate heaters
{
   // do cool things... ha ha
 
   heaterPWM();   // update PWM setting

   if( extruder1_PWM.check() )
   {
      digitalWrite(EXTRUDER1_PWM_PIN, HIGH);
   }
   else
   {
      digitalWrite(EXTRUDER1_PWM_PIN, LOW);
   }

   if( bed_PWM.check() )
   {
      digitalWrite(BED_HEATER_PWM_PIN, HIGH);
   }
   else
   {
      digitalWrite(BED_HEATER_PWM_PIN, LOW);
   }
 
}

