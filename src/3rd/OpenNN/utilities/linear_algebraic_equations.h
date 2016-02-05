/****************************************************************************************************************/
/*                                                                                                              */
/*   L I N E A R   A L G E B R A I C   E Q U A T I O N S   C L A S S   H E A D E R                              */
/*                                                                                                              */ 
/*   Roberto Lopez                                                                                              */ 
/*   International Center for Numerical Methods in Engineering (CIMNE)                                          */
/*   Technical University of Catalonia (UPC)                                                                    */
/*   Barcelona, Spain                                                                                           */
/*   E-mail: rlopez@cimne.upc.edu                                                                               */ 
/*                                                                                                              */
/****************************************************************************************************************/

#ifndef __LINEARALGEBRAICEQUATIONS_H__
#define __LINEARALGEBRAICEQUATIONS_H__

#include "../utilities/vector.h"
#include "../utilities/matrix.h"

namespace OpenNN
{

/// This class contains methods for the solution of linear algebraic equations. 
/// In particular it implements the Gauss-Jordan eliminiation, LU decomposition and
/// Cholesky decomposition methods.

class LinearAlgebraicEquations 
{
   public:

   // CONSTRUCTOR

   LinearAlgebraicEquations(void);


   // DESTRUCTOR

   ~LinearAlgebraicEquations(void);


   // ASSIGNMENT OPERATOR

   LinearAlgebraicEquations& operator = (const LinearAlgebraicEquations&);

   // EQUAL TO OPERATOR

   bool operator == (const LinearAlgebraicEquations&) const;

   // METHODS

   // Gauss Jordan methods

   void perform_Gauss_Jordan_elimination(Matrix<double>&, Matrix<double>&) const;
   void perform_Gauss_Jordan_elimination(Matrix<double>&, Vector<double>&) const;

   Vector<double> calculate_Gauss_Jordan_solution(Matrix<double>, Vector<double>) const;

   private: 

   void swap(double&, double&) const;

};

}

#endif


// OpenNN: Open Neural Networks Library.
// Copyright (C) 2005-2012 Roberto Lopez 
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.

// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
