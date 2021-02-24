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

#ifndef _DMOTTRANSLATIONSECTIONTEXT_H_
#define _DMOTTRANSLATIONSECTIONTEXT_H_

#include "../foxtoolkit.h"



/**
 * @brief Translation Section Text.
 * Defines a text to translate in a translation section object. Stores
 * the position relative to the record, the number of wide char
 * characters and a name to show in the editing table.
 * @author Plüss Roland
 * @version 1.0
 * @date 2010
 */
class dmotTranslationSectionText{
public:
	/** Text format. */
	enum eTextFormats{
		/** Text stored as wide char. */
		etfWideChar,
		/** Text stored as utf8. */
		etfUTF8,
		/** Text stored as 0-terminated wide char. */
		etf0TermWideChar,
		/** Text stored as 0-terminated utf8. */
		etf0TermUTF8
	};
	
private:
	FXString pHeaderName;
	int pHeaderWidth;
	int pPosition;
	int pLength;
	int pFormat;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new translation file text. */
	dmotTranslationSectionText();
	/** Cleans up the translation file text. */
	~dmotTranslationSectionText();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the header name. */
	inline const FXString &GetHeaderName() const{ return pHeaderName; }
	/** Sets the header name. */
	void SetHeaderName( const FXString &name );
	/** Retrieves the header width. */
	inline int GetHeaderWidth() const{ return pHeaderWidth; }
	/** Sets the header width. */
	void SetHeaderWidth( int width );
	/** Retrieves the position. */
	inline int GetPosition() const{ return pPosition; }
	/** Sets the position. */
	void SetPosition( int position );
	/** Retrieves the length. */
	inline int GetLength() const{ return pLength; }
	/** Sets the length. */
	void SetLength( int length );
	/** Retrieves the text format. */
	inline int GetFormat() const{ return pFormat; }
	/** Sets the text format. */
	void SetFormat( int format );
	/*@}*/
};

// end of include only once
#endif
