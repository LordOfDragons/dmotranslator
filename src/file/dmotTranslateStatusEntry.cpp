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

#include "dmotTranslateStatusEntry.h"
#include "../translation/dmotTranslationArchive.h"
#include "../translation/dmotTranslationFile.h"
#include "../exceptions.h"



// Class dmotTranslateStatusEntry
///////////////////////////////////

// Constructor, destructor
////////////////////////////

dmotTranslateStatusEntry::dmotTranslateStatusEntry( const dmotTranslationArchive *archive, const dmotTranslationFile *file ){
	if( ! archive ) THROW( dueInvalidParam );
	
	pArchive = archive;
	pFile = file;
}

dmotTranslateStatusEntry::~dmotTranslateStatusEntry(){
}



// Management
///////////////

void dmotTranslateStatusEntry::SetText( const FXString &text ){
	pText = text;
}

void dmotTranslateStatusEntry::AppendText( const FXString &text ){
	if( pText.length() == 0 ){
		pText = text;
		
	}else{
		pText.append( "\n" );
		pText.append( text );
	}
}

void dmotTranslateStatusEntry::AppendException( const duException &exception ){
	FXString text;
	
	text.format( "Exception: File='%s', Line='%i', Reason='%s'.", exception.GetFile(), exception.GetLine(), exception.GetDescription() );
	
	AppendText( text );
}

void dmotTranslateStatusEntry::AppendTextException( const FXString &text, const duException &exception ){
	FXString etext;
	
	etext.format( "Exception: File='%s', Line='%i', Reason='%s'.", exception.GetFile(), exception.GetLine(), exception.GetDescription() );
	
	AppendText( text );
	AppendText( etext );
}
