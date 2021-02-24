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

#ifndef _DMOTTABLETEXT_H_
#define _DMOTTABLETEXT_H_

#include "../foxtoolkit.h"



/**
 * @brief Text Table.
 * Special version of the FXTable providing support for individual cell
 * background color to signal a too long text as well as support for handling
 * multi line text.
 * @author Plüss Roland
 * @version 1.0
 * @date 2010
 */
class dmotTableText : public FXTable{
	FXDECLARE( dmotTableText )
	
protected:
	dmotTableText();
	
private:
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new table. */
	dmotTableText( FXComposite *p, FXObject *tgt = NULL, FXSelector sel = 0, FXuint opts = 0, FXint x = 0,
	FXint y = 0, FXint w = 0, FXint h = 0, FXint pl = DEFAULT_MARGIN, FXint pr = DEFAULT_MARGIN,
	FXint pt = DEFAULT_MARGIN, FXint pb = DEFAULT_MARGIN );
	/** Cleans up the table. */
	virtual ~dmotTableText();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Set cell back color. */
	void SetCellBackColor( int row, int col, FXColor color );
	
	/** Create item. */
	virtual FXTableItem *createItem( const FXString &text, FXIcon *icon, void *ptr );
	/*@}*/
};

// end of include only once
#endif