#ifndef UUID140663056114512
#define UUID140663056114512

#include <cstring>


namespace ContinousMotionPipeline {

    
        #ifndef UUID140663060446032
#define UUID140663060446032

/**
  * MinMaxScaler(low=0, high=1)
 */
class Step0 {
    public:

        /**
         * Transform input vector
         */
        bool transform(float *x) {
            
    for (uint16_t i = 0; i < 6; i++) {
        x[i] = (x[i] - offset[i]) * scale[i] + 0;

        if (x[i] < 0) x[i] = 0;
        else if (x[i] > 1) x[i] = 1;
    }

    return true;


            return true;
        }

    protected:
        
    float offset[6] = {-7.18000000000f, -2.77000000000f, 4.33000000000f, -1.23000000000f, -0.99000000000f, -1.49000000000f};
    float scale[6] = {0.07547169811f, 0.16233766234f, 0.10152284264f, 0.41322314050f, 0.59523809524f, 0.36630036630f};

};



#endif
    
        #ifndef UUID140663086655312
#define UUID140663086655312

/**
  * Window(length=60, shift=18)
 */
class Step1 {
    public:

        /**
         * Transform input vector
         */
        bool transform(float *x) {
            
    // append x to queue
    memcpy(queue + head, x, sizeof(float) * 6);
    head += 6;

    if (head != 360)
        return false;

    // copy queue to x and shift
    memcpy(x, queue, sizeof(float) * 360);

    
        memcpy(queue, queue + 108, sizeof(float) * 252);
    

    head -= 108;



            return true;
        }

    protected:
        
    uint16_t head = 0;
    float queue[360] = {0};

};



#endif
    
        #ifndef UUID140663081594576
#define UUID140663081594576

/**
  * SpectralFeatures(num_inputs=6, window_length=60, order=2, num_features=20)
 */
class Step2 {
    public:

        /**
         * Transform input vector
         */
        bool transform(float *x) {
            
    // spectral features
    uint16_t idx = 0;

    for (uint16_t k = 0; k < 6; k++) {
        float minimum = 9999999;
        float maximum = -9999999;
        float abs_minimum = 999999;
        float abs_maximum = 0;
        float mean = 0;
        float abs_energy = 0;
        float mean_abs_change = 0;
        float cid_ce = 0;
        float xi_prev = x[k];

        // first-order features
        for (uint16_t i = k; i < 360; i += 6) {
            float xi = x[i];
            float abs_xi = abs(xi);
            float xi_prev2 = (i >= k + 12) ? x[i - 12] : xi;

            float xi2 = xi * xi;
            float diff = xi - xi_prev;

            // start of features
            minimum = xi < minimum ? xi : minimum;
            maximum = xi > maximum ? xi : maximum;
            abs_minimum = abs_xi < abs_minimum ? abs_xi : abs_minimum;
            abs_maximum = abs_xi > abs_maximum ? abs_xi : abs_maximum;
            mean += xi;
            abs_energy += xi2;
            mean_abs_change += abs(diff);
            cid_ce += diff * diff;
            xi_prev = xi;
        }

        mean /= 60;
        abs_energy /= 60;
        mean_abs_change /= 60;
        cid_ce /= 60;

        buffer[idx++] = maximum;
        buffer[idx++] = minimum;
        buffer[idx++] = abs_maximum;
        buffer[idx++] = abs_minimum;
        buffer[idx++] = mean;
        buffer[idx++] = abs_energy;
        buffer[idx++] = mean_abs_change;
        buffer[idx++] = cid_ce;

        
            // second-order features
            float xi_mean_prev = x[k] - mean;
            float count_above_mean = 0;
            float count_below_mean = 0;
            float first_position_of_max = 360;
            float first_position_of_min = 360;
            float max_count = 0;
            float min_count = 0;
            float max_thresh = maximum - abs(maximum) * 0.02;
            float min_thresh = minimum + abs(minimum) * 0.02;
            float var = 0;
            float std = 0;
            float kurtosis = 0;
            float skew = 0;
            float has_large_std = 0;
            float variation_coefficient = 0;

            for (uint16_t i = k; i < 360; i += 6) {
                float xi = x[i];
                float xi_mean = xi - mean;
                float var_i = xi_mean * xi_mean;

                // start of features
                var += var_i;
                count_above_mean += xi_mean >  0.001 ? 1 : 0;
                count_below_mean += xi_mean < -0.001 ? 1 : 0;
                first_position_of_max = (i < first_position_of_max && abs(xi - maximum) < 0.001) ? i : first_position_of_max;
                first_position_of_min = (i < first_position_of_min && abs(xi - minimum) < 0.001) ? i : first_position_of_min;

                if (var_i > 0.001) {
                    skew += var_i * xi_mean;
                    kurtosis += var_i * var_i;
                }
                if (xi > max_thresh) { max_count += 1; }
                if (xi < min_thresh) { min_count += 1; }

                xi_mean_prev = xi_mean;
            }

            var /= 60;
            std = sqrt(var);
            first_position_of_max = int((first_position_of_max - k) / 60);
            first_position_of_min = int((first_position_of_min - k) / 60);
            skew = var > 0.001 ? skew / pow(var, 1.5) : 0;
            kurtosis = var > 0.001 ? kurtosis / (var * var) : 0;
            has_large_std = std > 0.25 * (maximum - minimum);
            variation_coefficient = (mean > 0.001) ? var / mean : 0;

            buffer[idx++] = std;
            buffer[idx++] = var;
            buffer[idx++] = count_above_mean;
            buffer[idx++] = count_below_mean;
            buffer[idx++] = first_position_of_max;
            buffer[idx++] = first_position_of_min;
            buffer[idx++] = max_count;
            buffer[idx++] = min_count;
            buffer[idx++] = has_large_std;
            buffer[idx++] = skew;
            buffer[idx++] = kurtosis;
            buffer[idx++] = variation_coefficient;
        
    }

    memcpy(x, buffer, 120 * sizeof(float));


            return true;
        }

    protected:
        
    float buffer[120];

};



#endif
    

     /**
      * Pipeline:
 * ---------
 *  > MinMaxScaler(low=0, high=1)
 *  > Window(length=60, shift=18)
 *  > SpectralFeatures(num_inputs=6, window_length=60, order=2, num_features=20)
     */
    class Pipeline {
        public:
            static const uint16_t NUM_INPUTS = 6;
            static const uint16_t NUM_OUTPUTS = 120;
            static const uint16_t WORKING_SIZE = 360;
            float X[360];

            /**
             * Apply pipeline to input vector
             */
            template<typename T>
            bool transform(T *x) {
                for (uint16_t i = 0; i < 6; i++)
                    this->X[i] = x[i];

                size_t start = micros();
                bool isOk =
                
                     step0.transform(X)
                
                    && step1.transform(X)
                
                    && step2.transform(X)
                ;

                latency = micros() - start;

                return isOk;
            }

            /**
             * Debug output feature vector
             */
            template<typename PrinterInterface>
            void debugTo(PrinterInterface &printer, uint8_t precision=5) {
                printer.print(X[0], precision);

                for (uint16_t i = 1; i < 120; i++) {
                    printer.print(", ");
                    printer.print(X[i], precision);
                }

                printer.print('\n');
            }

            /**
 * Get latency in micros
 */
uint32_t latencyInMicros() {
    return latency;
}

/**
 * Get latency in millis
 */
uint16_t latencyInMillis() {
    return latency / 1000;
}

        protected:
            float latency;
            
                ContinousMotionPipeline::Step0 step0;
            
                ContinousMotionPipeline::Step1 step1;
            
                ContinousMotionPipeline::Step2 step2;
            
    };
}


static ContinousMotionPipeline::Pipeline pipeline;


#endif