/* AluI.cpp -- Alu interface implementation

Copyright (C) 2010 Javier Angulo, César Mora

This file is part of Distributed Matrix Inversion

Distributed Matrix Inversion is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
Distributed Matrix Inversion is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with Distributed Matrix Inversion.  If not, see <http://www.gnu.org/licenses/>. */

#include <AluI.h>

AluI::AluI (MasterPrx master, StorePrx store) : _master (master), _store (store)
{
}

AluI::~AluI ()
{
}

void
AluI::start (const Ice::Current& c)
{
	_me = AluPrx::uncheckedCast (c.adapter->createProxy (c.id));
	_master->subscribe (_me);
}

void
AluI::reduceRow (const WorkT& work, const Ice::Current& c)
{
	WorkT w = work;
	/* Until there's no more work to do */
	while (w.r != -1)
		{
			RowT pivot = _store->getRow (w.p, w.c);	
			try
				{
					RowT row = _store->getRow (w.r, w.c);
					/* Reduce the specified row */
					for (int i = row.size () - 1; i >= 0; i--)
						row[i] = row[i] - (pivot[i] * row[0]);
					_store->setRow (w.r, w.c, row);
					w = _master->workFinished ();
				}
			catch (StoreException& ex)
				{
					AluException aluex;
					aluex.reason = ex.reason + ". Problem while getting data";
					throw aluex;
				}
			catch (...)
				{
					AluException aluex;
					aluex.reason = "Storage unaccessible";
					throw aluex;
				}
		}
}

