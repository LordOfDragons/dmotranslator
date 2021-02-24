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
#include <vector>

#include "dmotLoadSaveCSV.h"
#include "../translation/dmotTranslation.h"
#include "../translation/dmotTranslationFile.h"
#include "../translation/dmotTranslationSection.h"
#include "../translation/dmotTranslationSectionText.h"
#include "../translation/dmotTranslationRecord.h"
#include "../exceptions.h"



// Class dmotLoadSaveCSV
//////////////////////////

// Constructor, destructor
////////////////////////////

dmotLoadSaveCSV::dmotLoadSaveCSV(){
}

dmotLoadSaveCSV::~dmotLoadSaveCSV(){
}



// Management
///////////////

void dmotLoadSaveCSV::LoadFromFile( const FXString &filename, dmotTranslation &translation, dmotTranslationFile &file, dmotTranslationSection &section, bool replace ){
	dmotTranslationRecord *record = NULL;
	dmotTranslationRecord *safeRecord;
	FXFileStream *stream = NULL;
	FXString contentTemp, content;
	std::vector<int> fieldIndex;
	std::vector<FXString> fieldName;
	FXchar delimiterField = ',';
	FXchar delimiterText = '"';
	FXwchar character;
	FXString field;
	FXint i, j, size;
	FXuint recordIdentifier;
	int mode;
	// mode = 0: at the beginning of a field, looking for text or delimiterText
	// mode = 1: inside field without a delimiterText
	// mode = 2: inside field with a delimiterText
	// mode = 3: potential end of field having found a delimiterText
	// mode = 4: end of field, process it and continue
	// mode = 5: end of field, process it and end line
	// mode = 6: error
	
	try{
		// open the file
		printf( "reading section from '%s'...\n", filename.text() );
		stream = new FXFileStream;
		if( ! stream ) THROW( dueOutOfMemory );
		stream->open( filename, FXStreamLoad );
		
		// read in the content in one go
		stream->position( 0, FXFromEnd );
		size = stream->position();
		contentTemp.assign( '\n', size );
		stream->position( 0, FXFromStart );
		stream->load( ( FXchar* )contentTemp.text(), size );
		
		// close the file
		printf( "closing file.\n" );
		delete stream;
		stream = NULL;
		
		content = dosToUnix( contentTemp );
		contentTemp = "";
		
		// read header line to determine the number of fields and to match them
		i = 0;
		mode = 0;
		field = "";
		size = content.length();
		while( i < size ){
			character = content.wc( i );
			i = content.inc( i );
			
			if( mode == 0 ){
				if( character == '\n' ){
					break;
					
				}else if( character == delimiterText ){
					mode = 2;
					
				}else if( character == delimiterField ){
					mode = 4;
					
				}else{
					field.append( &character, 1 );
					mode = 1;
				}
				
			}else if( mode == 1 ){
				if( character == '\n' ){
					mode = 5;
					
				}else if( character == delimiterField ){
					mode = 4;
					
				}else{
					field.append( &character, 1 );
				}
				
			}else if( mode == 2 ){
				if( character == '\n' ){
					THROW_INFO( dueInvalidFileFormat, "Premature newline found" );
					
				}else if( character == delimiterText ){
					mode = 3;
					
				}else{
					field.append( &character, 1 );
				}
				
			}else if( mode == 3 ){
				if( character == '\n' ){
					mode = 5;
					
				}else if( character == delimiterField ){
					mode = 4;
					
				}else if( character == delimiterText ){
					field.append( delimiterText );
					mode = 2;
					
				}else{
					field.append( &character, 1 );
				}
			}
			
			if( mode == 4 || mode == 5 ){
				fieldIndex.push_back( section.IndexOfTextNamed( field ) );
				field.substitute( "\\r", "\r" );
				field.substitute( "\\n", "\n" );
				field.substitute( "\\t", "\t" );
				fieldName.push_back( field );
				field = "";
				
				if( mode == 4 ){
					mode = 0;
					
				}else{
					break;
				}
			}
		}
		
		// we need at last the first field to be the id and another text field otherwise this is useless to us
		if( fieldName.size() < 2 ) THROW_INFO( dueInvalidFileFormat, "At least the ID field and one text field is required" );
		if( fieldName.at( 0 ) != section.GetHeaderIDName() ) THROW_INFO( dueInvalidFileFormat, "First field has to be the ID field" );
		
		// read in the records
		mode = 0;
		field = "";
		fieldName.clear();
		while( i < size ){
			character = content.wc( i );
			i = content.inc( i );
			
			if( mode == 0 ){
				if( character == '\n' ){
					mode = 5;
					
				}else if( character == delimiterText ){
					mode = 2;
					
				}else if( character == delimiterField ){
					mode = 4;
					
				}else{
					field.append( &character, 1 );
					mode = 1;
				}
				
			}else if( mode == 1 ){
				if( character == '\n' ){
					mode = 5;
					
				}else if( character == delimiterField ){
					mode = 4;
					
				}else{
					field.append( &character, 1 );
				}
				
			}else if( mode == 2 ){
				if( character == '\n' ){
					THROW_INFO( dueInvalidFileFormat, "Premature newline found" );
					
				}else if( character == delimiterText ){
					mode = 3;
					
				}else{
					field.append( &character, 1 );
				}
				
			}else if( mode == 3 ){
				if( character == '\n' ){
					mode = 5;
					
				}else if( character == delimiterField ){
					mode = 4;
					
				}else if( character == delimiterText ){
					field.append( delimiterText );
					mode = 2;
					
				}else{
					field.append( &character, 1 );
				}
			}
			
			if( mode == 4 || mode == 5 ){
				if( fieldName.size() == 0 && field.empty() ){
					continue;
				}
				
				fieldName.push_back( field );
				field = "";
				
				if( mode == 5 ){
					// add record if existing and replacement is matching
					recordIdentifier = FXUIntVal( fieldName.at( 0 ) );
					
					safeRecord = section.GetRecordWithID( recordIdentifier );
					
					if( ! safeRecord || replace ){
						if( ! safeRecord ){
							record = new dmotTranslationRecord;
							if( ! record ) THROW( dueOutOfMemory );
							
							record->SetIdentifier( recordIdentifier );
							
							safeRecord = record;
						}
						
						while( safeRecord->GetTextCount() < section.GetTextCount() ){
							safeRecord->AddText( FXString() );
						}
						
						for( j=1; j<(int)fieldIndex.size(); j++ ){
							if( fieldIndex.at( j ) != -1 ){
								fieldName.at( j ).substitute( "\\r", "\r" );
								fieldName.at( j ).substitute( "\\n", "\n" );
								fieldName.at( j ).substitute( "\\t", "\t" );
								safeRecord->SetTextAt( fieldIndex.at( j ), fieldName.at( j ) );
							}
						}
						
						if( record ){
							section.AddRecord( record );
							record = NULL;
						}
					}
					
					fieldName.clear();
				}
				mode = 0;
			}
		}
		
	}catch( duException ){
		if( record ) delete record;
		if( stream ) delete stream;
		throw;
	}
}

void dmotLoadSaveCSV::SaveToFile( const FXString &filename, const dmotTranslation &translation, const dmotTranslationFile &file, const dmotTranslationSection &section ){
	int r, recordCount = section.GetRecordCount();
	int t, textCount = section.GetTextCount();
	FXFileStream *stream = NULL;
	FXchar delimiterField = ',';
	FXchar delimiterText = '"';
	FXString findDelimiterText = "\"";
	FXString replaceDelimiterText = "\"\"";
	FXString text;
	FXString line;
	
	try{
		// open the file
		printf( "writing csv to '%s'...\n", filename.text() );
		stream = new FXFileStream;
		if( ! stream ) THROW( dueOutOfMemory );
		stream->open( filename, FXStreamSave );
		
		// write header fields
		line = "";
		
		text = section.GetHeaderIDName();
		text.substitute( "\r", "\\r" );
		text.substitute( "\n", "\\n" );
		text.substitute( "\t", "\\t" );
		if( text.contains( delimiterField ) > 0 ){
			text.substitute( findDelimiterText, replaceDelimiterText );
			text.prepend( delimiterText );
			text.append( delimiterText );
		}
		line.append( text );
		for( t=0; t<textCount; t++ ){
			const dmotTranslationSectionText &stext = *section.GetTextAt( t );
			
			line.append( delimiterField );
			text = stext.GetHeaderName();
			text.substitute( "\r", "\\r" );
			text.substitute( "\n", "\\n" );
			text.substitute( "\t", "\\t" );
			if( text.contains( delimiterField ) > 0 ){
				text.substitute( findDelimiterText, replaceDelimiterText );
				text.prepend( delimiterText );
				text.append( delimiterText );
			}
			line.append( text );
		}
		line.append( '\n' );
		stream->save( ( FXchar* )line.text(), ( FXuval )line.length() );
		
		// write records
		for( r=0; r<recordCount; r++ ){
			const dmotTranslationRecord &record = *section.GetRecordAt( r );
			
			line = "";
			
			text.format( "%u", record.GetIdentifier() );
			line.append( text );
			
			for( t=0; t<textCount; t++ ){
				line.append( delimiterField );
				text = record.GetTextAt( t );
				text.substitute( "\r", "\\r" );
				text.substitute( "\n", "\\n" );
				text.substitute( "\t", "\\t" );
				if( text.contains( delimiterField ) > 0 ){
					text.substitute( findDelimiterText, replaceDelimiterText );
					text.prepend( delimiterText );
					text.append( delimiterText );
				}
				line.append( text );
			}
			
			line.append( '\n' );
			stream->save( ( FXchar* )line.text(), ( FXuval )line.length() );
		}
		
		// close the file
		printf( "closing file.\n" );
		delete stream;
		
	}catch( duException ){
		if( stream ) delete stream;
		throw;
	}
}
