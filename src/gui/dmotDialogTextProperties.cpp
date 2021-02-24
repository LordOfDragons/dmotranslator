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
#include "dmotDialogTextProperties.h"
#include "../translation/dmotTranslationSectionText.h"
#include "../exceptions.h"



// Events
///////////

FXDEFMAP( dmotDialogTextProperties ) dmotDialogTextPropertiesMap[] = {
	FXMAPFUNC( SEL_COMMAND, dmotDialogTextProperties::ID_BTN_ACCEPT, dmotDialogTextProperties::onBtnAccept ),
};



// Class dmotDialogTextProperties
///////////////////////////////////

// Constructor, destructor
////////////////////////////

FXIMPLEMENT( dmotDialogTextProperties, FXDialogBox, dmotDialogTextPropertiesMap, ARRAYNUMBER( dmotDialogTextPropertiesMap ) )

dmotDialogTextProperties::dmotDialogTextProperties(){ }

dmotDialogTextProperties::dmotDialogTextProperties( FXWindow *owner, dmotWindowMain *windowMain, dmotTranslationSectionText *text ) :
FXDialogBox( owner, "Text Properties", DECOR_TITLE | DECOR_BORDER, 0, 0, 0, 0, 10, 10, 10, 5 ){
	if( ! windowMain || ! text ) THROW( dueInvalidParam );
	
	FXVerticalFrame *content;
	FXVerticalFrame *frameButtons;
	FXHorizontalFrame *frameLine;
	int blockPadding = 15;
	FXMatrix *frameBlock;
	const char *tooltip;
	FXLabel *label;
	
	pWindowMain = windowMain;
	pText = text;
	
	// create content
	content = new FXVerticalFrame( this, LAYOUT_SIDE_TOP | LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20 );
	if( ! content ) THROW( dueOutOfMemory );
	
	frameBlock = new FXMatrix( content, 2, MATRIX_BY_COLUMNS | LAYOUT_SIDE_BOTTOM | LAYOUT_FILL_X );
	if( ! frameBlock ) THROW( dueOutOfMemory );
	
	tooltip = "Name of the header displaying the text";
	label = new FXLabel( frameBlock, "Header Name:", NULL, LABEL_NORMAL, 0, 0, 0, 0, 0, blockPadding );
	if( ! label ) THROW( dueOutOfMemory );
	label->setTipText( tooltip );
	pEditHeaderName = new FXTextField( frameBlock, 15, NULL, 0, FRAME_SUNKEN | LAYOUT_FILL_X | LAYOUT_FILL_COLUMN );
	if( ! pEditHeaderName ) THROW( dueOutOfMemory );
	pEditHeaderName->setTipText( tooltip );
	
	/*
	tooltip = "Width in pixels of the table column displaying the text";
	label = new FXLabel( frameBlock, "Header Width:", NULL, LABEL_NORMAL, 0, 0, 0, 0, 0, blockPadding );
	if( ! label ) THROW( dueOutOfMemory );
	label->setTipText( tooltip );
	frameLine = new FXHorizontalFrame( frameBlock, LAYOUT_SIDE_TOP | LAYOUT_FILL_X | LAYOUT_FILL_COLUMN, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
	if( ! frameLine ) THROW( dueOutOfMemory );
	pEditHeaderWidth = new FXTextField( frameLine, 5, NULL, 0, FRAME_SUNKEN );
	if( ! pEditHeaderWidth ) THROW( dueOutOfMemory );
	pEditHeaderWidth->setTipText( tooltip );
	*/
	
	tooltip = "Byte position of the text relative to the record byte position";
	label = new FXLabel( frameBlock, "Byte Position:", NULL, LABEL_NORMAL, 0, 0, 0, 0, 0, blockPadding );
	if( ! label ) THROW( dueOutOfMemory );
	label->setTipText( tooltip );
	frameLine = new FXHorizontalFrame( frameBlock, LAYOUT_SIDE_TOP | LAYOUT_FILL_X | LAYOUT_FILL_COLUMN, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
	if( ! frameLine ) THROW( dueOutOfMemory );
	pEditPosition = new FXTextField( frameLine, 5, NULL, 0, FRAME_SUNKEN );
	if( ! pEditPosition ) THROW( dueOutOfMemory );
	pEditPosition->setTipText( tooltip );
	
	tooltip = "Maximal number of 16-bit characters (2 bytes each) of the text";
	label = new FXLabel( frameBlock, "Characters:", NULL, LABEL_NORMAL, 0, 0, 0, 0, 0, blockPadding );
	if( ! label ) THROW( dueOutOfMemory );
	label->setTipText( tooltip );
	frameLine = new FXHorizontalFrame( frameBlock, LAYOUT_SIDE_TOP | LAYOUT_FILL_X | LAYOUT_FILL_COLUMN, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
	if( ! frameLine ) THROW( dueOutOfMemory );
	pEditLength = new FXTextField( frameLine, 5, NULL, 0, FRAME_SUNKEN );
	if( ! pEditLength ) THROW( dueOutOfMemory );
	pEditLength->setTipText( tooltip );
	
	tooltip = "Format of the text";
	label = new FXLabel( frameBlock, "Format:", NULL, LABEL_NORMAL, 0, 0, 0, 0, 0, blockPadding );
	if( ! label ) THROW( dueOutOfMemory );
	label->setTipText( tooltip );
	frameLine = new FXHorizontalFrame( frameBlock, LAYOUT_SIDE_TOP | LAYOUT_FILL_X | LAYOUT_FILL_COLUMN, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
	if( ! frameLine ) THROW( dueOutOfMemory );
	pCBFormat = new FXComboBox( frameLine, 20, NULL, 0, FRAME_SUNKEN | LAYOUT_FILL_X | LAYOUT_FILL_COLUMN );
	if( ! pCBFormat ) THROW( dueOutOfMemory );
	pCBFormat->setEditable( FALSE );
	pCBFormat->setNumVisible( 4 );
	pCBFormat->setTipText( tooltip );
	pCBFormat->appendItem( "Wide Char" );
	pCBFormat->appendItem( "UTF-8" );
	pCBFormat->appendItem( "0-Terminated Wide Char" );
	pCBFormat->appendItem( "0-Terminated UTF-8" );
	
	
	
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

dmotDialogTextProperties::~dmotDialogTextProperties(){
}



// Management
///////////////

void dmotDialogTextProperties::ResetValues(){
	FXString text;
	
	pEditHeaderName->setText( pText->GetHeaderName() );
	//text.format( "%i", pText->GetHeaderWidth() );
	//pEditHeaderWidth->setText( text );
	text.format( "%i", pText->GetPosition() );
	pEditPosition->setText( text );
	text.format( "%i", pText->GetLength() );
	pEditLength->setText( text );
	pCBFormat->setCurrentItem( pText->GetFormat() );
}

bool dmotDialogTextProperties::ValidInput(){
	//int headerWidth = atoi( pEditHeaderWidth->getText().text() );
	int position = atoi( pEditPosition->getText().text() );
	int length = atoi( pEditLength->getText().text() );
	
	/*
	if( headerWidth < 0 ){
		FXMessageBox::error( this, MBOX_OK, "Invalid Input", "Header Width has to be non-negative." );
		return false;
	}
	*/
	if( position < 0 ){
		FXMessageBox::error( this, MBOX_OK, "Invalid Input", "Byte Position has to be non-negative." );
		return false;
	}
	if( length < 1 ){
		FXMessageBox::error( this, MBOX_OK, "Invalid Input", "Characters has to be at least 1." );
		return false;
	}
	
	return true;
}

void dmotDialogTextProperties::ApplyChanges(){
	//int headerWidth = atoi( pEditHeaderWidth->getText().text() );
	int position = atoi( pEditPosition->getText().text() );
	int length = atoi( pEditLength->getText().text() );
	
	pText->SetHeaderName( pEditHeaderName->getText() );
	//pText->SetHeaderWidth( headerWidth );
	pText->SetPosition( position );
	pText->SetLength( length );
	pText->SetFormat( pCBFormat->getCurrentItem() );
}



// Events
///////////

long dmotDialogTextProperties::onBtnAccept( FXObject *sender, FXSelector selector, void *data ){
	if( ValidInput() ){
		ApplyChanges();
		return handle( this, FXSEL( SEL_COMMAND, ID_ACCEPT ), NULL );
	}
	
	return 0;
}
