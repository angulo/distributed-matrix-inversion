// -*- mode: c++; coding: utf-8 -*-

/* MatrixInversion.ice -- Slice definition for the inversion system

Copyright (C) 2010 Javier Angulo, César Mora

This file is part of Distributed Matrix Inversion

Distributed Matrix Inversion is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
Distributed Matrix Inversion is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with Distributed Matrix Inversion.  If not, see <http://www.gnu.org/licenses/>. */

#ifndef _MATRIX_INVERSION
#define _MATRIX_INVERSION

#include <Ice/Identity.ice>

module MatrixInversion {
	
	/**
	* Type to represent a row of a matrix.
	**/
	sequence<float> RowT;

	/**
	* Type to represent a matrix.
	**/
	sequence<RowT> MatrixT;

	/**
	* Result codes
	**/
	enum CodeT {ok, notinvertible};

	/**
	* Type to represent a work
	**/
	struct WorkT
	{
		/** 
		* Row
		**/
		long r;
		/**
		* Column 
		**/
		long c;
		/** 
		Pivot 
		**/
		long p;
	};

	/**
	* General exception.
	**/
	exception BaseException {
		string reason;
	};
	
	/**
	* To be thrown with storage problems.
	**/
	exception StoreException extends BaseException {
	};

	/**
	* To be thrown with inversion problems.
	**/
	exception AluException extends BaseException {
	};

	/**
	* To obtain the results
	**/
	interface Client {

		/**
		* Return the result of the inversion
		* @param c Result code
		* @param m Matrix
		**/
		void result (CodeT c, MatrixT m);
	};

	/**
	* Provides arithmetic operations inteface used to
	* reduce each row of a matrix */
	interface Alu {
	
		/**
		* Start the alu
		**/
		void start ();

		/**
		* Compute the reduction of the requested row.
		* @param mid Matrix id.
		* @param r Row to reduce.
		* @param c Column of the row to start the reduction from.
		* @param p Row number to be used as pivot to reduce.
		**/
		void reduceRow (WorkT w) throws AluException;
	};

	/**
	* Provides matrix inversion calls interface.
	**/
	interface Master {
		
		/**
		* Calculate the inverse of a matrix.
		* @param src Original matrix.
		* @return Inverse of the original matrix.
		* @throws MatrixNotInvertibleException.
		**/
		void inverse (MatrixT src, Client* clpx);

		/**
		* Notify the end of a work
		* @param r Row of the matrix.
		* @param c Column of the matrix.
		* @return A new work.
		**/
		WorkT workFinished ();

		/**
		* Subscribe to the master.
		* @param a Proxy to the subscribed alu.
		**/
		void subscribe (Alu* a);
	
	};

	/**
	* Provides the matrix storage and gets
	* both partial and final results
	**/
	interface Store {

		/**
		* Set the matrix to store.
		* @param m Matrix to store.
		**/
		void setMatrix (MatrixT m);
	
		/**
		* Get the stored matrix.
		* @throws StoreException There isn't a stored matrix.
		* @return Stored matrix.
		**/
		MatrixT getMatrix () throws StoreException;

		/**
		* Get the r'th row of the stored matrix.
		* @param r Row number, from 0 to n-1.
		* @param c Column number to start from, from 0 to n-1.
		* @return R'th row starting from C'th column.
		**/
		RowT getRow (long r, long c) throws StoreException;

		/**
		* Set the r'th row of the stored matrix. 
		* @param r Row number, from 0 to n-1.
		* @param c Column number to start from, from 0 to n-1.
		* @param row Row to be set.
		**/
		void setRow (long r, long c, RowT row) throws StoreException;
	};
};

#endif
