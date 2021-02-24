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

#include "dmotReadBinFile.h"
#include "dmotTranslateStatus.h"
#include "dmotTranslateStatusEntry.h"
#include "../config/dmotConfig.h"
#include "../translation/dmotTranslation.h"
#include "../translation/dmotTranslationFile.h"
#include "../translation/dmotTranslationSection.h"
#include "../translation/dmotTranslationSectionText.h"
#include "../translation/dmotTranslationRecord.h"
#include "../translation/dmotTranslationArchive.h"
#include "../exceptions.h"

//#define DEBUG_ENABLED 1



// Functors
/////////////

struct fEntryEqualID{
	FXuint pID;
	
	fEntryEqualID( FXuint id ){
		pID = id;
	}
	
	bool operator()( dmotReadBinFile::sFileMapEntry &entry ){
		return entry.identifier == pID;
	}
};



// Class dmotReadBinFile
//////////////////////////

// Constructor, destructor
////////////////////////////

dmotReadBinFile::dmotReadBinFile(){
	pTextBufferWC = NULL;
	pTextBufferWCLen = 0;
	pTextBufferUTF8 = NULL;
	pTextBufferUTF8Len = 0;
}

dmotReadBinFile::~dmotReadBinFile(){
	if( pTextBufferWC ) delete [] pTextBufferWC;
	if( pTextBufferUTF8 ) delete [] pTextBufferUTF8;
}



// Management
///////////////

void dmotReadBinFile::ReadFile( dmotTranslateStatus &status, dmotConfig &config, dmotTranslationArchive &archive, dmotTranslationFile &file, bool replace ){
	const FXString &pathDMO = config.GetPathDMO();
	const FXString &pathFile = archive.GetPathArchive();
	const FXString pathAbsolute( FXPath::absolute( pathDMO, pathFile ) );
	std::vector<sFileMapEntry>::const_iterator iterFileMap;
	int s, sectionCount = file.GetSectionCount();
	dmotTranslationRecord *record = NULL;
	std::vector<sFileMapEntry> fileMap;
	dmotTranslationRecord *safeRecord;
	FXFileStream *stream = NULL;
	FXint r, recordCount = 0;
	FXUTF16Codec utf16coded;
	FXint paddingCheckValue;
	FXuint recordIdentifier;
	FXushort recordIdentifierUShort;
	FXString recordString;
	int paddingCheckCount;
	FXint t, textCount;
	int recordSize = 0;
	FXuint fileEndPos;
	FXuint curFilePos;
	int recordOffset;
	int paddingCount;
	int paddingType;
	int textFormat;
	int bufferLen;
	FXint b; //, blockEntryCount;
	
	// if the file has a 0 identifier it is skipped. this is the same as an empty file with no records read
	if( file.GetIdentifier() == 0 ){
		status.GetEntryWith( &archive, &file )->AppendText( FXString( "File has 0 identifier (skipped)." ) );
		return;
	}
	
	// read the file map so we know where the individual files are
	if( ! ReadFileMap( status, config, archive, fileMap ) ){
		return;
	}
	
	// read the file if it exists in the file map
	iterFileMap = std::find_if( fileMap.begin(), fileMap.end(), fEntryEqualID( file.GetIdentifier() ) );
	if( iterFileMap == fileMap.end() ){
		status.GetEntryWith( &archive, &file )->AppendText( FXString( "File not found in file map (skipped)." ) );
		return;
	}
	
	const sFileMapEntry &fileMapEntry = *iterFileMap;
	
	try{
		// open the archive file
		printf( "reading archive file '%s'...\n", pathAbsolute.text() );
		stream = new FXFileStream;
		if( ! stream ) THROW( dueOutOfMemory );
		if( ! stream->open( pathAbsolute, FXStreamLoad ) ) THROW( dueFileNotFound );
		
		curFilePos = fileMapEntry.offset; // move to the start of the file in the archive
		fileEndPos = fileMapEntry.offset + fileMapEntry.size;
		
		// process all sections
		for( s=0; s<sectionCount; s++ ){
			if( curFilePos >= fileEndPos ){
				break; // deal with buggy files claiming more entries than are actually stored
			}
			
			dmotTranslationSection &section = *file.GetSectionAt( s );
			recordSize = section.GetRecordSize();
			printf( "processing section '%s'...\n", section.GetName().text() );
			
			// read the number of records
			curFilePos += section.GetSectionPosition();
			stream->position( curFilePos );
			*stream >> recordCount;
			printf( "number of records = %i\n", recordCount );
			if( recordCount > 100000 ) THROW( dueInvalidFileFormat );
			
			curFilePos += sizeof( recordCount );
			
			paddingType = section.GetPaddingType();
			paddingCount = section.GetPaddingCount();
			
			// read in all records
			for( r=0; r<recordCount; r++ ){
				if( curFilePos >= fileEndPos ){
					break; // deal with buggy files claiming more entries than are actually stored
				}
				
				recordOffset = curFilePos; //4 + recordSize * r;
				
				stream->position( recordOffset + section.GetIdentifierPosition() );
				if( section.GetIdentifierType() == dmotTranslationSection::eidtUShort ){
					*stream >> recordIdentifierUShort;
					recordIdentifier = ( FXuint )recordIdentifierUShort;
					
				}else{
					*stream >> recordIdentifier;
				}
#ifdef DEBUG_ENABLED
				printf( "debug: record %i, identifier %u, offset %i\n", r, recordIdentifier, curFilePos - fileMapEntry.offset );
#endif
				
				safeRecord = section.GetRecordWithID( recordIdentifier );
				
				if( ! safeRecord || replace ){
					if( ! safeRecord ){
						record = new dmotTranslationRecord;
						if( ! record ) THROW( dueOutOfMemory );
						
						record->SetIdentifier( recordIdentifier );
						
						safeRecord = record;
					}
					
					textCount = section.GetTextCount();
					for( t=0; t<textCount; t++ ){
						const dmotTranslationSectionText &secText = *section.GetTextAt( t );
						textFormat = secText.GetFormat();
						
						if( textFormat == dmotTranslationSectionText::etfWideChar ){
							bufferLen = secText.GetLength();
							PrepareBufferWC( bufferLen );
							
							stream->position( recordOffset + secText.GetPosition() );
							stream->load( ( FXushort* )pTextBufferWC, bufferLen );
							
							ConvertBufferWC( recordString );
							
							if( safeRecord->GetTextCount() == t ){
								safeRecord->AddText( recordString );
								
							}else{
								safeRecord->SetTextAt( t, recordString );
							}
							
						}else if( textFormat == dmotTranslationSectionText::etfUTF8 ){
							bufferLen = secText.GetLength();
							PrepareBufferUTF8( bufferLen );
							
							stream->position( recordOffset + secText.GetPosition() );
							stream->load( pTextBufferUTF8, bufferLen );
							
							if( safeRecord->GetTextCount() == t ){
								safeRecord->AddText( pTextBufferUTF8 );
								
							}else{
								safeRecord->SetTextAt( t, pTextBufferUTF8 );
							}
							
						}else if( textFormat == dmotTranslationSectionText::etf0TermWideChar){
							bufferLen = secText.GetLength() + 1;
							PrepareBufferWC( bufferLen );
							
							stream->position( recordOffset + secText.GetPosition() );
							stream->load( ( FXushort* )pTextBufferWC, bufferLen );
							
							ConvertBufferWC( recordString );
							
							if( recordString.count() > secText.GetLength() ) THROW( dueInvalidParam );
							
							if( safeRecord->GetTextCount() == t ){
								safeRecord->AddText( recordString );
								
							}else{
								safeRecord->SetTextAt( t, recordString );
							}
							
						}else if( textFormat == dmotTranslationSectionText::etf0TermUTF8){
							bufferLen = secText.GetLength() + 1;
							PrepareBufferUTF8( bufferLen );
							
							stream->position( recordOffset + secText.GetPosition() );
							stream->load( pTextBufferUTF8, bufferLen );
							if( ( int )strlen( pTextBufferUTF8 ) > secText.GetLength() ) THROW( dueInvalidParam );
							
							if( safeRecord->GetTextCount() == t ){
								safeRecord->AddText( pTextBufferUTF8 );
								
							}else{
								safeRecord->SetTextAt( t, pTextBufferUTF8 );
							}
						}
					}
					
					if( record ){
						section.AddRecord( record );
						record = NULL;
					}
				}
				
				curFilePos += recordSize;
				
				if( r == recordCount - 1 ){
					// there seems to be a bug in the FOX file reader reading wrong
					// values. to prevent problems we skip the padding in the last
					// record as it should reach the end of file anyways
					break;
				}
				
				if( paddingType == dmotTranslationSection::eptSequenceInt32 ){
					stream->position( curFilePos );
					
					paddingCheckCount = 0;
					
					while( paddingCheckCount < paddingCount ){
						*stream >> paddingCheckValue;
						curFilePos += sizeof( paddingCheckValue );
						
						if( paddingCheckValue == 0 ){
							paddingCheckCount++;
							
						}else{
							paddingCheckCount = 0;
						}
					}
					
				}else if( paddingType == dmotTranslationSection::eptSpecialNPC ){
					stream->position( curFilePos );
					*stream >> paddingCheckValue;
					if( paddingCheckValue < paddingCount ){
						curFilePos += sizeof( paddingCheckValue ) * ( 1 + ( int )paddingCheckValue );
						stream->position( curFilePos );
						*stream >> paddingCheckValue;
					}
					
					if( paddingCheckValue < paddingCount ){
						curFilePos += sizeof( paddingCheckValue ) * ( 1 + ( int )paddingCheckValue );
						stream->position( curFilePos );
						*stream >> paddingCheckValue;
					}
					
					int betterSafeThanSorry = 0;
					while( paddingCheckValue < paddingCount ){
						if( betterSafeThanSorry++ == 100 ) THROW( dueInvalidParam );
						curFilePos += sizeof( paddingCheckValue );
						stream->position( curFilePos );
						*stream >> paddingCheckValue;
					}
					
				}else if( paddingType == dmotTranslationSection::eptSpecialQuest ){
#ifdef DEBUG_ENABLED
					printf( "special padding 1: offset %i, record %u\n", curFilePos - fileMapEntry.offset, recordIdentifier );
#endif
#if 0
					// dummy
					stream->position( curFilePos );
					*stream >> blockEntryCount;
#ifdef DEBUG_ENABLED
					printf( "   dummy: value=%i\n", blockEntryCount );
#endif
					if( blockEntryCount == 0 ){
						curFilePos += sizeof( FXint );
					}
					
					// run of 3-int blocks
					stream->position( curFilePos );
					*stream >> blockEntryCount;
#ifdef DEBUG_ENABLED
					printf( "   block of 3-int entries: count=%i\n", blockEntryCount );
#endif
					curFilePos += sizeof( FXint ) * ( 1 + 3 * blockEntryCount );
					
					// two runs of 4-int blocks
					for( b=0; b<2; b++ ){
						stream->position( curFilePos );
						*stream >> blockEntryCount;
#ifdef DEBUG_ENABLED
						printf( "   block of 4-int entries: count=%i\n", blockEntryCount );
#endif
						curFilePos += sizeof( FXint ) * ( 1 + 4 * blockEntryCount );
					}
					
					// run of 1-int blocks (should be all 0)
					stream->position( curFilePos );
					*stream >> paddingCheckCount;
#ifdef DEBUG_ENABLED
					printf( "   padding of ints: count=%i\n", paddingCheckCount );
#endif
					curFilePos += sizeof( FXint ) * ( 1 + paddingCheckCount );
#endif
					// locate a run of 5 ints where the first contains 4 and all others 0 as value
					b = 0;
					while( b < 4 ){
						paddingCheckValue = 0;
						while( paddingCheckValue != 4 ){
							stream->position( curFilePos );
							*stream >> paddingCheckValue;
							curFilePos += sizeof( paddingCheckValue );
						}
						for( b=0; b<4; b++ ){
							stream->position( curFilePos );
							*stream >> paddingCheckValue;
							curFilePos += sizeof( paddingCheckValue );
							if( paddingCheckValue != 0 ){
								break;
							}
						}
					}
					
				}else if( paddingType == dmotTranslationSection::eptBlocksInt32 ){
					if( paddingCount == 0 ){
						paddingCheckValue = 1;
						while( paddingCheckValue > 0 ){
							stream->position( curFilePos );
							*stream >> paddingCheckValue;
							if( paddingCheckValue < 0 || paddingCheckValue > 100 ){
								THROW( deeInvalidFormat );
							}
							curFilePos += sizeof( paddingCheckValue ) * ( 1 + ( int )paddingCheckValue );
						}
						
					}else{
						for( paddingCheckCount=0; paddingCheckCount<paddingCount; paddingCheckCount++ ){
							stream->position( curFilePos );
							*stream >> paddingCheckValue;
							if( paddingCheckValue < 0 || paddingCheckValue > 100 ){
								THROW( deeInvalidFormat );
							}
							curFilePos += sizeof( paddingCheckValue ) * ( 1 + ( int )paddingCheckValue );
						}
					}
				}
			}
		}
		
		// close the file
		printf( "closing archive file.\n" );
		delete stream;
		
		status.GetEntryWith( &archive, &file )->SetText( FXString( "File read successfully." ) );
		
	}catch( duException e ){
		if( record ) delete record;
		if( stream ) delete stream;
		FXString text;
		text.format( "Failed to process file (archive file '%s').", pathAbsolute.text() );
		status.GetEntryWith( &archive, &file )->AppendTextException( text, e );
	}
}

void dmotReadBinFile::WriteFile( dmotTranslateStatus &status, dmotConfig &config, const dmotTranslationArchive &archive ){
	const int fileCount = archive.GetFileCount();
	const FXString &pathDMO = config.GetPathDMO();
	const FXString &pathFile = archive.GetPathArchive(); // TODO file.GetPathFile();
	const FXString pathAbsolute( FXPath::absolute( pathDMO, pathFile ) );
	std::vector<sFileMapEntry>::const_iterator iterFileMap;
	std::vector<sFileMapEntry> fileMap;
	float entryPassPercentage = 0.75f;
	dmotTranslationRecord *record;
	int requiredUpdateEntryCount;
	FXFileStream *stream = NULL;
	FXint paddingCheckValue;
	FXchar *archiveData = NULL;
	FXchar *fileData = NULL;
	FXuint recordIdentifier;
	FXushort recordIdentifierUShort;
	int paddingCheckCount;
	int updatedEntryCount;
	bool fileUpdateValid;
	FXint r, recordCount;
	FXint t, textCount;
	FXuint curFilePos;
	FXuint fileEndPos;
	int recordOffset;
	int fileSize = 0;
	int paddingCount;
	int paddingType;
	int recordSize;
	int textFormat;
	int textLen;
	int f, s;
	FXint b; //, blockEntryCount;
	
	if( ! ReadFileMap( status, config, archive, fileMap ) ){
		return;
	}
	
	try{
		// make copy, overwrite the old file
		printf( "copy '%s' to '%s'...\n", pathFile.text(), ( pathFile + ".bak" ).text() );
		if( ! FXFile::copy( pathAbsolute, pathAbsolute + ".bak", TRUE ) ) THROW( dueWriteFile );
		
		printf( "patching '%s'...\n", pathFile.text() );
		
		// create buffer and read the entire file into memory
		stream = new FXFileStream;
		if( ! stream ) THROW( dueOutOfMemory );
		if( ! stream->open( pathAbsolute, FXStreamLoad ) ) THROW( dueFileNotFound );
		
		stream->position( 0, FXFromEnd );
		fileSize = ( int )stream->position();
		stream->position( 0 );
		
		archiveData = new FXchar[ fileSize ];
		if( ! archiveData ) THROW( dueOutOfMemory );
		
		stream->load( archiveData, fileSize );
		
		delete stream;
		stream = NULL;
		
	}catch( duException e ){
		if( archiveData ) delete [] archiveData;
		if( stream ) delete stream;
		FXString text;
		text.format( "Reading in archive failed (archive file '%s').", pathAbsolute.text() );
		status.GetEntryWith( &archive, NULL )->AppendTextException( text, e );
		return;
	}
	
	for( f=0; f<fileCount; f++ ){
		const dmotTranslationFile &file = *archive.GetFileAt( f );
		const int sectionCount = file.GetSectionCount();
		
		// if the file has a 0 identifier it is skipped. this is the same as an empty file with no records read
		if( file.GetIdentifier() == 0 ){
			status.GetEntryWith( &archive, &file )->AppendText( FXString( "File skipped. File has 0 identifier." ) );
			continue;
		}
		
		// read the file if it exists in the file map
		iterFileMap = std::find_if( fileMap.begin(), fileMap.end(), fEntryEqualID( file.GetIdentifier() ) );
		if( iterFileMap == fileMap.end() ){
			status.GetEntryWith( &archive, &file )->AppendText( FXString( "File skipped. File not found in file map." ) );
			continue;
		}
		
		const sFileMapEntry &fileMapEntry = *iterFileMap;
		
		// if the file is empty skip it
		if( fileMapEntry.size == 0 ){
			status.GetEntryWith( &archive, &file )->AppendText( FXString( "File skipped. File is empty." ) );
			continue;
		}
		
		// create buffer and copy content from archive data
		fileData = new FXchar[ fileMapEntry.size ];
		if( ! fileData ) THROW( dueOutOfMemory );
		
		memcpy( fileData, archiveData + fileMapEntry.offset, fileMapEntry.size );
		
		curFilePos = 0;
		fileEndPos = fileMapEntry.size;
		fileUpdateValid = true;
		
		try{
			// patch all sections
			for( s=0; s<sectionCount; s++ ){
				if( curFilePos >= fileEndPos ){
					break; // deal with buggy files claiming more entries than are actually stored
				}
				
				const dmotTranslationSection &section = *file.GetSectionAt( s );
				recordSize = section.GetRecordSize();
				
				// patch all records. if a record with the given identifier exists it is
				// updated. otherwise it is ignored.
				curFilePos += section.GetSectionPosition();
				if( curFilePos + sizeof( FXshort ) > fileMapEntry.size ) THROW( dueInvalidFileFormat );
				recordCount = *( ( FXshort* )( fileData + curFilePos ) );
				printf( "number of records = %i\n", recordCount );
				if( recordCount > 100000 ) THROW( dueInvalidFileFormat );
				curFilePos += sizeof( recordCount );
				
				paddingType = section.GetPaddingType();
				paddingCount = section.GetPaddingCount();
				updatedEntryCount = 0;
				
				for( r=0; r<recordCount; r++ ){
					if( curFilePos >= fileEndPos ){
						break; // deal with buggy files claiming more entries than are actually stored
					}
					
					recordOffset = curFilePos; // section.GetSectionPosition() + sizeof( FXint ) + r * recordSize;
					
					if( recordOffset + section.GetIdentifierPosition() + sizeof( FXuint ) > fileMapEntry.size ) THROW( dueInvalidFileFormat );
					if( section.GetIdentifierType() == dmotTranslationSection::eidtUShort ){
						recordIdentifierUShort = *( ( FXushort* )( fileData + ( recordOffset + section.GetIdentifierPosition() ) ) );
						recordIdentifier = ( FXuint )recordIdentifierUShort;
						
					}else{
						recordIdentifier = *( ( FXuint* )( fileData + ( recordOffset + section.GetIdentifierPosition() ) ) );
					}
					
					record = section.GetRecordWithID( recordIdentifier );
					
					if( record ){
						textCount = section.GetTextCount();
						for( t=0; t<textCount; t++ ){
							const dmotTranslationSectionText &secText = *section.GetTextAt( t );
							FXString recordText = record->GetTextAt( t );
							textFormat = secText.GetFormat();
							
							if( recordText.empty() ){
								continue;
							}
							
							if( textFormat == dmotTranslationSectionText::etfWideChar ){
								textLen = BufferWCFromUTF8( recordText );
								
								if( textLen > secText.GetLength() ){
									printf( "text '%s' too long (is %i, max %i), truncated.\n",
										secText.GetHeaderName().text(), textLen, secText.GetLength() );
									if( recordOffset + secText.GetPosition() + sizeof( FXnchar ) * secText.GetLength() > fileMapEntry.size ) THROW( dueInvalidFileFormat );
									memcpy( fileData + ( recordOffset + secText.GetPosition() ), pTextBufferWC, sizeof( FXnchar ) * secText.GetLength() );
									
								}else if( textLen == secText.GetLength() ){
									if( recordOffset + secText.GetPosition() + sizeof( FXnchar ) * secText.GetLength() > fileMapEntry.size ) THROW( dueInvalidFileFormat );
									memcpy( fileData + ( recordOffset + secText.GetPosition() ), pTextBufferWC, sizeof( FXnchar ) * secText.GetLength() );
									
								}else{
									if( recordOffset + secText.GetPosition() + sizeof( FXnchar ) * ( textLen + 1 ) > fileMapEntry.size ) THROW( dueInvalidFileFormat );
									memcpy( fileData + ( recordOffset + secText.GetPosition() ), pTextBufferWC, sizeof( FXnchar ) * ( textLen + 1 ) );
								}
								
							}else if( textFormat == dmotTranslationSectionText::etfUTF8 ){
								textLen = recordText.length();
								
								if( textLen > secText.GetLength() ){
									printf( "text '%s' too long (is %i, max %i), truncated.\n",
										secText.GetHeaderName().text(), textLen, secText.GetLength() );
									if( recordOffset + secText.GetPosition() + sizeof( FXchar ) * secText.GetLength() > fileMapEntry.size ) THROW( dueInvalidFileFormat );
									memcpy( fileData + ( recordOffset + secText.GetPosition() ), recordText.text(), sizeof( FXchar ) * secText.GetLength() );
									
								}else if( textLen == secText.GetLength() ){
									if( recordOffset + secText.GetPosition() + sizeof( FXchar ) * secText.GetLength() > fileMapEntry.size ) THROW( dueInvalidFileFormat );
									memcpy( fileData + ( recordOffset + secText.GetPosition() ), recordText.text(), sizeof( FXchar ) * secText.GetLength() );
									
								}else{
									if( recordOffset + secText.GetPosition() + sizeof( FXchar ) * ( textLen + 1 ) > fileMapEntry.size ) THROW( dueInvalidFileFormat );
									memcpy( fileData + ( recordOffset + secText.GetPosition() ), recordText.text(), sizeof( FXchar ) * ( textLen + 1 ) );
								}
								
							}else if( textFormat == dmotTranslationSectionText::etf0TermWideChar ){
								textLen = BufferWCFromUTF8( recordText );
								
								if( textLen > secText.GetLength() ){
									printf( "text '%s' too long (is %i, max %i), truncated.\n",
										secText.GetHeaderName().text(), textLen, secText.GetLength() );
									textLen = secText.GetLength();
									pTextBufferWC[ textLen ] = 0;
								}
								if( recordOffset + secText.GetPosition() + sizeof( FXnchar ) * ( textLen + 1 ) > fileMapEntry.size ) THROW( dueInvalidFileFormat );
								memcpy( fileData + ( recordOffset + secText.GetPosition() ), pTextBufferWC, sizeof( FXnchar ) * ( textLen + 1 ) );
								
							}else if( textFormat == dmotTranslationSectionText::etf0TermUTF8 ){
								textLen = recordText.length();
								
								if( textLen > secText.GetLength() ){
									printf( "text '%s' too long (is %i, max %i), truncated.\n",
										secText.GetHeaderName().text(), textLen, secText.GetLength() );
									textLen = secText.GetLength();
									recordText[ textLen ] = 0;
								}
								if( recordOffset + secText.GetPosition() + sizeof( FXchar ) * ( textLen + 1 ) > fileMapEntry.size ) THROW( dueInvalidFileFormat );
								memcpy( fileData + ( recordOffset + secText.GetPosition() ), recordText.text(), sizeof( FXchar ) * ( textLen + 1 ) );
							}
						}
						
						updatedEntryCount++;
						
					}else{
						printf( "record %u: no matching translation, skipped.\n", recordIdentifier );
					}
					
					curFilePos += recordSize;
					
					if( r == recordCount - 1 ){
						// there seems to be a bug in the FOX file reader reading wrong
						// values. to prevent problems we skip the padding in the last
						// record as it should reach the end of file anyways
						break;
					}
					
					if( paddingType == dmotTranslationSection::eptSequenceInt32 ){
						paddingCheckCount = 0;
						
						while( paddingCheckCount < paddingCount ){
							if( curFilePos + sizeof( FXint ) > fileMapEntry.size ) THROW( dueInvalidFileFormat );
							paddingCheckValue = *( ( FXint* )( fileData + curFilePos ) );
							curFilePos += sizeof( paddingCheckValue );
							
							if( paddingCheckValue == 0 ){
								paddingCheckCount++;
								
							}else{
								paddingCheckCount = 0;
							}
						}
						
					}else if( paddingType == dmotTranslationSection::eptSpecialNPC ){
						if( curFilePos + sizeof( FXint ) > fileMapEntry.size ) THROW( dueInvalidFileFormat );
						paddingCheckValue = *( ( FXint* )( fileData + curFilePos ) );
						if( paddingCheckValue < paddingCount ){
							curFilePos += sizeof( paddingCheckValue ) * ( 1 + ( int )paddingCheckValue );
							if( curFilePos + sizeof( FXint ) > fileMapEntry.size ) THROW( dueInvalidFileFormat );
							paddingCheckValue = *( ( FXint* )( fileData + curFilePos ) );
						}
						
						if( paddingCheckValue < paddingCount ){
							curFilePos += sizeof( paddingCheckValue ) * ( 1 + ( int )paddingCheckValue );
							if( curFilePos + sizeof( FXint ) > fileMapEntry.size ) THROW( dueInvalidFileFormat );
							paddingCheckValue = *( ( FXint* )( fileData + curFilePos ) );
						}
						
						int betterSafeThanSorry = 0;
						while( paddingCheckValue < paddingCount ){
							if( betterSafeThanSorry++ == 100 ) THROW( dueInvalidParam );
							curFilePos += sizeof( paddingCheckValue );
							if( curFilePos + sizeof( FXint ) > fileMapEntry.size ) THROW( dueInvalidFileFormat );
							paddingCheckValue = *( ( FXint* )( fileData + curFilePos ) );
						}
						
					}else if( paddingType == dmotTranslationSection::eptSpecialQuest ){
#if 0
						// dummy
						if( curFilePos + sizeof( FXint ) > fileMapEntry.size ) THROW( dueInvalidFileFormat );
						blockEntryCount = *( ( FXint* )( fileData + curFilePos ) );
						if( blockEntryCount == 0 ){
							curFilePos += sizeof( blockEntryCount );
						}
						
						// run of 3-int blocks
						if( curFilePos + sizeof( FXint ) > fileMapEntry.size ) THROW( dueInvalidFileFormat );
						blockEntryCount = *( ( FXint* )( fileData + curFilePos ) );
						curFilePos += sizeof( blockEntryCount );
						
						if( curFilePos + sizeof( FXint ) * 3 * blockEntryCount > fileMapEntry.size ) THROW( dueInvalidFileFormat );
						curFilePos += sizeof( FXint ) * 3 * blockEntryCount;
						
						// 2 runs of 4-int blocks
						for( b=0; b<2; b++ ){
							if( curFilePos + sizeof( FXint ) > fileMapEntry.size ) THROW( dueInvalidFileFormat );
							blockEntryCount = *( ( FXint* )( fileData + curFilePos ) );
							curFilePos += sizeof( blockEntryCount );
							
							if( curFilePos + sizeof( FXint ) * 4 * blockEntryCount > fileMapEntry.size ) THROW( dueInvalidFileFormat );
							curFilePos += sizeof( FXint ) * 4 * blockEntryCount;
						}
						
						// run of 1-int blocks (should all be 0)
						if( curFilePos + sizeof( FXint ) > fileMapEntry.size ) THROW( dueInvalidFileFormat );
						paddingCheckCount = *( ( FXint* )( fileData + curFilePos ) );
						curFilePos += sizeof( paddingCheckCount );
						if( curFilePos + sizeof( FXint ) * paddingCheckCount > fileMapEntry.size ) THROW( dueInvalidFileFormat );
						curFilePos += sizeof( FXint ) * paddingCheckCount;
#endif
						// locate a run of 5 ints where the first contains 4 and all others 0 as value
						b = 0;
						while( b < 4 ){
							paddingCheckValue = 0;
							while( paddingCheckValue != 4 ){
								if( curFilePos + sizeof( FXint ) > fileMapEntry.size ){
									THROW( dueInvalidFileFormat );
								}
								paddingCheckValue = *( ( FXint* )( fileData + curFilePos ) );
								curFilePos += sizeof( paddingCheckValue );
							}
							for( b=0; b<4; b++ ){
								if( curFilePos + sizeof( FXint ) > fileMapEntry.size ){
									THROW( dueInvalidFileFormat );
								}
								paddingCheckValue = *( ( FXint* )( fileData + curFilePos ) );
								curFilePos += sizeof( paddingCheckValue );
								if( paddingCheckValue != 0 ){
									break;
								}
							}
						}
						
					}else if( paddingType == dmotTranslationSection::eptBlocksInt32 ){
						if( paddingCount == 0 ){
							paddingCheckValue = 1;
							while( paddingCheckValue > 0 ){
								if( curFilePos + sizeof( FXint ) > fileMapEntry.size ) THROW( dueInvalidFileFormat );
								paddingCheckValue = *( ( FXint* )( fileData + curFilePos ) );
								if( paddingCheckValue < 0 || paddingCheckValue > 100 ){
									THROW( deeInvalidFormat );
								}
								curFilePos += sizeof( paddingCheckValue ) * ( 1 + ( int )paddingCheckValue );
							}
							
						}else{
							for( paddingCheckCount=0; paddingCheckCount<paddingCount; paddingCheckCount++ ){
								if( curFilePos + sizeof( FXint ) > fileMapEntry.size ) THROW( dueInvalidFileFormat );
								paddingCheckValue = *( ( FXint* )( fileData + curFilePos ) );
								if( paddingCheckValue < 0 || paddingCheckValue > 100 ){
									THROW( deeInvalidFormat );
								}
								curFilePos += sizeof( paddingCheckValue ) * ( 1 + ( int )paddingCheckValue );
							}
						}
					}
				}
				
				// check the number of updated entries. if this number is less than a given percentage of
				// the total number of entries chances are high the file layout somehow changed and the
				// update would break the game. in this case the update is not written. this check is not
				// done if less than 2 updated entries are required as then the result is hard to verify
				requiredUpdateEntryCount = ( int )( ( float )recordCount * entryPassPercentage );
				
				if( ! fileUpdateValid ){
					break;
				}
				if( requiredUpdateEntryCount >= 2 && updatedEntryCount < requiredUpdateEntryCount ){
					status.GetEntryWith( &archive, &file )->SetText( FXString( "File skipped. Too many records do not match. Most probably changed layout." ) );
					fileUpdateValid = false;
					break;
				}
			}
			
			// update archive data with file data if required
			if( fileUpdateValid ){
				memcpy( archiveData + fileMapEntry.offset, fileData, fileMapEntry.size );
				
				status.GetEntryWith( &archive, &file )->SetText( FXString( "File updated successfully." ) );
			}
			
			delete [] fileData;
			fileData = NULL;
			
		}catch( duException e ){
			if( fileData ) delete [] fileData;
			status.GetEntryWith( &archive, &file )->AppendTextException( FXString( "Update failed." ), e );
		}
	}
	
	try{
		// write memory back to file. we move it in place after we are done.
		// this avoids a broken file if the patch fails for some reason.
		stream = new FXFileStream;
		if( ! stream ) THROW( dueOutOfMemory );
		if( ! stream->open( pathAbsolute + ".temp", FXStreamSave ) ) THROW( dueFileNotFound );
		
		stream->save( archiveData, fileSize );
		
		delete [] archiveData;
		archiveData = NULL;
		
		delete stream;
		stream = NULL;
		
		FXFile::moveFiles( pathAbsolute + ".temp", pathAbsolute, TRUE );
		
		printf( "done patching file '%s'.\n", pathFile.text() );
		
		status.GetEntryWith( &archive, NULL )->AppendText( "Archive file updated successfully." );
		
	}catch( duException e ){
		if( archiveData ) delete [] archiveData;
		if( stream ) delete stream;
		FXString text;
		text.format( "Writing archive failed (archive file '%s').", pathAbsolute.text() );
		status.GetEntryWith( &archive, NULL )->AppendTextException( text, e );
	}
}



bool dmotReadBinFile::ReadFileMap( dmotTranslateStatus &status, dmotConfig &config, const dmotTranslationArchive &archive, std::vector<sFileMapEntry> &fileMap ){
	const FXString &pathDMO = config.GetPathDMO();
	const FXString &pathFile = archive.GetPathFileMap();
	const FXString pathAbsolute( FXPath::absolute( pathDMO, pathFile ) );
	FXFileStream *stream = NULL;
	int e, entryCount;
	FXuint valueUInt;
	
	try{
		// open the file
		printf( "reading file map '%s'...\n", pathAbsolute.text() );
		stream = new FXFileStream;
		if( ! stream ) THROW( dueOutOfMemory );
		if( ! stream->open( pathAbsolute, FXStreamLoad ) ) THROW( dueFileNotFound );
		
		// first value is always 16
		*stream >> valueUInt;
		if( valueUInt != 16 ) THROW( dueInvalidFileFormat );
		
		// second value is the number of entries
		*stream >> valueUInt;
		entryCount = ( int )valueUInt;
		fileMap.resize( entryCount );
		//printf( "%i entries:\n", entryCount );
		
		// read maps
		for( e=0; e<entryCount; e++ ){
			sFileMapEntry &entry = fileMap.at( e );
			
			// always 1
			*stream >> valueUInt;
			if( valueUInt != 1 ) THROW( dueInvalidFileFormat );
			
			// file size
			*stream >> entry.size;
			
			// file size again
			*stream >> valueUInt;
			if( valueUInt != entry.size ) THROW( dueInvalidFileFormat );
			
			// file identifier
			*stream >> entry.identifier;
			
			// file offset
			*stream >> entry.offset;
			
			// always 0
			*stream >> valueUInt;
			if( valueUInt != 0 ) THROW( dueInvalidFileFormat );
			
			//printf( "entry %i: id=%u offset=%u size=%u\n", e, entry.identifier, entry.offset, entry.size );
		}
		
		// close the file
		printf( "closing file.\n" );
		delete stream;
		
		return true;
		
	}catch( duException e ){
		if( stream ) delete stream;
		FXString text;
		text.format( "Failed to retrieve file map (map file '%s').", pathAbsolute.text() );
		status.GetEntryWith( &archive, NULL )->AppendTextException( text, e );
		return false;
	}
}

void dmotReadBinFile::PrepareBufferWC( int size ){
	int i;
	
	if( size > pTextBufferWCLen ){
		FXnchar *buffer = NULL;
		
		if( size > 0 ){
			buffer = new FXnchar[ size + 1 ];
			if( ! buffer ) THROW( dueOutOfMemory );
		}
		
		if( pTextBufferWC ) delete [] pTextBufferWC;
		pTextBufferWC = buffer;
		pTextBufferWCLen = size;
	}
	
	for( i=0; i<=size; i++ ){
		pTextBufferWC[ i ] = '\0';
	}
}

void dmotReadBinFile::ConvertBufferWC( FXString &string ){
	FXchar charsUTF8[ 4 ];
	FXint count;
	int i;
	
	string.clear();
	
	for( i=0; i<pTextBufferWCLen; i++ ){
		if( pTextBufferWC[ i ] == 0 ) break;
		
		count = pCodecUTF8.wc2mb( &charsUTF8[ 0 ], 4, pTextBufferWC[ i ] );
		if( count > 0 ){
			string.append( charsUTF8, count );
		}
	}
}

int dmotReadBinFile::BufferWCFromUTF8( const FXString &string ){
	const FXchar *charsUTF8 = string.text();
	int i, c, countWC = string.count();
	FXwchar charUnicode;
	FXint countUTF8;
	
	PrepareBufferWC( countWC );
	
	for( i=0, c=0; c<countWC; i+=countUTF8, c++ ){
		countUTF8 = string.extent( i );
		pCodecUTF8.mb2wc( charUnicode, charsUTF8 + i, countUTF8 );
		pTextBufferWC[ c ] = ( FXnchar )charUnicode;
	}
	
	return countWC;
}

void dmotReadBinFile::PrepareBufferUTF8( int size ){
	int i;
	
	if( size > pTextBufferUTF8Len ){
		FXchar *buffer = NULL;
		
		if( size > 0 ){
			buffer = new FXchar[ size + 1 ];
			if( ! buffer ) THROW( dueOutOfMemory );
		}
		
		if( pTextBufferUTF8 ) delete [] pTextBufferUTF8;
		pTextBufferUTF8 = buffer;
		pTextBufferUTF8Len = size;
	}
	
	for( i=0; i<=size; i++ ){
		pTextBufferUTF8[ i ] = '\0';
	}
}
