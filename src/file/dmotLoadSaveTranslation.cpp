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

#include "dmotLoadSaveTranslation.h"
#include "../translation/dmotTranslation.h"
#include "../translation/dmotTranslationFile.h"
#include "../translation/dmotTranslationSection.h"
#include "../translation/dmotTranslationSectionText.h"
#include "../translation/dmotTranslationRecord.h"
#include "../translation/dmotTranslationArchive.h"
#include "../exceptions.h"



// Class dmotLoadSaveTranslation
//////////////////////////////////

// Constructor, destructor
////////////////////////////

dmotLoadSaveTranslation::dmotLoadSaveTranslation(){
}

dmotLoadSaveTranslation::~dmotLoadSaveTranslation(){
}



// Management
///////////////

void dmotLoadSaveTranslation::LoadFromFile( const FXString &filename, dmotTranslation &translation ){
	FXchar fileSignature[] = "DMOTranslation=="; // 16 chars
	FXchar checkFileSignature[ 16 ];
	FXshort versionMajor, versionMinor;
	FXFileStream *stream = NULL;
	
	try{
		// open the file
		printf( "reading translation from '%s'...\n", filename.text() );
		stream = new FXFileStream;
		if( ! stream ) THROW( dueOutOfMemory );
		stream->open( filename, FXStreamLoad );
		
		// magic string
		stream->load( &checkFileSignature[ 0 ], 16 );
		if( compare( &checkFileSignature[ 0 ], &fileSignature[ 0 ], 16 ) != 0 ) THROW( dueInvalidFileFormat );
		
		// version
		*stream >> versionMajor;
		*stream >> versionMinor;
		
		if( versionMajor == 1 and versionMinor == 0 ){
			LoadFromFileV1p0( *stream, translation );
			
		}else if( versionMajor == 1 and versionMinor == 1 ){
			LoadFromFileV1p1( *stream, translation );
			
		}else if( versionMajor == 1 and versionMinor == 2 ){
			LoadFromFileV1p2( *stream, translation );
			
		}else if( versionMajor == 1 and versionMinor == 3 ){
			LoadFromFileV1p3( *stream, translation );
			
		}else{
			printf( "unrecognized version %i.%i\n", versionMajor, versionMinor );
			THROW( dueInvalidFileFormat );
		}
		
		// close the file
		printf( "closing file.\n" );
		delete stream;
		
	}catch( duException ){
		if( stream ) delete stream;
		throw;
	}
}

void dmotLoadSaveTranslation::LoadFromFileV1p0( FXFileStream &stream, dmotTranslation &translation ){
	dmotTranslationRecord *record = NULL;
	dmotTranslationSection *section = NULL;
	dmotTranslationSectionText *text = NULL;
	dmotTranslationArchive *archive = NULL;
	dmotTranslationFile *file = NULL;
	FXshort s, sectionCount;
	FXshort f, fileCount;
	FXint r, recordCount;
	FXshort t, textCount;
	FXshort valueShort;
	FXuint valueUInt;
	FXString string;
	FXint valueInt;
	
	archive = new dmotTranslationArchive;
	if( ! archive ) THROW( dueOutOfMemory );
	
	archive->SetPathArchive( "PackX.pf" );
	archive->SetPathFileMap( "PackX.hf" );
	
	translation.AddArchive( archive );
	
	// read translation files
	// short len(files)
	stream >> fileCount;
	for( f=0; f<fileCount; f++ ){
		file = new dmotTranslationFile;
		if( ! file ) THROW( dueOutOfMemory );
		
		file->SetIdentifier( 0 );
		
		// short len(name)
		// char[] name
		stream >> valueShort;
		string.assign( ' ', valueShort );
		stream.load( ( FXchar* )string.text(), valueShort );
		file->SetName( string );
		
		// short len(pathFile)
		// char[] pathFile
		stream >> valueShort;
		string.assign( ' ', valueShort );
		stream.load( ( FXchar* )string.text(), valueShort );
		//file->SetPathFile( string ); // no more supported
		
		// read sections
		// short len(sections)
		stream >> sectionCount;
		for( s=0; s<sectionCount; s++ ){
			section = new dmotTranslationSection;
			if( ! section ) THROW( dueOutOfMemory );
			
			// short len(name)
			// char[] name
			stream >> valueShort;
			string.assign( ' ', valueShort );
			stream.load( ( FXchar* )string.text(), valueShort );
			section->SetName( string );
			
			// int sectionPosition
			stream >> valueInt;
			section->SetSectionPosition( valueInt );
			
			// short len(headerIDName)
			// char[] headerIDName
			stream >> valueShort;
			string.assign( ' ', valueShort );
			stream.load( ( FXchar* )string.text(), valueShort );
			section->SetHeaderIDName( string );
			
			// short headerIDWidth
			stream >> valueShort;
			section->SetHeaderIDWidth( valueShort );
			
			// short idPosition
			stream >> valueShort;
			section->SetIdentifierPosition( valueShort );
			
			// short recordSize
			stream >> valueShort;
			section->SetRecordSize( valueShort );
			
			// read texts
			// short len(texts)
			stream >> textCount;
			for( t=0; t<textCount; t++ ){
				text = new dmotTranslationSectionText;
				if( ! text ) THROW( dueOutOfMemory );
				
				// short len(headerName)
				// char[] headerName
				stream >> valueShort;
				string.assign( ' ', valueShort );
				stream.load( ( FXchar* )string.text(), valueShort );
				text->SetHeaderName( string );
				
				// short headerWidth
				stream >> valueShort;
				text->SetHeaderWidth( valueShort );
				
				// short position
				stream >> valueShort;
				text->SetPosition( valueShort );
				
				// short length
				stream >> valueShort;
				text->SetLength( valueShort );
				
				// short format
				stream >> valueShort;
				text->SetFormat( valueShort );
				
				section->AddText( text );
				text = NULL;
			}
			
			// read records
			// int len(records)
			stream >> recordCount;
			for( r=0; r<recordCount; r++ ){
				record = new dmotTranslationRecord;
				if( ! record ) THROW( dueOutOfMemory );
				
				// int recordIdentifier
				stream >> valueUInt;
				record->SetIdentifier( valueUInt );
				
				// read texts. count not required since it is the same as in the file
				for( t=0; t<textCount; t++ ){
					// short len(text[i])
					// char[] text[i]
					stream >> valueShort;
					string.assign( ' ', valueShort );
					stream.load( ( FXchar* )string.text(), valueShort );
					record->AddText( string );
				}
				
				section->AddRecord( record );
				record = NULL;
			}
			
			file->AddSection( section );
			section = NULL;
		}
		
		archive->AddFile( file );
		file = NULL;
	}
}

void dmotLoadSaveTranslation::LoadFromFileV1p1( FXFileStream &stream, dmotTranslation &translation ){
	dmotTranslationRecord *record = NULL;
	dmotTranslationSection *section = NULL;
	dmotTranslationSectionText *text = NULL;
	dmotTranslationFile *file = NULL;
	dmotTranslationArchive *archive = NULL;
	FXshort s, sectionCount;
	FXshort f, fileCount;
	FXint r, recordCount;
	FXshort t, textCount;
	FXshort valueShort;
	FXuint valueUInt;
	FXString string;
	FXint valueInt;
	
	archive = new dmotTranslationArchive;
	if( ! archive ) THROW( dueOutOfMemory );
	
	archive->SetPathArchive( "PackX.pf" );
	archive->SetPathFileMap( "PackX.hf" );
	
	translation.AddArchive( archive );
	
	// read translation files
	// short len(files)
	stream >> fileCount;
	for( f=0; f<fileCount; f++ ){
		file = new dmotTranslationFile;
		if( ! file ) THROW( dueOutOfMemory );
		
		file->SetIdentifier( 0 );
		
		// short len(name)
		// char[] name
		stream >> valueShort;
		string.assign( ' ', valueShort );
		stream.load( ( FXchar* )string.text(), valueShort );
		file->SetName( string );
		
		// short len(pathFile)
		// char[] pathFile
		stream >> valueShort;
		string.assign( ' ', valueShort );
		stream.load( ( FXchar* )string.text(), valueShort );
		//file->SetPathFile( string ); // no more supported
		
		// read sections
		// short len(sections)
		stream >> sectionCount;
		for( s=0; s<sectionCount; s++ ){
			section = new dmotTranslationSection;
			if( ! section ) THROW( dueOutOfMemory );
			
			// short len(name)
			// char[] name
			stream >> valueShort;
			string.assign( ' ', valueShort );
			stream.load( ( FXchar* )string.text(), valueShort );
			section->SetName( string );
			
			// int sectionPosition
			stream >> valueInt;
			section->SetSectionPosition( valueInt );
			
			// short len(headerIDName)
			// char[] headerIDName
			stream >> valueShort;
			string.assign( ' ', valueShort );
			stream.load( ( FXchar* )string.text(), valueShort );
			section->SetHeaderIDName( string );
			
			// short headerIDWidth
			stream >> valueShort;
			section->SetHeaderIDWidth( valueShort );
			
			// short idPosition
			stream >> valueShort;
			section->SetIdentifierPosition( valueShort );
			
			// short recordSize
			stream >> valueShort;
			section->SetRecordSize( valueShort );
			
			// short paddingType
			stream >> valueShort;
			section->SetPaddingType( valueShort );
			
			// short paddingCount
			stream >> valueShort;
			section->SetPaddingCount( valueShort );
			
			// read texts
			// short len(texts)
			stream >> textCount;
			for( t=0; t<textCount; t++ ){
				text = new dmotTranslationSectionText;
				if( ! text ) THROW( dueOutOfMemory );
				
				// short len(headerName)
				// char[] headerName
				stream >> valueShort;
				string.assign( ' ', valueShort );
				stream.load( ( FXchar* )string.text(), valueShort );
				text->SetHeaderName( string );
				
				// short headerWidth
				stream >> valueShort;
				text->SetHeaderWidth( valueShort );
				
				// short position
				stream >> valueShort;
				text->SetPosition( valueShort );
				
				// short length
				stream >> valueShort;
				text->SetLength( valueShort );
				
				// short format
				stream >> valueShort;
				text->SetFormat( valueShort );
				
				section->AddText( text );
				text = NULL;
			}
			
			// read records
			// int len(records)
			stream >> recordCount;
			for( r=0; r<recordCount; r++ ){
				record = new dmotTranslationRecord;
				if( ! record ) THROW( dueOutOfMemory );
				
				// int recordIdentifier
				stream >> valueUInt;
				record->SetIdentifier( valueUInt );
				
				// read texts. count not required since it is the same as in the file
				for( t=0; t<textCount; t++ ){
					// short len(text[i])
					// char[] text[i]
					stream >> valueShort;
					string.assign( ' ', valueShort );
					stream.load( ( FXchar* )string.text(), valueShort );
					record->AddText( string );
				}
				
				section->AddRecord( record );
				record = NULL;
			}
			
			file->AddSection( section );
			section = NULL;
		}
		
		archive->AddFile( file );
		file = NULL;
	}
}

void dmotLoadSaveTranslation::LoadFromFileV1p2( FXFileStream &stream, dmotTranslation &translation ){
	dmotTranslationRecord *record = NULL;
	dmotTranslationSection *section = NULL;
	dmotTranslationSectionText *text = NULL;
	dmotTranslationFile *file = NULL;
	dmotTranslationArchive *archive = NULL;
	FXshort s, sectionCount;
	FXshort a, archiveCount;
	FXshort f, fileCount;
	FXint r, recordCount;
	FXshort t, textCount;
	FXshort valueShort;
	FXuint valueUInt;
	FXString string;
	FXint valueInt;
	
	// read translation archives
	stream >> archiveCount;
	for( a=0; a<archiveCount; a++ ){
		archive = new dmotTranslationArchive;
		if( ! archive ) THROW( dueOutOfMemory );
		
		// short len(name)
		// char[] name
		stream >> valueShort;
		string.assign( ' ', valueShort );
		stream.load( ( FXchar* )string.text(), valueShort );
		archive->SetName( string );
		
		// short len(pathArchive)
		// char[] pathArchive
		stream >> valueShort;
		string.assign( ' ', valueShort );
		stream.load( ( FXchar* )string.text(), valueShort );
		archive->SetPathArchive( string );
		
		// short len(pathFileMap)
		// char[] pathFileMap
		stream >> valueShort;
		string.assign( ' ', valueShort );
		stream.load( ( FXchar* )string.text(), valueShort );
		archive->SetPathFileMap( string );
		
		// read translation files
		// short len(files)
		stream >> fileCount;
		for( f=0; f<fileCount; f++ ){
			file = new dmotTranslationFile;
			if( ! file ) THROW( dueOutOfMemory );
			
			// short len(name)
			// char[] name
			stream >> valueShort;
			string.assign( ' ', valueShort );
			stream.load( ( FXchar* )string.text(), valueShort );
			file->SetName( string );
			
			// uint identifier
			stream >> valueUInt;
			file->SetIdentifier( valueUInt );
			
			// read sections
			// short len(sections)
			stream >> sectionCount;
			for( s=0; s<sectionCount; s++ ){
				section = new dmotTranslationSection;
				if( ! section ) THROW( dueOutOfMemory );
				
				// short len(name)
				// char[] name
				stream >> valueShort;
				string.assign( ' ', valueShort );
				stream.load( ( FXchar* )string.text(), valueShort );
				section->SetName( string );
				
				// int sectionPosition
				stream >> valueInt;
				section->SetSectionPosition( valueInt );
				
				// short len(headerIDName)
				// char[] headerIDName
				stream >> valueShort;
				string.assign( ' ', valueShort );
				stream.load( ( FXchar* )string.text(), valueShort );
				section->SetHeaderIDName( string );
				
				// short headerIDWidth
				stream >> valueShort;
				section->SetHeaderIDWidth( valueShort );
				
				// short idPosition
				stream >> valueShort;
				section->SetIdentifierPosition( valueShort );
				
				// short recordSize
				stream >> valueShort;
				section->SetRecordSize( valueShort );
				
				// short paddingType
				stream >> valueShort;
				section->SetPaddingType( valueShort );
				
				// short paddingCount
				stream >> valueShort;
				section->SetPaddingCount( valueShort );
				
				// read texts
				// short len(texts)
				stream >> textCount;
				for( t=0; t<textCount; t++ ){
					text = new dmotTranslationSectionText;
					if( ! text ) THROW( dueOutOfMemory );
					
					// short len(headerName)
					// char[] headerName
					stream >> valueShort;
					string.assign( ' ', valueShort );
					stream.load( ( FXchar* )string.text(), valueShort );
					text->SetHeaderName( string );
					
					// short headerWidth
					stream >> valueShort;
					text->SetHeaderWidth( valueShort );
					
					// short position
					stream >> valueShort;
					text->SetPosition( valueShort );
					
					// short length
					stream >> valueShort;
					text->SetLength( valueShort );
					
					// short format
					stream >> valueShort;
					text->SetFormat( valueShort );
					
					section->AddText( text );
					text = NULL;
				}
				
				// read records
				// int len(records)
				stream >> recordCount;
				for( r=0; r<recordCount; r++ ){
					record = new dmotTranslationRecord;
					if( ! record ) THROW( dueOutOfMemory );
					
					// uint recordIdentifier
					stream >> valueUInt;
					record->SetIdentifier( valueUInt );
					
					// read texts. count not required since it is the same as in the file
					for( t=0; t<textCount; t++ ){
						// short len(text[i])
						// char[] text[i]
						stream >> valueShort;
						string.assign( ' ', valueShort );
						stream.load( ( FXchar* )string.text(), valueShort );
						record->AddText( string );
					}
					
					section->AddRecord( record );
					record = NULL;
				}
				
				file->AddSection( section );
				section = NULL;
			}
			
			archive->AddFile( file );
			file = NULL;
		}
		
		translation.AddArchive( archive );
		archive = NULL;
	}
}

void dmotLoadSaveTranslation::LoadFromFileV1p3( FXFileStream &stream, dmotTranslation &translation ){
	dmotTranslationRecord *record = NULL;
	dmotTranslationSection *section = NULL;
	dmotTranslationSectionText *text = NULL;
	dmotTranslationFile *file = NULL;
	dmotTranslationArchive *archive = NULL;
	FXshort s, sectionCount;
	FXshort a, archiveCount;
	FXshort f, fileCount;
	FXint r, recordCount;
	FXshort t, textCount;
	FXshort valueShort;
	FXuint valueUInt;
	FXString string;
	FXint valueInt;
	
	// read translation archives
	stream >> archiveCount;
	for( a=0; a<archiveCount; a++ ){
		archive = new dmotTranslationArchive;
		if( ! archive ) THROW( dueOutOfMemory );
		
		// short len(name)
		// char[] name
		stream >> valueShort;
		string.assign( ' ', valueShort );
		stream.load( ( FXchar* )string.text(), valueShort );
		archive->SetName( string );
		
		// short len(pathArchive)
		// char[] pathArchive
		stream >> valueShort;
		string.assign( ' ', valueShort );
		stream.load( ( FXchar* )string.text(), valueShort );
		archive->SetPathArchive( string );
		
		// short len(pathFileMap)
		// char[] pathFileMap
		stream >> valueShort;
		string.assign( ' ', valueShort );
		stream.load( ( FXchar* )string.text(), valueShort );
		archive->SetPathFileMap( string );
		
		// read translation files
		// short len(files)
		stream >> fileCount;
		for( f=0; f<fileCount; f++ ){
			file = new dmotTranslationFile;
			if( ! file ) THROW( dueOutOfMemory );
			
			// short len(name)
			// char[] name
			stream >> valueShort;
			string.assign( ' ', valueShort );
			stream.load( ( FXchar* )string.text(), valueShort );
			file->SetName( string );
			
			// uint identifier
			stream >> valueUInt;
			file->SetIdentifier( valueUInt );
			
			// read sections
			// short len(sections)
			stream >> sectionCount;
			for( s=0; s<sectionCount; s++ ){
				section = new dmotTranslationSection;
				if( ! section ) THROW( dueOutOfMemory );
				
				// short len(name)
				// char[] name
				stream >> valueShort;
				string.assign( ' ', valueShort );
				stream.load( ( FXchar* )string.text(), valueShort );
				section->SetName( string );
				
				// int sectionPosition
				stream >> valueInt;
				section->SetSectionPosition( valueInt );
				
				// short len(headerIDName)
				// char[] headerIDName
				stream >> valueShort;
				string.assign( ' ', valueShort );
				stream.load( ( FXchar* )string.text(), valueShort );
				section->SetHeaderIDName( string );
				
				// short headerIDWidth
				stream >> valueShort;
				section->SetHeaderIDWidth( valueShort );
				
				// short idPosition
				stream >> valueShort;
				section->SetIdentifierPosition( valueShort );
				
				// short idType
				stream >> valueShort;
				section->SetIdentifierType( valueShort );
				
				// short recordSize
				stream >> valueShort;
				section->SetRecordSize( valueShort );
				
				// short paddingType
				stream >> valueShort;
				section->SetPaddingType( valueShort );
				
				// short paddingCount
				stream >> valueShort;
				section->SetPaddingCount( valueShort );
				
				// read texts
				// short len(texts)
				stream >> textCount;
				for( t=0; t<textCount; t++ ){
					text = new dmotTranslationSectionText;
					if( ! text ) THROW( dueOutOfMemory );
					
					// short len(headerName)
					// char[] headerName
					stream >> valueShort;
					string.assign( ' ', valueShort );
					stream.load( ( FXchar* )string.text(), valueShort );
					text->SetHeaderName( string );
					
					// short headerWidth
					stream >> valueShort;
					text->SetHeaderWidth( valueShort );
					
					// short position
					stream >> valueShort;
					text->SetPosition( valueShort );
					
					// short length
					stream >> valueShort;
					text->SetLength( valueShort );
					
					// short format
					stream >> valueShort;
					text->SetFormat( valueShort );
					
					section->AddText( text );
					text = NULL;
				}
				
				// read records
				// int len(records)
				stream >> recordCount;
				for( r=0; r<recordCount; r++ ){
					record = new dmotTranslationRecord;
					if( ! record ) THROW( dueOutOfMemory );
					
					// uint recordIdentifier
					stream >> valueUInt;
					record->SetIdentifier( valueUInt );
					
					// read texts. count not required since it is the same as in the file
					for( t=0; t<textCount; t++ ){
						// short len(text[i])
						// char[] text[i]
						stream >> valueShort;
						string.assign( ' ', valueShort );
						stream.load( ( FXchar* )string.text(), valueShort );
						record->AddText( string );
					}
					
					section->AddRecord( record );
					record = NULL;
				}
				
				file->AddSection( section );
				section = NULL;
			}
			
			archive->AddFile( file );
			file = NULL;
		}
		
		translation.AddArchive( archive );
		archive = NULL;
	}
}



void dmotLoadSaveTranslation::SaveToFile( const FXString &filename, const dmotTranslation &translation ){
	FXshort a, archiveCount = translation.GetArchiveCount();
	FXchar fileSignature[] = "DMOTranslation=="; // 16 chars
	FXFileStream *stream = NULL;
	FXshort s, sectionCount;
	FXshort f, fileCount;
	FXint r, recordCount;
	FXshort t, textCount;
	FXshort valueShort;
	FXuint valueUInt;
	FXint valueInt;
	
	try{
		// open the file
		printf( "writing translation to '%s'...\n", filename.text() );
		stream = new FXFileStream;
		if( ! stream ) THROW( dueOutOfMemory );
		stream->open( filename, FXStreamSave );
		
		// magic string
		stream->save( &fileSignature[ 0 ], 16 );
		
		// version just for the case we change the formal later on
		valueShort = 1;
		*stream << valueShort;
		valueShort = 3;
		*stream << valueShort;
		
		// write translation archives
		// short len(archives)
		*stream << archiveCount;
		for( a=0; a<archiveCount; a++ ){
			const dmotTranslationArchive &archive = *translation.GetArchiveAt( a );
			
			// short len(name)
			// char[] name
			valueShort = archive.GetName().length();
			*stream << valueShort;
			stream->save( archive.GetName().text(), valueShort );
			
			// short len(pathArchive)
			// char[] pathArchive
			valueShort = archive.GetPathArchive().length();
			*stream << valueShort;
			stream->save( archive.GetPathArchive().text(), valueShort );
			
			// short len(pathFileMap)
			// char[] pathFileMap
			valueShort = archive.GetPathFileMap().length();
			*stream << valueShort;
			stream->save( archive.GetPathFileMap().text(), valueShort );
			
			// write translation files
			// short len(files)
			fileCount = ( FXshort )archive.GetFileCount();
			*stream << fileCount;
			for( f=0; f<fileCount; f++ ){
				const dmotTranslationFile &file = *archive.GetFileAt( f );
				
				// short len(name)
				// char[] name
				valueShort = file.GetName().length();
				*stream << valueShort;
				stream->save( file.GetName().text(), valueShort );
				
				// uint identifier
				valueUInt = file.GetIdentifier();
				*stream << valueUInt;
				
				// write sections
				// short len(sections)
				sectionCount = file.GetSectionCount();
				*stream << sectionCount;
				for( s=0; s<sectionCount; s++ ){
					const dmotTranslationSection &section = *file.GetSectionAt( s );
					
					// short len(name)
					// char[] name
					valueShort = section.GetName().length();
					*stream << valueShort;
					stream->save( section.GetName().text(), valueShort );
					
					// int sectionPosition
					valueInt = section.GetSectionPosition();
					*stream << valueInt;
					
					// short len(headerIDName)
					// char[] headerIDName
					valueShort = section.GetHeaderIDName().length();
					*stream << valueShort;
					stream->save( section.GetHeaderIDName().text(), valueShort );
					
					// short headerIDWidth
					valueShort = ( FXshort )section.GetHeaderIDWidth();
					*stream << valueShort;
					
					// short idPosition
					valueShort = ( FXshort )section.GetIdentifierPosition();
					*stream << valueShort;
					
					// short idType
					valueShort = ( FXshort )section.GetIdentifierType();
					*stream << valueShort;
					
					// short recordSize
					valueShort = section.GetRecordSize();
					*stream << valueShort;
					
					// short paddingType
					valueShort = section.GetPaddingType();
					*stream << valueShort;
					
					// short paddingCount
					valueShort = section.GetPaddingCount();
					*stream << valueShort;
					
					// write texts
					// short len(texts)
					textCount = section.GetTextCount();
					*stream << textCount;
					for( t=0; t<textCount; t++ ){
						const dmotTranslationSectionText &text = *section.GetTextAt( t );
						
						// short len(headerName)
						// char[] headerName
						valueShort = text.GetHeaderName().length();
						*stream << valueShort;
						stream->save( text.GetHeaderName().text(), valueShort );
						
						// short headerWidth
						valueShort = text.GetHeaderWidth();
						*stream << valueShort;
						
						// short position
						valueShort = text.GetPosition();
						*stream << valueShort;
						
						// short length
						valueShort = text.GetLength();
						*stream << valueShort;
						
						// short format
						valueShort = text.GetFormat();
						*stream << valueShort;
					}
					
					// write records
					// int len(records)
					recordCount = section.GetRecordCount();
					*stream << recordCount;
					for( r=0; r<recordCount; r++ ){
						const dmotTranslationRecord &record = *section.GetRecordAt( r );
						
						// int recordIdentifier
						valueUInt = record.GetIdentifier();
						*stream << valueUInt;
						
						// write texts. count not required since it is the same as in the file
						for( t=0; t<textCount; t++ ){
							const FXString &text = record.GetTextAt( t );
							
							// short len(text[i])
							// char[] text[i]
							valueShort = text.length();
							*stream << valueShort;
							stream->save( text.text(), valueShort );
						}
					}
				}
			}
		}
		
		// close the file
		printf( "closing file.\n" );
		delete stream;
		
	}catch( duException ){
		if( stream ) delete stream;
		throw;
	}
}
