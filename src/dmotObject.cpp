/* 
 * DMO Translator
 *
 * Copyright (C) 2010, Plüss Roland ( roland@rptd.ch )
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dmotObject.h"



// Class dmotObject
/////////////////////

// Constructors and Destructors
/////////////////////////////////

dmotObject::dmotObject(){
	pRefCount = 1;
}

dmotObject::~dmotObject(){
}



// Management
///////////////

void dmotObject::AddReference(){
	pRefCount++;
}

void dmotObject::FreeReference(){
	pRefCount--;
	
	if( pRefCount == 0 ){
		delete this;
		
	}else if( pRefCount < 0 ){
		printf( "WARNING! FreeReference called on Object %p having %i reference count!\n", this, pRefCount );
	}
}
