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

#ifndef _DMOTLOADSAVETRANSLATION_H_
#define _DMOTLOADSAVETRANSLATION_H_

#include "../foxtoolkit.h"

class dmotTranslation;



/**
 * @brief Load/Save Translation.
 * @author Plüss Roland
 * @version 1.0
 * @date 2010
 */
class dmotLoadSaveTranslation{
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new load/save translation. */
	dmotLoadSaveTranslation();
	/** Cleans up the load/save translation. */
	~dmotLoadSaveTranslation();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Load translation from file. */
	void LoadFromFile( const FXString &filename, dmotTranslation &translation );
	void LoadFromFileV1p0( FXFileStream &stream, dmotTranslation &translation );
	void LoadFromFileV1p1( FXFileStream &stream, dmotTranslation &translation );
	void LoadFromFileV1p2( FXFileStream &stream, dmotTranslation &translation );
	void LoadFromFileV1p3( FXFileStream &stream, dmotTranslation &translation );
	/** Save translation to file. */
	void SaveToFile( const FXString &filename, const dmotTranslation &translation );
	/*@}*/
};

// end of include only once
#endif
