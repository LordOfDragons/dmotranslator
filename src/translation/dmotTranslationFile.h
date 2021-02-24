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

#ifndef _DMOTTRANSLATIONFILE_H_
#define _DMOTTRANSLATIONFILE_H_

#include <vector>
#include "../foxtoolkit.h"

class dmotTranslationSection;



/**
 * @brief Translation File.
 * Defines a file to translate.
 * @author Plüss Roland
 * @version 1.0
 * @date 2010
 */
class dmotTranslationFile{
private:
	FXString pName;
	FXuint pIdentifier;
	std::vector<dmotTranslationSection*> pSections;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new translation file. */
	dmotTranslationFile();
	/** Cleans up the translation file. */
	~dmotTranslationFile();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the identifier. */
	inline FXuint GetIdentifier() const{ return pIdentifier; }
	/** Sets the identifier. */
	void SetIdentifier( FXuint identifier );
	/** Retrieves the display name. */
	inline const FXString &GetName() const{ return pName; }
	/** Sets the display name. */
	void SetName( const FXString &name );
	
	/** Retrieves the number of sections. */
	int GetSectionCount() const;
	/** Retrieves the section at the given position. */
	dmotTranslationSection *GetSectionAt( int index ) const;
	/** Adds a section. */
	void AddSection( dmotTranslationSection *section );
	/** Removes a section. */
	void RemoveSection( dmotTranslationSection *section );
	/** Removes all sections. */
	void RemoveAllSections();
	/*@}*/
};

// end of include only once
#endif
