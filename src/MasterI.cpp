/* MasterI.cpp -- Master interface implementation

Copyright (C) 2010 Javier Angulo, César Mora

This file is part of Distributed Matrix Inversion

Distributed Matrix Inversion is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
Distributed Matrix Inversion is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with Distributed Matrix Inversion.  If not, see <http://www.gnu.org/licenses/>. */

#include <MasterI.h>

MasterI::MasterI (StorePrx store) : _store (store)
{
}

MasterI::~MasterI ()
{
}

void
MasterI::_inverse_step ()
{
	if (_below)
		{
			_c++;
			if (_c == _width)
				{
					cout << "First step completed" << endl;
					_below = false;
					_c = _width - 1;
				}
		}
	else
		{
			_c--;
			if (_c == 0)
				{
					cout << "Second step completed" << endl;
					/* The result is ready to be sent */
					MatrixT result;
					for (int i = 0; i < _width; i++)
						result.push_back (_store->getRow (i, _width));
					_client->result (ok, result);
					return;
				}
		}

	/* Reduce below the diagonal */
	if (_below)
		{
			RowT pivot_row = _store->getRow (_c, _c);
			/* Not invertible matrix */
			if (pivot_row[0] == 0)
				{
					MatrixT dummy;
					cout << "0 in diagonal: impossible to invert" << endl;
					_client->result (notinvertible, dummy);
					return;
				}
			for (int j = pivot_row.size () - 1; j >= 0; j--)
				/* Divide the row by the pivot */
				pivot_row[j] /= pivot_row[0];

			/* Store the pivot row */
			_store->setRow (_c, _c, pivot_row);
			cout << "Pivot row stored" << endl;

			for (int j = _c + 1; j < _width; j++)
				{
					/* Create the works */
					WorkT w;
					w.r = j;
					w.c = _c;
					w.p = _c;
					_works_todo.push_back (w);
				}
		}
	else
		{
			for (int j = _c - 1; j >= 0; j--)
				{
					/* Create the works */
					WorkT w;
					w.r = j;
					w.c = _c;
					w.p = _c;
					_works_todo.push_back (w);
				}
		}
	/* Restart the counters */
	_todo_count = _works_todo.size ();
	_works_done_count = 0;
	/* Check for no-works step */
	if (_todo_count == 0)
		{
			_inverse_step ();
			return;
		}
	cout << _todo_count << " works of column " << _c << " to be done" << endl;

	/* Assign works to subscribed alus */
	int count = _todo_count;
	_mutex.lock ();
	for (vector<AluPrx>::iterator it = _alus.begin (); it != _alus.end () && count > 0; it++)
		{
			(*it)->begin_reduceRow (_works_todo.front ());
			_works_todo.erase (_works_todo.begin ());
			count--;
		}
	_mutex.unlock ();	
}

void
MasterI::inverse (const MatrixT& m, const ClientPrx& clpx, const Ice::Current& c)
{
	cout << "Matrix requested" << endl;
	MatrixT src = m;	
	_client = clpx;

	/* Check if it's a square matrix */
	_width = src.size ();
	for (int i = 0; i < _width; i++)
		if (src[i].size () != _width)
			{
				cout << src[i].size () << " & " << _width << endl;
				cout << "Matrix is not squared: impossible to invert" << endl;
				MatrixT dummy;
				_client->result (notinvertible, dummy);
				return;
			}

	/* Extend the matrix */
	int one_position = 0;
	for (int k = 0; k < _width; k++)
		{	
			for (int i = 0; i < _width; i++)
				if (i == one_position)
					{
						src[k].push_back (1.0);
					}
				else
					src[k].push_back (0.0);
			one_position++;
		}
	cout << _width << "x" << _width << " matrix extended" << endl;		
	_store->setMatrix (src);
	cout << "Extended matrix stored" << endl;
	
	/* Set the inversion initial properties */
	_c = -1;
	_below = true;
	/* Make the first call */
	_inverse_step ();
}

WorkT
MasterI::workFinished (const Ice::Current& c)
{
	IceUtil::RecMutex::Lock lock (_mutex);
	_works_done_count++;
	if (_works_done_count == _todo_count)
		{
			cout << "Column " << _c << "/" << (_width - 1) << string (" of ") +  (_below? "first" : "second") <<  " step completed" << endl;
			_inverse_step ();
			WorkT w = {-1, -1, -1};
			return w;
		}
		
	if (_works_todo.size () > 0)
		{
			WorkT w = _works_todo.front ();
			_works_todo.erase (_works_todo.begin ());
			return w;
		}
	else
		{
			WorkT w = {-1, -1, -1};
			return w;
		}
}

void
MasterI::subscribe (const AluPrx& alu, const Ice::Current&)
{
	IceUtil::RecMutex::Lock lock (_mutex);
	cout << "Alu subscribed" << endl;
	_alus.push_back (alu);
}
