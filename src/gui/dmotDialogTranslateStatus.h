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

#ifndef _DMOTDIALOGTRANSLATESTATUS_H_
#define _DMOTDIALOGTRANSLATESTATUS_H_

#include "../foxtoolkit.h"

class dmotWindowMain;
class dmotTranslateStatus;



/**
 * @brief Translate Status Dialog.
 * @author Plüss Roland
 * @version 1.0
 * @date 2010
 */
class dmotDialogTranslateStatus : public FXDialogBox{
	FXDECLARE( dmotDialogTranslateStatus )
	
protected:
	dmotDialogTranslateStatus();
	
public:
	enum eFoxIDs{
		ID_LAST = FXDialogBox::ID_LAST
	};
	
private:
	dmotWindowMain *pWindowMain;
	dmotTranslateStatus *pStatus;
	
	FXTable *pTblEntries;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new dialog. */
	dmotDialogTranslateStatus( FXWindow *owner, dmotWindowMain *windowMain, dmotTranslateStatus *status );
	/** Cleans up the dialog. */
	virtual ~dmotDialogTranslateStatus();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Fill log table with entries. */
	void UpdateEntries();
	/*@}*/
};

#endif
