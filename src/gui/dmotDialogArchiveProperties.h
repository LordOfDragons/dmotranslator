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

#ifndef _DMOTDIALOGARCHIVEPROPERTIES_H_
#define _DMOTDIALOGARCHIVEPROPERTIES_H_

#include "../foxtoolkit.h"

class dmotWindowMain;
class dmotTranslationArchive;



/**
 * @brief Archive Properties Dialog.
 * @author Plüss Roland
 * @version 1.0
 * @date 2010
 */
class dmotDialogArchiveProperties : public FXDialogBox{
	FXDECLARE( dmotDialogArchiveProperties )
	
protected:
	dmotDialogArchiveProperties();
	
public:
	enum eFoxIDs{
		ID_LAST = FXDialogBox::ID_LAST,
		ID_BTN_PATHARCHIVE,
		ID_BTN_PATHFILEMAP,
		ID_BTN_ACCEPT
	};
	
private:
	dmotWindowMain *pWindowMain;
	dmotTranslationArchive *pArchive;
	
	FXTextField *pEditName;
	FXTextField *pEditPathArchive;
	FXButton *pBtnPathArchive;
	FXTextField *pEditPathFileMap;
	FXButton *pBtnPathFileMap;
	
	FXButton *pBtnAccept;
	FXButton *pBtnCancel;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new dialog. */
	dmotDialogArchiveProperties( FXWindow *owner, dmotWindowMain *windowMain, dmotTranslationArchive *archive );
	/** Cleans up the dialog. */
	virtual ~dmotDialogArchiveProperties();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Reset values. */
	void ResetValues();
	/** Verify if the input is value. */
	bool ValidInput();
	/** Apply changes to the file. */
	void ApplyChanges();
	/*@}*/
	
	/** @name Events */
	/*@{*/
	long onBtnPathArchive( FXObject *sender, FXSelector selector, void *data );
	long onBtnPathFileMap( FXObject *sender, FXSelector selector, void *data );
	
	long onBtnAccept( FXObject *sender, FXSelector selector, void *data );
	/*@}*/
};

#endif
