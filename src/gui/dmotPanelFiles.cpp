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

#include "dmotPanelFiles.h"
#include "dmotWindowMain.h"
#include "dmotTableText.h"
#include "../translation/dmotTranslation.h"
#include "../translation/dmotTranslationArchive.h"
#include "../translation/dmotTranslationFile.h"
#include "../translation/dmotTranslationSection.h"
#include "../translation/dmotTranslationSectionText.h"
#include "../translation/dmotTranslationRecord.h"
#include "../exceptions.h"



// Events
///////////

FXDEFMAP( dmotPanelFiles ) dmotPanelFilesMap[]={
	FXMAPFUNC( SEL_COMMAND, dmotPanelFiles::ID_CB_ARCHIVE, dmotPanelFiles::onCBArchiveChanged ),
	FXMAPFUNC( SEL_COMMAND, dmotPanelFiles::ID_CB_FILE, dmotPanelFiles::onCBFileChanged ),
	FXMAPFUNC( SEL_COMMAND, dmotPanelFiles::ID_CB_SECTION, dmotPanelFiles::onCBSectionChanged ),
	FXMAPFUNC( SEL_REPLACED, dmotPanelFiles::ID_TABLE_TEXT, dmotPanelFiles::onTableTextChanged ),
	
	FXMAPFUNC( SEL_CHANGED, FXTable::ID_SELECT_COLUMN_INDEX, dmotPanelFiles::onTableTextHeaderResized ),
	FXMAPFUNC( SEL_COMMAND, FXTable::ID_SELECT_COLUMN_INDEX, dmotPanelFiles::onTableTextHeaderForward ),
};



// Class dmotPanelFiles
/////////////////////////////

FXIMPLEMENT( dmotPanelFiles, FXVerticalFrame, dmotPanelFilesMap, ARRAYNUMBER( dmotPanelFilesMap ) )

// Constructor, destructor
////////////////////////////

dmotPanelFiles::dmotPanelFiles(){ }

dmotPanelFiles::dmotPanelFiles( dmotWindowMain *windowMain, FXComposite *container ) :
FXVerticalFrame( container, LAYOUT_FILL_X | LAYOUT_FILL_Y | LAYOUT_SIDE_LEFT | FRAME_SUNKEN, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ){
	FXHorizontalFrame *frameLine;
	FXLabel *label;
	
	pWindowMain = windowMain;
	
	// create content
	frameLine = new FXHorizontalFrame( this, LAYOUT_SIDE_TOP | LAYOUT_FILL_X );
	if( ! frameLine ) THROW( dueOutOfMemory );
	
	label = new FXLabel( frameLine, "Archive:", NULL, LABEL_NORMAL );
	if( ! label ) THROW( dueOutOfMemory );
	label->setTipText( "Select archive to show translations for" );
	
	pCBArchive = new FXComboBox( frameLine, 10, this, ID_CB_ARCHIVE, FRAME_SUNKEN | LAYOUT_FILL_X | LAYOUT_FILL_COLUMN );
	if( ! pCBArchive ) THROW( dueOutOfMemory );
	pCBArchive->setEditable( FALSE );
	pCBArchive->setNumVisible( 10 );
	pCBArchive->setTipText( "Select archive to show translations for" );
	
	label = new FXLabel( frameLine, "File:", NULL, LABEL_NORMAL );
	if( ! label ) THROW( dueOutOfMemory );
	label->setTipText( "Select file to show translations for" );
	
	pCBFile = new FXComboBox( frameLine, 10, this, ID_CB_FILE, FRAME_SUNKEN | LAYOUT_FILL_X | LAYOUT_FILL_COLUMN );
	if( ! pCBFile ) THROW( dueOutOfMemory );
	pCBFile->setEditable( FALSE );
	pCBFile->setNumVisible( 10 );
	pCBFile->setTipText( "Select file to show translations for" );
	
	label = new FXLabel( frameLine, "Section:", NULL, LABEL_NORMAL );
	if( ! label ) THROW( dueOutOfMemory );
	label->setTipText( "Select section to show translations for" );
	
	pCBSection = new FXComboBox( frameLine, 10, this, ID_CB_SECTION, FRAME_SUNKEN | LAYOUT_FILL_X | LAYOUT_FILL_COLUMN );
	if( ! pCBSection ) THROW( dueOutOfMemory );
	pCBSection->setEditable( FALSE );
	pCBSection->setNumVisible( 10 );
	pCBSection->setTipText( "Select section to show translations for" );
	
	pTableText = new dmotTableText( this, this, ID_TABLE_TEXT, TABLE_COL_SIZABLE | TABLE_NO_COLSELECT
		| TABLE_NO_ROWSELECT | LAYOUT_FILL_X | LAYOUT_FILL_Y );
	if( ! pTableText ) THROW( dueOutOfMemory );
	pTableText->getRowHeader()->setWidth( 0 );
	
	pTableText->getColumnHeader()->setTarget( this );
}

dmotPanelFiles::~dmotPanelFiles(){
}



// Management
///////////////

dmotTranslationArchive *dmotPanelFiles::GetSelectedArchive() const{
	int selection = pCBArchive->getCurrentItem();
	
	if( selection != -1 ){
		return ( dmotTranslationArchive* )pCBArchive->getItemData( selection );
	}
	
	return NULL;
}

void dmotPanelFiles::SelectLatestArchive(){
	dmotTranslation *translation = pWindowMain->GetTranslation();
	
	if( translation && translation->GetArchiveCount() > 0 ){
		int selection = pCBArchive->findItemByData( translation->GetArchiveAt( translation->GetArchiveCount() - 1 ) );
		
		if( selection != -1 ){
			pCBArchive->setCurrentItem( selection );
			UpdateFileList();
		}
	}
}

dmotTranslationFile *dmotPanelFiles::GetSelectedFile() const{
	dmotTranslationArchive *archive = GetSelectedArchive();
	dmotTranslationFile *file = NULL;
	
	if( archive ){
		int selection = pCBFile->getCurrentItem();
		
		if( selection != -1 ){
			return ( dmotTranslationFile* )pCBFile->getItemData( selection );
		}
	}
	
	return file;
}

void dmotPanelFiles::SelectLatestFile(){
	dmotTranslationArchive *archive = GetSelectedArchive();
	
	if( archive && archive->GetFileCount() > 0 ){
		int selection = pCBFile->findItemByData( archive->GetFileAt( archive->GetFileCount() - 1 ) );
		
		if( selection != -1 ){
			pCBFile->setCurrentItem( selection );
			UpdateSectionList();
		}
	}
}

dmotTranslationSection *dmotPanelFiles::GetSelectedSection() const{
	dmotTranslationFile *file = GetSelectedFile();
	dmotTranslationSection *section = NULL;
	
	if( file ){
		int selection = pCBSection->getCurrentItem();
		
		if( selection != -1 ){
			return ( dmotTranslationSection* )pCBSection->getItemData( selection );
		}
	}
	
	return section;
}

void dmotPanelFiles::SelectLatestSection(){
	dmotTranslationFile *file = GetSelectedFile();
	
	if( file && file->GetSectionCount() > 0 ){
		int selection = pCBSection->findItemByData( file->GetSectionAt( file->GetSectionCount() - 1 ) );
		
		if( selection != -1 ){
			pCBSection->setCurrentItem( selection );
			UpdateTextTable();
		}
	}
}

int dmotPanelFiles::GetCurrentTextIndex() const{
	dmotTranslationSection *section = GetSelectedSection();
	
	if( section ){
		int col = pTableText->getCurrentColumn();
		
		if( col > 0 && col <= section->GetTextCount() ){
			return col - 1;
		}
	}
	
	return -1;
}

int dmotPanelFiles::GetSelectedRecord() const{
	dmotTranslationSection *section = GetSelectedSection();
	
	if( section ){
		int row = pTableText->getCurrentRow();
		
		if( row >= 0 && row < section->GetRecordCount() ){
			return row;
		}
	}
	
	return -1;
}

void dmotPanelFiles::GetSelectedRecordRange( int &from, int &to ) const{
	dmotTranslationSection *section = GetSelectedSection();
	
	from = -1;
	to = -1;
	
	if( section ){
		from = pTableText->getSelStartRow();
		to = pTableText->getSelEndRow();
	}
}

void dmotPanelFiles::UpdateArchiveList(){
	dmotTranslation *translation = pWindowMain->GetTranslation();
	dmotTranslationArchive *activeArchive = GetSelectedArchive();
	bool updateFiles = false;
	int i, count = 0;
	
	pCBArchive->clearItems();
	
	if( translation ){
		count = translation->GetArchiveCount();
		dmotTranslationArchive *archive;
		
		for( i=0; i<count; i++ ){
			archive = translation->GetArchiveAt( i );
			pCBArchive->appendItem( archive->GetName(), archive );
		}
	}
	
	i = pCBArchive->findItemByData( activeArchive );
	
	if( i == -1 && count > 0 ){
		i = 0;
		updateFiles = true;
	}
	
	pCBArchive->setCurrentItem( i );
	
	if( updateFiles ){
		UpdateFileList();
	}
}

void dmotPanelFiles::UpdateFileList(){
	dmotTranslationArchive *archive = GetSelectedArchive();
	dmotTranslationFile *activeFile = GetSelectedFile();
	bool updateSections = false;
	int i, count = 0;
	
	pCBFile->clearItems();
	
	if( archive ){
		count = archive->GetFileCount();
		dmotTranslationFile *file;
		
		for( i=0; i<count; i++ ){
			file = archive->GetFileAt( i );
			pCBFile->appendItem( file->GetName(), file );
		}
	}
	
	i = pCBFile->findItemByData( activeFile );
	
	if( i == -1 && count > 0 ){
		i = 0;
		updateSections = true;
	}
	
	pCBFile->setCurrentItem( i );
	
	if( updateSections ){
		UpdateSectionList();
	}
}

void dmotPanelFiles::UpdateSectionList(){
	dmotTranslationSection *activeSection = GetSelectedSection();
	dmotTranslationFile *file = GetSelectedFile();
	bool updateText = false;
	int i, count = 0;
	
	pCBSection->clearItems();
	
	if( file ){
		count = file->GetSectionCount();
		dmotTranslationSection *section;
		
		for( i=0; i<count; i++ ){
			section = file->GetSectionAt( i );
			pCBSection->appendItem( section->GetName(), section );
		}
	}
	
	i = pCBSection->findItemByData( activeSection );
	
	if( i == -1 && count > 0 ){
		i = 0;
		updateText = true;
	}
	
	pCBSection->setCurrentItem( i );
	
	if( updateText ) UpdateTextTable();
}

void dmotPanelFiles::UpdateTextTable(){
	dmotTranslationSection *section = GetSelectedSection();
	
	if( section ){
		int r, recordCount = section->GetRecordCount();
		int t, textCount = section->GetTextCount();
		dmotTranslationSectionText *text;
		FXColor colorTooLong = FXRGB( 255, 128, 128 );
		FXString foxtext;
		
		// resize the tabel. this removes all items from the tabel and leaves us
		// with the correct column headers
		pTableText->setTableSize( recordCount, textCount + 1 );
		
		pTableText->setColumnText( 0, section->GetHeaderIDName() );
		pTableText->setColumnJustify( 0, FXHeaderItem::CENTER_X );
		pTableText->setColumnWidth( 0, section->GetHeaderIDWidth() );
		
		for( t=0; t<textCount; t++ ){
			text = section->GetTextAt( t );
			
			pTableText->setColumnText( 1 + t, text->GetHeaderName() );
			pTableText->setColumnJustify( 1 + t, FXHeaderItem::CENTER_X );
			pTableText->setColumnWidth( 1 + t, text->GetHeaderWidth() );
		}
		
		// populate with records
		for( r=0; r<recordCount; r++ ){
			const dmotTranslationRecord &record = *section->GetRecordAt( r );
			
			foxtext.format( "%u", record.GetIdentifier() );
			pTableText->setItemText( r, 0, foxtext );
			pTableText->setItemJustify( r, 0, FXTableItem::RIGHT );
			pTableText->disableItem( r, 0 );
			
			for( t=0; t<textCount; t++ ){
				// WARNING! fox has a strange behavior here which can lead to a segfault.
				// the clever way would be to first set the back color and then replacing
				// strings to set the text. this way the text is not fecthed multiple
				// times from the array. if this is done like this though a segfault
				// happens. most probably the table does not create the item properly
				// before the text has been set (maybe NULL). so the old way is used.
				FXString text( record.GetTextAt( t ) );
				
				text.substitute( "\r", "\\r" );
				text.substitute( "\n", "\\n" );
				text.substitute( "\t", "\\t" );
				
				pTableText->setItemText( r, 1 + t, text );
				pTableText->setItemJustify( r, 1 + t, FXTableItem::LEFT );
				
				if( record.GetTextAt( t ).count() > section->GetTextAt( t )->GetLength() ){
					pTableText->SetCellBackColor( r, 1 + t, colorTooLong );
					
				}else{
					pTableText->SetCellBackColor( r, 1 + t, pTableText->getBackColor() );
				}
			}
		}
		
	}else{
		pTableText->clearItems();
		pTableText->setTableSize( 0, 0 );
	}
}

void dmotPanelFiles::UpdateRecord( int index ){

}

void dmotPanelFiles::UnicodeFontChanged(){
	FXFont *font = pWindowMain->GetUnicodeFont();
	
	if( ! font ){
		font = pWindowMain->getApp()->getNormalFont();
	}
	
	pTableText->setFont( font );
}



// Events
///////////

long dmotPanelFiles::onCBArchiveChanged( FXObject *sender, FXSelector selector, void *data ){
	UpdateFileList();
	UpdateTextTable();
	return 1;
}

long dmotPanelFiles::onCBFileChanged( FXObject *sender, FXSelector selector, void *data ){
	UpdateSectionList();
	UpdateTextTable();
	return 1;
}

long dmotPanelFiles::onCBSectionChanged( FXObject *sender, FXSelector selector, void *data ){
	UpdateTextTable();
	return 1;
}

long dmotPanelFiles::onTableTextChanged( FXObject *sender, FXSelector selector, void *data ){
	dmotTranslationSection *section = GetSelectedSection();
	
	if( section ){
		const FXTableRange &cell = *( ( FXTableRange* )data );
		int row = cell.fm.row;
		
		if( row >= 0 && row < section->GetRecordCount() ){
			dmotTranslationRecord &record = *section->GetRecordAt( row );
			int col = cell.fm.col;
			
			if( col > 0 && col <= record.GetTextCount() ){
				FXString text( pTableText->getItemText( row, col ) );
				FXColor colorTooLong = FXRGB( 255, 128, 128 );
				
				if( pTableText->getItemText( row, col ).count() > section->GetTextAt( col - 1 )->GetLength() ){
					pTableText->SetCellBackColor( row, col, colorTooLong );
					
				}else{
					pTableText->SetCellBackColor( row, col, pTableText->getBackColor() );
				}
				
				text.substitute( "\\r", "\r" );
				text.substitute( "\\n", "\n" );
				text.substitute( "\\t", "\t" );
				
				record.SetTextAt( col - 1, text );
				pWindowMain->GetTranslation()->SetChanged( true );
			}
		}
	}
	
	return 0;
}

long dmotPanelFiles::onTableTextHeaderResized( FXObject *sender, FXSelector selector, void *data ){
	dmotTranslationSection *section = GetSelectedSection();
	
	if( section ){
		int col = ( intptr_t )data;
		
		if( col == 0 ){
			section->SetHeaderIDWidth( pTableText->getColumnWidth( col ) );
			pWindowMain->GetTranslation()->SetChanged( true );
			
		}else if( col <= section->GetTextCount() ){
			section->GetTextAt( col - 1 )->SetHeaderWidth( pTableText->getColumnWidth( col ) );
			pWindowMain->GetTranslation()->SetChanged( true );
		}
	}
	
	return 0;
}

long dmotPanelFiles::onTableTextHeaderForward( FXObject *sender, FXSelector selector, void *data ){
	return pTableText->handle( sender, selector, data );
}
