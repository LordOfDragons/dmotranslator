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
#include <functional>

#include "dmotTranslationFile.h"
#include "dmotTranslationSection.h"
#include "../exceptions.h"



// Class dmotTranslationFile
//////////////////////////////

// Constructor, destructor
////////////////////////////

dmotTranslationFile::dmotTranslationFile(){
	pIdentifier = 0;
	pName = "File";
}

dmotTranslationFile::~dmotTranslationFile(){
	RemoveAllSections();
}



// Management
///////////////

void dmotTranslationFile::SetIdentifier( FXuint identifier ){
	pIdentifier = identifier;
}

void dmotTranslationFile::SetName( const FXString &name ){
	pName = name;
}



int dmotTranslationFile::GetSectionCount() const{
	return pSections.size();
}

dmotTranslationSection *dmotTranslationFile::GetSectionAt( int index ) const{
	if( index < 0 || index >= ( int )pSections.size() ) THROW( dueInvalidParam );
	
	return pSections.at( index );
}

void dmotTranslationFile::AddSection( dmotTranslationSection *section ){
	if( ! section ) THROW( dueInvalidParam );
	
	pSections.push_back( section );
}

void dmotTranslationFile::RemoveSection( dmotTranslationSection *section ){
	if( ! section ) THROW( dueInvalidParam );
	
	std::vector<dmotTranslationSection*>::iterator iter = std::find( pSections.begin(), pSections.end(), section );
	
	if( iter == pSections.end() ) THROW( dueInvalidParam );
	pSections.erase( iter );
	delete section;
}

void dmotTranslationFile::RemoveAllSections(){
	while( ! pSections.empty() ){
		delete pSections.back();
		pSections.pop_back();
	}
}
