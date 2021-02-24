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

#ifndef _DMOTTRANSLATESTATUSENTRY_H_
#define _DMOTTRANSLATESTATUSENTRY_H_

#include "../foxtoolkit.h"

class dmotTranslationArchive;
class dmotTranslationFile;
class duException;



/**
 * @brief Translate Status Entry.
 * @author Plüss Roland
 * @version 1.0
 * @date 2010
 */
class dmotTranslateStatusEntry{
private:
	const dmotTranslationArchive *pArchive;
	const dmotTranslationFile *pFile;
	FXString pText;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new translate status entry. */
	dmotTranslateStatusEntry( const dmotTranslationArchive *archive, const dmotTranslationFile *file );
	/** Cleans up the translate status entry. */
	~dmotTranslateStatusEntry();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the archive. */
	inline const dmotTranslationArchive *GetArchive() const{ return pArchive; }
	/** Retrieves the file. */
	inline const dmotTranslationFile *GetFile() const{ return pFile; }
	/** Retrieves the text. */
	inline const FXString &GetText() const{ return pText; }
	/** Sets the text. */
	void SetText( const FXString &text );
	/** Append to text. */
	void AppendText( const FXString &text );
	/** Append exception to text. */
	void AppendException( const duException &exception );
	/** Append exception to text. */
	void AppendTextException( const FXString &text, const duException &exception );
	/*@}*/
};

#endif
