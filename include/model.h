#pragma once
#include <cstdarg>
namespace Eloquent {
    namespace ML {
        namespace Port {
            class OneClassSVM {
                public:
                    /**
                    * Predict class for features vector
                    */
                    int predict(float *x) {
                        float kernels[8] = { 0 };
                        kernels[0] = compute_kernel(x,   454.0  , 14.0  , 0.0 );
                        kernels[1] = compute_kernel(x,   454.0  , 15.0  , 0.0 );
                        kernels[2] = compute_kernel(x,   464.0  , 16.0  , 0.0 );
                        kernels[3] = compute_kernel(x,   207.0  , 0.0  , 151.0 );
                        kernels[4] = compute_kernel(x,   199.0  , 0.0  , 135.0 );
                        kernels[5] = compute_kernel(x,   208.0  , 0.0  , 147.0 );
                        kernels[6] = compute_kernel(x,   208.0  , 0.0  , 155.0 );
                        kernels[7] = compute_kernel(x,   379.0  , 147.0  , 0.0 );
                        float decision = -7.032449371815 + ( + kernels[0]   + kernels[1] * 0.932812134548  + kernels[2]   + kernels[3]   + kernels[4]   + kernels[5] * 0.527187865452  + kernels[6]   + kernels[7]  );

                        return decision > -0.50 ? 0 : 1;
                    }

                protected:
                    /**
                    * Compute kernel between feature vector and support vector.
                    * Kernel type: rbf
                    */
                    float compute_kernel(float *x, ...) {
                        va_list w;
                        va_start(w, x);
                        float kernel = 0.0;

                        for (uint16_t i = 0; i < 3; i++) {
                            kernel += pow(x[i] - va_arg(w, double), 2);
                        }

                        return exp(-1.4177429076019413e-06 * kernel);
                    }
                };
            }
        }
    }