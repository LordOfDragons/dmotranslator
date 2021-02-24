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

#include "dmotTranslation.h"
#include "dmotTranslationArchive.h"
#include "../config/dmotConfig.h"
#include "../file/dmotReadBinFile.h"
#include "../exceptions.h"



// Class dmotTranslation
//////////////////////////

// Constructor, destructor
////////////////////////////

dmotTranslation::dmotTranslation(){
	pFilename = "Translation.dmot";
	pSaved = false;
	pChanged = false;
}

dmotTranslation::~dmotTranslation(){
	RemoveAllArchives();
}



// Management
///////////////

void dmotTranslation::SetFilename( const FXString &filename ){
	pFilename = filename;
}

void dmotTranslation::SetSaved( bool saved ){
	pSaved = saved;
}

void dmotTranslation::SetChanged( bool changed ){
	pChanged = changed;
}



void dmotTranslation::ImportBinFiles( dmotTranslateStatus &status, dmotConfig &config, bool replace ){
	std::vector<dmotTranslationArchive*>::iterator iter = pArchives.begin();
	
	while( iter != pArchives.end() ){
		(*iter)->ImportBinFiles( status, config, replace );
		iter++;
	}
}

void dmotTranslation::PatchBinFiles( dmotTranslateStatus &status, dmotConfig &config ){
	std::vector<dmotTranslationArchive*>::iterator iter = pArchives.begin();
	
	while( iter != pArchives.end() ){
		(*iter)->PatchBinFiles( status, config );
		iter++;
	}
}



int dmotTranslation::GetArchiveCount() const{
	return pArchives.size();
}

dmotTranslationArchive *dmotTranslation::GetArchiveAt( int index ) const{
	if( index < 0 || index >= ( int )pArchives.size() ) THROW( dueInvalidParam );
	
	return pArchives.at( index );
}

void dmotTranslation::AddArchive( dmotTranslationArchive *archive ){
	if( ! archive ) THROW( dueInvalidParam );
	
	pArchives.push_back( archive );
}

void dmotTranslation::RemoveArchive( dmotTranslationArchive *archive ){
	if( ! archive ) THROW( dueInvalidParam );
	
	std::vector<dmotTranslationArchive*>::iterator iter = std::find( pArchives.begin(), pArchives.end(), archive );
	
	if( iter == pArchives.end() ) THROW( dueInvalidParam );
	pArchives.erase( iter );
	delete archive;
}

void dmotTranslation::RemoveAllArchives(){
	while( ! pArchives.empty() ){
		delete pArchives.back();
		pArchives.pop_back();
	}
}
