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

#ifndef _DMOTTRANSLATIONARCHIVE_H_
#define _DMOTTRANSLATIONARCHIVE_H_

#include <vector>
#include "../foxtoolkit.h"

class dmotTranslationFile;
class dmotConfig;
class dmotTranslateStatus;



/**
 * @brief Translation Archive.
 * Defines an archive in a translation. Archives stores multiple files.
 * @author Plüss Roland
 * @version 1.0
 * @date 2010
 */
class dmotTranslationArchive{
private:
	FXString pName;
	FXString pPathArchive;
	FXString pPathFileMap;
	std::vector<dmotTranslationFile*> pFiles;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new translation archive. */
	dmotTranslationArchive();
	/** Cleans up the translation archive. */
	~dmotTranslationArchive();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the display name. */
	inline const FXString &GetName() const{ return pName; }
	/** Sets the display name. */
	void SetName( const FXString &name );
	/** Retrieves the archive file path. */
	inline const FXString &GetPathArchive() const{ return pPathArchive; }
	/** Sets the archive file path. */
	void SetPathArchive( const FXString &path );
	/** Retrieves the file map file path. */
	inline const FXString &GetPathFileMap() const{ return pPathFileMap; }
	/** Sets the file map file path. */
	void SetPathFileMap( const FXString &path );
	
	/** Import bin-files. */
	void ImportBinFiles( dmotTranslateStatus &status, dmotConfig &config, bool replace );
	/** Patch bin-files. */
	void PatchBinFiles( dmotTranslateStatus &status, dmotConfig &config );
	
	/** Retrieves the number of files. */
	int GetFileCount() const;
	/** Retrieves the file at the given position. */
	dmotTranslationFile *GetFileAt( int index ) const;
	/** Adds a file. */
	void AddFile( dmotTranslationFile *file );
	/** Removes a file. */
	void RemoveFile( dmotTranslationFile *file );
	/** Removes all files. */
	void RemoveAllFiles();
	/*@}*/
};

// end of include only once
#endif
