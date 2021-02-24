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

#ifndef _DMOTTRANSLATESTATUS_H_
#define _DMOTTRANSLATESTATUS_H_

#include <vector>
#include "../foxtoolkit.h"

class dmotTranslateStatusEntry;
class dmotTranslationArchive;
class dmotTranslationFile;



/**
 * @brief Translate Status.
 * @author Plüss Roland
 * @version 1.0
 * @date 2010
 */
class dmotTranslateStatus{
private:
	std::vector<dmotTranslateStatusEntry*> pEntries;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new translate log. */
	dmotTranslateStatus();
	/** Cleans up the translate log. */
	~dmotTranslateStatus();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the number of entries. */
	int GetEntryCount() const;
	/** Retrieves the entry at the given position. */
	dmotTranslateStatusEntry *GetEntryAt( int index ) const;
	/** Retrieves the entry with the given archive and file. if not existing creates a new one. */
	dmotTranslateStatusEntry *GetEntryWith( const dmotTranslationArchive *archive, const dmotTranslationFile *file );
	/** Adds an entry. */
	void AddEntry( dmotTranslateStatusEntry *entry );
	/** Removes a entry. */
	void RemoveEntryAt( dmotTranslateStatusEntry *entry );
	/** Removes all entries. */
	void RemoveAllEntries();
	/*@}*/
};

#endif
