/*
* AntiDupl Dynamic-Link Library.
*
* Copyright (c) 2016 Borisov Dmitry.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy 
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
* copies of the Software, and to permit persons to whom the Software is 
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in 
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

#include "adNeuralNetwork.h"
#include "adStrings.h"
#include "adStatisticsOfDeleting.h"
#include "adFileUtils.h"
#include "adResult.h"
#include "OpenNN/opennn.h"

using namespace OpenNN;

namespace ad
{
	TNeuralNetwork::TNeuralNetwork(void)
		//m_neuralNetworkPath()
		//:m_neural_network_point()
		//:m_neural_network_pointer(NULL)
	{
		//m_neuralNetworkPath = TString(GetApplicationDirectory() + TEXT("\\NeuralNetwork\\neural_network.xml"));
		m_neural_network_pointer = new OpenNN::NeuralNetwork();
		//m_neural_network = new OpenNN::NeuralNetwork();
	}

	TNeuralNetwork::~TNeuralNetwork(void)
	{
	}

	bool TNeuralNetwork::Load()
	{
		m_neural_network_pointer->load("NeuralNetwork/neural_network.xml");
		return true;
	}

	bool TNeuralNetwork::GetPredict(TResult *pResult)
	{
		//m_neural_network_point = new OpenNN::NeuralNetwork();
		//if(m_neural_network_point)
			//m_neural_network_point = new OpenNN::NeuralNetwork();
		//	return AD_ERROR_INVALID_POINTER;
		//NeuralNetwork neural_network(9, 8, 1);

		//m_neural_network_pointer->load("NeuralNetwork/neural_network.xml");
		m_neural_network_pointer->set_scaling_unscaling_layers_flag(false);

		// Print results to screen

		Vector<double> inputs(9, 0.0);
		Vector<double> outputs(1, 0.0);

		//std::cout << "pregnant glucose pressure thickness insulin mass_index pedigree age" << std::endl;

		inputs[0] = pResult->difference;
		inputs[1] = pResult->first->Area();
		inputs[2] = pResult->first->size;
		inputs[3] = pResult->first->blockiness;
		inputs[4] = pResult->first->blurring;
		inputs[5] = pResult->second->Area();
		inputs[6] = pResult->second->size;
		inputs[7] = pResult->second->blockiness;
		inputs[8] = pResult->second->blurring;

		outputs = m_neural_network_pointer->calculate_outputs(inputs);
		double output = outputs[0];

		bool deleteFirst = outputs.calculate_binary()[0];
		return deleteFirst;
	}

	adError TNeuralNetwork::Train()
	{
		TString path = TStatisticsOfDeleting::GetStatisticsPath();
		//TString m_neuralNetworkPath = TString(GetApplicationDirectory() + TEXT("\\NeuralNetwork\\neural_network.xml"));

		DataSet data_set;
	      
		data_set.load_data(path.ToString());

		// Variables information

		VariablesInformation* variables_information_pointer = data_set.get_variables_information_pointer(); 

		//variables_information_pointer->set(1, 8);

		variables_information_pointer->set_name(0, "difference");

		variables_information_pointer->set_name(1, "1area");

		variables_information_pointer->set_name(2, "1size");
		variables_information_pointer->set_units(2, "byte");

		variables_information_pointer->set_name(3, "1blockiness");

		variables_information_pointer->set_name(4, "1blurring");

		variables_information_pointer->set_name(5, "2area");

		variables_information_pointer->set_name(6, "2size");
		variables_information_pointer->set_units(6, "byte");

		variables_information_pointer->set_name(7, "2blockiness");	

		variables_information_pointer->set_name(8, "2blurring");

		variables_information_pointer->set_name(9, "firstDeleted"); //вывод  

		const Vector< Vector<std::string> > inputs_targets_information = variables_information_pointer->arrange_inputs_targets_information();

		// Instances information

		//InstancesInformation* instances_information_pointer = data_set.get_instances_information_pointer();

		//instances_information_pointer->split_random_indices(0.75, 0.0, 0.25);

		  //const Vector< Vector<double> > inputs_targets_statistics = data_set.scale_inputs_minimum_maximum();
		const Vector< Vector<double> > inputs_targets_statistics = data_set.scale_inputs_targets();  

		// Neural network 
		  
		//NeuralNetwork neural_network(8, 6, 1);

		const unsigned int inputs_number = data_set.get_variables_information().count_inputs_number();
		const unsigned int hidden_neurons_number = 9;
		const unsigned int outputs_number = data_set.get_variables_information().count_targets_number();

		//NeuralNetwork neural_network(inputs_number, hidden_neurons_number, outputs_number);
		//m_neural_network_point = new OpenNN::NeuralNetwork(inputs_number, hidden_neurons_number, outputs_number);
		m_neural_network_pointer = new OpenNN::NeuralNetwork(inputs_number, hidden_neurons_number, outputs_number);

		m_neural_network_pointer->set_inputs_outputs_information(inputs_targets_information); 
		m_neural_network_pointer->set_inputs_outputs_statistics(inputs_targets_statistics); 

		m_neural_network_pointer->set_scaling_unscaling_layers_flag(false);
		//neural_network.set_scaling_unscaling_layers_flag(true);

		// Performance functional

		PerformanceFunctional performance_functional(m_neural_network_pointer, &data_set);

		// Training strategy

		TrainingStrategy training_strategy(&performance_functional);

		QuasiNewtonMethod* quasi_Newton_method_pointer = new QuasiNewtonMethod(&performance_functional);
		quasi_Newton_method_pointer->set_minimum_performance_increase(1.0e-6);
		quasi_Newton_method_pointer->set_reserve_evaluation_history(true);
		  //quasi_Newton_method_pointer->set_maximum_epochs_number(10000);
		training_strategy.set_main_training_algorithm_pointer(quasi_Newton_method_pointer);
		  
		//std::cout << "initialization training: " << training_strategy.write_initialization_training_algorithm_type() << std::endl;
		//std::cout << "main training: " << training_strategy.write_main_training_algorithm_type() << std::endl;
		//std::cout << "refinement training: " << training_strategy.write_refinement_training_algorithm_type() << std::endl;

		TrainingStrategy::Results training_strategy_results = training_strategy.perform_training();

		//neural_network.set_inputs_scaling_outputs_unscaling_methods("MinimumMaximum");
		//neural_network.set_scaling_unscaling_layers_flag(true);

		// Testing analysis
		//TestingAnalysis testing_analysis(&neural_network, &data_set);
		//testing_analysis.construct_pattern_recognition_testing();
		//PatternRecognitionTesting* pattern_recognition_testing_pointer = testing_analysis.get_pattern_recognition_testing_pointer();

		// Save results

		//data_set.save("NeuralNetwork/data_set.xml");

		m_neural_network_pointer->save("NeuralNetwork/neural_network.xml");
		m_neural_network_pointer->save_expression("NeuralNetwork/expression.txt");

		training_strategy.save("NeuralNetwork/training_strategy.xml");
		training_strategy_results.save("NeuralNetwork/training_strategy_results.dat");

		//pattern_recognition_testing_pointer->save_confusion("pima_indians_diabetes/confusion.dat");   
		//pattern_recognition_testing_pointer->save_binary_classification_test("pima_indians_diabetes/binary_classification_test.dat");  
		  
		return adError::AD_OK;
	}
}