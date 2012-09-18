/* ClientI.cpp -- Client interface implementation

Copyright (C) 2010 Javier Angulo, César Mora

This file is part of Distributed Matrix Inversion

Distributed Matrix Inversion is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
Distributed Matrix Inversion is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with Distributed Matrix Inversion.  If not, see <http://www.gnu.org/licenses/>. */

#include <MatrixInversion.h>
#include <ClientI.h>

#include <iostream>

using namespace std;

ClientI::ClientI (string result_file): _result_file (result_file)
{
}

ClientI::~ClientI ()
{
}

void
ClientI::write_result (MatrixT m)
{
	ofstream f (_result_file.c_str ());
	
	for (int i = 0; i < m.size (); i++)
		{
			for (int j = 0; j < m[i].size (); j++)
				f << m[i][j] << " ";
			f << endl;
		}
	f.close ();
		
}

void
ClientI::result (CodeT c, const MatrixT& m, const Ice::Current& curr)
{
	if (c == ok)
		write_result (m);
	else
		cout << "Not invertible matrix" << endl;
	curr.adapter->getCommunicator ()->shutdown ();	
}
