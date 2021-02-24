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
#include "dmotDialogArchiveProperties.h"
#include "../translation/dmotTranslationArchive.h"
#include "../exceptions.h"



// Events
///////////

FXDEFMAP( dmotDialogArchiveProperties ) dmotDialogArchivePropertiesMap[] = {
	FXMAPFUNC( SEL_COMMAND, dmotDialogArchiveProperties::ID_BTN_PATHARCHIVE, dmotDialogArchiveProperties::onBtnPathArchive ),
	FXMAPFUNC( SEL_COMMAND, dmotDialogArchiveProperties::ID_BTN_PATHFILEMAP, dmotDialogArchiveProperties::onBtnPathFileMap ),
	FXMAPFUNC( SEL_COMMAND, dmotDialogArchiveProperties::ID_BTN_ACCEPT, dmotDialogArchiveProperties::onBtnAccept ),
};



// Class dmotDialogArchiveProperties
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

FXIMPLEMENT( dmotDialogArchiveProperties, FXDialogBox, dmotDialogArchivePropertiesMap, ARRAYNUMBER( dmotDialogArchivePropertiesMap ) )

dmotDialogArchiveProperties::dmotDialogArchiveProperties(){ }

dmotDialogArchiveProperties::dmotDialogArchiveProperties( FXWindow *owner, dmotWindowMain *windowMain, dmotTranslationArchive *file ) :
FXDialogBox( owner, "Archive Properties", DECOR_TITLE | DECOR_BORDER, 0, 0, 0, 0, 10, 10, 10, 5 ){
	if( ! windowMain || ! file ) THROW( dueInvalidParam );
	
	FXVerticalFrame *content;
	FXVerticalFrame *frameButtons;
	FXHorizontalFrame *frameLine;
	int blockPadding = 15;
	FXMatrix *frameBlock;
	const char *tooltip;
	FXLabel *label;
	
	pWindowMain = windowMain;
	pArchive = file;
	
	// create content
	content = new FXVerticalFrame( this, LAYOUT_SIDE_TOP | LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20 );
	if( ! content ) THROW( dueOutOfMemory );
	
	frameBlock = new FXMatrix( content, 2, MATRIX_BY_COLUMNS | LAYOUT_SIDE_BOTTOM | LAYOUT_FILL_X );
	if( ! frameBlock ) THROW( dueOutOfMemory );
	
	tooltip = "Display name of the file";
	label = new FXLabel( frameBlock, "Name:", NULL, LABEL_NORMAL, 0, 0, 0, 0, 0, blockPadding );
	if( ! label ) THROW( dueOutOfMemory );
	label->setTipText( tooltip );
	pEditName = new FXTextField( frameBlock, 10, NULL, 0, FRAME_SUNKEN | LAYOUT_FILL_X | LAYOUT_FILL_COLUMN );
	if( ! pEditName ) THROW( dueOutOfMemory );
	pEditName->setTipText( tooltip );
	
	tooltip = "Path to the archive file (*.pf) relative to the game directory";
	label = new FXLabel( frameBlock, "Archive:", NULL, LABEL_NORMAL, 0, 0, 0, 0, 0, blockPadding );
	if( ! label ) THROW( dueOutOfMemory );
	label->setTipText( tooltip );
	frameLine = new FXHorizontalFrame( frameBlock, LAYOUT_SIDE_TOP | LAYOUT_FILL_X | LAYOUT_FILL_COLUMN, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
	if( ! frameLine ) THROW( dueOutOfMemory );
	pEditPathArchive = new FXTextField( frameLine, 30, NULL, 0, FRAME_SUNKEN | LAYOUT_FILL_X );
	if( ! pEditPathArchive ) THROW( dueOutOfMemory );
	pEditPathArchive->setTipText( tooltip );
	pBtnPathArchive = new FXButton( frameLine, "...", NULL, this, ID_BTN_PATHARCHIVE, FRAME_RAISED | JUSTIFY_NORMAL | ICON_BEFORE_TEXT );
	if( ! pBtnPathArchive ) THROW( dueOutOfMemory );
	
	tooltip = "Path to the file map file (*.hf) relative to the game directory";
	label = new FXLabel( frameBlock, "File Map:", NULL, LABEL_NORMAL, 0, 0, 0, 0, 0, blockPadding );
	if( ! label ) THROW( dueOutOfMemory );
	label->setTipText( tooltip );
	frameLine = new FXHorizontalFrame( frameBlock, LAYOUT_SIDE_TOP | LAYOUT_FILL_X | LAYOUT_FILL_COLUMN, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
	if( ! frameLine ) THROW( dueOutOfMemory );
	pEditPathFileMap = new FXTextField( frameLine, 30, NULL, 0, FRAME_SUNKEN | LAYOUT_FILL_X );
	if( ! pEditPathFileMap ) THROW( dueOutOfMemory );
	pEditPathFileMap->setTipText( tooltip );
	pBtnPathFileMap = new FXButton( frameLine, "...", NULL, this, ID_BTN_PATHFILEMAP, FRAME_RAISED | JUSTIFY_NORMAL | ICON_BEFORE_TEXT );
	if( ! pBtnPathFileMap ) THROW( dueOutOfMemory );
	
	
	
	// buttons below
	frameButtons = new FXVerticalFrame( content, LAYOUT_SIDE_TOP | LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5 );
	if( ! frameButtons ) THROW( dueOutOfMemory );
	
	new FXSeparator( frameButtons );
	
	frameLine = new FXHorizontalFrame( frameButtons, LAYOUT_CENTER_X, 0, 0, 0, 0, 0, 0, 0, 0, 20, 0 );
	if( ! frameLine ) THROW( dueOutOfMemory );
	pBtnAccept = new FXButton( frameLine, "Accept", NULL, this, ID_BTN_ACCEPT, BUTTON_DEFAULT | BUTTON_INITIAL | LAYOUT_CENTER_X | FRAME_RAISED | JUSTIFY_NORMAL | ICON_BEFORE_TEXT, 0, 0, 0, 0, 30, 30 );
	if( ! pBtnAccept ) THROW( dueOutOfMemory );
	pBtnCancel = new FXButton( frameLine, "Cancel", NULL, this, ID_CANCEL, LAYOUT_CENTER_X | FRAME_RAISED | JUSTIFY_NORMAL | ICON_BEFORE_TEXT, 0, 0, 0, 0, 30, 30 );
	if( ! pBtnCancel ) THROW( dueOutOfMemory );
	
	// reset values
	ResetValues();
}

dmotDialogArchiveProperties::~dmotDialogArchiveProperties(){
}



// Management
///////////////

void dmotDialogArchiveProperties::ResetValues(){
	FXString text;
	
	pEditName->setText( pArchive->GetName() );
	pEditPathArchive->setText( pArchive->GetPathArchive() );
	pEditPathFileMap->setText( pArchive->GetPathFileMap() );
}

bool dmotDialogArchiveProperties::ValidInput(){
	return true;
}

void dmotDialogArchiveProperties::ApplyChanges(){
	pArchive->SetName( pEditName->getText() );
	pArchive->SetPathArchive( pEditPathArchive->getText() );
	pArchive->SetPathFileMap( pEditPathFileMap->getText() );
}



// Events
///////////

long dmotDialogArchiveProperties::onBtnPathArchive( FXObject *sender, FXSelector selector, void *data ){
	return 0;
}

long dmotDialogArchiveProperties::onBtnPathFileMap( FXObject *sender, FXSelector selector, void *data ){
	return 0;
}



long dmotDialogArchiveProperties::onBtnAccept( FXObject *sender, FXSelector selector, void *data ){
	if( ValidInput() ){
		ApplyChanges();
		return handle( this, FXSEL( SEL_COMMAND, ID_ACCEPT ), NULL );
	}
	
	return 0;
}
