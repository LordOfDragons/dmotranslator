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

#include "dmotTranslationSectionText.h"
#include "../exceptions.h"



// Class dmotTranslationSectionText
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

dmotTranslationSectionText::dmotTranslationSectionText(){
	pHeaderName = "Text";
	pHeaderWidth = 100;
	pPosition = 0;
	pLength = 1;
	pFormat = etfWideChar;
}

dmotTranslationSectionText::~dmotTranslationSectionText(){
}



// Management
///////////////

void dmotTranslationSectionText::SetHeaderName( const FXString &name ){
	pHeaderName = name;
}

void dmotTranslationSectionText::SetHeaderWidth( int width ){
	if( width < 0 ) THROW( dueInvalidParam );
	
	pHeaderWidth = width;
}

void dmotTranslationSectionText::SetPosition( int position ){
	if( position < 0 ) THROW( dueInvalidParam );
	
	pPosition = position;
}

void dmotTranslationSectionText::SetLength( int length ){
	if( length < 0 ) THROW( dueInvalidParam );
	
	pLength = length;
}

void dmotTranslationSectionText::SetFormat( int format ){
	pFormat = format;
}
