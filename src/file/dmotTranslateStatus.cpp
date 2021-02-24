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

#include "dmotTranslateStatus.h"
#include "dmotTranslateStatusEntry.h"
#include "../exceptions.h"



// Functors
/////////////

struct fEntryEquals{
	const dmotTranslationArchive *pArchive;
	const dmotTranslationFile *pFile;
	
	fEntryEquals( const dmotTranslationArchive *archive, const dmotTranslationFile *file ){
		pArchive = archive;
		pFile = file;
	}
	
	bool operator()( dmotTranslateStatusEntry *entry ){
		return entry->GetArchive() ==  pArchive && entry->GetFile() == pFile;
	}
};



// Class dmotTranslateStatus
//////////////////////////////

// Constructor, destructor
////////////////////////////

dmotTranslateStatus::dmotTranslateStatus(){
}

dmotTranslateStatus::~dmotTranslateStatus(){
	RemoveAllEntries();
}



// Management
///////////////

int dmotTranslateStatus::GetEntryCount() const{
	return pEntries.size();
}

dmotTranslateStatusEntry *dmotTranslateStatus::GetEntryAt( int index ) const{
	if( index < 0 || index >= ( int )pEntries.size() ) THROW( dueInvalidParam );
	
	return pEntries.at( index );
}

dmotTranslateStatusEntry *dmotTranslateStatus::GetEntryWith( const dmotTranslationArchive *archive, const dmotTranslationFile *file ){
	std::vector<dmotTranslateStatusEntry*>::const_iterator iter = std::find_if( pEntries.begin(), pEntries.end(), fEntryEquals( archive, file ) );
	
	if( iter == pEntries.end() ){
		dmotTranslateStatusEntry *entry = NULL;
		
		try{
			entry = new dmotTranslateStatusEntry( archive, file );
			if( ! entry ) THROW( dueOutOfMemory );
			
			AddEntry( entry );
			
		}catch( duException ){
			if( entry ) delete entry;
			throw;
		}
		
		return entry;
		
	}else{
		return *iter;
	}
}

void dmotTranslateStatus::AddEntry( dmotTranslateStatusEntry *entry ){
	pEntries.push_back( entry );
}

void dmotTranslateStatus::RemoveEntryAt( dmotTranslateStatusEntry *entry ){
	if( ! entry ) THROW( dueInvalidParam );
	
	std::vector<dmotTranslateStatusEntry*>::iterator iter = std::find( pEntries.begin(), pEntries.end(), entry );
	
	if( iter == pEntries.end() ) THROW( dueInvalidParam );
	pEntries.erase( iter );
	delete entry;
}

void dmotTranslateStatus::RemoveAllEntries(){
	while( ! pEntries.empty() ){
		delete pEntries.back();
		pEntries.pop_back();
	}
}
