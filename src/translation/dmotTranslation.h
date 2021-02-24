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

#ifndef _DMOTTRANSLATION_H_
#define _DMOTTRANSLATION_H_

#include <vector>
#include "../foxtoolkit.h"

class dmotTranslationArchive;
class dmotConfig;
class dmotTranslateStatus;



/**
 * @brief Translation.
 * @author Plüss Roland
 * @version 1.0
 * @date 2010
 */
class dmotTranslation{
private:
	std::vector<dmotTranslationArchive*> pArchives;
	FXString pFilename;
	bool pSaved;
	bool pChanged;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new translation. */
	dmotTranslation();
	/** Cleans up the translation. */
	~dmotTranslation();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the filename. */
	inline const FXString &GetFilename() const{ return pFilename; }
	/** Sets the path to the filename. */
	void SetFilename( const FXString &filename );
	/** Determines if the file has been saved. */
	inline bool GetSaved() const{ return pSaved; }
	/** Sets if the file has been saved. */
	void SetSaved( bool saved );
	/** Determines if the file has changed. */
	inline bool GetChanged() const{ return pChanged; }
	/** Sets if the file has changed. */
	void SetChanged( bool changed );
	
	/** Import bin-files. */
	void ImportBinFiles( dmotTranslateStatus &status, dmotConfig &config, bool replace );
	/** Patch bin-files. */
	void PatchBinFiles( dmotTranslateStatus &status, dmotConfig &config );
	
	/** Retrieves the number of archives. */
	int GetArchiveCount() const;
	/** Retrieves the archive at the given position. */
	dmotTranslationArchive *GetArchiveAt( int index ) const;
	/** Adds a archive. */
	void AddArchive( dmotTranslationArchive *archive );
	/** Removes a archive. */
	void RemoveArchive( dmotTranslationArchive *archive );
	/** Removes all archives. */
	void RemoveAllArchives();
	/*@}*/
};

// end of include only once
#endif
