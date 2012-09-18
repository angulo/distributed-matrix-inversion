/* StoreI.h -- Store interface header

Copyright (C) 2010 Javier Angulo, César Mora

This file is part of Distributed Matrix Inversion

Distributed Matrix Inversion is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
Distributed Matrix Inversion is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with Distributed Matrix Inversion.  If not, see <http://www.gnu.org/licenses/>. */

#ifndef _STOREI_H
#define _STOREI_H

#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>

#include <MatrixInversion.h>

#include <iostream>

using namespace MatrixInversion;
using namespace std;

class StoreI : public Store {

	private:
		
		MatrixT _matrix;
		IceUtil::RecMutex _mutex;

	public:
	
		StoreI ();
		~StoreI ();

		virtual void setMatrix (const MatrixT& m, const Ice::Current& c);
		virtual MatrixT getMatrix (const Ice::Current& c);
		virtual RowT getRow (long r, long c, const Ice::Current&);
		virtual void setRow (long r, long c, const RowT& row, const Ice::Current&);

};

#endif
