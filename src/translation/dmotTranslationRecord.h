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

#ifndef _DMOTTRANSLATIONRECORD_H_
#define _DMOTTRANSLATIONRECORD_H_

#include <vector>
#include "../foxtoolkit.h"



/**
 * @brief Translation Record.
 * Record in a translation file. The ordering of record texts matches
 * the ordering of file texts. The identifier is used to match the
 * translations.
 * @author Plüss Roland
 * @version 1.0
 * @date 2010
 */
class dmotTranslationRecord{
private:
	unsigned int pIdentifier;
	std::vector<FXString> pTexts;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new translation record. */
	dmotTranslationRecord();
	/** Cleans up the translation record. */
	~dmotTranslationRecord();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the identifier. */
	inline unsigned int GetIdentifier() const{ return pIdentifier; }
	/** Sets the identifier. */
	void SetIdentifier( unsigned int identifier );
	
	/** Retrieves the number of texts. */
	int GetTextCount() const;
	/** Retrieves the text at the given position. */
	const FXString &GetTextAt( int index ) const;
	/** Sets the text at the given position. */
	void SetTextAt( int index, const FXString &text );
	/** Adds a text. */
	void AddText( const FXString &text );
	/** Removes a text. */
	void RemoveTextAt( int index );
	/** Removes all texts. */
	void RemoveAllTexts();
	/*@}*/
};

// end of include only once
#endif
