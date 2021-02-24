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
#include <algorithm>

#include "dmotTranslationRecord.h"
#include "../exceptions.h"



// Class dmotTranslationRecord
////////////////////////////////

// Constructor, destructor
////////////////////////////

dmotTranslationRecord::dmotTranslationRecord(){
}

dmotTranslationRecord::~dmotTranslationRecord(){
	RemoveAllTexts();
}



// Management
///////////////

void dmotTranslationRecord::SetIdentifier( unsigned int identifier ){
	if( identifier < 0 ) THROW( dueInvalidParam );
	
	pIdentifier = identifier;
}



int dmotTranslationRecord::GetTextCount() const{
	return pTexts.size();
}

const FXString &dmotTranslationRecord::GetTextAt( int index ) const{
	if( index < 0 || index >= ( int )pTexts.size() ) THROW( dueInvalidParam );
	
	return pTexts.at( index );
}

void dmotTranslationRecord::SetTextAt( int index, const FXString &text ){
	if( index < 0 || index >= ( int )pTexts.size() ) THROW( dueInvalidParam );
	
	pTexts.at( index ) = text;
}

void dmotTranslationRecord::AddText( const FXString &text ){
	pTexts.push_back( text );
}

void dmotTranslationRecord::RemoveTextAt( int index ){
	pTexts.erase( pTexts.begin() + index );
}

void dmotTranslationRecord::RemoveAllTexts(){
	pTexts.clear();
}
