/* MasterI.h -- Master interface header

Copyright (C) 2010 Javier Angulo, César Mora

This file is part of Distributed Matrix Inversion

Distributed Matrix Inversion is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
Distributed Matrix Inversion is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with Distributed Matrix Inversion.  If not, see <http://www.gnu.org/licenses/>. */

#ifndef _MASTERI_H
#define _MASTERI_H

#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>

#include <MatrixInversion.h>

#include <set>
#include <vector>
#include <iostream>

using namespace MatrixInversion;
using namespace std;

class MasterI : public Master {

	private:
		
		StorePrx _store;
		vector <WorkT> _works_todo;	
		vector <AluPrx> _alus;
		long _works_done_count;
		long _todo_count;
		IceUtil::RecMutex _mutex;

		/* Current inversion */
		void _inverse_step ();
		/* Column */
		long _c;
		/* Below or above the diagonal */
		bool _below;
		/* Source matrix width */
		long _width;
		/* Client proxy */
		ClientPrx _client;

	public:
	
		MasterI (StorePrx store);
		~MasterI ();
	
		virtual void inverse (const MatrixT& m, const ClientPrx& clpx, const Ice::Current& c);
		virtual WorkT workFinished (const Ice::Current &c);
		virtual void subscribe (const AluPrx& alu, const Ice::Current &c);

};

#endif
