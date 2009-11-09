/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Written (W) 1999-2008 Soeren Sonnenburg
 * Written (W) 1999-2008 Gunnar Raetsch
 * Copyright (C) 1999-2008 Fraunhofer Institute FIRST and Max-Planck-Society
 */

#include "lib/config.h"

#ifndef HAVE_SWIG
#include <unistd.h>

#include "lib/common.h"

#include "guilib/GUIHMM.h"
#include "interface/SGInterface.h"

#include "features/StringFeatures.h"
#include "features/Labels.h"

#define TMP_DIR "/tmp/"


CGUIHMM::CGUIHMM(CSGInterface* ui_)
: CSGObject(), ui(ui_)
{
	working=NULL;

	pos=NULL;
	neg=NULL;
	test=NULL;

	PSEUDO=1e-10;
	M=4;
}

CGUIHMM::~CGUIHMM()
{

}

bool CGUIHMM::new_hmm(int32_t n, int32_t m)
{
	delete working;
	working=new CHMM(n, m, NULL, PSEUDO);
	M=m;
	return true;
}

bool CGUIHMM::baum_welch_train()
{
	if (!working)
		SG_ERROR("Create HMM first.\n");

	CFeatures* trainfeatures=ui->ui_features->get_train_features();
	if (!trainfeatures)
		SG_ERROR("Assign train features first.\n");
	if (trainfeatures->get_feature_type()!=F_WORD ||
		trainfeatures->get_feature_class()!=C_STRING)
		SG_ERROR("Features must be STRING of type WORD.\n");

	CStringFeatures<uint16_t>* sf=(CStringFeatures<uint16_t>*) trainfeatures;
	SG_DEBUG("Stringfeatures have %ld orig_symbols %ld symbols %d order %ld max_symbols\n",  (int64_t) sf->get_original_num_symbols(), (int64_t) sf->get_num_symbols(), sf->get_order(), (int64_t) sf->get_max_num_symbols());

	working->set_observations(sf);

	return working->baum_welch_viterbi_train(BW_NORMAL);
}


bool CGUIHMM::baum_welch_trans_train()
{
	if (!working)
		SG_ERROR("Create HMM first.\n");

	CFeatures* trainfeatures=ui->ui_features->get_train_features();
	if (!trainfeatures)
		SG_ERROR("Assign train features first.\n");
	if (trainfeatures->get_feature_type()!=F_WORD ||
		trainfeatures->get_feature_class()!=C_STRING)
		SG_ERROR("Features must be STRING of type WORD.\n");

	working->set_observations((CStringFeatures<uint16_t>*) trainfeatures);

	return working->baum_welch_viterbi_train(BW_TRANS);
}


bool CGUIHMM::baum_welch_train_defined()
{
	if (!working)
		SG_ERROR("Create HMM first.\n");
	if (!working->get_observations())
		SG_ERROR("Assign observation first.\n");

	return working->baum_welch_viterbi_train(BW_DEFINED);
}

bool CGUIHMM::viterbi_train()
{
	if (!working)
		SG_ERROR("Create HMM first.\n");
	if (!working->get_observations())
		SG_ERROR("Assign observation first.\n");

	return working->baum_welch_viterbi_train(VIT_NORMAL);
}

bool CGUIHMM::viterbi_train_defined()
{
	if (!working)
		SG_ERROR("Create HMM first.\n");
	if (!working->get_observations())
		SG_ERROR("Assign observation first.\n");

	return working->baum_welch_viterbi_train(VIT_DEFINED);
}

bool CGUIHMM::linear_train(char align)
{
	if (!working)
		SG_ERROR("Create HMM first.\n");

	CFeatures* trainfeatures=ui->ui_features->get_train_features();
	if (!trainfeatures)
		SG_ERROR("Assign train features first.\n");
	if (trainfeatures->get_feature_type()!=F_WORD ||
		trainfeatures->get_feature_class()!=C_STRING)
		SG_ERROR("Features must be STRING of type WORD.\n");

	working->set_observations((CStringFeatures<uint16_t>*) ui->
		ui_features->get_train_features());

	bool right_align=false;
	if (align=='r')
	{
		SG_INFO("Using alignment to right.\n");
		right_align=true;
	}
	else
		SG_INFO("Using alignment to left.\n");
	working->linear_train(right_align);

	return true;
}

bool CGUIHMM::one_class_test(
	char* filename_out, char* filename_roc, bool is_linear)
{
	bool result=false;
	FILE* file_out=stdout;
	FILE* file_roc=NULL;

	if (filename_out)
	{
		file_out=fopen(filename_out, "w");

		if (!file_out)
			SG_ERROR("Could not open file %s.\n", filename_out);

		if (filename_roc)
		{
			file_roc=fopen(filename_roc, "w");

			if (!file_roc)
				SG_ERROR("Could not open %s.\n", filename_roc);
		}
	}

	if (!working)
		SG_ERROR("No HMM defined!\n");

	if (ui->ui_features->get_test_features())
		SG_ERROR("Assign posttest and negtest observations first!\n");

	CStringFeatures<uint16_t>* obs= (CStringFeatures<uint16_t>*) ui->
		ui_features->get_test_features();
	working->set_observations(obs);
	CStringFeatures<uint16_t>* old_test=working->get_observations();
	CLabels* lab=ui->ui_labels->get_test_labels();
	int32_t total=obs->get_num_vectors();
	ASSERT(lab && total==lab->get_num_labels());
	float64_t* output=new float64_t[total];
	int32_t* label=new int32_t[total];

	for (int32_t dim=0; dim<total; dim++)
	{
		output[dim]= is_linear ? working->linear_model_probability(dim) : working->model_probability(dim);
		label[dim]= lab->get_int_label(dim);
	}

	ui->ui_math->evaluate_results(output, label, total, file_out, file_roc);
	working->set_observations(old_test);
	result=true;

	delete[] output;
	delete[] label;
	delete obs;

	if (file_roc)
		fclose(file_roc);
	if (file_out && file_out!=stdout)
		fclose(file_out);

	return result;
}

bool CGUIHMM::hmm_classify(char* param)
{
	bool result=false;
	char outputname[1024];
	char rocfname[1024];
	FILE* outputfile=stdout;
	FILE* rocfile=NULL;
	int32_t numargs=-1;
	int32_t poslinear=0;
	int32_t neglinear=0;

	param=CIO::skip_spaces(param);

	numargs=sscanf(param, "%s %d %d", outputname, &neglinear, &poslinear);

	if (numargs>=1)
	{
		outputfile=fopen(outputname, "w");

		if (!outputfile)
		{
			SG_ERROR( "could not open %s\n",outputname);
			return false;
		}

		if (numargs>=2) 
		{
			rocfile=fopen(rocfname, "w");

			if (!rocfile)
			{
				SG_ERROR( "could not open %s\n",rocfname);
				return false;
			}
		}
	}

	if (pos && neg)
	{
		if (ui->ui_features->get_test_features())
		{
			CStringFeatures<uint16_t>* o= (CStringFeatures<uint16_t>*) ui->
				ui_features->get_test_features();
			CLabels* lab= ui->ui_labels->get_test_labels();

			//CStringFeatures<uint16_t>* old_pos=pos->get_observations();
			//CStringFeatures<uint16_t>* old_neg=neg->get_observations();

			pos->set_observations(o);
			neg->set_observations(o);

			int32_t total=o->get_num_vectors();

			float64_t* output = new float64_t[total];	
			int32_t* label= new int32_t[total];	

			SG_INFO( "classifying using neg %s hmm vs. pos %s hmm\n", neglinear ? "linear" : "", poslinear ? "linear" : "");

			for (int32_t dim=0; dim<total; dim++)
			{
				output[dim]= 
					(poslinear ? pos->linear_model_probability(dim) : pos->model_probability(dim)) -
					(neglinear ? neg->linear_model_probability(dim) : neg->model_probability(dim));
				label[dim]= lab->get_int_label(dim);
			}

			ui->ui_math->evaluate_results(output, label, total, outputfile);

			delete[] output;
			delete[] label;

			//pos->set_observations(old_pos);
			//neg->set_observations(old_neg);
			result=true;
		}
		else
			printf("load test features first!\n");
	}
	else
		SG_ERROR( "assign positive and negative models first!\n");

	if ((outputfile) && (outputfile!=stdout))
		fclose(outputfile);

	return result;
}

bool CGUIHMM::hmm_test(
	char* filename_out, char* filename_roc,
	bool is_pos_linear, bool is_neg_linear)
{
	bool result=false;
	FILE* file_output=stdout;
	FILE* file_roc=NULL;

	if (filename_out)
	{
		file_output=fopen(filename_out, "w");

		if (!file_output)
			SG_ERROR("Could not open file %s.\n", filename_out);

		if (filename_roc)
		{
			file_roc=fopen(filename_roc, "w");

			if (!file_roc)
				SG_ERROR("Could not open file %s.\n", filename_roc);
		}
	}

	if (!(pos && neg))
		SG_ERROR("Assign positive and negative models first!\n");

	if (!ui->ui_features->get_test_features())
		SG_ERROR("Assign test features first!\n");

	CStringFeatures<uint16_t>* o=(CStringFeatures<uint16_t>*) ui->
		ui_features->get_test_features();
	ASSERT(o);
	CLabels* lab=ui->ui_labels->get_test_labels();
	CStringFeatures<uint16_t>* old_pos=pos->get_observations();
	CStringFeatures<uint16_t>* old_neg=neg->get_observations();
	pos->set_observations(o);
	neg->set_observations(o);
	int32_t total=o->get_num_vectors();
	ASSERT(lab && total==lab->get_num_labels());
	float64_t* output=new float64_t[total];
	int32_t* label=new int32_t[total];

	SG_INFO("Testing using neg %s hmm vs. pos %s hmm\n", is_neg_linear ? "linear" : "", is_pos_linear ? "linear" : "");

	for (int32_t dim=0; dim<total; dim++)
	{
		output[dim]=
			(is_pos_linear ? pos->linear_model_probability(dim) : pos->model_probability(dim)) -
			(is_neg_linear ? neg->linear_model_probability(dim) : neg->model_probability(dim));
		label[dim]= lab->get_int_label(dim);
		//fprintf(file_output, "%+d: %f - %f = %f\n", label[dim], pos->model_probability(dim), neg->model_probability(dim), output[dim]);
	}

	ui->ui_math->evaluate_results(output, label, total, file_output, file_roc);

	delete[] output;
	delete[] label;

	pos->set_observations(old_pos);
	neg->set_observations(old_neg);

	result=true;

	if (file_roc)
		fclose(file_roc);
	if (file_output && file_output!=stdout)
		fclose(file_output);

	return result;
}

CLabels* CGUIHMM::classify(CLabels* result)
{
	CStringFeatures<uint16_t>* obs= (CStringFeatures<uint16_t>*) ui->
		ui_features->get_test_features();
	ASSERT(obs);
	int32_t num_vec=obs->get_num_vectors();

	//CStringFeatures<uint16_t>* old_pos=pos->get_observations();
	//CStringFeatures<uint16_t>* old_neg=neg->get_observations();

	pos->set_observations(obs);
	neg->set_observations(obs);

	if (!result)
		result=new CLabels(num_vec);

	for (int32_t i=0; i<num_vec; i++)
		result->set_label(i, pos->model_probability(i) - neg->model_probability(i));

	//pos->set_observations(old_pos);
	//neg->set_observations(old_neg);
	return result;
}

float64_t CGUIHMM::classify_example(int32_t idx)
{
	CStringFeatures<uint16_t>* obs= (CStringFeatures<uint16_t>*) ui->
		ui_features->get_test_features();
	ASSERT(obs);

	//CStringFeatures<uint16_t>* old_pos=pos->get_observations();
	//CStringFeatures<uint16_t>* old_neg=neg->get_observations();

	pos->set_observations(obs);
	neg->set_observations(obs);

	float64_t result=pos->model_probability(idx) - neg->model_probability(idx);
	//pos->set_observations(old_pos);
	//neg->set_observations(old_neg);
	return result;
}

CLabels* CGUIHMM::one_class_classify(CLabels* result)
{
	ASSERT(working);

	CStringFeatures<uint16_t>* obs= (CStringFeatures<uint16_t>*) ui->
		ui_features->get_test_features();
	ASSERT(obs);
	int32_t num_vec=obs->get_num_vectors();

	//CStringFeatures<uint16_t>* old_pos=working->get_observations();
	working->set_observations(obs);

	if (!result)
		result=new CLabels(num_vec);

	for (int32_t i=0; i<num_vec; i++)
		result->set_label(i, working->model_probability(i));

	//working->set_observations(old_pos);
	return result;
}

CLabels* CGUIHMM::linear_one_class_classify(CLabels* result)
{
	ASSERT(working);

	CStringFeatures<uint16_t>* obs= (CStringFeatures<uint16_t>*) ui->
		ui_features->get_test_features();
	ASSERT(obs);
	int32_t num_vec=obs->get_num_vectors();

	//CStringFeatures<uint16_t>* old_pos=working->get_observations();
	working->set_observations(obs);

	if (!result)
		result=new CLabels(num_vec);

	for (int32_t i=0; i<num_vec; i++)
		result->set_label(i, working->linear_model_probability(i));

	//working->set_observations(old_pos);
	return result;
}


float64_t CGUIHMM::one_class_classify_example(int32_t idx)
{
	ASSERT(working);

	CStringFeatures<uint16_t>* obs= (CStringFeatures<uint16_t>*) ui->
		ui_features->get_test_features();
	ASSERT(obs);

	//CStringFeatures<uint16_t>* old_pos=pos->get_observations();

	pos->set_observations(obs);
	neg->set_observations(obs);

	float64_t result=working->model_probability(idx);
	//working->set_observations(old_pos);
	return result;
}

bool CGUIHMM::append_model(char* filename, int32_t base1, int32_t base2)
{
	if (!working)
		SG_ERROR("Create HMM first.\n");
	if (!filename)
		SG_ERROR("Invalid filename.\n");

	FILE* model_file=fopen(filename, "r");
	if (!model_file)
		SG_ERROR("Opening file %s failed.\n", filename);

	CHMM* h=new CHMM(model_file,PSEUDO);
	if (!h || !h->get_status())
	{
		delete h;
		fclose(model_file);
		SG_ERROR("Reading file %s failed.\n", filename);
	}

	fclose(model_file);
	SG_INFO("File %s successfully read.\n", filename);

	SG_DEBUG("h %d , M: %d\n", h, h->get_M());
	if (base1!=-1 && base2!=-1)
	{
		float64_t* cur_o=new float64_t[h->get_M()];
		float64_t* app_o=new float64_t[h->get_M()];

		for (int32_t i=0; i<h->get_M(); i++)
		{
			if (i==base1)
				cur_o[i]=0;
			else
				cur_o[i]=-1000;

			if (i==base2)
				app_o[i]=0;
			else
				app_o[i]=-1000;
		}

		working->append_model(h, cur_o, app_o);

		delete[] cur_o;
		delete[] app_o;
	}
	else
		working->append_model(h);

	delete h;
	SG_INFO("New model has %i states.\n", working->get_N());
	return true;
}

bool CGUIHMM::add_states(int32_t num_states, float64_t value)
{
	if (!working)
		SG_ERROR("Create HMM first.\n");

	working->add_states(num_states, value);
	SG_INFO("New model has %i states, value %f.\n", working->get_N(), value);
	return true;
}

bool CGUIHMM::set_pseudo(float64_t pseudo)
{
	PSEUDO=pseudo;
	SG_INFO("Current setting: pseudo=%e.\n", PSEUDO);
	return true;
}

bool CGUIHMM::convergence_criteria(int32_t num_iterations, float64_t epsilon)
{
	if (!working)
		SG_ERROR("Create HMM first.\n");

	working->set_iterations(num_iterations);
	working->set_epsilon(epsilon);

	SG_INFO("Current HMM convergence criteria: iterations=%i, epsilon=%e\n", working->get_iterations(), working->get_epsilon());
	return true;
}

bool CGUIHMM::set_hmm_as(char* target)
{
	if (!working)
		SG_ERROR("Create HMM first!\n");

	if (strncmp(target, "POS", 3)==0)
	{
		delete pos;
		pos=working;
		working=NULL;
	}
	else if (strncmp(target, "NEG", 3)==0)
	{
		delete neg;
		neg=working;
		working=NULL;
	}
	else if (strncmp(target, "TEST", 4)==0)
	{
		delete test;
		test=working;
		working=NULL;
	}
	else
		SG_ERROR("Target POS|NEG|TEST is missing.\n");

	return true;
}

bool CGUIHMM::load(char* filename)
{
	bool result=false;

	FILE* model_file=fopen(filename, "r");
	if (!model_file)
		SG_ERROR("Opening file %s failed.\n", filename);

	delete working;
	working=new CHMM(model_file, PSEUDO);
	fclose(model_file);

	if (working && working->get_status())
	{
		SG_INFO("Loaded HMM successfully from file %s.\n", filename);
		result=true;
	}

	M=working->get_M();

	return result;
}

bool CGUIHMM::save(char* filename, bool is_binary)
{
	bool result=false;

	if (!working)
		SG_ERROR("Create HMM first.\n");

	FILE* file=fopen(filename, "w");
	if (file)
	{
		if (is_binary)
			result=working->save_model_bin(file);
		else
			result=working->save_model(file);
	}

	if (!file || !result)
		SG_ERROR("Writing to file %s failed!\n", filename);
	else
		SG_INFO("Successfully written model into %s!\n", filename);

	if (file)
		fclose(file);

	return result;
}

bool CGUIHMM::load_definitions(char* filename, bool do_init)
{
	if (!working)
		SG_ERROR("Create HMM first.\n");

	bool result=false;
	FILE* def_file=fopen(filename, "r");
	if (!def_file)
		SG_ERROR("Opening file %s failed\n", filename);
	
	if (working->load_definitions(def_file, true, do_init))
	{
		SG_INFO("Definitions successfully read from %s.\n", filename);
		result=true;
	}
	else
		SG_ERROR("Couldn't load definitions form file %s.\n", filename);

	fclose(def_file);
	return result;
}

bool CGUIHMM::save_likelihood(char* filename, bool is_binary)
{
	bool result=false;

	if (!working)
		SG_ERROR("Create HMM first\n");

	FILE* file=fopen(filename, "w");
	if (file)
	{
		/// ..future
		//if (binary)
		//	result=working->save_model_bin(file);
		//else

		result=working->save_likelihood(file);
	}

	if (!file || !result)
		SG_ERROR("Writing to file %s failed!\n", filename);
	else
		SG_INFO("Successfully written likelihoods into %s!\n", filename);

	if (file)
		fclose(file);

	return result;
}

bool CGUIHMM::save_path(char* filename, bool is_binary)
{
	bool result=false;
	if (!working)
		SG_ERROR("Create HMM first.\n");

	FILE* file=fopen(filename, "w");
	if (file)
	{
		/// ..future
		//if (binary)
		//_train()/	result=working->save_model_bin(file);
		//else
		CStringFeatures<uint16_t>* obs=(CStringFeatures<uint16_t>*) ui->
			ui_features->get_test_features();
		ASSERT(obs);
		working->set_observations(obs);

		result=working->save_path(file);
	}

	if (!file || !result)
		SG_ERROR("Writing to file %s failed!\n", filename);
	else
		SG_INFO("Successfully written path into %s!\n", filename);

	if (file)
		fclose(file);

	return result;
}

bool CGUIHMM::chop(float64_t value)
{
	if (!working)
		SG_ERROR("Create HMM first.\n");

	working->chop(value);
	return true;
}

bool CGUIHMM::likelihood()
{
	if (!working)
		SG_ERROR("Create HMM first!\n");

	working->output_model(false);
	return true;
}

bool CGUIHMM::output_hmm()
{
	if (!working)
		SG_ERROR("Create HMM first!\n");

	working->output_model(true);
	return true;
}

bool CGUIHMM::output_hmm_defined()
{
	if (!working)
		SG_ERROR("Create HMM first!\n");

	working->output_model_defined(true);
	return true;
}

bool CGUIHMM::best_path(int32_t from, int32_t to)
{
	// FIXME: from unused???

	if (!working)
		SG_ERROR("Create HMM first.\n");

	//get path
	working->best_path(0);

	for (int32_t t=0; t<working->get_observations()->get_vector_length(0)-1 && t<to; t++)
		SG_PRINT("%d ", working->get_best_path_state(0, t));
	SG_PRINT("\n");

	//for (t=0; t<p_observations->get_vector_length(0)-1 && t<to; t++)
	//	SG_PRINT( "%d ", PATH(0)[t]);
	//
	return true;
}

bool CGUIHMM::normalize(bool keep_dead_states)
{
	if (!working)
		SG_ERROR("Create HMM first.\n");

	working->normalize(keep_dead_states);
	return true;
}

bool CGUIHMM::relative_entropy(float64_t*& values, int32_t& len)
{
	if (!pos || !neg)
		SG_ERROR("Set pos and neg HMM first!\n");

	int32_t pos_N=pos->get_N();
	int32_t neg_N=neg->get_N();
	int32_t pos_M=pos->get_M();
	int32_t neg_M=neg->get_M();
	if (pos_M!=neg_M || pos_N!=neg_N)
		SG_ERROR("Pos and neg HMM's differ in number of emissions or states.\n");

	float64_t* p=new float64_t[pos_M];
	float64_t* q=new float64_t[neg_M];

	delete[] values;
	values=new float64_t[pos_N];

	for (int32_t i=0; i<pos_N; i++)
	{
		for (int32_t j=0; j<pos_M; j++)
		{
			p[j]=pos->get_b(i, j);
			q[j]=neg->get_b(i, j);
		}

		values[i]=CMath::relative_entropy(p, q, pos_M);
	}
	delete[] p;
	delete[] q;

	len=pos_N;
	return true;
}

bool CGUIHMM::entropy(float64_t*& values, int32_t& len)
{
	if (!working)
		SG_ERROR("Create HMM first!\n");

	int32_t n=working->get_N();
	int32_t m=working->get_M();
	float64_t* p=new float64_t[m];

	delete[] values;
	values=new float64_t[n];

	for (int32_t i=0; i<n; i++)
	{
		for (int32_t j=0; j<m; j++)
			p[j]=working->get_b(i, j);

		values[i]=CMath::entropy(p, m);
	}
	delete[] p;

	len=m;
	return true;
}

bool CGUIHMM::permutation_entropy(int32_t width, int32_t seq_num)
{
	if (!working)
		SG_ERROR("Create hmm first.\n");

	if (!working->get_observations())
		SG_ERROR("Set observations first.\n");

	return working->permutation_entropy(width, seq_num);
}
#endif