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

#ifndef _DMOTREADBINFILE_H_
#define _DMOTREADBINFILE_H_

#include <vector>
#include "../foxtoolkit.h"

class dmotTranslation;
class dmotTranslationFile;
class dmotTranslationArchive;
class dmotConfig;
class dmotTranslateStatus;



/**
 * @brief Read Bin File.
 * @author Plüss Roland
 * @version 1.0
 * @date 2010
 */
class dmotReadBinFile{
public:
	struct sFileMapEntry{
		FXuint identifier;
		FXuint offset;
		FXuint size;
	};
	
private:
	FXUTF8Codec pCodecUTF8;
	FXnchar *pTextBufferWC;
	int pTextBufferWCLen;
	FXchar *pTextBufferUTF8;
	int pTextBufferUTF8Len;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new read bin file. */
	dmotReadBinFile();
	/** Cleans up the read bin file. */
	~dmotReadBinFile();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Read bin file into the given file replacing all records. */
	void ReadFile( dmotTranslateStatus &status, dmotConfig &config, dmotTranslationArchive &archive, dmotTranslationFile &file, bool replace );
	/** Save translation to bin files. Makes copy of the existing files first. */
	void WriteFile( dmotTranslateStatus &status, dmotConfig &config, const dmotTranslationArchive &archive );
	
	/** Read file map. */
	bool ReadFileMap( dmotTranslateStatus &status, dmotConfig &config, const dmotTranslationArchive &archive, std::vector<sFileMapEntry> &fileMap );
	/** Prepare wide char text buffer as an empty string. */
	void PrepareBufferWC( int size );
	/** Convert from wide char into utf8. */
	void ConvertBufferWC( FXString &string );
	/** Convert from utf8 into wide char. */
	int BufferWCFromUTF8( const FXString &string );
	/** Prepare utf8 text buffer as an empty string. */
	void PrepareBufferUTF8( int size );
	/*@}*/
};

// end of include only once
#endif
