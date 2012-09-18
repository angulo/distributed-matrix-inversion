/* AluI.h -- Alu interface header

Copyright (C) 2010 Javier Angulo, César Mora

This file is part of Distributed Matrix Inversion

Distributed Matrix Inversion is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
Distributed Matrix Inversion is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with Distributed Matrix Inversion.  If not, see <http://www.gnu.org/licenses/>. */

#ifndef _ALUI_H
#define _ALUI_H

#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>

#include <MatrixInversion.h>

using namespace MatrixInversion;

class AluI : public Alu {

	private:
	
		MasterPrx _master;
		StorePrx _store;
		AluPrx _me;

	public:
	
		AluI (MasterPrx master, StorePrx store);
		~AluI ();

		virtual void start (const Ice::Current&);
		virtual void reduceRow (const WorkT& w, const Ice::Current&);
};

#endif
