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

#ifndef _DMOTPANELFILES_H_
#define _DMOTPANELFILES_H_

#include <vector>

#include "../foxtoolkit.h"

class dmotWindowMain;
class dmotTranslationArchive;
class dmotTranslationFile;
class dmotTranslationSection;
class dmotTableText;



/**
 * @brief Files Panel.
 * @author Plüss Roland
 * @version 1.0
 * @date 2010
 */
class dmotPanelFiles : public FXVerticalFrame{
	FXDECLARE( dmotPanelFiles )
protected:
	dmotPanelFiles();
	
public:
	enum eFoxIDs{
		ID_CANVAS = FXVerticalFrame::ID_LAST,
		ID_CB_ARCHIVE,
		ID_CB_FILE,
		ID_CB_SECTION,
		ID_TABLE_TEXT,
		ID_LAST
	};
	
private:
	dmotWindowMain *pWindowMain;
	
	FXComboBox *pCBArchive;
	FXComboBox *pCBFile;
	FXComboBox *pCBSection;
	dmotTableText *pTableText;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new panel. */
	dmotPanelFiles( dmotWindowMain *windowMain, FXComposite *container );
	/** Cleans up the panel. */
	virtual ~dmotPanelFiles();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the main window. */
	inline dmotWindowMain *GetWindowMain() const{ return pWindowMain; }
	
	/** Retrieves the selected archive. */
	dmotTranslationArchive *GetSelectedArchive() const;
	/** Select the latest archive in the translation. */
	void SelectLatestArchive();
	/** Retrieves the selected file. */
	dmotTranslationFile *GetSelectedFile() const;
	/** Select the latest file in the archive. */
	void SelectLatestFile();
	/** Retrieves the selected section. */
	dmotTranslationSection *GetSelectedSection() const;
	/** Select the latest section in the translation. */
	void SelectLatestSection();
	/** Retrieves the index of the text the current item belongs to or -1 if none. */
	int GetCurrentTextIndex() const;
	/** Retrieves the selected record index. */
	int GetSelectedRecord() const;
	/** Retrieves the range of selected record indices. */
	void GetSelectedRecordRange( int &from, int &to ) const;
	
	/** Update archive list. */
	void UpdateArchiveList();
	/** Update file list. */
	void UpdateFileList();
	/** Update section list. */
	void UpdateSectionList();
	/** Update the text table. */
	void UpdateTextTable();
	/** Update the given record. */
	void UpdateRecord( int index );
	/** Unicode font changed. */
	void UnicodeFontChanged();
	/*@}*/
	
	/** @name Events */
	/*@{*/
	long onCBArchiveChanged( FXObject *sender, FXSelector selector, void *data );
	long onCBFileChanged( FXObject *sender, FXSelector selector, void *data );
	long onCBSectionChanged( FXObject *sender, FXSelector selector, void *data );
	long onTableTextChanged( FXObject *sender, FXSelector selector, void *data );
	long onTableTextHeaderResized( FXObject *sender, FXSelector selector, void *data );
	long onTableTextHeaderForward( FXObject *sender, FXSelector selector, void *data );
	/*@}*/
};

// end of include only once
#endif
