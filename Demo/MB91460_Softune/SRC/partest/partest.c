/*
	FreeRTOS.org V4.7.1 - Copyright (C) 2003-2008 Richard Barry.

	This file is part of the FreeRTOS.org distribution.

	FreeRTOS.org is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	FreeRTOS.org is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with FreeRTOS.org; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

	A special exception to the GPL can be applied should you wish to distribute
	a combined work that includes FreeRTOS.org, without being obliged to provide
	the source code for any proprietary components.  See the licensing section
	of http://www.FreeRTOS.org for full details of how and when the exception
	can be applied.

	***************************************************************************

	Please ensure to read the configuration and relevant port sections of the 
	online documentation.

	+++ http://www.FreeRTOS.org +++
	Documentation, latest information, license and contact details.  

	+++ http://www.SafeRTOS.com +++
	A version that is certified for use in safety critical systems.

	+++ http://www.OpenRTOS.com +++
	Commercial support, development, porting, licensing and training services.

	***************************************************************************
*/


/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"

#define partstNUM_LEDs	8

static unsigned portCHAR sState[ 2 ] = { 0xFF, 0xFF };

/*-----------------------------------------------------------*/
void vParTestInitialise( void )
{
	DDR00 = 0xFF;
	PDR00 = 0xFF;
	DDR09 = 0xFF;
	PDR09 = 0xFF;
}
/*-----------------------------------------------------------*/

void vParTestToggleLED( unsigned portBASE_TYPE uxLED )
{
	if( uxLED < partstNUM_LEDs )
	{
		vTaskSuspendAll();

		/* Toggle the state of the single genuine on board LED. */
		if( ( sState[ 0 ] & ( ( unsigned portCHAR ) ( 1 << uxLED ) ) ) == 0 )
		{
			PDR09 |= ( 1 << uxLED );
			sState[ 0 ] |= ( 1 << uxLED );
		}
		else
		{
			PDR09 &= ~( 1 << uxLED );
			sState[ 0 ] &= ~( 1 << uxLED );
		}

		xTaskResumeAll();
	}
	else
	{
		vTaskSuspendAll();

		uxLED -= partstNUM_LEDs;

		if( ( sState[ 1 ] & ( ( unsigned portCHAR ) ( 1 << uxLED ) ) ) == 0 )
		{
			PDR00 |= ( 1 << uxLED );
			sState[ 1 ] |= ( 1 << uxLED );
		}
		else
		{
			PDR00 &= ~( 1 << uxLED );
			sState[ 1 ] &= ~( 1 << uxLED );
		}

		xTaskResumeAll();
	}
}
/*-----------------------------------------------------------*/

void vParTestSetLED( unsigned portBASE_TYPE uxLED, signed portBASE_TYPE xValue )
{
	/* Set or clear the output [in this case show or hide the '*' character. */
	if( uxLED < partstNUM_LEDs )
	{
		vTaskSuspendAll();
		{
			if( xValue )
			{
				PDR09 &= ~( 1 << uxLED );
				sState[ 0 ] &= ~( 1 << uxLED );
			}
			else
			{
				PDR09 |= ( 1 << uxLED );
				sState[ 0 ] |= ( 1 << uxLED );
			}
		}

		xTaskResumeAll();
	}
	else
	{
		vTaskSuspendAll();
		{
			if( xValue )
			{
				PDR00 &= ~( 1 << uxLED );
				sState[ 1 ] &= ~( 1 << uxLED );
			}
			else
			{
				PDR00 |= ( 1 << uxLED );
				sState[ 1 ] |= ( 1 << uxLED );
			}
		}

		xTaskResumeAll();
	}
}
