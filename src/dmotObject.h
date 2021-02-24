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

#ifndef _DMOTOBJECT_H_
#define _DMOTOBJECT_H_



/**
 * @brief Object Class.
 * Base class for all objects using reference counting.
 * @author Plüss Roland
 * @version 1.0
 * @date 2010
 */
class dmotObject{
private:
	int pRefCount;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new object. */
	dmotObject();
	/** Cleans up the object. */
	virtual ~dmotObject();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the reference count. */
	inline int GetRefCount() const{ return pRefCount; }
	/** Adds a reference and increases the reference count by 1. */
	void AddReference();
	/** Decreases the reference count by one and deletes the widget if the count reaches 0. */
	void FreeReference();
	/*@}*/
};

// end of include only once
#endif
