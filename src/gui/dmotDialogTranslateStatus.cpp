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

#include "dmotWindowMain.h"
#include "dmotDialogTranslateStatus.h"
#include "../file/dmotTranslateStatus.h"
#include "../file/dmotTranslateStatusEntry.h"
#include "../translation/dmotTranslationArchive.h"
#include "../translation/dmotTranslationFile.h"
#include "../exceptions.h"



// Events
///////////

FXDEFMAP( dmotDialogTranslateStatus ) dmotDialogTranslateStatusMap[] = { };



// Class dmotDialogTranslateStatus
///////////////////////////////////

// Constructor, destructor
////////////////////////////

FXIMPLEMENT( dmotDialogTranslateStatus, FXDialogBox, dmotDialogTranslateStatusMap, ARRAYNUMBER( dmotDialogTranslateStatusMap ) )

dmotDialogTranslateStatus::dmotDialogTranslateStatus(){ }

dmotDialogTranslateStatus::dmotDialogTranslateStatus( FXWindow *owner, dmotWindowMain *windowMain, dmotTranslateStatus *log ) :
FXDialogBox( owner, "Status", DECOR_TITLE | DECOR_BORDER | DECOR_RESIZE | DECOR_STRETCHABLE | DECOR_CLOSE, 0, 0, 800, 400, 10, 10, 10, 5 ){
	if( ! windowMain || ! log ) THROW( dueInvalidParam );
	
	FXVerticalFrame *content;
	FXVerticalFrame *frameButtons;
	FXHorizontalFrame *frameLine;
	
	pWindowMain = windowMain;
	pStatus = log;
	
	// create content
	content = new FXVerticalFrame( this, LAYOUT_SIDE_TOP | LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20 );
	if( ! content ) THROW( dueOutOfMemory );
	
	pTblEntries = new FXTable( content, NULL, 0, TABLE_READONLY | TABLE_NO_COLSELECT | TABLE_COL_SIZABLE | LAYOUT_FILL_X | LAYOUT_FILL_Y );
	pTblEntries->setTableSize( 0, 3 );
	pTblEntries->setRowHeaderWidth( 0 );
	pTblEntries->setColumnText( 0, "Archive" );
	pTblEntries->setColumnWidth( 0, 140 );
	pTblEntries->setColumnJustify( 0, FXHeaderItem::CENTER_X );
	pTblEntries->setColumnText( 1, "File" );
	pTblEntries->setColumnWidth( 1, 140 );
	pTblEntries->setColumnJustify( 1, FXHeaderItem::CENTER_X );
	pTblEntries->setColumnText( 2, "Status" );
	pTblEntries->setColumnWidth( 2, 480 );
	pTblEntries->setColumnJustify( 2, FXHeaderItem::CENTER_X );
	
	// buttons below
	frameButtons = new FXVerticalFrame( content, LAYOUT_SIDE_TOP | LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5 );
	if( ! frameButtons ) THROW( dueOutOfMemory );
	
	new FXSeparator( frameButtons );
	
	frameLine = new FXHorizontalFrame( frameButtons, LAYOUT_CENTER_X, 0, 0, 0, 0, 0, 0, 0, 0, 20, 0 );
	if( ! frameLine ) THROW( dueOutOfMemory );
	new FXButton( frameLine, "Close", NULL, this, ID_CANCEL, BUTTON_DEFAULT | BUTTON_INITIAL | LAYOUT_CENTER_X
		| FRAME_RAISED | JUSTIFY_NORMAL | ICON_BEFORE_TEXT, 0, 0, 0, 0, 30, 30 );
	
	// update table
	UpdateEntries();
}

dmotDialogTranslateStatus::~dmotDialogTranslateStatus(){
}



// Management
///////////////

void dmotDialogTranslateStatus::UpdateEntries(){
	const int count = pStatus->GetEntryCount();
	int e;
	
	pTblEntries->removeRows( 0, pTblEntries->getNumRows() );
	pTblEntries->insertRows( 0, count );
	
	for( e=0; e<count; e++ ){
		const dmotTranslateStatusEntry &entry = *pStatus->GetEntryAt( e );
		
		if( entry.GetArchive() ){
			pTblEntries->setItemText( e, 0, entry.GetArchive()->GetName() );
			
		}else{
			pTblEntries->setItemText( e, 0, "-" );
		}
		
		if( entry.GetFile() ){
			pTblEntries->setItemText( e, 1, entry.GetFile()->GetName() );
			
		}else{
			pTblEntries->setItemText( e, 1, "-" );
		}
		
		pTblEntries->setItemText( e, 2, entry.GetText() );
		
		pTblEntries->setItemJustify( e, 0, FXTableItem::LEFT );
		pTblEntries->setItemJustify( e, 1, FXTableItem::LEFT );
		pTblEntries->setItemJustify( e, 2, FXTableItem::LEFT );
	}
	
	if( count > 0 ){
		pTblEntries->fitColumnsToContents( 0, 3 );
		pTblEntries->fitRowsToContents( 0, count );
		
		pTblEntries->setColumnWidth( 0, pTblEntries->getColumnWidth( 0 ) + 5 );
		pTblEntries->setColumnWidth( 1, pTblEntries->getColumnWidth( 1 ) + 5 );
		pTblEntries->setColumnWidth( 2, pTblEntries->getColumnWidth( 2 ) + 5 );
	}
}
