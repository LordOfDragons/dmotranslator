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

#ifndef _DMOTDIALOGFILEPROPERTIES_H_
#define _DMOTDIALOGFILEPROPERTIES_H_

#include "../foxtoolkit.h"

class dmotWindowMain;
class dmotTranslationFile;



/**
 * @brief File Properties Dialog.
 * @author Plüss Roland
 * @version 1.0
 * @date 2010
 */
class dmotDialogFileProperties : public FXDialogBox{
	FXDECLARE( dmotDialogFileProperties )
	
protected:
	dmotDialogFileProperties();
	
public:
	enum eFoxIDs{
		ID_LAST = FXDialogBox::ID_LAST,
		ID_BTN_PATHFILE,
		ID_BTN_ACCEPT
	};
	
private:
	dmotWindowMain *pWindowMain;
	dmotTranslationFile *pFile;
	
	FXTextField *pEditIdentifier;
	FXTextField *pEditName;
	
	FXButton *pBtnAccept;
	FXButton *pBtnCancel;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new dialog. */
	dmotDialogFileProperties( FXWindow *owner, dmotWindowMain *windowMain, dmotTranslationFile *file );
	/** Cleans up the main window prototype. */
	virtual ~dmotDialogFileProperties();
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
	long onBtnAccept( FXObject *sender, FXSelector selector, void *data );
	/*@}*/
};

// end of include only once
#endif
