/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Written (W) 1999-2008 Gunnar Raetsch
 * Copyright (C) 1999-2008 Fraunhofer Institute FIRST and Max-Planck-Society
 */

#include "lib/common.h"
#include "lib/io.h"
#include "kernel/SimpleLocalityImprovedStringKernel.h"
#include "features/Features.h"
#include "features/StringFeatures.h"

CSimpleLocalityImprovedStringKernel::CSimpleLocalityImprovedStringKernel(
	int32_t size, int32_t l, int32_t id, int32_t od)
: CStringKernel<char>(size), length(l), inner_degree(id), outer_degree(od),
	pyramid_weights(NULL)
{
}

CSimpleLocalityImprovedStringKernel::CSimpleLocalityImprovedStringKernel(
	CStringFeatures<char>* l, CStringFeatures<char>* r,
	int32_t len, int32_t id, int32_t od)
: CStringKernel<char>(10), length(len), inner_degree(id), outer_degree(od),
	pyramid_weights(NULL)
{
	init(l, r);
}

CSimpleLocalityImprovedStringKernel::~CSimpleLocalityImprovedStringKernel()
{
	cleanup();
}

bool CSimpleLocalityImprovedStringKernel::init(CFeatures* l, CFeatures* r)
{
	bool result = CStringKernel<char>::init(l,r);

	if (!result)
		return false;
	int32_t num_features = ((CStringFeatures<char>*) l)->get_max_vector_length();
	pyramid_weights = new float64_t[num_features];
	ASSERT(pyramid_weights);
	SG_INFO("initializing pyramid weights: size=%ld length=%i\n",
		num_features, length);

	const int32_t PYRAL = 2 * length - 1; // total window length
	float64_t PYRAL_pot;
	int32_t DEGREE1_1  = (inner_degree & 0x1)==0;
	int32_t DEGREE1_1n = (inner_degree & ~0x1)!=0;
	int32_t DEGREE1_2  = (inner_degree & 0x2)!=0;
	int32_t DEGREE1_3  = (inner_degree & ~0x3)!=0;
	int32_t DEGREE1_4  = (inner_degree & 0x4)!=0;
	{
	float64_t PYRAL_ = PYRAL;
	PYRAL_pot = DEGREE1_1 ? 1.0 : PYRAL_;
	if (DEGREE1_1n)
	{
		PYRAL_ *= PYRAL_;
		if (DEGREE1_2)
			PYRAL_pot *= PYRAL_;
		if (DEGREE1_3)
		{
			PYRAL_ *= PYRAL_;
			if (DEGREE1_4)
				PYRAL_pot *= PYRAL_;
		}
	}
	}

	int32_t pyra_len  = num_features-PYRAL+1;
	int32_t pyra_len2 = (int32_t) pyra_len/2;
	{
	int32_t j;
	for (j = 0; j < pyra_len; j++)
		pyramid_weights[j] = 4*((float64_t)((j < pyra_len2)? j+1 : pyra_len-j))/((float64_t)pyra_len);
	for (j = 0; j < pyra_len; j++)
		pyramid_weights[j] /= PYRAL_pot;
	}

	return init_normalizer();
}

void CSimpleLocalityImprovedStringKernel::cleanup()
{
	delete[] pyramid_weights;
	pyramid_weights = NULL;

	CKernel::cleanup();
}

bool CSimpleLocalityImprovedStringKernel::load_init(FILE* src)
{
	return false;
}

bool CSimpleLocalityImprovedStringKernel::save_init(FILE* dest)
{
	return false;
}

float64_t CSimpleLocalityImprovedStringKernel::dot_pyr (const char* const x1,
	     const char* const x2, const int32_t NOF_NTS, const int32_t NTWIDTH,
	     const int32_t DEGREE1, const int32_t DEGREE2, float64_t *pyra)
{
	const int32_t PYRAL = 2*NTWIDTH-1; // total window length
	int32_t pyra_len, pyra_len2;
	float64_t pot, PYRAL_pot;
	float64_t sum;
	int32_t DEGREE1_1 = (DEGREE1 & 0x1)==0;
	int32_t DEGREE1_1n = (DEGREE1 & ~0x1)!=0;
	int32_t DEGREE1_2 = (DEGREE1 & 0x2)!=0;
	int32_t DEGREE1_3 = (DEGREE1 & ~0x3)!=0;
	int32_t DEGREE1_4 = (DEGREE1 & 0x4)!=0;
	{
	float64_t PYRAL_ = PYRAL;
	PYRAL_pot = DEGREE1_1 ? 1.0 : PYRAL_;
	if (DEGREE1_1n)
	{
		PYRAL_ *= PYRAL_;
		if (DEGREE1_2) PYRAL_pot *= PYRAL_;
		if (DEGREE1_3)
		{
			PYRAL_ *= PYRAL_;
			if (DEGREE1_4) PYRAL_pot *= PYRAL_;
		}
	}
	}

	ASSERT((DEGREE1 & ~0x7) == 0);
	ASSERT((DEGREE2 & ~0x7) == 0);

	pyra_len = NOF_NTS-PYRAL+1;
	pyra_len2 = (int32_t) pyra_len/2;
	{
	int32_t j;
	for (j = 0; j < pyra_len; j++)
		pyra[j] = 4*((float64_t)((j < pyra_len2) ? j+1 : pyra_len-j))/((float64_t)pyra_len);
	for (j = 0; j < pyra_len; j++)
		pyra[j] /= PYRAL_pot;
	}

	register int32_t conv;
	register int32_t i;
	register int32_t j;

	sum = 0.0;
	conv = 0;
	for (j = 0; j < PYRAL; j++)
		conv += (x1[j] == x2[j]) ? 1 : 0;

	for (i = 0; i < NOF_NTS-PYRAL+1; i++)
	{
		register float64_t pot2;
		if (i>0)
			conv += ((x1[i+PYRAL-1] == x2[i+PYRAL-1]) ? 1 : 0 ) - 
				((x1[i-1] == x2[i-1]) ? 1 : 0);
		{ /* potencing of conv -- float64_t is faster*/
		register float64_t conv2 = conv;
		pot2 = (DEGREE1_1) ? 1.0 : conv2;
			if (DEGREE1_1n)
			{
				conv2 *= conv2;
				if (DEGREE1_2)
					pot2 *= conv2;
				if (DEGREE1_3 && DEGREE1_4)
					pot2 *= conv2*conv2;
			}
		}
		sum += pot2*pyra[i];
	}

	pot = ((DEGREE2 & 0x1) == 0) ? 1.0 : sum;
	if ((DEGREE2 & ~0x1) != 0)
	{
		sum *= sum;
		if ((DEGREE2 & 0x2) != 0)
			pot *= sum;
		if ((DEGREE2 & ~0x3) != 0)
		{
			sum *= sum;
			if ((DEGREE2 & 0x4) != 0)
				pot *= sum;
		}
	}
	return pot;
}

float64_t CSimpleLocalityImprovedStringKernel::compute(
	int32_t idx_a, int32_t idx_b)
{
	int32_t alen, blen;

	char* avec = ((CStringFeatures<char>*) lhs)->get_feature_vector(idx_a, alen);
	char* bvec = ((CStringFeatures<char>*) rhs)->get_feature_vector(idx_b, blen);

	// can only deal with strings of same length
	ASSERT(alen==blen);

	float64_t dpt;

	dpt = dot_pyr(avec, bvec, alen, length, inner_degree, outer_degree, pyramid_weights);
	dpt = dpt / pow((float64_t)alen, (float64_t)outer_degree);
	return (float64_t) dpt;
}