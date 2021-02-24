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
#include "dmotDialogFileProperties.h"
#include "../translation/dmotTranslationFile.h"
#include "../exceptions.h"



// Events
///////////

FXDEFMAP( dmotDialogFileProperties ) dmotDialogFilePropertiesMap[] = {
	FXMAPFUNC( SEL_COMMAND, dmotDialogFileProperties::ID_BTN_ACCEPT, dmotDialogFileProperties::onBtnAccept ),
};



// Class dmotDialogFileProperties
///////////////////////////////////

// Constructor, destructor
////////////////////////////

FXIMPLEMENT( dmotDialogFileProperties, FXDialogBox, dmotDialogFilePropertiesMap, ARRAYNUMBER( dmotDialogFilePropertiesMap ) )

dmotDialogFileProperties::dmotDialogFileProperties(){ }

dmotDialogFileProperties::dmotDialogFileProperties( FXWindow *owner, dmotWindowMain *windowMain, dmotTranslationFile *file ) :
FXDialogBox( owner, "File Properties", DECOR_TITLE | DECOR_BORDER, 0, 0, 0, 0, 10, 10, 10, 5 ){
	if( ! windowMain || ! file ) THROW( dueInvalidParam );
	
	FXVerticalFrame *content;
	FXVerticalFrame *frameButtons;
	FXHorizontalFrame *frameLine;
	int blockPadding = 15;
	FXMatrix *frameBlock;
	const char *tooltip;
	FXLabel *label;
	
	pWindowMain = windowMain;
	pFile = file;
	
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
	
	tooltip = "Identifier of the file in the archive file map";
	label = new FXLabel( frameBlock, "Identifier:", NULL, LABEL_NORMAL, 0, 0, 0, 0, 0, blockPadding );
	if( ! label ) THROW( dueOutOfMemory );
	label->setTipText( tooltip );
	frameLine = new FXHorizontalFrame( frameBlock, LAYOUT_SIDE_TOP | LAYOUT_FILL_X | LAYOUT_FILL_COLUMN, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
	if( ! frameLine ) THROW( dueOutOfMemory );
	pEditIdentifier = new FXTextField( frameLine, 12, NULL, 0, FRAME_SUNKEN );
	if( ! pEditIdentifier ) THROW( dueOutOfMemory );
	pEditIdentifier->setTipText( tooltip );
	
	
	
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

dmotDialogFileProperties::~dmotDialogFileProperties(){
}



// Management
///////////////

void dmotDialogFileProperties::ResetValues(){
	FXString text;
	
	pEditName->setText( pFile->GetName() );
	text.format( "%u", pFile->GetIdentifier() );
	pEditIdentifier->setText( text );
}

bool dmotDialogFileProperties::ValidInput(){
	return true;
}

void dmotDialogFileProperties::ApplyChanges(){
	FXuint identifier = FXUIntVal( pEditIdentifier->getText(), 10 );
	
	pFile->SetName( pEditName->getText() );
	pFile->SetIdentifier( identifier );
}



// Events
///////////

long dmotDialogFileProperties::onBtnAccept( FXObject *sender, FXSelector selector, void *data ){
	if( ValidInput() ){
		ApplyChanges();
		return handle( this, FXSEL( SEL_COMMAND, ID_ACCEPT ), NULL );
	}
	
	return 0;
}
