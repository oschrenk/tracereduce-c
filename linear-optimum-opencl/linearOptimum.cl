	inline float orthodromeBearing(float lon1, float lat1, float lon2, float lat2) {
	float radLat1 = radians(lat1);
	float radLat2 = radians(lat2);

	float deltaLongitude = radians(lon2 - lon1);

	float y = sin(deltaLongitude) * cos(radLat2);
	float x = cos(radLat1) * sin(radLat2) - sin(radLat1)
			* cos(radLat2) * cos(deltaLongitude);

	return atan2(y, x);
}

inline float haversineDistance(float longitudeFrom, float latitudeFrom,
			float longitudeTo, float latitudeTo
) {
	float deltaLatitude = radians(latitudeFrom - latitudeTo);
	float deltaLongitude = radians((longitudeFrom - longitudeTo));

	float sinusHalfDeltaLatitude = sin(deltaLatitude / 2);
	float sinusHalfDeltaLongitude = sin(deltaLongitude / 2);

	float a = sinusHalfDeltaLatitude * sinusHalfDeltaLatitude
			+ cos(radians(latitudeFrom))
			* cos(radians(latitudeTo))
			* sinusHalfDeltaLongitude * sinusHalfDeltaLongitude;
	float c = 2 * atan2(sqrt(a), sqrt(1 - a));

	return 6371000 * c;
	}

__kernel void haversine2dPointLineDistance(
	__global const float *longitudeX,
	__global const float *latitudeY,
	__global float *distance,
	const uint offset,
	const float fromLongitudeX,
	const float fromLatitudeY,
	const float toLongitudeX,
	const float toLatitudeY
) {
	// earth volumetric mean radius in meter
	float radius = 6371000;

	int tid = get_global_id(0) + offset;

	float b12 = orthodromeBearing(fromLongitudeX, fromLatitudeY, toLongitudeX, toLatitudeY );
	float b13 = orthodromeBearing(fromLongitudeX, fromLatitudeY, longitudeX[tid], latitudeY[tid] );
	float d13 = haversineDistance(fromLongitudeX, fromLatitudeY, longitudeX[tid], latitudeY[tid] );

	float dt = asin( sin(d13 / radius) * sin(b13 - b12)  ) * radius;

	distance[tid] = fabs(dt);
}

__kernel void maximumWithPositionAndOffsetFloat(
	__global float* io,
	const uint leftOffset,
	const uint rightOffset,
	const uint pass
) {
	uint left = (1 << (pass + 1)) * get_global_id(0) + leftOffset;
	uint right = left + (1 << pass);
	uint length = rightOffset - leftOffset + 1;

	if (right < rightOffset && left < rightOffset) {
		if ( ((__global float*)io)[right] > ((__global float*)io)[left] ) {
			((__global float*)io)[left] = ((__global float*)io)[right];
			((__global float*)io)[left+1] = (pass == 0 || right == length - 1) ? right : io[right+1];
		} else {
			((__global float*)io)[left+1] = (pass == 0) ? left : io[left+1];
		}
	}
}


__kernel  void dijkstra_sssp1(
	__global uint *vertexArray,
	__global uint *edgeArray,
	__global uint *maskArray,
	__global uint *costArray,
	__global uint *updatingCostArray,
	__global uint *parentVertexArray,
	const uint vertexCount,
	const uint edgeCount
) {
    // access thread id
    int tid = get_global_id(0);
	int defaultWeight = 1;

    if ( maskArray[tid] != 0 )
    {
        maskArray[tid] = 0;

        int edgeStart = vertexArray[tid];
        int edgeEnd;
        if (tid + 1 < (vertexCount)) {
            edgeEnd = vertexArray[tid + 1];
        } else {
            edgeEnd = edgeCount;
        }

		for(int edge = edgeStart; edge < edgeEnd; edge++) {
			int nid = edgeArray[edge];

			if (updatingCostArray[nid] > (costArray[tid] + defaultWeight)) {
			    updatingCostArray[nid] = (costArray[tid] + defaultWeight);
			    parentVertexArray[nid] = tid;
			}
		}
    }
}


__kernel  void dijkstra_sssp2(
	__global uint *maskArray,
	__global uint *costArray,
	__global uint *updatingCostArray
) {
    // access thread id
    int tid = get_global_id(0);

	    if (costArray[tid] > updatingCostArray[tid]) {
	        costArray[tid] = updatingCostArray[tid];
	        maskArray[tid] = 1;
	    }

	    updatingCostArray[tid] = costArray[tid];
}

__kernel void dijkstra_initialize(
	__global uint *maskArray,
	__global uint *costArray,
	__global uint *updatingCostArray,
	__global uint *parentVertexArray,
	const uint sourceVertex
) {
    // access thread id
    int tid = get_global_id(0);

	if (sourceVertex == tid) {
	    maskArray[tid] = 1;
	    costArray[tid] = 0;
	    updatingCostArray[tid] = 0;
	    // source is always connected with itself
	    parentVertexArray[tid] = tid;
	} else {
	    maskArray[tid] = 0;
	    costArray[tid] = INT_MAX;
	    updatingCostArray[tid] = INT_MAX;
	    // all edges are connected
	    parentVertexArray[tid] = tid-1;
	}
}




