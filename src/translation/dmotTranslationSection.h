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

#ifndef _DMOTTRANSLATIONSECTION_H_
#define _DMOTTRANSLATIONSECTION_H_

#include <vector>
#include "../foxtoolkit.h"

class dmotTranslationSectionText;
class dmotTranslationRecord;



/**
 * @brief Translation Section.
 * Defines a section inside a file to translate. The section offset defines
 * where in the file the section starts. The number of entries is located
 * at this position. The identifier position indicates where in the file
 * record the identifier is located.
 * @author Plüss Roland
 * @version 1.0
 * @date 2010
 */
class dmotTranslationSection{
public:
	/** Padding types. */
	enum ePaddingTypes{
		/** No padding, all records are of the same size. */
		eptNoPadding,
		/** Records end with a sequence of int32 with value 0. */
		eptSequenceInt32,
		/** A value less than a threshold (padding count) is the number of int32 to follow. */
		eptSpecialNPC,
		/**
		 * Special padding. If first int is 0 skip 4 more ints. If first int is 1 skip 7 more ints.
		 * Then counted blocks with padding. Has the form "N, block1, ..., blockN, X, int1, ..., intX"
		 * hence first a block count value followed by N blocks composed of 4 ints each then a
		 * padding count value followed by X int padding values. The padding count value indicates
		 * how many counted blocks exist before the padding follows.
		 */
		eptSpecialQuest,
		/** Skips blocks of the form (count,uint[count]) until a count of 0 is found. */
		eptBlocksInt32,
	};
	
	/** ID types. */
	enum eIDTypes{
		/** ID is an unsigned integer. */
		eidtUInt,
		/** ID is an unsigned short. */
		eidtUShort,
	};
	
private:
	FXString pName;
	int pSectionPosition;
	FXString pHeaderIDName;
	int pHeaderIDWidth;
	int pIDPosition;
	int pIDType;
	int pRecordSize;
	int pPaddingType;
	int pPaddingCount;
	std::vector<dmotTranslationSectionText*> pTexts;
	std::vector<dmotTranslationRecord*> pRecords;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new translation section. */
	dmotTranslationSection();
	/** Cleans up the translation section. */
	~dmotTranslationSection();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the display name. */
	inline const FXString &GetName() const{ return pName; }
	/** Sets the display name. */
	void SetName( const FXString &name );
	/** Retrieves the section position. */
	inline int GetSectionPosition() const{ return pSectionPosition; }
	/** Sets the section position. */
	void SetSectionPosition( int position );
	/** Retrieves the header name for the identifier. */
	inline const FXString &GetHeaderIDName() const{ return pHeaderIDName; }
	/** Sets the header name for the identifier. */
	void SetHeaderIDName( const FXString &name );
	/** Retrieves the header width. */
	inline int GetHeaderIDWidth() const{ return pHeaderIDWidth; }
	/** Sets the header width. */
	void SetHeaderIDWidth( int width );
	/** Retrieves the identifier position. */
	inline int GetIdentifierPosition() const{ return pIDPosition; }
	/** Sets the identifier position. */
	void SetIdentifierPosition( int position );
	/** Retrieves the type of the identifier. */
	inline int GetIdentifierType() const{ return pIDType; }
	/** Sets the type of the identifier. */
	void SetIdentifierType( int type );
	/** Retrieves the size of the record. */
	inline int GetRecordSize() const{ return pRecordSize; }
	/** Sets the size of the record. */
	void SetRecordSize( int size );
	/** Retrieves the padding type. */
	inline int GetPaddingType() const{ return pPaddingType; }
	/** Sets the padding type. */
	void SetPaddingType( int paddingType );
	/** Retrieves the padding count. */
	inline int GetPaddingCount() const{ return pPaddingCount; }
	/** Sets the padding count. */
	void SetPaddingCount( int paddingCount );
	
	/** Retrieves the number of texts. */
	int GetTextCount() const;
	/** Retrieves the index of the text with the matching name or -1 if not found. */
	int IndexOfTextNamed( const FXString &name );
	/** Retrieves the text at the given position. */
	dmotTranslationSectionText *GetTextAt( int index ) const;
	/** Adds a text. */
	void AddText( dmotTranslationSectionText *text );
	/** Removes a text. */
	void RemoveText( dmotTranslationSectionText *text );
	/** Removes all texts. */
	void RemoveAllTexts();
	
	/** Retrieves the number of records. */
	int GetRecordCount() const;
	/** Determines if a record with the given identifier exists. */
	bool HasRecordWithID( int identifier ) const;
	/** Retrieves the record at the given position. */
	dmotTranslationRecord *GetRecordAt( int index ) const;
	/** Retrieves the record with the given identifier or NULL if not found. */
	dmotTranslationRecord *GetRecordWithID( unsigned int identifier ) const;
	/** Adds a record. */
	void AddRecord( dmotTranslationRecord *record );
	/** Removes a record. */
	void RemoveRecord( dmotTranslationRecord *record );
	/** Removes all records. */
	void RemoveAllRecords();
	/** Add text at the end in all records. */
	void AddTextInRecords( const FXString &text );
	/** Remove text at the given position from all records. */
	void RemoveTextFromRecordsAt( int index );
	/*@}*/
};

// end of include only once
#endif
