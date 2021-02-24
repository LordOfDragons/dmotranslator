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

#include "dmotTranslationSection.h"
#include "dmotTranslationSectionText.h"
#include "dmotTranslationRecord.h"
#include "../exceptions.h"



// Functors
/////////////

struct fTextEqualName{
	FXString pName;
	
	fTextEqualName( const FXString &name ){
		pName = name;
	}
	
	bool operator()( dmotTranslationSectionText *text ){
		return text->GetHeaderName() == pName;
	}
};

struct fRecordEqualID{
	unsigned int pID;
	
	fRecordEqualID( unsigned int id ){
		pID = id;
	}
	
	bool operator()( dmotTranslationRecord *record ){
		return record->GetIdentifier() == pID;
	}
};

struct fRecordDeleteText{
	int pIndex;
	
	fRecordDeleteText( int index ){
		pIndex = index;
	}
	
	void operator()( dmotTranslationRecord *record ){
		record->RemoveTextAt( pIndex );
	}
};

struct fRecordAddText{
	void operator()( dmotTranslationRecord *record ){
		record->AddText( "" );
	}
};



// Class dmotTranslationSection
/////////////////////////////////

// Constructor, destructor
////////////////////////////

dmotTranslationSection::dmotTranslationSection(){
	pName = "Section";
	pSectionPosition = 0;
	pHeaderIDName = "ID";
	pHeaderIDWidth = 100;
	pIDPosition = 0;
	pIDType = eidtUInt;
	pRecordSize = 100;
	pPaddingType = eptNoPadding;
	pPaddingCount = 0;
}

dmotTranslationSection::~dmotTranslationSection(){
	RemoveAllTexts();
}



// Management
///////////////

void dmotTranslationSection::SetName( const FXString &name ){
	pName = name;
}

void dmotTranslationSection::SetHeaderIDName( const FXString &name ){
	pHeaderIDName = name;
}

void dmotTranslationSection::SetHeaderIDWidth( int width ){
	if( width < 0 ) THROW( dueInvalidParam );
	
	pHeaderIDWidth = width;
}

void dmotTranslationSection::SetSectionPosition( int position ){
	if( position < 0 ) THROW( dueInvalidParam );
	
	pSectionPosition = position;
}

void dmotTranslationSection::SetIdentifierPosition( int position ){
	if( position < 0 ) THROW( dueInvalidParam );
	
	pIDPosition = position;
}

void dmotTranslationSection::SetIdentifierType( int type ){
	pIDType = type;
}

void dmotTranslationSection::SetRecordSize( int size ){
	if( size < 0 ) THROW( dueInvalidParam );
	
	pRecordSize = size;
}

void dmotTranslationSection::SetPaddingType( int paddingType ){
	pPaddingType = paddingType;
}

void dmotTranslationSection::SetPaddingCount( int paddingCount ){
	if( paddingCount < 0 ) THROW( dueInvalidParam );
	
	pPaddingCount = paddingCount;
}



int dmotTranslationSection::GetTextCount() const{
	return pTexts.size();
}

int dmotTranslationSection::IndexOfTextNamed( const FXString &name ){
	std::vector<dmotTranslationSectionText*>::const_iterator iter = std::find_if( pTexts.begin(), pTexts.end(), fTextEqualName( name ) );
	return ( iter == pTexts.end() ) ? -1 : iter - pTexts.begin();
}

dmotTranslationSectionText *dmotTranslationSection::GetTextAt( int index ) const{
	if( index < 0 || index >= ( int )pTexts.size() ) THROW( dueInvalidParam );
	
	return pTexts.at( index );
}

void dmotTranslationSection::AddText( dmotTranslationSectionText *text ){
	if( ! text ) THROW( dueInvalidParam );
	
	pTexts.push_back( text );
}

void dmotTranslationSection::RemoveText( dmotTranslationSectionText *text ){
	if( ! text ) THROW( dueInvalidParam );
	
	std::vector<dmotTranslationSectionText*>::iterator iter = std::find( pTexts.begin(), pTexts.end(), text );
	
	if( iter == pTexts.end() ) THROW( dueInvalidParam );
	pTexts.erase( iter );
	delete text;
}

void dmotTranslationSection::RemoveAllTexts(){
	while( ! pTexts.empty() ){
		delete pTexts.back();
		pTexts.pop_back();
	}
}



int dmotTranslationSection::GetRecordCount() const{
	return pRecords.size();
}

bool dmotTranslationSection::HasRecordWithID( int identifier ) const{
	std::vector<dmotTranslationRecord*>::const_iterator iter = std::find_if( pRecords.begin(), pRecords.end(), fRecordEqualID( identifier ) );
	return iter != pRecords.end();
}

dmotTranslationRecord *dmotTranslationSection::GetRecordAt( int index ) const{
	if( index < 0 || index >= ( int )pRecords.size() ) THROW( dueInvalidParam );
	
	return pRecords.at( index );
}

dmotTranslationRecord *dmotTranslationSection::GetRecordWithID( unsigned int identifier ) const{
	std::vector<dmotTranslationRecord*>::const_iterator iter = std::find_if( pRecords.begin(), pRecords.end(), fRecordEqualID( identifier ) );
	return ( iter == pRecords.end() ) ? NULL : *iter;
	
	/*
	int i, count = pRecords.size();
	dmotTranslationRecord *record;
	
	for( i=0; i<count; i++ ){
		record = pRecords.at( i );
		if( record->GetIdentifier() == identifier ){
			return record;
		}
	}
	
	return NULL;
	*/
}

void dmotTranslationSection::AddRecord( dmotTranslationRecord *record ){
	if( ! record || HasRecordWithID( record->GetIdentifier() ) ) THROW( dueInvalidParam );
	
	pRecords.push_back( record );
}

void dmotTranslationSection::RemoveRecord( dmotTranslationRecord *record ){
	if( ! record ) THROW( dueInvalidParam );
	
	std::vector<dmotTranslationRecord*>::iterator iter = std::find( pRecords.begin(), pRecords.end(), record );
	
	if( iter == pRecords.end() ) THROW( dueInvalidParam );
	pRecords.erase( iter );
	delete record;
}

void dmotTranslationSection::RemoveAllRecords(){
	while( ! pRecords.empty() ){
		delete pRecords.back();
		pRecords.pop_back();
	}
}

void dmotTranslationSection::AddTextInRecords( const FXString &text ){
	std::for_each( pRecords.begin(), pRecords.end(), fRecordAddText() );
}

void dmotTranslationSection::RemoveTextFromRecordsAt( int index ){
	std::for_each( pRecords.begin(), pRecords.end(), fRecordDeleteText( index ) );
}
