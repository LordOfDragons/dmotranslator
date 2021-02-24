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

#include "dmotTableText.h"
#include "dmotTableTextItem.h"



// Class dmotTableText
////////////////////////

FXIMPLEMENT( dmotTableText, FXTable, NULL, 0 )

// Constructor, destructor
////////////////////////////

dmotTableText::dmotTableText(){
}

dmotTableText::dmotTableText( FXComposite *p, FXObject *tgt, FXSelector sel, FXuint opts, FXint x, FXint y, FXint w, FXint h,
FXint pl, FXint pr, FXint pt, FXint pb ) : FXTable( p, tgt, sel, opts, x, y, w, h, pl, pr, pt, pb ){
}

dmotTableText::~dmotTableText(){
}



// Management
///////////////

void dmotTableText::SetCellBackColor( int row, int col, FXColor color ){
	( ( dmotTableTextItem* )getItem( row, col ) )->SetBackColor( color );
	updateItem( row, col );
}

FXTableItem *dmotTableText::createItem( const FXString &text, FXIcon *icon, void *ptr ){
	return new dmotTableTextItem( text, icon, ptr );
}
