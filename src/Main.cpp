/* Main.cpp -- Client executable used to request inversions

Copyright (C) 2010 Javier Angulo, César Mora

This file is part of Distributed Matrix Inversion

Distributed Matrix Inversion is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
Distributed Matrix Inversion is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with Distributed Matrix Inversion.  If not, see <http://www.gnu.org/licenses/>. */

#include <Ice/Ice.h>

#include <ClientI.h>
#include <MatrixInversion.h>

#include <iostream>
#include <vector>
#include <fstream>

using namespace MatrixInversion;
using namespace std;

class MainNode : public Ice::Application {
	
	public:
		
		virtual int 
		run (int argc, char** argv)
		{
			if (argc != 3)
				{
					cout << string ("Usage:\n\t ") + argv[0] + " <file_with_matrix> <file_to_store_result>" << endl;
					return 1;
				}
			
			Ice::PropertiesPtr properties = communicator ()->getProperties ();
			Ice::ObjectAdapterPtr _adapter = communicator ()->createObjectAdapterWithEndpoints (properties->getProperty ("Adapter.name"), properties->getProperty ("Adapter.endpoints"));

			_adapter->activate ();
			shutdownOnInterrupt ();
		
			ClientPtr client = new ClientI (string (argv[2]));
			ClientPrx cl_px = ClientPrx::checkedCast (_adapter->add (client, communicator ()->stringToIdentity (IceUtil::generateUUID ())));
		
			MasterPrx master = MasterPrx::checkedCast (communicator ()->propertyToProxy ("Master.proxy"));
	
			/* File with the matrix */

			ifstream f (argv[1]);
			if (!f.is_open ())
				{
					cerr << "Invalid file" << endl;
					return 1;
				}
			MatrixT m;
			RowT r;
			char c;
			string number;
			while (1)
				{
					f.read (&c, 1);
					if (f.eof ())
						break;
					if (isblank (c) && number.size () > 0)
						{
							r.push_back (atof (number.c_str ()));
							number.clear ();
						}		
					else if (isblank (c))
						continue;
					else if (c == '\n')
						{
							if (number.size () > 0)
								r.push_back (atof (number.c_str ()));
							if (r.size () > 0)
								m.push_back (r);
							number.clear ();
							r.clear ();
						}
					else
						number.append (1, c);
				}
	
			/* Do the inversion */
			try
				{
					master->inverse (m, cl_px);
				}
			catch (BaseException& ex)
				{
					cout << ex.reason << endl;
					return 1;
				}
			catch (Ice::Exception& ex)
				{
					cout << ex.what () << endl; 
					return 1;
				}
			communicator ()->waitForShutdown ();
			return 0;
		}
};

int 
main (int argc, char **argv)
{
	MainNode app;
	return app.main (argc, argv, "main.cfg");
}
