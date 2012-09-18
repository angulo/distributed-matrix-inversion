/* StoreI.cpp -- Store interface implementation

Copyright (C) 2010 Javier Angulo, César Mora

This file is part of Distributed Matrix Inversion

Distributed Matrix Inversion is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
Distributed Matrix Inversion is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with Distributed Matrix Inversion.  If not, see <http://www.gnu.org/licenses/>. */

#include <StoreI.h>

StoreI::StoreI ()
{

}

StoreI::~StoreI ()
{

}

void
StoreI::setMatrix (const MatrixT& m, const Ice::Current &c)
{
	IceUtil::RecMutex::Lock lock (_mutex);
	_matrix = m;
}

MatrixT
StoreI::getMatrix (const Ice::Current& c)
{
	IceUtil::RecMutex::Lock lock (_mutex);
	cout << "Stored matrix requested" << endl;
	if (!_matrix.empty () > 0)
		return _matrix;
	else
		{
			StoreException ex;
			ex.reason = "No matrix stored";
			throw ex;
		}
}

RowT
StoreI::getRow (long r, long c, const Ice::Current&)
{
	IceUtil::RecMutex::Lock lock (_mutex);
	if (r > _matrix.size () || c > _matrix[r].size ())
		{
			StoreException ex;
			ex.reason = "Invalid index when accessing matrix";
			throw ex;
		}
	RowT row;
	for (RowT::iterator it = _matrix[r].begin () + c; it != _matrix[r].end (); it++)
		row.push_back (*it);
	return row;
}

void
StoreI::setRow (long r, long c, const RowT& row, const Ice::Current&)
{
	IceUtil::RecMutex::Lock lock (_mutex);
	if (r > _matrix.size () || c + row.size () > _matrix[r].size ())
		{
			StoreException ex;
			ex.reason = "Invalid index when accessing matrix";
			throw ex;
		}
	for (int i = 0; i < row.size (); i++)
		_matrix[r][c + i] = row[i];
}
