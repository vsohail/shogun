/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Written (W) 2007-2008 Soeren Sonnenburg
 * Copyright (C) 2007-2008 Fraunhofer Institute FIRST and Max-Planck-Society
 */

#include "lib/config.h"

#ifdef USE_CPLEX

#include "classifier/LPM.h"
#include "features/Labels.h"
#include "lib/Mathematics.h"
#include "lib/Cplex.h"

CLPM::CLPM()
: CSparseLinearClassifier(), C1(1), C2(1), use_bias(true), epsilon(1e-3)
{
}


CLPM::~CLPM()
{
}

bool CLPM::train()
{
	ASSERT(labels);
	ASSERT(features);
	int32_t num_train_labels=labels->get_num_labels();
	int32_t num_feat=features->get_num_features();
	int32_t num_vec=features->get_num_vectors();

	ASSERT(num_vec==num_train_labels);
	delete[] w;
	w=new float64_t[num_feat];
	w_dim=num_feat;

	int32_t num_params=1+2*num_feat+num_vec; //b,w+,w-,xi
	float64_t* params=new float64_t[num_params];
	memset(params,0,sizeof(float64_t)*num_params);

	CCplex solver;
	solver.init(E_LINEAR);
	SG_INFO("C=%f\n", C1);
	solver.setup_lpm(C1, features, labels, get_bias_enabled());
	if (get_max_train_time()>0)
		solver.set_time_limit(get_max_train_time());
	bool result=solver.optimize(params);
	solver.cleanup();

	set_bias(params[0]);
	for (int32_t i=0; i<num_feat; i++)
		w[i]=params[1+i]-params[1+num_feat+i];

//#define LPM_DEBUG
#ifdef LPM_DEBUG
	CMath::display_vector(params,num_params, "params");
	SG_PRINT("bias=%f\n", bias);
	CMath::display_vector(w,w_dim, "w");
	CMath::display_vector(&params[1],w_dim, "w+");
	CMath::display_vector(&params[1+w_dim],w_dim, "w-");
#endif
	delete[] params;

	return result;
}
#endif