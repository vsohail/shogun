/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Written (W) 1999-2008 Soeren Sonnenburg
 * Copyright (C) 1999-2008 Fraunhofer Institute FIRST and Max-Planck-Society
 */

#ifndef _POLYKERNEL_H___
#define _POLYKERNEL_H___

#include "lib/common.h"
#include "kernel/SimpleKernel.h"
#include "features/RealFeatures.h"

/** Computes the standard polynomial kernel on dense real valued features
 * \f[
 * k({\bf x},{\bf x'})= ({\bf x}\cdot {\bf x'}+c)^d
 * \f]
 *
 * Note that additional normalisation is applied, i.e.
 * \f[
 *     k'({\bf x}, {\bf x'})=\frac{k({\bf x}, {\bf x'})}{\sqrt{k({\bf x}, {\bf x})k({\bf x'}, {\bf x'})}}
 * \f]
 */
class CPolyKernel: public CSimpleKernel<float64_t>
{
	public:
		/** constructor
		 *
		 * @param l features of left-hand side
		 * @param r features of right-hand side
		 * @param d degree
		 * @param inhom is inhomogeneous
		 * @param size cache size
		 */
		CPolyKernel(CRealFeatures* l, CRealFeatures* r,
			int32_t d, bool inhom, int32_t size=10);

		/** constructor
		 *
		 * @param size cache size
		 * @param degree degree
		 * @param inhomogene is inhomogeneous
		 */
		CPolyKernel(int32_t size, int32_t degree, bool inhomogene=true);

		virtual ~CPolyKernel();

		/** initialize kernel
		 *
		 * @param l features of left-hand side
		 * @param r features of right-hand side
		 * @return if initializing was successful
		 */
		virtual bool init(CFeatures* l, CFeatures* r);

		/** clean up kernel */
		virtual void cleanup();

		/** load kernel init_data
		 *
		 * @param src file to load from
		 * @return if loading was successful
		 */
		virtual bool load_init(FILE* src);

		/** save kernel init_data
		 *
		 * @param dest file to save to
		 * @return if saving was successful
		 */
		virtual bool save_init(FILE* dest);

		/** return what type of kernel we are
		 *
		 * @return kernel type POLY
		 */
		virtual EKernelType get_kernel_type() { return K_POLY; }

		/** return the kernel's name
		 *
		 * @return name Poly
		 */
		virtual const char* get_name() { return "Poly"; };

	protected:
		/** compute kernel function for features a and b
		 * idx_{a,b} denote the index of the feature vectors
		 * in the corresponding feature object
		 *
		 * @param idx_a index a
		 * @param idx_b index b
		 * @return computed kernel function at indices a,b
		 */
		virtual float64_t compute(int32_t idx_a, int32_t idx_b);

	protected:
		/** degree */
		int32_t degree;
		/** if kernel is inhomogeneous */
		bool inhomogene;
};

#endif /* _POLYKERNEL_H__ */