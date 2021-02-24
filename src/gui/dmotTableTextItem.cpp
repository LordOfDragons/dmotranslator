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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "dmotTableTextItem.h"



// Class dmotTableTextItem
/////////////////////////////

FXIMPLEMENT( dmotTableTextItem, FXTableItem, NULL, 0 )

// Constructor, destructor
////////////////////////////

dmotTableTextItem::dmotTableTextItem(){ }

dmotTableTextItem::dmotTableTextItem( const FXString &text, FXIcon *ic, void *ptr ) : FXTableItem( text, ic, ptr ){
	pBackColor = 0;
}

dmotTableTextItem::~dmotTableTextItem(){
}



// Management
///////////////

void dmotTableTextItem::SetBackColor( FXColor color ){
	pBackColor = color;
}

void dmotTableTextItem::drawBackground( const FXTable *table, FXDC &dc, FXint x, FXint y, FXint w, FXint h ) const{
	FXint hg = table->isHorzGridShown() ? 1 : 0;
	FXint vg = table->isVertGridShown() ? 1 : 0;
	
	if( pBackColor != 0 ){
		if( isSelected() ){
			dc.setForeground( table->getSelBackColor() );
			
		}else{
			dc.setForeground( pBackColor );
		}
	}
	
	dc.fillRectangle( x + vg, y + hg, w - vg, h - hg );
}
