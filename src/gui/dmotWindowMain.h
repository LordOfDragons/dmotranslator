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

#ifndef _DMOTWINDOWMAIN_H_
#define _DMOTWINDOWMAIN_H_

#include "../foxtoolkit.h"

class duException;
class dmotTranslation;
class dmotPanelFiles;
class dmotConfig;



/**
 * @brief Main Window.
 * @author Plüss Roland
 * @version 1.0
 * @date 2010
 */
class dmotWindowMain : public FXMainWindow{
	FXDECLARE( dmotWindowMain )
	
protected:
	dmotWindowMain();
	
public:
	enum eFoxIDs{
		ID_LAST = FXMainWindow::ID_LAST,
		ID_FILE_NEW,
		ID_FILE_OPEN,
		ID_FILE_SAVE,
		ID_FILE_SAVEAS,
		ID_FILE_SELUNICODEFONT,
		ID_FILE_SETDMOPATH,
		ID_FILE_CSV_IMPORT,
		ID_FILE_CSV_EXPORT,
		ID_FILE_QUIT,
		ID_TRANS_ARCHIVEADD,
		ID_TRANS_ARCHIVEEDIT,
		ID_TRANS_ARCHIVEDEL,
		ID_TRANS_FILEADD,
		ID_TRANS_FILEEDIT,
		ID_TRANS_FILEDEL,
		ID_TRANS_SECADD,
		ID_TRANS_SECEDIT,
		ID_TRANS_SECDEL,
		ID_TRANS_TEXTADD,
		ID_TRANS_TEXTEDIT,
		ID_TRANS_TEXTDEL,
		ID_TRANS_RECORD_DEL,
		ID_TRANS_CLEARRECORDS,
		ID_TRANS_REPLACE,
		ID_BINFILE_IMPORT,
		ID_BINFILE_PATCH,
	};
	
private:
	FXFontDesc pUnicodeFontDesc;
	FXFont *pUnicodeFont;
	
	FXMenuBar *pMenuBar;
	
	FXStatusBar *pStatusBar;
	FXProgressBar *pSBProgress;
	
	FXToolTip *pToolTip;
	
	dmotPanelFiles *pPanelFiles;
	
	dmotConfig *pConfig;
	dmotTranslation *pTranslation;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new main window. */
	dmotWindowMain( FXApp* app, int argc, char **argv );
	/** Cleans up the main window prototype. */
	virtual ~dmotWindowMain();
	/** Fox request for creating the window. */
	virtual void create();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Displays an exception error. */
	void DisplayException( const duException &exception );
	
	/** Retrieves the font to use for unicode text. */
	inline FXFont *GetUnicodeFont() const{ return pUnicodeFont; }
	/** Sets the font to use for unicode text. */
	void SetUnicodeFont( FXFont *font );
	
	/** Retrieves the menu bar. */
	inline FXMenuBar *GetMenuBar() const{ return pMenuBar; }
	
	/** Sets the visibility of the progress bar in the status bar. */
	void SetProgressVisible( bool visible );
	/** Sets the progress bar progress. */
	void SetProgress( float progress );
	/** Sets the progress text. */
	void SetProgressText( const FXString &text );
	
	/** Asks the user if it is okay to quit the application. */
	bool QuitRequest();
	
	/** Retrieves the configuration. */
	inline dmotConfig *GetConfiguration() const{ return pConfig; }
	/** Retrieves the translation. */
	inline dmotTranslation *GetTranslation() const{ return pTranslation; }
	/** Sets the translation. */
	void SetTranslation( dmotTranslation *translation );
	
	/** Create new translation. */
	void NewTranslation();
	/** Save translation. */
	void SaveTranslation( const FXString &filename );
	/*@}*/
	
	/** @name Events */
	/*@{*/
	long onClose( FXObject *sender, FXSelector selector, void *data );
	
	long onFileNew( FXObject *sender, FXSelector selector, void *data );
	long onFileOpen( FXObject *sender, FXSelector selector, void *data );
	long onFileSave( FXObject *sender, FXSelector selector, void *data );
	long onFileSaveAs( FXObject *sender, FXSelector selector, void *data );
	long onFileSelectUnicodeFont( FXObject *sender, FXSelector selector, void *data );
	long onFileSetDMOPath( FXObject *sender, FXSelector selector, void *data );
	long onFileCsvImport( FXObject *sender, FXSelector selector, void *data );
	long onFileCsvExport( FXObject *sender, FXSelector selector, void *data );
	long onFileQuit( FXObject *sender, FXSelector selector, void *data );
	long onTransArchiveAdd( FXObject *sender, FXSelector selector, void *data );
	long onTransArchiveEdit( FXObject *sender, FXSelector selector, void *data );
	long onTransArchiveDel( FXObject *sender, FXSelector selector, void *data );
	long onTransFileAdd( FXObject *sender, FXSelector selector, void *data );
	long onTransFileEdit( FXObject *sender, FXSelector selector, void *data );
	long onTransFileDel( FXObject *sender, FXSelector selector, void *data );
	long onTransSecAdd( FXObject *sender, FXSelector selector, void *data );
	long onTransSecEdit( FXObject *sender, FXSelector selector, void *data );
	long onTransSecDel( FXObject *sender, FXSelector selector, void *data );
	long onTransTextAdd( FXObject *sender, FXSelector selector, void *data );
	long onTransTextEdit( FXObject *sender, FXSelector selector, void *data );
	long onTransTextDel( FXObject *sender, FXSelector selector, void *data );
	long onTransRecordDel( FXObject *sender, FXSelector selector, void *data );
	long onTransClearRecords( FXObject *sender, FXSelector selector, void *data );
	long onTransReplace( FXObject *sender, FXSelector selector, void *data );
	long onBinFileImport( FXObject *sender, FXSelector selector, void *data );
	long onBinFilePatch( FXObject *sender, FXSelector selector, void *data );
	/*@}*/
};

// end of include only once
#endif
