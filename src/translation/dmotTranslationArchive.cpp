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

#include "dmotTranslationArchive.h"
#include "dmotTranslationFile.h"
#include "../exceptions.h"
#include "../config/dmotConfig.h"
#include "../file/dmotReadBinFile.h"



// Class dmotTranslationArchive
/////////////////////////////////

// Constructor, destructor
////////////////////////////

dmotTranslationArchive::dmotTranslationArchive(){
	pName = "Archive";
}

dmotTranslationArchive::~dmotTranslationArchive(){
	RemoveAllFiles();
}



// Management
///////////////

void dmotTranslationArchive::SetName( const FXString &name ){
	pName = name;
}

void dmotTranslationArchive::SetPathArchive( const FXString &path ){
	pPathArchive = path;
}

void dmotTranslationArchive::SetPathFileMap( const FXString &path ){
	pPathFileMap = path;
}



void dmotTranslationArchive::ImportBinFiles( dmotTranslateStatus &status, dmotConfig &config, bool replace ){
	std::vector<dmotTranslationFile*>::iterator iter = pFiles.begin();
	dmotReadBinFile readBinFile;
	
	while( iter != pFiles.end() ){
		readBinFile.ReadFile( status, config, *this, *(*iter++), replace );
	}
}

void dmotTranslationArchive::PatchBinFiles( dmotTranslateStatus &status, dmotConfig &config ){
	dmotReadBinFile readBinFile;
	
	readBinFile.WriteFile( status, config, *this );
}



int dmotTranslationArchive::GetFileCount() const{
	return pFiles.size();
}

dmotTranslationFile *dmotTranslationArchive::GetFileAt( int index ) const{
	if( index < 0 || index >= ( int )pFiles.size() ) THROW( dueInvalidParam );
	
	return pFiles.at( index );
}

void dmotTranslationArchive::AddFile( dmotTranslationFile *file ){
	if( ! file ) THROW( dueInvalidParam );
	
	pFiles.push_back( file );
}

void dmotTranslationArchive::RemoveFile( dmotTranslationFile *file ){
	if( ! file ) THROW( dueInvalidParam );
	
	std::vector<dmotTranslationFile*>::iterator iter = std::find( pFiles.begin(), pFiles.end(), file );
	
	if( iter == pFiles.end() ) THROW( dueInvalidParam );
	pFiles.erase( iter );
	delete file;
}

void dmotTranslationArchive::RemoveAllFiles(){
	while( ! pFiles.empty() ){
		delete pFiles.back();
		pFiles.pop_back();
	}
}
