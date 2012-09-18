/* StoreNode.cpp -- Store executable implementation

Copyright (C) 2010 Javier Angulo, César Mora

This file is part of Distributed Matrix Inversion

Distributed Matrix Inversion is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
Distributed Matrix Inversion is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with Distributed Matrix Inversion.  If not, see <http://www.gnu.org/licenses/>. */

#include <Ice/Ice.h>

#include <StoreI.h>
#include <MatrixInversion.h>

#include <iostream>

using namespace MatrixInversion;
using namespace std;

class StoreNode : public Ice::Application {
	
	public:
		
		virtual int 
		run (int, char**)
		{
			Ice::PropertiesPtr properties = communicator ()->getProperties ();
			
			Ice::ObjectAdapterPtr _adapter = communicator ()->createObjectAdapter (properties->getProperty ("Adapter.name"));

			_adapter->activate ();
			shutdownOnInterrupt ();

			StorePtr s = new StoreI ();
			_adapter->add (s, communicator ()->stringToIdentity ("Store"));

			communicator ()->waitForShutdown ();
			return 0;
		}
};

int 
main (int argc, char **argv)
{
	StoreNode app;
	return app.main (argc, argv, "store.cfg");
}
