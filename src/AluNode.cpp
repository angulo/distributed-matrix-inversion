/* AluNode.cpp -- Alu executable implementation  

Copyright (C) 2010 Javier Angulo, César Mora

This file is part of Distributed Matrix Inversion

Distributed Matrix Inversion is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
Distributed Matrix Inversion is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with Distributed Matrix Inversion.  If not, see <http://www.gnu.org/licenses/>.  */


#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>

#include <AluI.h>
#include <MatrixInversion.h>

#include <iostream>

using namespace MatrixInversion;
using namespace std;

class AluNode : public Ice::Application {
	
	public:
		
		virtual int 
		run (int, char**)
		{
			Ice::PropertiesPtr properties = communicator ()->getProperties ();
			
			Ice::ObjectAdapterPtr _adapter = communicator ()->createObjectAdapterWithEndpoints (properties->getProperty ("Adapter.name"), properties->getProperty ("Adapter.endpoints"));

			_adapter->activate ();
			shutdownOnInterrupt ();

			MasterPrx master = MasterPrx::checkedCast (communicator ()->propertyToProxy ("Master.proxy"));
			StorePrx store = StorePrx::checkedCast (communicator ()->propertyToProxy ("Store.proxy"));
	
			AluPtr a = new AluI (master, store);
			AluPrx alu_px = AluPrx::checkedCast (_adapter->add (a, communicator ()->stringToIdentity (IceUtil::generateUUID ())));
	
			alu_px->start ();
			communicator ()->waitForShutdown ();
			return 0;
		}
};

int 
main (int argc, char **argv)
{
	AluNode app;
	return app.main (argc, argv, argv[1]);
}
