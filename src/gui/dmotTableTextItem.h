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

#ifndef _DMOTTABLETEXTITEM_H_
#define _DMOTTABLETEXTITEM_H_

#include "../foxtoolkit.h"



/**
 * @brief Text Table Item.
 * Special version of the FXTableItem providing support for individual cell
 * background color to signal a too long text as well as support for handling
 * multi line text.
 * @author Plüss Roland
 * @version 1.0
 * @date 2010
 */
class dmotTableTextItem : public FXTableItem{
	FXDECLARE( dmotTableTextItem )
	
protected:
	dmotTableTextItem();
	
private:
	FXColor pBackColor;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new item. */
	dmotTableTextItem( const FXString &text, FXIcon *ic = NULL, void *ptr = NULL );
	/** Cleans up the item. */
	virtual ~dmotTableTextItem();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the back color. */
	inline FXColor GetBackColor() const{ return pBackColor; }
	/** Sets the back color. */
	void SetBackColor( FXColor color );
	
	/*
	/ * Create input control for editing this item. * /
	virtual FXWindow *getControlFor( FXTable *table );
	
	/ * Set value from input control. * /
	virtual void setFromControl( FXWindow *control );
	*/
	
	/** Render background. */
	virtual void drawBackground( const FXTable *table, FXDC &dc, FXint x, FXint y, FXint w, FXint h ) const;
	/*@}*/
};

// end of include only once
#endif
