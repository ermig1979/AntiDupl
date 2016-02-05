/****************************************************************************************************************/
/*                                                                                                              */
/*   OpenNN: An Open Source Neural Networks C++ Library                                                         */
/*   www.cimne.com/flood                                                                                        */
/*                                                                                                              */
/*  O P E N   N E U R A L   N E T W O R K S   L I B R A R Y                                                     */
/*                                                                                                              */ 
/*   Roberto Lopez                                                                                              */ 
/*   International Center for Numerical Methods in Engineering (CIMNE)                                          */
/*   Technical University of Catalonia (UPC)                                                                    */
/*   Barcelona, Spain                                                                                           */
/*   E-mail: rlopez@cimne.upc.edu                                                                               */ 
/*                                                                                                              */
/****************************************************************************************************************/

#ifndef __OPENNN_H__
#define __OPENNN_H__


// Data set

#include "data_set/data_set.h"
#include "data_set/instances_information.h"
#include "data_set/variables_information.h"


// Mathematical model

#include "mathematical_model/mathematical_model.h"
#include "mathematical_model/ordinary_differential_equations.h"
#include "mathematical_model/plug_in.h"


// Model selection

#include "model_selection/model_selection.h"

// Neural network

#include "neural_network/conditions_layer.h"
#include "neural_network/bounding_layer.h"
#include "neural_network/independent_parameters.h"
#include "neural_network/inputs_outputs_information.h"
#include "neural_network/perceptron_layer.h"
#include "neural_network/multilayer_perceptron.h"
#include "neural_network/perceptron.h"
#include "neural_network/probabilistic_layer.h"
#include "neural_network/scaling_layer.h"
#include "neural_network/unscaling_layer.h"
#include "neural_network/neural_network.h"

// Performance functional

#include "performance_functional/performance_functional.h"

#include "performance_functional/performance_term.h"

#include "performance_functional/neural_parameters_norm.h"

#include "performance_functional/cross_entropy_error.h"
#include "performance_functional/inverse_sum_squared_error.h"
#include "performance_functional/mean_squared_error.h"
#include "performance_functional/minkowski_error.h"
#include "performance_functional/normalized_squared_error.h"
#include "performance_functional/root_mean_squared_error.h"
#include "performance_functional/sum_squared_error.h"

#include "performance_functional/final_solutions_error.h"
#include "performance_functional/solutions_error.h"
#include "performance_functional/independent_parameters_error.h"

// Testing analysis

#include "testing_analysis/function_regression_testing.h"
#include "testing_analysis/inverse_problem_testing.h"
#include "testing_analysis/pattern_recognition_testing.h"
#include "testing_analysis/testing_analysis.h"
#include "testing_analysis/time_series_prediction_testing.h"

// Training strategy

#include "training_strategy/conjugate_gradient.h"
#include "training_strategy/evolutionary_algorithm.h"
#include "training_strategy/gradient_descent.h"
#include "training_strategy/levenberg_marquardt_algorithm.h"
#include "training_strategy/newton_method.h"
#include "training_strategy/quasi_newton_method.h"
#include "training_strategy/random_search.h"
#include "training_strategy/training_algorithm.h"
#include "training_strategy/training_rate_algorithm.h"

// Utilities

#include "utilities/linear_algebraic_equations.h"
#include "utilities/matrix.h"
#include "utilities/numerical_differentiation.h"
#include "utilities/numerical_integration.h"
#include "utilities/vector.h"

// TinyXml includes

#include "../tinyxml/tinyxml.h"

#endif

// OpenNN: An Open Source Neural Networks C++ Library.
// Copyright (C) 2005-2012 Roberto Lopez 
//
// This library is free software; you can redistribute it and/or
// modify it under the s of the GNU Lesser General Public
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

