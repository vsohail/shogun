/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Written (W) 2006 Christian Gehl
 * Copyright (C) 2006-2008 Fraunhofer Institute FIRST
 */

#ifndef _GEODESICMETRIC_H___
#define _GEODESICMETRIC_H___

#include "lib/common.h"
#include "distance/SimpleDistance.h"
#include "features/RealFeatures.h"

/** class GeodesicMetric
 *
 * The Geodesic distance (Great circle distance) computes the shortest path
 * between two data points on a sphere (the radius is set to one for the
 * evaluation).
 * 
 * \f[\displaystyle
 *  d(\bf{x},\bf{x'}) = arccos\sum_{i=1}^{n} \frac{\bf{x_{i}}\cdot\bf{x'_{i}}}
 *  {\sqrt{x_{i}x_{i} x'_{i}x'_{i}}}
 * \f]
 *
 * @see <a href="http://en.wikipedia.org/wiki/Great_circle_distance">Wikipedia:
 * Geodesic distance</a>
 *          
 */
class CGeodesicMetric: public CSimpleDistance<float64_t>
{
	public:
		/** default constructor */
		CGeodesicMetric();

		/** constructor
		 *
		 * @param l features of left-hand side
		 * @param r features of right-hand side
		 */
		CGeodesicMetric(CRealFeatures* l, CRealFeatures* r);
		virtual ~CGeodesicMetric();

		/** init distance
		 *
		 * @param l features of left-hand side
		 * @param r features of right-hand side
		 * @return if init was successful
		 */
		virtual bool init(CFeatures* l, CFeatures* r);

		/** cleanup distance */
		virtual void cleanup();

		/** load init data from file
		 *
		 * @param src file to load from
		 * @return if loading was successful
		 */
		virtual bool load_init(FILE* src);

		/** save init data to file
		 *
		 * @param dest file to save to
		 * @return if saving was successful
		 */
		virtual bool save_init(FILE* dest);

		/** get distance type we are
		 *
		 * @return distance type GEODESIC
		 */
		virtual EDistanceType get_distance_type() { return D_GEODESIC; }

		/** get name of the distance
		 *
		 * @return name Chebyshew-Metric
		 */
		virtual const char* get_name() { return "Geodesic-Metric"; }

	protected:
		/// compute distance for features a and b
		/// idx_{a,b} denote the index of the feature vectors
		/// in the corresponding feature object
		virtual float64_t compute(int32_t idx_a, int32_t idx_b);
};

#endif /* _GEODESICMETRIC_H___ */