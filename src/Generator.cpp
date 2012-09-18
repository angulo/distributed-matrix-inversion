/* Generator.cpp -- Identity matrix generator

Copyright (C) 2010 Javier Angulo, César Mora

This file is part of Distributed Matrix Inversion

Distributed Matrix Inversion is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
Distributed Matrix Inversion is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with Distributed Matrix Inversion.  If not, see <http://www.gnu.org/licenses/>. */

#include <iostream>
#include <cstdlib>
#include <fstream>

using namespace std;

int main (int argc, char **argv)
{	
	if (argc != 3)
		{
			cout << string ("Usage:\n\t ") + argv[0] + " <file_name> <width>" << endl;
			return 1;
		}
	ofstream f (argv[1]);
	long count = atol (argv[2]);
	long one = 0;
	
	for (long i = 0; i < count; i++)
		{
			for (long j = 0; j < count; j++)
				if (j == one)
					f << "1 ";
				else
					f << "0 ";
			f << endl;
			one++;
		}
	f.close ();
}
