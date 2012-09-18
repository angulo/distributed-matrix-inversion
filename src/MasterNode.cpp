/* MasterNode.cpp -- Master executable implementation

Copyright (C) 2010 Javier Angulo, César Mora

This file is part of Distributed Matrix Inversion

Distributed Matrix Inversion is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
Distributed Matrix Inversion is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with Distributed Matrix Inversion.  If not, see <http://www.gnu.org/licenses/>. */

#include <Ice/Ice.h>

#include <MasterI.h>
#include <MatrixInversion.h>

#include <iostream>

using namespace MatrixInversion;
using namespace std;

class MasterNode : public Ice::Application {
	
	public:
		
		virtual int 
		run (int, char**)
		{
			Ice::PropertiesPtr properties = communicator ()->getProperties ();
			
			Ice::ObjectPrx obj = communicator ()->propertyToProxy ("Store.proxy");
			StorePrx st_px = StorePrx::checkedCast (obj);

			Ice::ObjectAdapterPtr _adapter = communicator ()->createObjectAdapter (properties->getProperty ("Adapter.name"));

			_adapter->activate ();
			shutdownOnInterrupt ();

			MasterPtr m = new MasterI (st_px);
			_adapter->add (m, communicator ()->stringToIdentity ("Master"));
			cout << "Master running" << endl;

			communicator ()->waitForShutdown ();
			return 0;
		}
};

int 
main (int argc, char **argv)
{
	MasterNode app;
	return app.main (argc, argv, "master.cfg");
}
