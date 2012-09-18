/* ClientI.h -- Client interface header

Copyright (C) 2010 Javier Angulo, César Mora

This file is part of Distributed Matrix Inversion

Distributed Matrix Inversion is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
Distributed Matrix Inversion is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with Distributed Matrix Inversion.  If not, see <http://www.gnu.org/licenses/>. */

#ifndef _CLIENT_H
#define _CLIENT_H

#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>

#include <MatrixInversion.h>

#include <iostream>
#include <fstream>

using namespace MatrixInversion;
using namespace std;

class ClientI : public Client {

	private:
		
		/* File to store the result */
		string _result_file;

		void write_result (MatrixT m);
	
	public:
	
		ClientI (string result_file);
		~ClientI ();

		virtual void result (CodeT c, const MatrixT& m, const Ice::Current&);
};

#endif
