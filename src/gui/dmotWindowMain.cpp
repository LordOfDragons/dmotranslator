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
#include "dmotPanelFiles.h"
#include "dmotDialogArchiveProperties.h"
#include "dmotDialogFileProperties.h"
#include "dmotDialogSectionProperties.h"
#include "dmotDialogTextProperties.h"
#include "dmotDialogTranslateStatus.h"
#include "../config/dmotConfig.h"
#include "../file/dmotReadBinFile.h"
#include "../file/dmotLoadSaveCSV.h"
#include "../file/dmotLoadSaveTranslation.h"
#include "../file/dmotTranslateStatus.h"
#include "../translation/dmotTranslation.h"
#include "../translation/dmotTranslationArchive.h"
#include "../translation/dmotTranslationFile.h"
#include "../translation/dmotTranslationSection.h"
#include "../translation/dmotTranslationSectionText.h"
#include "../translation/dmotTranslationRecord.h"
#include "../exceptions.h"



// Events
///////////

FXDEFMAP( dmotWindowMain ) dmotWindowMainMap[] = {
	FXMAPFUNC( SEL_CLOSE, 0, dmotWindowMain::onClose ),
	FXMAPFUNC( SEL_COMMAND, dmotWindowMain::ID_FILE_NEW, dmotWindowMain::onFileNew ),
	FXMAPFUNC( SEL_COMMAND, dmotWindowMain::ID_FILE_OPEN, dmotWindowMain::onFileOpen ),
	FXMAPFUNC( SEL_COMMAND, dmotWindowMain::ID_FILE_SAVE, dmotWindowMain::onFileSave ),
	FXMAPFUNC( SEL_COMMAND, dmotWindowMain::ID_FILE_SAVEAS, dmotWindowMain::onFileSaveAs ),
	FXMAPFUNC( SEL_COMMAND, dmotWindowMain::ID_FILE_SELUNICODEFONT, dmotWindowMain::onFileSelectUnicodeFont ),
	FXMAPFUNC( SEL_COMMAND, dmotWindowMain::ID_FILE_SETDMOPATH, dmotWindowMain::onFileSetDMOPath ),
	FXMAPFUNC( SEL_COMMAND, dmotWindowMain::ID_FILE_CSV_IMPORT, dmotWindowMain::onFileCsvImport ),
	FXMAPFUNC( SEL_COMMAND, dmotWindowMain::ID_FILE_CSV_EXPORT, dmotWindowMain::onFileCsvExport ),
	FXMAPFUNC( SEL_COMMAND, dmotWindowMain::ID_FILE_QUIT, dmotWindowMain::onFileQuit ),
	FXMAPFUNC( SEL_COMMAND, dmotWindowMain::ID_TRANS_ARCHIVEADD, dmotWindowMain::onTransArchiveAdd ),
	FXMAPFUNC( SEL_COMMAND, dmotWindowMain::ID_TRANS_ARCHIVEEDIT, dmotWindowMain::onTransArchiveEdit ),
	FXMAPFUNC( SEL_COMMAND, dmotWindowMain::ID_TRANS_ARCHIVEDEL, dmotWindowMain::onTransArchiveDel ),
	FXMAPFUNC( SEL_COMMAND, dmotWindowMain::ID_TRANS_FILEADD, dmotWindowMain::onTransFileAdd ),
	FXMAPFUNC( SEL_COMMAND, dmotWindowMain::ID_TRANS_FILEEDIT, dmotWindowMain::onTransFileEdit ),
	FXMAPFUNC( SEL_COMMAND, dmotWindowMain::ID_TRANS_FILEDEL, dmotWindowMain::onTransFileDel ),
	FXMAPFUNC( SEL_COMMAND, dmotWindowMain::ID_TRANS_SECADD, dmotWindowMain::onTransSecAdd ),
	FXMAPFUNC( SEL_COMMAND, dmotWindowMain::ID_TRANS_SECEDIT, dmotWindowMain::onTransSecEdit ),
	FXMAPFUNC( SEL_COMMAND, dmotWindowMain::ID_TRANS_SECDEL, dmotWindowMain::onTransSecDel ),
	FXMAPFUNC( SEL_COMMAND, dmotWindowMain::ID_TRANS_TEXTADD, dmotWindowMain::onTransTextAdd ),
	FXMAPFUNC( SEL_COMMAND, dmotWindowMain::ID_TRANS_TEXTEDIT, dmotWindowMain::onTransTextEdit ),
	FXMAPFUNC( SEL_COMMAND, dmotWindowMain::ID_TRANS_TEXTDEL, dmotWindowMain::onTransTextDel ),
	FXMAPFUNC( SEL_COMMAND, dmotWindowMain::ID_TRANS_RECORD_DEL, dmotWindowMain::onTransRecordDel ),
	FXMAPFUNC( SEL_COMMAND, dmotWindowMain::ID_TRANS_CLEARRECORDS, dmotWindowMain::onTransClearRecords ),
	FXMAPFUNC( SEL_COMMAND, dmotWindowMain::ID_TRANS_REPLACE, dmotWindowMain::onTransReplace ),
	FXMAPFUNC( SEL_COMMAND, dmotWindowMain::ID_BINFILE_IMPORT, dmotWindowMain::onBinFileImport ),
	FXMAPFUNC( SEL_COMMAND, dmotWindowMain::ID_BINFILE_PATCH, dmotWindowMain::onBinFilePatch )
};



// HACK
// Dirty hack to get access to the replace dialog components to mess with their font

class dmotReplaceDialog : public FXReplaceDialog{
	FXDECLARE( dmotReplaceDialog )
protected:
	dmotReplaceDialog(){ }
public:
	dmotReplaceDialog( FXWindow *owner, const FXString &caption ) : FXReplaceDialog( owner, caption ){ }
	void setFont( FXFont *font ){
		searchtext->setFont( font );
	}
};
FXIMPLEMENT( dmotReplaceDialog, FXReplaceDialog, NULL, 0 )



// Class dmotWindowMain
/////////////////////////

// Constructor, destructor
////////////////////////////

FXIMPLEMENT( dmotWindowMain, FXMainWindow, dmotWindowMainMap, ARRAYNUMBER( dmotWindowMainMap ) )

dmotWindowMain::dmotWindowMain(){ }

void dmotWindowMain::create(){
	FXMainWindow::create();
	
	show( PLACEMENT_SCREEN );
	
	// ask for dmo path if not set
	if( pConfig->GetPathDMO().empty() ){
		FXMessageBox::information( this, MBOX_OK, "DMO Path", "There is no DMO Path set. Please select the path where the DigimonMaster.exe is located." );
		
		FXDirDialog dialog( this, "Select DMO Path" );
		
		if( dialog.execute( PLACEMENT_OWNER ) == TRUE ){
			pConfig->SetPathDMO( dialog.getDirectory() );
			
		}else{
			FXMessageBox::information( this, MBOX_OK, "DMO Path", "You have to set the DMO path otherwise this application can not function properly. Use 'File'->'DMO Path' to do this." );
		}
	}
	
	// create font
	FXFont *font = NULL;
	
	try{
		font = new FXFont( getApp(), pUnicodeFontDesc );
		if( ! font ) THROW( dueOutOfMemory );
		
		font->create();
		
		SetUnicodeFont( font );
		font = NULL;
		
	}catch( duException e ){
		if( font ) delete font;
		throw;
	}
	
	// create an empty translation
	NewTranslation();
}

dmotWindowMain::dmotWindowMain( FXApp *app, int argc, char **argv ) :
FXMainWindow( app, "DMO Translator", NULL, NULL, DECOR_ALL, 0, 0, 800, 600 ){
	pUnicodeFont = NULL;
	pPanelFiles = NULL;
	pConfig = NULL;
	pTranslation = NULL;
	
	// load configuration
	pConfig = new dmotConfig;
	if( ! pConfig ) THROW( dueOutOfMemory );
	
	// create menu bar
	pMenuBar = new FXMenuBar( this, LAYOUT_SIDE_TOP | LAYOUT_FILL_X );
	
	FXMenuPane *menuFile = new FXMenuPane( this );
	new FXMenuTitle( pMenuBar, "&File", NULL, menuFile );
	
	new FXMenuCommand( menuFile, "&New...\t\tNew Translation", NULL, this, ID_FILE_NEW );
	new FXMenuCommand( menuFile, "&Open...\t\tOpen Translation", NULL, this, ID_FILE_OPEN );
	new FXMenuCommand( menuFile, "&Save...\t\tSave Translation", NULL, this, ID_FILE_SAVE );
	new FXMenuCommand( menuFile, "Save &As...\t\tSave Translation under a different file", NULL, this, ID_FILE_SAVEAS );
	
	new FXMenuSeparator( menuFile );
	new FXMenuCommand( menuFile, "&Set DMO Path\t\tSet DMO Path", NULL, this, ID_FILE_SETDMOPATH );
	new FXMenuCommand( menuFile, "&Select Unicode Font\t\tSelect Unicode aware font", NULL, this, ID_FILE_SELUNICODEFONT );
	
	new FXMenuSeparator( menuFile );
	new FXMenuCommand( menuFile, "&Import CSV...\t\tImport Section from CSV", NULL, this, ID_FILE_CSV_IMPORT );
	new FXMenuCommand( menuFile, "&Export CSV...\t\tExport Section to CSV", NULL, this, ID_FILE_CSV_EXPORT );
	
	new FXMenuSeparator( menuFile );
	new FXMenuCommand( menuFile, "&Quit\t\tQuits application", NULL, this, ID_FILE_QUIT );
	
	FXMenuPane *menuTrans = new FXMenuPane( this );
	new FXMenuTitle( pMenuBar, "&Translation", NULL, menuTrans );
	
	new FXMenuCommand( menuTrans, "Add Archive\t\tAdd an archive", NULL, this, ID_TRANS_ARCHIVEADD );
	new FXMenuCommand( menuTrans, "Edit Archive\t\tEdit the selected archive", NULL, this, ID_TRANS_ARCHIVEEDIT );
	new FXMenuCommand( menuTrans, "Delete Archive\t\tDelete the selected archive", NULL, this, ID_TRANS_ARCHIVEDEL );
	
	new FXMenuSeparator( menuTrans );
	new FXMenuCommand( menuTrans, "Add File\t\tAdd a file", NULL, this, ID_TRANS_FILEADD );
	new FXMenuCommand( menuTrans, "Edit File\t\tEdit the selected file", NULL, this, ID_TRANS_FILEEDIT );
	new FXMenuCommand( menuTrans, "Delete File\t\tDelete the selected file", NULL, this, ID_TRANS_FILEDEL );
	
	new FXMenuSeparator( menuTrans );
	new FXMenuCommand( menuTrans, "Add Section\t\tAdd a section", NULL, this, ID_TRANS_SECADD );
	new FXMenuCommand( menuTrans, "Edit Section\t\tEdit the selected section", NULL, this, ID_TRANS_SECEDIT );
	new FXMenuCommand( menuTrans, "Delete Section\t\tDelete the selected section", NULL, this, ID_TRANS_SECDEL );
	
	new FXMenuSeparator( menuTrans );
	new FXMenuCommand( menuTrans, "Add Text\t\tAdd a text", NULL, this, ID_TRANS_TEXTADD );
	new FXMenuCommand( menuTrans, "Edit Text\t\tEdit the selected text", NULL, this, ID_TRANS_TEXTEDIT );
	new FXMenuCommand( menuTrans, "Delete Text\t\tDelete the selected text", NULL, this, ID_TRANS_TEXTDEL );
	
	new FXMenuSeparator( menuTrans );
	new FXMenuCommand( menuTrans, "Delete Records\t\tDelete selected records", NULL, this, ID_TRANS_RECORD_DEL );
	new FXMenuCommand( menuTrans, "Clear Records\t\tRemove all records from the current section", NULL, this, ID_TRANS_CLEARRECORDS );
	new FXMenuCommand( menuTrans, "Replace Text\t\tReplace text in the current column", NULL, this, ID_TRANS_REPLACE );
	
	FXMenuPane *menuBinFile = new FXMenuPane( this );
	new FXMenuTitle( pMenuBar, "&Bin-File", NULL, menuBinFile );
	
	new FXMenuCommand( menuBinFile, "Import &Bin-Files\t\tImport bin-file into the selected file", NULL, this, ID_BINFILE_IMPORT );
	new FXMenuCommand( menuBinFile, "Patch &Bin-Files\t\tPatch bin-files with translation", NULL, this, ID_BINFILE_PATCH );
	
	// create content area
	FXPacker *content = new FXPacker( this, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
	
	pStatusBar = new FXStatusBar( content, LAYOUT_SIDE_BOTTOM | LAYOUT_FILL_X );
	pStatusBar->getStatusLine()->setNormalText( "Ready" );
	
	pSBProgress = new FXProgressBar( pStatusBar, NULL, 0, PROGRESSBAR_HORIZONTAL | LAYOUT_FIX_WIDTH | LAYOUT_FILL_Y | FRAME_SUNKEN );
	pSBProgress->setWidth( 200 );
	pSBProgress->setTotal( 100 );
	pSBProgress->setProgress( 0 );
	pSBProgress->hide();
	
	pPanelFiles = new dmotPanelFiles( this, content );
	if( ! pPanelFiles ) THROW( dueOutOfMemory );
	
	// tool tips
	pToolTip = new FXToolTip( app, 0 );
	
	// load configuration
	FXRegistry &registry = app->reg();
	const char *sectionApp = "application";
	const char *sectionUnicodeFont = "unicodefont";
	
	pConfig->SetPathDMO( registry.readStringEntry( sectionApp, "pathDMO", "" ) );
	
	app->getNormalFont()->getFontDesc( pUnicodeFontDesc );
	
	pUnicodeFontDesc.encoding = ( FXushort )registry.readIntEntry( sectionUnicodeFont, "encoding", pUnicodeFontDesc.encoding );
	strcpy( &pUnicodeFontDesc.face[ 0 ], registry.readStringEntry( sectionUnicodeFont, "face", pUnicodeFontDesc.face ) );
	pUnicodeFontDesc.flags = ( FXushort )registry.readIntEntry( sectionUnicodeFont, "flags", pUnicodeFontDesc.flags );
	pUnicodeFontDesc.setwidth = ( FXushort )registry.readIntEntry( sectionUnicodeFont, "setwidth", pUnicodeFontDesc.setwidth );
	pUnicodeFontDesc.size = ( FXushort )registry.readIntEntry( sectionUnicodeFont, "size", pUnicodeFontDesc.size );
	pUnicodeFontDesc.slant = ( FXushort )registry.readIntEntry( sectionUnicodeFont, "slant", pUnicodeFontDesc.slant );
	pUnicodeFontDesc.weight = ( FXushort )registry.readIntEntry( sectionUnicodeFont, "weight", pUnicodeFontDesc.weight );
}

dmotWindowMain::~dmotWindowMain(){
	FXRegistry &registry = getApp()->reg();
	const char *sectionApp = "application";
	const char *sectionUnicodeFont = "unicodefont";
	
	SetTranslation( NULL );
	
	registry.writeIntEntry( sectionUnicodeFont, "encoding", pUnicodeFontDesc.encoding );
	registry.writeStringEntry( sectionUnicodeFont, "face", &pUnicodeFontDesc.face[ 0 ] );
	registry.writeIntEntry( sectionUnicodeFont, "flags", pUnicodeFontDesc.flags );
	registry.writeIntEntry( sectionUnicodeFont, "setwidth", pUnicodeFontDesc.setwidth );
	registry.writeIntEntry( sectionUnicodeFont, "size", pUnicodeFontDesc.size );
	registry.writeIntEntry( sectionUnicodeFont, "slant", pUnicodeFontDesc.slant );
	registry.writeIntEntry( sectionUnicodeFont, "weight", pUnicodeFontDesc.weight );
	
	if( pUnicodeFont ) delete pUnicodeFont;
	
	registry.writeStringEntry( sectionApp, "pathDMO", pConfig->GetPathDMO().text() );
	
	if( pConfig ) delete pConfig;
}



// Management
///////////////

void dmotWindowMain::DisplayException( const duException &exception ){
	FXString caption, message;
	
	caption = "Application Error";
	
	message.format( "An exception occured. File='%s', Line='%i', Reason='%s'.",
		exception.GetFile(), exception.GetLine(), exception.GetDescription() );
	
	FXMessageBox::error( this, FX::MBOX_OK, caption.text(), "%s", message.text() );
}

void dmotWindowMain::SetUnicodeFont( FXFont *font ){
	if( font != pUnicodeFont ){
		if( pUnicodeFont ) delete pUnicodeFont;
		pUnicodeFont = font;
		
		pPanelFiles->UnicodeFontChanged();
	}
}



void dmotWindowMain::SetProgressVisible( bool visible ){
	if( visible ){
		pSBProgress->show();
		
	}else{
		pSBProgress->hide();
	}
	
	pSBProgress->recalc();
	pStatusBar->recalc();
}

void dmotWindowMain::SetProgress( float progress ){
	pSBProgress->setProgress( ( int )( progress * 100.0f ) );
}

void dmotWindowMain::SetProgressText( const FXString &text ){
	FXStatusLine *statusLine = pStatusBar->getStatusLine();
	
	statusLine->setNormalText( text );
	statusLine->setText( text );
}



bool dmotWindowMain::QuitRequest(){
	if( pTranslation && pTranslation->GetChanged() ){
		int result = FXMessageBox::question( this, MBOX_YES_NO_CANCEL, "Quit Application",
			"The translation has changed since the last time it has been saved. Do you want to save the translation before leaving?" );
		
		if( result == MBOX_CLICKED_YES ){
			if( onFileSave( this, SEL_COMMAND, NULL ) == 0 ){
				return false;
			}
			
		}else if( result == MBOX_CLICKED_CANCEL ){
			return false;
		}
	}
	
	return true;
}



void dmotWindowMain::SetTranslation( dmotTranslation *translation ){
	if( translation != pTranslation ){
		if( pTranslation ) delete pTranslation;
		pTranslation = translation;
		
		pPanelFiles->UpdateArchiveList();
		pPanelFiles->UpdateFileList();
		pPanelFiles->UpdateSectionList();
		pPanelFiles->UpdateTextTable();
	}
}

void dmotWindowMain::NewTranslation(){
	dmotTranslation *translation = NULL;
	
	try{
		translation = new dmotTranslation;
		if( ! translation ) THROW( dueOutOfMemory );
		
		SetTranslation( translation );
		translation = NULL;
		
	}catch( duException e ){
		DisplayException( e );
	}
}

void dmotWindowMain::SaveTranslation( const FXString &filename ){
	if( pTranslation ){
		dmotLoadSaveTranslation lstrans;
		
		lstrans.SaveToFile( filename, *pTranslation );
		
		pTranslation->SetFilename( filename );
		pTranslation->SetSaved( true );
		pTranslation->SetChanged( false );
	}
}



// Events
///////////

long dmotWindowMain::onClose( FXObject *sender, FXSelector selector, void *data ){
	if( QuitRequest() ){
		close();
		return 0;
	}
	
	return 1;
}

long dmotWindowMain::onFileNew( FXObject *sender, FXSelector selector, void *data ){
	if( pTranslation && pTranslation->GetChanged() ){
		int result = FXMessageBox::question( this, MBOX_YES_NO_CANCEL, "New Translations",
			"The translation has changed since the last time it has been saved. Do you want to save the translation before creating a new one?" );
		
		if( result == MBOX_CLICKED_YES ){
			if( onFileSave( this, SEL_COMMAND, NULL ) == 0 ){
				return 1;
			}
			
		}else if( result == MBOX_CLICKED_CANCEL ){
			return 1;
		}
	}
	
	NewTranslation();
	return 1;
}

long dmotWindowMain::onFileOpen( FXObject *sender, FXSelector selector, void *data ){
	if( pTranslation && pTranslation->GetChanged() ){
		int result = FXMessageBox::question( this, MBOX_YES_NO_CANCEL, "Open Translations",
			"The translation has changed since the last time it has been saved. Do you want to save the translation before opening a new one?" );
		
		if( result == MBOX_CLICKED_YES ){
			if( onFileSave( this, SEL_COMMAND, NULL ) == 0 ){
				return 1;
			}
			
		}else if( result == MBOX_CLICKED_CANCEL ){
			return 1;
		}
	}
	
	FXFileDialog dialog( this, "Open Translation" );
	
	dialog.setPatternList( "DMO Translation (*.dmot)\0.dmot" );
	dialog.setCurrentPattern( 0 );
	
	if( pTranslation ){
		dialog.setFilename( pTranslation->GetFilename() );
		
	}else{
		dialog.setFilename( "Translation.dmot" );
	}
	
	if( dialog.execute() == TRUE ){
		dmotTranslation *translation = NULL;
		dmotLoadSaveTranslation lstrans;
		
		try{
			translation = new dmotTranslation;
			if( ! translation ) THROW( dueOutOfMemory );
			
			lstrans.LoadFromFile( dialog.getFilename(), *translation );
			translation->SetFilename( dialog.getFilename() );
			translation->SetSaved( true );
			translation->SetChanged( false );
			
			SetTranslation( translation );
			
		}catch( duException e ){
			if( translation ) delete translation;
			DisplayException( e );
		}
	}
	
	return 1;
}

long dmotWindowMain::onFileSave( FXObject *sender, FXSelector selector, void *data ){
	if( pTranslation ){
		if( pTranslation->GetSaved() ){
			SaveTranslation( pTranslation->GetFilename() );
			return true;
			
		}else{
			FXFileDialog dialog( this, "Save Translation" );
			
			dialog.setPatternList( "DMO Translation (*.dmot)\0.dmot" );
			dialog.setCurrentPattern( 0 );
			dialog.setFilename( pTranslation->GetFilename() );
			if( dialog.execute() == TRUE ){
				try{
					SaveTranslation( dialog.getFilename() );
					
				}catch( duException e ){
					DisplayException( e );
				}
			}
		}
	}
	
	return 1;
}

long dmotWindowMain::onFileSaveAs( FXObject *sender, FXSelector selector, void *data ){
	if( pTranslation ){
		FXFileDialog dialog( this, "Save Translation As" );
		
		dialog.setPatternList( "DMO Translation (*.dmot)\0.dmot" );
		dialog.setCurrentPattern( 0 );
		dialog.setFilename( pTranslation->GetFilename() );
		if( dialog.execute() == TRUE ){
			try{
				SaveTranslation( dialog.getFilename() );
				
			}catch( duException e ){
				DisplayException( e );
			}
		}
	}
	
	return 1;
}

long dmotWindowMain::onFileSetDMOPath( FXObject *sender, FXSelector selector, void *data ){
	FXDirDialog dialog( this, "Select DMO Path" );
	
	dialog.setDirectory( pConfig->GetPathDMO() );
	
	if( dialog.execute( PLACEMENT_OWNER ) == TRUE ){
		pConfig->SetPathDMO( dialog.getDirectory() );
	}
	
	return 1;
}

long dmotWindowMain::onFileSelectUnicodeFont( FXObject *sender, FXSelector selector, void *data ){
	FXFontDialog fontDialog( this, "Select Unicode Font" );
	FXFont *font = NULL;
	
	fontDialog.setFontSelection( pUnicodeFontDesc );
	
	if( fontDialog.execute() == TRUE ){
		fontDialog.getFontSelection( pUnicodeFontDesc );
		
		font = new FXFont( getApp(), pUnicodeFontDesc );
		if( ! font ) THROW( dueOutOfMemory );
		
		font->create();
		
		SetUnicodeFont( font );
		font = NULL;
	}
	
	return 1;
}

long dmotWindowMain::onFileCsvImport( FXObject *sender, FXSelector selector, void *data ){
	dmotTranslationFile *file = pPanelFiles->GetSelectedFile();
	dmotTranslationSection *section = pPanelFiles->GetSelectedSection();
	
	if( file && section ){
		FXFileDialog dialog( this, "Import CSV" );
		
		dialog.setPatternList( "CSV File (*.csv)\0.csv" );
		dialog.setCurrentPattern( 0 );
		
		if( dialog.execute() == TRUE ){
			dmotLoadSaveCSV lscsv;
			int answer;
			
			answer = FXMessageBox::question( this, MBOX_YES_NO_CANCEL, "Import CSV", "Replace existing records? If 'No' only new ones will be added." );
			
			if( answer != MBOX_CLICKED_CANCEL ){
				FXString filename = dialog.getFilename();
				
				try{
					lscsv.LoadFromFile( filename, *pTranslation, *file, *section, answer == MBOX_CLICKED_YES );
					
				}catch( duException e ){
					DisplayException( e );
					return 1;
				}
				
				pTranslation->SetChanged( true );
				pPanelFiles->UpdateTextTable();
			}
		}
	}
	
	return 1;
}

long dmotWindowMain::onFileCsvExport( FXObject *sender, FXSelector selector, void *data ){
	dmotTranslationFile *file = pPanelFiles->GetSelectedFile();
	dmotTranslationSection *section = pPanelFiles->GetSelectedSection();
	
	if( file && section ){
		FXFileDialog dialog( this, "Export CSV" );
		
		dialog.setPatternList( "CSV File (*.csv)\0.csv" );
		dialog.setCurrentPattern( 0 );
		
		if( dialog.execute() == TRUE ){
			dmotLoadSaveCSV lscsv;
			FXString filename = dialog.getFilename();
			
			filename.lower();
			if( filename.rfind( ".csv" ) == -1 ){
				filename = dialog.getFilename() + ".csv";
				
			}else{
				filename = dialog.getFilename();
			}
			
			try{
				lscsv.SaveToFile( filename, *pTranslation, *file, *section );
				
			}catch( duException e ){
				DisplayException( e );
			}
		}
	}
	
	return 1;
}

long dmotWindowMain::onFileQuit( FXObject *sender, FXSelector selector, void *data ){
	if( QuitRequest() ){
		close( TRUE );
		return 1;
	}
	
	return 0;
}

long dmotWindowMain::onTransArchiveAdd( FXObject *sender, FXSelector selector, void *data ){
	if( pTranslation ){
		dmotTranslationArchive *archive = NULL;
		
		try{
			archive = new dmotTranslationArchive;
			if( ! archive ) THROW( dueOutOfMemory );
			
			dmotDialogArchiveProperties dialog( this, this, archive );
			
			dialog.setTitle( "Add Archive" );
			
			if( dialog.execute() == TRUE ){
				pTranslation->AddArchive( archive );
				archive = NULL;
				pTranslation->SetChanged( true );
				
				pPanelFiles->UpdateArchiveList();
				pPanelFiles->UpdateFileList();
				pPanelFiles->UpdateSectionList();
				pPanelFiles->SelectLatestFile();
				pPanelFiles->UpdateTextTable();
			}
			
		}catch( duException e ){
			if( archive ) delete archive;
			DisplayException( e );
		}
	}
	
	return 1;
}

long dmotWindowMain::onTransArchiveEdit( FXObject *sender, FXSelector selector, void *data ){
	dmotTranslationArchive *archive = pPanelFiles->GetSelectedArchive();
	
	if( archive ){
		dmotDialogArchiveProperties dialog( this, this, archive );
		
		if( dialog.execute() == TRUE ){
			pTranslation->SetChanged( true );
			pPanelFiles->UpdateArchiveList();
			pPanelFiles->UpdateFileList();
			pPanelFiles->UpdateSectionList();
			pPanelFiles->UpdateTextTable();
		}
	}
	
	return 1;
}

long dmotWindowMain::onTransArchiveDel( FXObject *sender, FXSelector selector, void *data ){
	dmotTranslationArchive *archive = pPanelFiles->GetSelectedArchive();
	
	if( archive ){
		FXString text;
		
		text.format( "Delete the archive '%s'? This deletes also all files attached to it.", archive->GetName().text() );
		
		if( FXMessageBox::question( this, MBOX_YES_NO, "Delete Archive", "%s", text.text() ) == MBOX_CLICKED_YES ){
			pTranslation->RemoveArchive( archive );
			pTranslation->SetChanged( true );
			
			pPanelFiles->UpdateArchiveList();
			pPanelFiles->UpdateFileList();
			pPanelFiles->UpdateSectionList();
			pPanelFiles->UpdateTextTable();
		}
	}
	
	return 1;
}

long dmotWindowMain::onTransFileAdd( FXObject *sender, FXSelector selector, void *data ){
	dmotTranslationArchive *archive = pPanelFiles->GetSelectedArchive();
	
	if( archive ){
		dmotTranslationFile *file = NULL;
		
		try{
			file = new dmotTranslationFile;
			if( ! file ) THROW( dueOutOfMemory );
			
			dmotDialogFileProperties dialog( this, this, file );
			
			dialog.setTitle( "Add File" );
			
			if( dialog.execute() == TRUE ){
				archive->AddFile( file );
				file = NULL;
				pTranslation->SetChanged( true );
				
				pPanelFiles->UpdateFileList();
				pPanelFiles->UpdateSectionList();
				pPanelFiles->SelectLatestFile();
				pPanelFiles->UpdateTextTable();
			}
			
		}catch( duException e ){
			if( file ) delete file;
			DisplayException( e );
		}
	}
	
	return 1;
}

long dmotWindowMain::onTransFileEdit( FXObject *sender, FXSelector selector, void *data ){
	dmotTranslationFile *file = pPanelFiles->GetSelectedFile();
	
	if( file ){
		dmotDialogFileProperties dialog( this, this, file );
		
		if( dialog.execute() == TRUE ){
			pTranslation->SetChanged( true );
			pPanelFiles->UpdateFileList();
			pPanelFiles->UpdateSectionList();
			pPanelFiles->UpdateTextTable();
		}
	}
	
	return 1;
}

long dmotWindowMain::onTransFileDel( FXObject *sender, FXSelector selector, void *data ){
	dmotTranslationFile *file = pPanelFiles->GetSelectedFile();
	
	if( file ){
		dmotTranslationArchive *archive = pPanelFiles->GetSelectedArchive();
		FXString text;
		
		text.format( "Delete the file '%s'? This deletes also all records attached to it.", file->GetName().text() );
		
		if( FXMessageBox::question( this, MBOX_YES_NO, "Delete File", "%s", text.text() ) == MBOX_CLICKED_YES ){
			archive->RemoveFile( file );
			pTranslation->SetChanged( true );
			
			pPanelFiles->UpdateFileList();
			pPanelFiles->UpdateSectionList();
			pPanelFiles->UpdateTextTable();
		}
	}
	
	return 1;
}

long dmotWindowMain::onTransSecAdd( FXObject *sender, FXSelector selector, void *data ){
	dmotTranslationFile *file = pPanelFiles->GetSelectedFile();
	
	if( file ){
		dmotTranslationSection *section = NULL;
		
		try{
			section = new dmotTranslationSection;
			if( ! section ) THROW( dueOutOfMemory );
			
			dmotDialogSectionProperties dialog( this, this, section );
			
			dialog.setTitle( "Add Section" );
			
			if( dialog.execute() == TRUE ){
				file->AddSection( section );
				section = NULL;
				pTranslation->SetChanged( true );
				
				pPanelFiles->UpdateSectionList();
				pPanelFiles->SelectLatestSection();
				pPanelFiles->UpdateTextTable();
			}
			
		}catch( duException e ){
			if( section ) delete section;
			DisplayException( e );
		}
	}
	
	return 1;
}

long dmotWindowMain::onTransSecEdit( FXObject *sender, FXSelector selector, void *data ){
	dmotTranslationSection *section = pPanelFiles->GetSelectedSection();
	
	if( section ){
		dmotDialogSectionProperties dialog( this, this, section );
		
		if( dialog.execute() == TRUE ){
			pTranslation->SetChanged( true );
			pPanelFiles->UpdateSectionList();
			pPanelFiles->UpdateTextTable();
		}
	}
	
	return 1;
}

long dmotWindowMain::onTransSecDel( FXObject *sender, FXSelector selector, void *data ){
	dmotTranslationSection *section = pPanelFiles->GetSelectedSection();
	
	if( section ){
		FXString text;
		
		text.format( "Delete the section '%s'? This deletes also all texts in records attached to it.", section->GetName().text() );
		
		if( FXMessageBox::question( this, MBOX_YES_NO, "Delete Text", "%s", text.text() ) == MBOX_CLICKED_YES ){
			pPanelFiles->GetSelectedFile()->RemoveSection( section );
			pTranslation->SetChanged( true );
			
			pPanelFiles->UpdateSectionList();
			pPanelFiles->UpdateTextTable();
		}
	}
	
	return 1;
}

long dmotWindowMain::onTransTextAdd( FXObject *sender, FXSelector selector, void *data ){
	dmotTranslationSection *section = pPanelFiles->GetSelectedSection();
	
	if( section ){
		dmotTranslationSectionText *secText = NULL;
		
		try{
			secText = new dmotTranslationSectionText;
			if( ! secText ) THROW( dueOutOfMemory );
			
			dmotDialogTextProperties dialog( this, this, secText );
			
			dialog.setTitle( "Add Text" );
			
			if( dialog.execute() == TRUE ){
				section->AddText( secText );
				secText = NULL;
				section->AddTextInRecords( "" );
				pTranslation->SetChanged( true );
				
				pPanelFiles->UpdateTextTable();
			}
			
		}catch( duException e ){
			if( secText ) delete secText;
			DisplayException( e );
		}
	}
	
	return 1;
}

long dmotWindowMain::onTransTextEdit( FXObject *sender, FXSelector selector, void *data ){
	dmotTranslationSection *section = pPanelFiles->GetSelectedSection();
	
	if( section ){
		int textIndex = pPanelFiles->GetCurrentTextIndex();
		
		if( textIndex != -1 ){
			dmotDialogTextProperties dialog( this, this, section->GetTextAt( textIndex ) );
			
			if( dialog.execute() == TRUE ){
				pTranslation->SetChanged( true );
				pPanelFiles->UpdateTextTable();
			}
		}
	}
	
	return 1;
}

long dmotWindowMain::onTransTextDel( FXObject *sender, FXSelector selector, void *data ){
	dmotTranslationSection *section = pPanelFiles->GetSelectedSection();
	
	if( section ){
		int textIndex = pPanelFiles->GetCurrentTextIndex();
		
		if( textIndex != -1 ){
			dmotTranslationSectionText *secText = section->GetTextAt( textIndex );
			FXString text;
			
			text.format( "Delete the text '%s'? This deletes also all texts in records attached to it.", secText->GetHeaderName().text() );
			
			if( FXMessageBox::question( this, MBOX_YES_NO, "Delete Text", "%s", text.text() ) == MBOX_CLICKED_YES ){
				section->RemoveTextFromRecordsAt( textIndex );
				section->RemoveText( secText );
				pTranslation->SetChanged( true );
				
				pPanelFiles->UpdateTextTable();
			}
		}
	}
	
	return 1;
}

long dmotWindowMain::onTransRecordDel( FXObject *sender, FXSelector selector, void *data ){
	dmotTranslationFile *file = pPanelFiles->GetSelectedFile();
	dmotTranslationSection *section = pPanelFiles->GetSelectedSection();
	int recordFrom, recordTo;
	
	pPanelFiles->GetSelectedRecordRange( recordFrom, recordTo );
	
	if( file && section && recordFrom != -1 && recordTo != -1 ){
		int i;
		
		for( i=recordTo; i>=recordFrom; i-- ){
			section->RemoveRecord( section->GetRecordAt( i ) );
		}
		pTranslation->SetChanged( true );
		
		pPanelFiles->UpdateTextTable();
	}
	
	return 1;
}

long dmotWindowMain::onTransClearRecords( FXObject *sender, FXSelector selector, void *data ){
	dmotTranslationFile *file = pPanelFiles->GetSelectedFile();
	dmotTranslationSection *section = pPanelFiles->GetSelectedSection();
	
	if( file && section ){
		FXString text;
		
		text.format( "Delete all records from section '%s' in text '%s'?", section->GetName().text(), file->GetName().text() );
		
		if( FXMessageBox::question( this, MBOX_YES_NO, "Delete Records", "%s", text.text() ) == MBOX_CLICKED_YES ){
			section->RemoveAllRecords();
			pTranslation->SetChanged( true );
			
			pPanelFiles->UpdateTextTable();
		}
	}
	
	return 1;
}

long dmotWindowMain::onTransReplace( FXObject *sender, FXSelector selector, void *data ){
	dmotTranslationSection *section = pPanelFiles->GetSelectedSection();
	
	if( section ){
		int textIndex = pPanelFiles->GetCurrentTextIndex();
		
		if( textIndex != -1 ){
			dmotTranslationSectionText *secText = section->GetTextAt( textIndex );
			FXString title;
			title.format( "Replace Text in '%s'", secText->GetHeaderName().text() );
			dmotReplaceDialog dialog( this, title );
			FXint result;
			
			dialog.setFont( pUnicodeFont );
			
			result = dialog.execute( PLACEMENT_OWNER );
			if( result == FXReplaceDialog::REPLACE ){
				int recordIndex = pPanelFiles->GetSelectedRecord();
				
				if( recordIndex != -1 ){
					dmotTranslationRecord &record = *section->GetRecordAt( recordIndex );
					FXString text = record.GetTextAt( textIndex );
					text.substitute( dialog.getSearchText(), dialog.getReplaceText() );
					record.SetTextAt( textIndex, text );
					
					pTranslation->SetChanged( true );
					
					pPanelFiles->UpdateTextTable();
				}
				
			}else if( result == FXReplaceDialog::REPLACE_ALL ){
				int r, recordCount = section->GetRecordCount();
				FXString text;
				
				for( r=0; r<recordCount; r++ ){
					dmotTranslationRecord &record = *section->GetRecordAt( r );
					
					text = record.GetTextAt( textIndex );
					text.substitute( dialog.getSearchText(), dialog.getReplaceText() );
					record.SetTextAt( textIndex, text );
				}
				
				pTranslation->SetChanged( true );
				
				pPanelFiles->UpdateTextTable();
			}
		}
	}
	
	return 1;
}



long dmotWindowMain::onBinFileImport( FXObject *sender, FXSelector selector, void *data ){
	dmotTranslationArchive *archive = pPanelFiles->GetSelectedArchive();
	dmotTranslationFile *file = pPanelFiles->GetSelectedFile();
	dmotTranslateStatus status;
	
	if( archive && file ){
		dmotReadBinFile readBinFile;
		int answer;
		
		answer = FXMessageBox::question( this, MBOX_YES_NO_CANCEL, "Import Bin-File", "Replace existing records? If 'No' only new ones will be added." );
		
		if( answer == MBOX_CLICKED_CANCEL ){
			return 1;
		}
		
		try{
			readBinFile.ReadFile( status, *pConfig, *archive, *file, answer == MBOX_CLICKED_YES );
			
		}catch( duException e ){
			DisplayException( e );
		}
		
		dmotDialogTranslateStatus dialogStatus( this, this, &status );
		dialogStatus.execute( PLACEMENT_OWNER );
		
		pTranslation->SetChanged( true );
		pPanelFiles->UpdateTextTable();
	}
	
	return 1;
}

long dmotWindowMain::onBinFilePatch( FXObject *sender, FXSelector selector, void *data ){
	if( pTranslation ){
		dmotTranslateStatus status;
		
		try{
			pTranslation->PatchBinFiles( status, *pConfig );
			
		}catch( duException e ){
			DisplayException( e );
		}
		
		dmotDialogTranslateStatus dialogStatus( this, this, &status );
		dialogStatus.execute( PLACEMENT_OWNER );
	}
	
	return 1;
}
