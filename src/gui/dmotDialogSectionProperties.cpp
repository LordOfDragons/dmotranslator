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
#include "dmotDialogSectionProperties.h"
#include "../translation/dmotTranslationSection.h"
#include "../exceptions.h"



// Events
///////////

FXDEFMAP( dmotDialogSectionProperties ) dmotDialogSectionPropertiesMap[] = {
	FXMAPFUNC( SEL_COMMAND, dmotDialogSectionProperties::ID_BTN_PATHFILE, dmotDialogSectionProperties::onBtnPathFile ),
	FXMAPFUNC( SEL_COMMAND, dmotDialogSectionProperties::ID_BTN_ACCEPT, dmotDialogSectionProperties::onBtnAccept ),
};



// Class dmotDialogSectionProperties
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

FXIMPLEMENT( dmotDialogSectionProperties, FXDialogBox, dmotDialogSectionPropertiesMap, ARRAYNUMBER( dmotDialogSectionPropertiesMap ) )

dmotDialogSectionProperties::dmotDialogSectionProperties(){ }

dmotDialogSectionProperties::dmotDialogSectionProperties( FXWindow *owner, dmotWindowMain *windowMain, dmotTranslationSection *section ) :
FXDialogBox( owner, "Section Properties", DECOR_TITLE | DECOR_BORDER, 0, 0, 0, 0, 10, 10, 10, 5 ){
	if( ! windowMain || ! section ) THROW( dueInvalidParam );
	
	FXVerticalFrame *content;
	FXVerticalFrame *frameButtons;
	FXHorizontalFrame *frameLine;
	int blockPadding = 15;
	FXMatrix *frameBlock;
	const char *tooltip;
	FXLabel *label;
	
	pWindowMain = windowMain;
	pSection = section;
	
	// create content
	content = new FXVerticalFrame( this, LAYOUT_SIDE_TOP | LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20 );
	if( ! content ) THROW( dueOutOfMemory );
	
	frameBlock = new FXMatrix( content, 2, MATRIX_BY_COLUMNS | LAYOUT_SIDE_BOTTOM | LAYOUT_FILL_X );
	if( ! frameBlock ) THROW( dueOutOfMemory );
	
	tooltip = "Display name of the section";
	label = new FXLabel( frameBlock, "Name:", NULL, LABEL_NORMAL, 0, 0, 0, 0, 0, blockPadding );
	if( ! label ) THROW( dueOutOfMemory );
	label->setTipText( tooltip );
	pEditName = new FXTextField( frameBlock, 10, NULL, 0, FRAME_SUNKEN | LAYOUT_FILL_X | LAYOUT_FILL_COLUMN );
	if( ! pEditName ) THROW( dueOutOfMemory );
	pEditName->setTipText( tooltip );
	
	tooltip = "Byte position of the section inside the file";
	label = new FXLabel( frameBlock, "Section Offset:", NULL, LABEL_NORMAL, 0, 0, 0, 0, 0, blockPadding );
	if( ! label ) THROW( dueOutOfMemory );
	label->setTipText( tooltip );
	frameLine = new FXHorizontalFrame( frameBlock, LAYOUT_SIDE_TOP | LAYOUT_FILL_X | LAYOUT_FILL_COLUMN, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
	if( ! frameLine ) THROW( dueOutOfMemory );
	pEditSectionOffset = new FXTextField( frameLine, 5, NULL, 0, FRAME_SUNKEN );
	if( ! pEditSectionOffset ) THROW( dueOutOfMemory );
	pEditSectionOffset->setTipText( tooltip );
	
	tooltip = "Name of the table header displaying the record identifier";
	label = new FXLabel( frameBlock, "ID-Header Name:", NULL, LABEL_NORMAL, 0, 0, 0, 0, 0, blockPadding );
	if( ! label ) THROW( dueOutOfMemory );
	label->setTipText( tooltip );
	pEditHeaderIDName = new FXTextField( frameBlock, 15, NULL, 0, FRAME_SUNKEN | LAYOUT_FILL_X | LAYOUT_FILL_COLUMN );
	if( ! pEditHeaderIDName ) THROW( dueOutOfMemory );
	pEditHeaderIDName->setTipText( tooltip );
	
	/*
	tooltip = "Width in pixels of the table column displaying the record identifier";
	label = new FXLabel( frameBlock, "ID-Header Width:", NULL, LABEL_NORMAL, 0, 0, 0, 0, 0, blockPadding );
	if( ! label ) THROW( dueOutOfMemory );
	label->setTipText( tooltip );
	frameLine = new FXHorizontalFrame( frameBlock, LAYOUT_SIDE_TOP | LAYOUT_FILL_X | LAYOUT_FILL_COLUMN, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
	if( ! frameLine ) THROW( dueOutOfMemory );
	pEditHeaderIDWidth = new FXTextField( frameLine, 5, NULL, 0, FRAME_SUNKEN );
	if( ! pEditHeaderIDWidth ) THROW( dueOutOfMemory );
	pEditHeaderIDWidth->setTipText( tooltip );
	*/
	
	tooltip = "Byte position of the record identifier relative to the record byte position";
	label = new FXLabel( frameBlock, "ID Record Offset:", NULL, LABEL_NORMAL, 0, 0, 0, 0, 0, blockPadding );
	if( ! label ) THROW( dueOutOfMemory );
	label->setTipText( tooltip );
	frameLine = new FXHorizontalFrame( frameBlock, LAYOUT_SIDE_TOP | LAYOUT_FILL_X | LAYOUT_FILL_COLUMN, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
	if( ! frameLine ) THROW( dueOutOfMemory );
	pEditIDPosition = new FXTextField( frameLine, 5, NULL, 0, FRAME_SUNKEN );
	if( ! pEditIDPosition ) THROW( dueOutOfMemory );
	pEditIDPosition->setTipText( tooltip );
	
	tooltip = "Type of the record identifier";
	label = new FXLabel( frameBlock, "ID Type:", NULL, LABEL_NORMAL, 0, 0, 0, 0, 0, blockPadding );
	if( ! label ) THROW( dueOutOfMemory );
	label->setTipText( tooltip );
	frameLine = new FXHorizontalFrame( frameBlock, LAYOUT_SIDE_TOP | LAYOUT_FILL_X | LAYOUT_FILL_COLUMN, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
	if( ! frameLine ) THROW( dueOutOfMemory );
	pCBIDType = new FXComboBox( frameLine, 20, NULL, 0, FRAME_SUNKEN | LAYOUT_FILL_X | LAYOUT_FILL_COLUMN );
	if( ! pCBIDType ) THROW( dueOutOfMemory );
	pCBIDType->setEditable( FALSE );
	pCBIDType->setNumVisible( 2 );
	pCBIDType->setTipText( tooltip );
	pCBIDType->appendItem( "Unsigned Integer" );
	pCBIDType->appendItem( "Unsigned Short" );
	
	tooltip = "Size in bytes of an entire record";
	label = new FXLabel( frameBlock, "Record Size:", NULL, LABEL_NORMAL, 0, 0, 0, 0, 0, blockPadding );
	if( ! label ) THROW( dueOutOfMemory );
	label->setTipText( tooltip );
	frameLine = new FXHorizontalFrame( frameBlock, LAYOUT_SIDE_TOP | LAYOUT_FILL_X | LAYOUT_FILL_COLUMN, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
	if( ! frameLine ) THROW( dueOutOfMemory );
	pEditRecordSize = new FXTextField( frameLine, 5, NULL, 0, FRAME_SUNKEN );
	if( ! pEditRecordSize ) THROW( dueOutOfMemory );
	pEditRecordSize->setTipText( tooltip );
	
	tooltip = "Padding type of the record";
	label = new FXLabel( frameBlock, "Padding Type:", NULL, LABEL_NORMAL, 0, 0, 0, 0, 0, blockPadding );
	if( ! label ) THROW( dueOutOfMemory );
	label->setTipText( tooltip );
	frameLine = new FXHorizontalFrame( frameBlock, LAYOUT_SIDE_TOP | LAYOUT_FILL_X | LAYOUT_FILL_COLUMN, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
	if( ! frameLine ) THROW( dueOutOfMemory );
	pCBPaddingType = new FXComboBox( frameLine, 20, NULL, 0, FRAME_SUNKEN | LAYOUT_FILL_X | LAYOUT_FILL_COLUMN );
	if( ! pCBPaddingType ) THROW( dueOutOfMemory );
	pCBPaddingType->setEditable( FALSE );
	pCBPaddingType->setNumVisible( 5 );
	pCBPaddingType->setTipText( tooltip );
	pCBPaddingType->appendItem( "No Padding" );
	pCBPaddingType->appendItem( "Sequence of int32" );
	pCBPaddingType->appendItem( "Special NPC" );
	pCBPaddingType->appendItem( "Special Quest" );
	pCBPaddingType->appendItem( "Blocks of int32" );
	
	tooltip = "Number of padding elements";
	label = new FXLabel( frameBlock, "Padding Count:", NULL, LABEL_NORMAL, 0, 0, 0, 0, 0, blockPadding );
	if( ! label ) THROW( dueOutOfMemory );
	label->setTipText( tooltip );
	frameLine = new FXHorizontalFrame( frameBlock, LAYOUT_SIDE_TOP | LAYOUT_FILL_X | LAYOUT_FILL_COLUMN, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
	if( ! frameLine ) THROW( dueOutOfMemory );
	pEditPaddingCount = new FXTextField( frameLine, 5, NULL, 0, FRAME_SUNKEN );
	if( ! pEditPaddingCount ) THROW( dueOutOfMemory );
	pEditPaddingCount->setTipText( tooltip );
	
	
	
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

dmotDialogSectionProperties::~dmotDialogSectionProperties(){
}



// Management
///////////////

void dmotDialogSectionProperties::ResetValues(){
	FXString text;
	
	pEditName->setText( pSection->GetName() );
	text.format( "%i", pSection->GetSectionPosition() );
	pEditSectionOffset->setText( text );
	pEditHeaderIDName->setText( pSection->GetHeaderIDName() );
	//text.format( "%i", pSection->GetHeaderIDWidth() );
	//pEditHeaderIDWidth->setText( text );
	text.format( "%i", pSection->GetIdentifierPosition() );
	pEditIDPosition->setText( text );
	pCBIDType->setCurrentItem( pSection->GetIdentifierType() );
	text.format( "%i", pSection->GetRecordSize() );
	pEditRecordSize->setText( text );
	
	pCBPaddingType->setCurrentItem( pSection->GetPaddingType() );
	text.format( "%i", pSection->GetPaddingCount() );
	pEditPaddingCount->setText( text );
}

bool dmotDialogSectionProperties::ValidInput(){
	int sectionPosition = atoi( pEditSectionOffset->getText().text() );
	//int headerIDWidth = atoi( pEditHeaderIDWidth->getText().text() );
	int idPosition = atoi( pEditIDPosition->getText().text() );
	int recordSize = atoi( pEditRecordSize->getText().text() );
	int paddingCount = atoi( pEditPaddingCount->getText().text() );
	
	if( sectionPosition < 0 ){
		FXMessageBox::error( this, MBOX_OK, "Invalid Input", "Section Position has to be non-negative." );
		return false;
	}
	/*
	if( headerIDWidth < 0 ){
		FXMessageBox::error( this, MBOX_OK, "Invalid Input", "ID-Header Width has to be non-negative." );
		return false;
	}
	*/
	if( idPosition < 0 ){
		FXMessageBox::error( this, MBOX_OK, "Invalid Input", "Identifier Position has to be non-negative." );
		return false;
	}
	if( recordSize < 4 ){
		FXMessageBox::error( this, MBOX_OK, "Invalid Input", "Record Size has to be at least 4 bytes." );
		return false;
	}
	if( paddingCount < 0 ){
		FXMessageBox::error( this, MBOX_OK, "Invalid Input", "Padding Count has to be non-negative." );
		return false;
	}
	
	return true;
}

void dmotDialogSectionProperties::ApplyChanges(){
	int sectionPosition = atoi( pEditSectionOffset->getText().text() );
	//int headerIDWidth = atoi( pEditHeaderIDWidth->getText().text() );
	int idPosition = atoi( pEditIDPosition->getText().text() );
	int idType = pCBIDType->getCurrentItem();
	int recordSize = atoi( pEditRecordSize->getText().text() );
	int paddingCount = atoi( pEditPaddingCount->getText().text() );
	int paddingType = pCBPaddingType->getCurrentItem();
	
	if( idType == -1 ){
		idType = dmotTranslationSection::eidtUInt;
	}
	if( paddingType == -1 ){
		paddingType = dmotTranslationSection::eptNoPadding;
	}
	
	pSection->SetName( pEditName->getText() );
	pSection->SetSectionPosition( sectionPosition );
	pSection->SetHeaderIDName( pEditHeaderIDName->getText() );
	//pSection->SetHeaderIDWidth( headerIDWidth );
	pSection->SetIdentifierPosition( idPosition );
	pSection->SetIdentifierType( idType );
	pSection->SetRecordSize( recordSize );
	pSection->SetPaddingType( paddingType );
	pSection->SetPaddingCount( paddingCount );
}



// Events
///////////

long dmotDialogSectionProperties::onBtnPathFile( FXObject *sender, FXSelector selector, void *data ){
	return 0;
}



long dmotDialogSectionProperties::onBtnAccept( FXObject *sender, FXSelector selector, void *data ){
	if( ValidInput() ){
		ApplyChanges();
		return handle( this, FXSEL( SEL_COMMAND, ID_ACCEPT ), NULL );
	}
	
	return 0;
}
