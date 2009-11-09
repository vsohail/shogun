/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Written (W) 1999-2008 Soeren Sonnenburg
 * Copyright (C) 1999-2008 Fraunhofer Institute FIRST and Max-Planck-Society
 */

#ifndef _CSORTWORDSTRING__H__
#define _CSORTWORDSTRING__H__

#include "features/StringFeatures.h"
#include "preproc/StringPreProc.h"
#include "lib/common.h"

#include <stdio.h>

/** Preprocessor SortWordString, sorts the indivual strings in ascending order.
 * This is useful in conjunction with the CCommWordStringKernel and will result
 * in the spectrum kernel. For this to work the strings have to be mapped into
 * a binary higher order representation first (cf. obtain_from_*() functions in
 * CStringFeatures)
 */
class CSortWordString : public CStringPreProc<uint16_t>
{
public:
	/** default constructor */
	CSortWordString();
	virtual ~CSortWordString();

	/// initialize preprocessor from features
	virtual bool init(CFeatures* f);

	/// initialize preprocessor from file
	virtual bool load_init_data(FILE* src);
	/// save init-data (like transforamtion matrices etc) to file
	virtual bool save_init_data(FILE* dst);
	/// cleanup
	virtual void cleanup();
	/// initialize preprocessor from file
	virtual bool load(FILE* f);
	/// save preprocessor init-data to file
	virtual bool save(FILE* f);

	/// apply preproc on feature matrix
	/// result in feature matrix
	/// return pointer to feature_matrix, i.e. f->get_feature_matrix();
	virtual bool apply_to_string_features(CFeatures* f);

	/// apply preproc on single feature vector
	/// result in feature matrix
	virtual uint16_t* apply_to_string(uint16_t* f, int32_t &len);
};
#endif