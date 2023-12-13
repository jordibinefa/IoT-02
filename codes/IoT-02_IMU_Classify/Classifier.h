#ifndef UUID140663061376080
#define UUID140663061376080

/**
  * RandomForestClassifier(base_estimator=deprecated, bootstrap=True, ccp_alpha=0.0, class_name=RandomForestClassifier, class_weight=None, criterion=gini, estimator=DecisionTreeClassifier(), estimator_params=('criterion', 'max_depth', 'min_samples_split', 'min_samples_leaf', 'min_weight_fraction_leaf', 'max_features', 'max_leaf_nodes', 'min_impurity_decrease', 'random_state', 'ccp_alpha'), max_depth=20, max_features=sqrt, max_leaf_nodes=None, max_samples=None, min_impurity_decrease=0.0, min_samples_leaf=1, min_samples_split=2, min_weight_fraction_leaf=0.0, n_estimators=20, n_jobs=None, num_outputs=4, oob_score=False, package_name=everywhereml.sklearn.ensemble, random_state=None, template_folder=everywhereml/sklearn/ensemble, verbose=0, warm_start=False)
 */
class RandomForestClassifier {
    public:

        /**
         * Predict class from features
         */
        int predict(float *x) {
            int predictedValue = 0;
            size_t startedAt = micros();

            
                    
            uint16_t votes[4] = { 0 };
            uint8_t classIdx = 0;
            float classScore = 0;

            
                tree0(x, &classIdx, &classScore);
                votes[classIdx] += classScore;
            
                tree1(x, &classIdx, &classScore);
                votes[classIdx] += classScore;
            
                tree2(x, &classIdx, &classScore);
                votes[classIdx] += classScore;
            
                tree3(x, &classIdx, &classScore);
                votes[classIdx] += classScore;
            
                tree4(x, &classIdx, &classScore);
                votes[classIdx] += classScore;
            
                tree5(x, &classIdx, &classScore);
                votes[classIdx] += classScore;
            
                tree6(x, &classIdx, &classScore);
                votes[classIdx] += classScore;
            
                tree7(x, &classIdx, &classScore);
                votes[classIdx] += classScore;
            
                tree8(x, &classIdx, &classScore);
                votes[classIdx] += classScore;
            
                tree9(x, &classIdx, &classScore);
                votes[classIdx] += classScore;
            
                tree10(x, &classIdx, &classScore);
                votes[classIdx] += classScore;
            
                tree11(x, &classIdx, &classScore);
                votes[classIdx] += classScore;
            
                tree12(x, &classIdx, &classScore);
                votes[classIdx] += classScore;
            
                tree13(x, &classIdx, &classScore);
                votes[classIdx] += classScore;
            
                tree14(x, &classIdx, &classScore);
                votes[classIdx] += classScore;
            
                tree15(x, &classIdx, &classScore);
                votes[classIdx] += classScore;
            
                tree16(x, &classIdx, &classScore);
                votes[classIdx] += classScore;
            
                tree17(x, &classIdx, &classScore);
                votes[classIdx] += classScore;
            
                tree18(x, &classIdx, &classScore);
                votes[classIdx] += classScore;
            
                tree19(x, &classIdx, &classScore);
                votes[classIdx] += classScore;
            

            // return argmax of votes
            uint8_t maxClassIdx = 0;
            float maxVote = votes[0];

            for (uint8_t i = 1; i < 4; i++) {
                if (votes[i] > maxVote) {
                    maxClassIdx = i;
                    maxVote = votes[i];
                }
            }

            predictedValue = maxClassIdx;

                    

            latency = micros() - startedAt;

            return (lastPrediction = predictedValue);
        }

        
            

            /**
             * Predict class label
             */
            String predictLabel(float *x) {
                return getLabelOf(predict(x));
            }

            /**
             * Get label of last prediction
             */
            String getLabel() {
                return getLabelOf(lastPrediction);
            }

            /**
             * Get label of given class
             */
            String getLabelOf(int8_t idx) {
                switch (idx) {
                    case -1:
                        return "ERROR";
                    
                        case 0:
                            return "quiet";
                    
                        case 1:
                            return "amunt-avall";
                    
                        case 2:
                            return "esquerra-dreta";
                    
                        case 3:
                            return "cercle";
                    
                    default:
                        return "UNKNOWN";
                }
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
        float latency = 0;
        int lastPrediction = 0;

        
            

        
            
                /**
                 * Random forest's tree #0
                 */
                void tree0(float *x, uint8_t *classIdx, float *classScore) {
                    
                        if (x[28] < 0.03583278600126505) {
                            
                                
                        *classIdx = 0;
                        *classScore = 74.0;
                        return;

                            
                        }
                        else {
                            
                                
                        if (x[48] < 0.08397645130753517) {
                            
                                
                        if (x[28] < 0.09337219968438148) {
                            
                                
                        if (x[102] < 0.8369963467121124) {
                            
                                
                        if (x[18] < 0.826737642288208) {
                            
                                
                        *classIdx = 1;
                        *classScore = 72.0;
                        return;

                            
                        }
                        else {
                            
                                
                        *classIdx = 2;
                        *classScore = 61.0;
                        return;

                            
                        }

                            
                        }
                        else {
                            
                                
                        *classIdx = 3;
                        *classScore = 68.0;
                        return;

                            
                        }

                            
                        }
                        else {
                            
                                
                        *classIdx = 3;
                        *classScore = 68.0;
                        return;

                            
                        }

                            
                        }
                        else {
                            
                                
                        *classIdx = 1;
                        *classScore = 72.0;
                        return;

                            
                        }

                            
                        }

                }
            
        
            
                /**
                 * Random forest's tree #1
                 */
                void tree1(float *x, uint8_t *classIdx, float *classScore) {
                    
                        if (x[29] < 0.0014043225382920355) {
                            
                                
                        if (x[85] < 0.33078111708164215) {
                            
                                
                        *classIdx = 0;
                        *classScore = 72.0;
                        return;

                            
                        }
                        else {
                            
                                
                        *classIdx = 1;
                        *classScore = 73.0;
                        return;

                            
                        }

                            
                        }
                        else {
                            
                                
                        if (x[6] < 0.025743523612618446) {
                            
                                
                        if (x[48] < 0.0872974619269371) {
                            
                                
                        if (x[19] < 0.04991123452782631) {
                            
                                
                        *classIdx = 1;
                        *classScore = 73.0;
                        return;

                            
                        }
                        else {
                            
                                
                        *classIdx = 3;
                        *classScore = 61.0;
                        return;

                            
                        }

                            
                        }
                        else {
                            
                                
                        *classIdx = 1;
                        *classScore = 73.0;
                        return;

                            
                        }

                            
                        }
                        else {
                            
                                
                        *classIdx = 2;
                        *classScore = 69.0;
                        return;

                            
                        }

                            
                        }

                }
            
        
            
                /**
                 * Random forest's tree #2
                 */
                void tree2(float *x, uint8_t *classIdx, float *classScore) {
                    
                        if (x[67] < 0.000434118730481714) {
                            
                                
                        if (x[8] < 0.01122010312974453) {
                            
                                
                        *classIdx = 0;
                        *classScore = 80.0;
                        return;

                            
                        }
                        else {
                            
                                
                        *classIdx = 1;
                        *classScore = 72.0;
                        return;

                            
                        }

                            
                        }
                        else {
                            
                                
                        if (x[58] < 0.8013572692871094) {
                            
                                
                        *classIdx = 2;
                        *classScore = 74.0;
                        return;

                            
                        }
                        else {
                            
                                
                        if (x[103] < 0.3424908518791199) {
                            
                                
                        if (x[47] < 0.0015187985263764858) {
                            
                                
                        if (x[63] < 0.16322313994169235) {
                            
                                
                        *classIdx = 2;
                        *classScore = 74.0;
                        return;

                            
                        }
                        else {
                            
                                
                        *classIdx = 3;
                        *classScore = 49.0;
                        return;

                            
                        }

                            
                        }
                        else {
                            
                                
                        if (x[104] < 0.4715201407670975) {
                            
                                
                        if (x[110] < 29.5) {
                            
                                
                        *classIdx = 3;
                        *classScore = 49.0;
                        return;

                            
                        }
                        else {
                            
                                
                        *classIdx = 1;
                        *classScore = 72.0;
                        return;

                            
                        }

                            
                        }
                        else {
                            
                                
                        *classIdx = 3;
                        *classScore = 49.0;
                        return;

                            
                        }

                            
                        }

                            
                        }
                        else {
                            
                                
                        if (x[105] < 0.4633884131908417) {
                            
                                
                        *classIdx = 1;
                        *classScore = 72.0;
                        return;

                            
                        }
                        else {
                            
                                
                        *classIdx = 3;
                        *classScore = 49.0;
                        return;

                            
                        }

                            
                        }

                            
                        }

                            
                        }

                }
            
        
            
                /**
                 * Random forest's tree #3
                 */
                void tree3(float *x, uint8_t *classIdx, float *classScore) {
                    
                        if (x[8] < 0.01048389170318842) {
                            
                                
                        *classIdx = 0;
                        *classScore = 64.0;
                        return;

                            
                        }
                        else {
                            
                                
                        if (x[20] < 0.5689935088157654) {
                            
                                
                        if (x[48] < 0.033030010759830475) {
                            
                                
                        *classIdx = 2;
                        *classScore = 57.0;
                        return;

                            
                        }
                        else {
                            
                                
                        if (x[28] < 0.07999740168452263) {
                            
                                
                        *classIdx = 1;
                        *classScore = 77.0;
                        return;

                            
                        }
                        else {
                            
                                
                        *classIdx = 3;
                        *classScore = 77.0;
                        return;

                            
                        }

                            
                        }

                            
                        }
                        else {
                            
                                
                        if (x[41] < 0.37309645116329193) {
                            
                                
                        if (x[59] < 0.15457946434617043) {
                            
                                
                        *classIdx = 2;
                        *classScore = 57.0;
                        return;

                            
                        }
                        else {
                            
                                
                        *classIdx = 1;
                        *classScore = 77.0;
                        return;

                            
                        }

                            
                        }
                        else {
                            
                                
                        if (x[109] < 0.00431111641228199) {
                            
                                
                        *classIdx = 1;
                        *classScore = 77.0;
                        return;

                            
                        }
                        else {
                            
                                
                        *classIdx = 3;
                        *classScore = 77.0;
                        return;

                            
                        }

                            
                        }

                            
                        }

                            
                        }

                }
            
        
            
                /**
                 * Random forest's tree #4
                 */
                void tree4(float *x, uint8_t *classIdx, float *classScore) {
                    
                        if (x[19] < 0.020528540015220642) {
                            
                                
                        *classIdx = 0;
                        *classScore = 73.0;
                        return;

                            
                        }
                        else {
                            
                                
                        if (x[8] < 0.04141577333211899) {
                            
                                
                        *classIdx = 1;
                        *classScore = 71.0;
                        return;

                            
                        }
                        else {
                            
                                
                        if (x[0] < 0.7837735712528229) {
                            
                                
                        if (x[7] < 0.00046799209667369723) {
                            
                                
                        *classIdx = 3;
                        *classScore = 70.0;
                        return;

                            
                        }
                        else {
                            
                                
                        *classIdx = 1;
                        *classScore = 71.0;
                        return;

                            
                        }

                            
                        }
                        else {
                            
                                
                        if (x[91] < 34.5) {
                            
                                
                        *classIdx = 2;
                        *classScore = 61.0;
                        return;

                            
                        }
                        else {
                            
                                
                        *classIdx = 3;
                        *classScore = 70.0;
                        return;

                            
                        }

                            
                        }

                            
                        }

                            
                        }

                }
            
        
            
                /**
                 * Random forest's tree #5
                 */
                void tree5(float *x, uint8_t *classIdx, float *classScore) {
                    
                        if (x[29] < 0.0014043225382920355) {
                            
                                
                        if (x[87] < 0.0006107315712142736) {
                            
                                
                        *classIdx = 0;
                        *classScore = 74.0;
                        return;

                            
                        }
                        else {
                            
                                
                        *classIdx = 1;
                        *classScore = 85.0;
                        return;

                            
                        }

                            
                        }
                        else {
                            
                                
                        if (x[41] < 0.3807106614112854) {
                            
                                
                        if (x[31] < 42.0) {
                            
                                
                        *classIdx = 1;
                        *classScore = 85.0;
                        return;

                            
                        }
                        else {
                            
                                
                        *classIdx = 2;
                        *classScore = 57.0;
                        return;

                            
                        }

                            
                        }
                        else {
                            
                                
                        if (x[69] < 0.004355574259534478) {
                            
                                
                        if (x[37] < 0.5587202608585358) {
                            
                                
                        if (x[15] < 4.0) {
                            
                                
                        *classIdx = 2;
                        *classScore = 57.0;
                        return;

                            
                        }
                        else {
                            
                                
                        if (x[90] < 36.0) {
                            
                                
                        *classIdx = 3;
                        *classScore = 59.0;
                        return;

                            
                        }
                        else {
                            
                                
                        *classIdx = 1;
                        *classScore = 85.0;
                        return;

                            
                        }

                            
                        }

                            
                        }
                        else {
                            
                                
                        *classIdx = 3;
                        *classScore = 59.0;
                        return;

                            
                        }

                            
                        }
                        else {
                            
                                
                        if (x[99] < 0.11449534073472023) {
                            
                                
                        if (x[7] < 0.0010482818361197133) {
                            
                                
                        *classIdx = 1;
                        *classScore = 85.0;
                        return;

                            
                        }
                        else {
                            
                                
                        *classIdx = 2;
                        *classScore = 57.0;
                        return;

                            
                        }

                            
                        }
                        else {
                            
                                
                        if (x[100] < 0.6446886658668518) {
                            
                                
                        *classIdx = 1;
                        *classScore = 85.0;
                        return;

                            
                        }
                        else {
                            
                                
                        *classIdx = 3;
                        *classScore = 59.0;
                        return;

                            
                        }

                            
                        }

                            
                        }

                            
                        }

                            
                        }

                }
            
        
            
                /**
                 * Random forest's tree #6
                 */
                void tree6(float *x, uint8_t *classIdx, float *classScore) {
                    
                        if (x[81] < 0.5178571343421936) {
                            
                                
                        if (x[80] < 0.693452388048172) {
                            
                                
                        if (x[0] < 0.7403773367404938) {
                            
                                
                        if (x[57] < -0.5938923191279173) {
                            
                                
                        *classIdx = 1;
                        *classScore = 79.0;
                        return;

                            
                        }
                        else {
                            
                                
                        *classIdx = 3;
                        *classScore = 75.0;
                        return;

                            
                        }

                            
                        }
                        else {
                            
                                
                        *classIdx = 2;
                        *classScore = 55.0;
                        return;

                            
                        }

                            
                        }
                        else {
                            
                                
                        if (x[59] < 0.13904061913490295) {
                            
                                
                        if (x[98] < 4.549886226654053) {
                            
                                
                        if (x[25] < 0.11301723122596741) {
                            
                                
                        *classIdx = 2;
                        *classScore = 55.0;
                        return;

                            
                        }
                        else {
                            
                                
                        if (x[109] < 0.0053651746129617095) {
                            
                                
                        *classIdx = 1;
                        *classScore = 79.0;
                        return;

                            
                        }
                        else {
                            
                                
                        *classIdx = 3;
                        *classScore = 75.0;
                        return;

                            
                        }

                            
                        }

                            
                        }
                        else {
                            
                                
                        if (x[26] < 0.042868148535490036) {
                            
                                
                        if (x[66] < 0.020451043732464314) {
                            
                                
                        *classIdx = 1;
                        *classScore = 79.0;
                        return;

                            
                        }
                        else {
                            
                                
                        *classIdx = 3;
                        *classScore = 75.0;
                        return;

                            
                        }

                            
                        }
                        else {
                            
                                
                        *classIdx = 2;
                        *classScore = 55.0;
                        return;

                            
                        }

                            
                        }

                            
                        }
                        else {
                            
                                
                        *classIdx = 1;
                        *classScore = 79.0;
                        return;

                            
                        }

                            
                        }

                            
                        }
                        else {
                            
                                
                        *classIdx = 0;
                        *classScore = 66.0;
                        return;

                            
                        }

                }
            
        
            
                /**
                 * Random forest's tree #7
                 */
                void tree7(float *x, uint8_t *classIdx, float *classScore) {
                    
                        if (x[88] < 0.022983032278716564) {
                            
                                
                        *classIdx = 0;
                        *classScore = 69.0;
                        return;

                            
                        }
                        else {
                            
                                
                        if (x[61] < 0.2623966932296753) {
                            
                                
                        if (x[48] < 0.09223230555653572) {
                            
                                
                        if (x[6] < 0.024675408378243446) {
                            
                                
                        *classIdx = 3;
                        *classScore = 62.0;
                        return;

                            
                        }
                        else {
                            
                                
                        *classIdx = 2;
                        *classScore = 67.0;
                        return;

                            
                        }

                            
                        }
                        else {
                            
                                
                        *classIdx = 1;
                        *classScore = 77.0;
                        return;

                            
                        }

                            
                        }
                        else {
                            
                                
                        if (x[88] < 0.059859225526452065) {
                            
                                
                        if (x[44] < 0.5459052622318268) {
                            
                                
                        *classIdx = 2;
                        *classScore = 67.0;
                        return;

                            
                        }
                        else {
                            
                                
                        if (x[91] < 50.0) {
                            
                                
                        *classIdx = 1;
                        *classScore = 77.0;
                        return;

                            
                        }
                        else {
                            
                                
                        *classIdx = 0;
                        *classScore = 69.0;
                        return;

                            
                        }

                            
                        }

                            
                        }
                        else {
                            
                                
                        if (x[2] < 0.6026415228843689) {
                            
                                
                        if (x[108] < 0.1066666878759861) {
                            
                                
                        *classIdx = 1;
                        *classScore = 77.0;
                        return;

                            
                        }
                        else {
                            
                                
                        *classIdx = 3;
                        *classScore = 62.0;
                        return;

                            
                        }

                            
                        }
                        else {
                            
                                
                        *classIdx = 3;
                        *classScore = 62.0;
                        return;

                            
                        }

                            
                        }

                            
                        }

                            
                        }

                }
            
        
            
                /**
                 * Random forest's tree #8
                 */
                void tree8(float *x, uint8_t *classIdx, float *classScore) {
                    
                        if (x[46] < 0.010195302311331034) {
                            
                                
                        *classIdx = 0;
                        *classScore = 75.0;
                        return;

                            
                        }
                        else {
                            
                                
                        if (x[8] < 0.04036501795053482) {
                            
                                
                        if (x[49] < 0.0022464609937742352) {
                            
                                
                        *classIdx = 3;
                        *classScore = 75.0;
                        return;

                            
                        }
                        else {
                            
                                
                        if (x[15] < 9.0) {
                            
                                
                        *classIdx = 1;
                        *classScore = 75.0;
                        return;

                            
                        }
                        else {
                            
                                
                        if (x[7] < 9.7222608019365e-05) {
                            
                                
                        *classIdx = 3;
                        *classScore = 75.0;
                        return;

                            
                        }
                        else {
                            
                                
                        *classIdx = 1;
                        *classScore = 75.0;
                        return;

                            
                        }

                            
                        }

                            
                        }

                            
                        }
                        else {
                            
                                
                        if (x[80] < 0.693452388048172) {
                            
                                
                        if (x[18] < 1.6607759594917297) {
                            
                                
                        *classIdx = 3;
                        *classScore = 75.0;
                        return;

                            
                        }
                        else {
                            
                                
                        *classIdx = 2;
                        *classScore = 50.0;
                        return;

                            
                        }

                            
                        }
                        else {
                            
                                
                        if (x[99] < 0.10899821668863297) {
                            
                                
                        *classIdx = 2;
                        *classScore = 50.0;
                        return;

                            
                        }
                        else {
                            
                                
                        if (x[48] < 0.16020427271723747) {
                            
                                
                        *classIdx = 3;
                        *classScore = 75.0;
                        return;

                            
                        }
                        else {
                            
                                
                        *classIdx = 1;
                        *classScore = 75.0;
                        return;

                            
                        }

                            
                        }

                            
                        }

                            
                        }

                            
                        }

                }
            
        
            
                /**
                 * Random forest's tree #9
                 */
                void tree9(float *x, uint8_t *classIdx, float *classScore) {
                    
                        if (x[80] < 0.6279761791229248) {
                            
                                
                        if (x[29] < 0.0019954791059717536) {
                            
                                
                        *classIdx = 0;
                        *classScore = 80.0;
                        return;

                            
                        }
                        else {
                            
                                
                        *classIdx = 2;
                        *classScore = 70.0;
                        return;

                            
                        }

                            
                        }
                        else {
                            
                                
                        if (x[41] < 0.3807106614112854) {
                            
                                
                        *classIdx = 1;
                        *classScore = 60.0;
                        return;

                            
                        }
                        else {
                            
                                
                        if (x[119] < 0.32538028061389923) {
                            
                                
                        if (x[8] < 0.12235240265727043) {
                            
                                
                        if (x[73] < 10.5) {
                            
                                
                        *classIdx = 1;
                        *classScore = 60.0;
                        return;

                            
                        }
                        else {
                            
                                
                        if (x[89] < 0.0043154514860361814) {
                            
                                
                        if (x[71] < 18.0) {
                            
                                
                        *classIdx = 0;
                        *classScore = 80.0;
                        return;

                            
                        }
                        else {
                            
                                
                        *classIdx = 1;
                        *classScore = 60.0;
                        return;

                            
                        }

                            
                        }
                        else {
                            
                                
                        *classIdx = 3;
                        *classScore = 65.0;
                        return;

                            
                        }

                            
                        }

                            
                        }
                        else {
                            
                                
                        *classIdx = 2;
                        *classScore = 70.0;
                        return;

                            
                        }

                            
                        }
                        else {
                            
                                
                        if (x[58] < 1.0498236417770386) {
                            
                                
                        *classIdx = 2;
                        *classScore = 70.0;
                        return;

                            
                        }
                        else {
                            
                                
                        *classIdx = 3;
                        *classScore = 65.0;
                        return;

                            
                        }

                            
                        }

                            
                        }

                            
                        }

                }
            
        
            
                /**
                 * Random forest's tree #10
                 */
                void tree10(float *x, uint8_t *classIdx, float *classScore) {
                    
                        if (x[19] < 0.01995633915066719) {
                            
                                
                        *classIdx = 0;
                        *classScore = 70.0;
                        return;

                            
                        }
                        else {
                            
                                
                        if (x[24] < 0.3899891823530197) {
                            
                                
                        if (x[81] < 0.3958333432674408) {
                            
                                
                        if (x[83] < 0.2678571343421936) {
                            
                                
                        if (x[24] < 0.38760823011398315) {
                            
                                
                        *classIdx = 1;
                        *classScore = 72.0;
                        return;

                            
                        }
                        else {
                            
                                
                        *classIdx = 2;
                        *classScore = 69.0;
                        return;

                            
                        }

                            
                        }
                        else {
                            
                                
                        *classIdx = 3;
                        *classScore = 64.0;
                        return;

                            
                        }

                            
                        }
                        else {
                            
                                
                        *classIdx = 2;
                        *classScore = 69.0;
                        return;

                            
                        }

                            
                        }
                        else {
                            
                                
                        if (x[9] < 0.0017155990353785455) {
                            
                                
                        if (x[85] < 0.26757872104644775) {
                            
                                
                        *classIdx = 3;
                        *classScore = 64.0;
                        return;

                            
                        }
                        else {
                            
                                
                        *classIdx = 1;
                        *classScore = 72.0;
                        return;

                            
                        }

                            
                        }
                        else {
                            
                                
                        if (x[59] < 0.24225781857967377) {
                            
                                
                        *classIdx = 3;
                        *classScore = 64.0;
                        return;

                            
                        }
                        else {
                            
                                
                        *classIdx = 1;
                        *classScore = 72.0;
                        return;

                            
                        }

                            
                        }

                            
                        }

                            
                        }

                }
            
        
            
                /**
                 * Random forest's tree #11
                 */
                void tree11(float *x, uint8_t *classIdx, float *classScore) {
                    
                        if (x[86] < 0.011854317970573902) {
                            
                                
                        *classIdx = 0;
                        *classScore = 65.0;
                        return;

                            
                        }
                        else {
                            
                                
                        if (x[49] < 0.007799098500981927) {
                            
                                
                        if (x[99] < 0.11258673667907715) {
                            
                                
                        if (x[48] < 0.03332744725048542) {
                            
                                
                        *classIdx = 2;
                        *classScore = 60.0;
                        return;

                            
                        }
                        else {
                            
                                
                        *classIdx = 1;
                        *classScore = 87.0;
                        return;

                            
                        }

                            
                        }
                        else {
                            
                                
                        if (x[21] < 0.34740258753299713) {
                            
                                
                        *classIdx = 3;
                        *classScore = 63.0;
                        return;

                            
                        }
                        else {
                            
                                
                        if (x[16] < 0.5) {
                            
                                
                        *classIdx = 1;
                        *classScore = 87.0;
                        return;

                            
                        }
                        else {
                            
                                
                        *classIdx = 3;
                        *classScore = 63.0;
                        return;

                            
                        }

                            
                        }

                            
                        }

                            
                        }
                        else {
                            
                                
                        *classIdx = 1;
                        *classScore = 87.0;
                        return;

                            
                        }

                            
                        }

                }
            
        
            
                /**
                 * Random forest's tree #12
                 */
                void tree12(float *x, uint8_t *classIdx, float *classScore) {
                    
                        if (x[26] < 0.022410851903259754) {
                            
                                
                        if (x[9] < 0.0001308393657382112) {
                            
                                
                        *classIdx = 0;
                        *classScore = 67.0;
                        return;

                            
                        }
                        else {
                            
                                
                        *classIdx = 1;
                        *classScore = 71.0;
                        return;

                            
                        }

                            
                        }
                        else {
                            
                                
                        if (x[40] < 0.7314720749855042) {
                            
                                
                        if (x[41] < 0.4720812290906906) {
                            
                                
                        if (x[88] < 0.05867693945765495) {
                            
                                
                        *classIdx = 2;
                        *classScore = 57.0;
                        return;

                            
                        }
                        else {
                            
                                
                        if (x[23] < 0.3482142835855484) {
                            
                                
                        if (x[6] < 0.01501119276508689) {
                            
                                
                        *classIdx = 3;
                        *classScore = 80.0;
                        return;

                            
                        }
                        else {
                            
                                
                        if (x[91] < 34.5) {
                            
                                
                        *classIdx = 1;
                        *classScore = 71.0;
                        return;

                            
                        }
                        else {
                            
                                
                        *classIdx = 3;
                        *classScore = 80.0;
                        return;

                            
                        }

                            
                        }

                            
                        }
                        else {
                            
                                
                        if (x[119] < 0.2693145163357258) {
                            
                                
                        *classIdx = 1;
                        *classScore = 71.0;
                        return;

                            
                        }
                        else {
                            
                                
                        *classIdx = 3;
                        *classScore = 80.0;
                        return;

                            
                        }

                            
                        }

                            
                        }

                            
                        }
                        else {
                            
                                
                        if (x[57] < 0.24824106693267822) {
                            
                                
                        *classIdx = 2;
                        *classScore = 57.0;
                        return;

                            
                        }
                        else {
                            
                                
                        *classIdx = 3;
                        *classScore = 80.0;
                        return;

                            
                        }

                            
                        }

                            
                        }
                        else {
                            
                                
                        *classIdx = 1;
                        *classScore = 71.0;
                        return;

                            
                        }

                            
                        }

                }
            
        
            
                /**
                 * Random forest's tree #13
                 */
                void tree13(float *x, uint8_t *classIdx, float *classScore) {
                    
                        if (x[3] < 0.3800000101327896) {
                            
                                
                        if (x[0] < 0.7445282936096191) {
                            
                                
                        *classIdx = 3;
                        *classScore = 60.0;
                        return;

                            
                        }
                        else {
                            
                                
                        *classIdx = 2;
                        *classScore = 72.0;
                        return;

                            
                        }

                            
                        }
                        else {
                            
                                
                        if (x[83] < 0.4880952388048172) {
                            
                                
                        if (x[59] < 0.15007372945547104) {
                            
                                
                        if (x[28] < 0.08033285662531853) {
                            
                                
                        *classIdx = 1;
                        *classScore = 73.0;
                        return;

                            
                        }
                        else {
                            
                                
                        *classIdx = 3;
                        *classScore = 60.0;
                        return;

                            
                        }

                            
                        }
                        else {
                            
                                
                        *classIdx = 1;
                        *classScore = 73.0;
                        return;

                            
                        }

                            
                        }
                        else {
                            
                                
                        *classIdx = 0;
                        *classScore = 70.0;
                        return;

                            
                        }

                            
                        }

                }
            
        
            
                /**
                 * Random forest's tree #14
                 */
                void tree14(float *x, uint8_t *classIdx, float *classScore) {
                    
                        if (x[9] < 0.0001308393657382112) {
                            
                                
                        *classIdx = 0;
                        *classScore = 76.0;
                        return;

                            
                        }
                        else {
                            
                                
                        if (x[83] < 0.4077381044626236) {
                            
                                
                        if (x[48] < 0.08073335140943527) {
                            
                                
                        if (x[85] < 0.36618392169475555) {
                            
                                
                        if (x[80] < 0.630952388048172) {
                            
                                
                        *classIdx = 2;
                        *classScore = 58.0;
                        return;

                            
                        }
                        else {
                            
                                
                        *classIdx = 3;
                        *classScore = 69.0;
                        return;

                            
                        }

                            
                        }
                        else {
                            
                                
                        *classIdx = 1;
                        *classScore = 72.0;
                        return;

                            
                        }

                            
                        }
                        else {
                            
                                
                        *classIdx = 1;
                        *classScore = 72.0;
                        return;

                            
                        }

                            
                        }
                        else {
                            
                                
                        if (x[9] < 0.010573979176115245) {
                            
                                
                        if (x[27] < 0.0028883677441626787) {
                            
                                
                        *classIdx = 1;
                        *classScore = 72.0;
                        return;

                            
                        }
                        else {
                            
                                
                        *classIdx = 3;
                        *classScore = 69.0;
                        return;

                            
                        }

                            
                        }
                        else {
                            
                                
                        *classIdx = 2;
                        *classScore = 58.0;
                        return;

                            
                        }

                            
                        }

                            
                        }

                }
            
        
            
                /**
                 * Random forest's tree #15
                 */
                void tree15(float *x, uint8_t *classIdx, float *classScore) {
                    
                        if (x[86] < 0.010794996051117778) {
                            
                                
                        *classIdx = 0;
                        *classScore = 73.0;
                        return;

                            
                        }
                        else {
                            
                                
                        if (x[1] < 0.36113207042217255) {
                            
                                
                        if (x[17] < -0.3451420068740845) {
                            
                                
                        *classIdx = 1;
                        *classScore = 71.0;
                        return;

                            
                        }
                        else {
                            
                                
                        *classIdx = 2;
                        *classScore = 67.0;
                        return;

                            
                        }

                            
                        }
                        else {
                            
                                
                        if (x[108] < 0.10048108920454979) {
                            
                                
                        if (x[0] < 0.6249056756496429) {
                            
                                
                        *classIdx = 1;
                        *classScore = 71.0;
                        return;

                            
                        }
                        else {
                            
                                
                        if (x[46] < 0.03978318814188242) {
                            
                                
                        *classIdx = 3;
                        *classScore = 64.0;
                        return;

                            
                        }
                        else {
                            
                                
                        *classIdx = 1;
                        *classScore = 71.0;
                        return;

                            
                        }

                            
                        }

                            
                        }
                        else {
                            
                                
                        if (x[59] < 0.2087005227804184) {
                            
                                
                        if (x[80] < 0.619047611951828) {
                            
                                
                        *classIdx = 2;
                        *classScore = 67.0;
                        return;

                            
                        }
                        else {
                            
                                
                        *classIdx = 3;
                        *classScore = 64.0;
                        return;

                            
                        }

                            
                        }
                        else {
                            
                                
                        *classIdx = 1;
                        *classScore = 71.0;
                        return;

                            
                        }

                            
                        }

                            
                        }

                            
                        }

                }
            
        
            
                /**
                 * Random forest's tree #16
                 */
                void tree16(float *x, uint8_t *classIdx, float *classScore) {
                    
                        if (x[86] < 0.011753430124372244) {
                            
                                
                        *classIdx = 0;
                        *classScore = 75.0;
                        return;

                            
                        }
                        else {
                            
                                
                        if (x[88] < 0.05867360718548298) {
                            
                                
                        *classIdx = 2;
                        *classScore = 64.0;
                        return;

                            
                        }
                        else {
                            
                                
                        if (x[16] < 0.5) {
                            
                                
                        if (x[77] < 0.5758343040943146) {
                            
                                
                        *classIdx = 1;
                        *classScore = 82.0;
                        return;

                            
                        }
                        else {
                            
                                
                        if (x[60] < 0.6363636255264282) {
                            
                                
                        *classIdx = 1;
                        *classScore = 82.0;
                        return;

                            
                        }
                        else {
                            
                                
                        *classIdx = 3;
                        *classScore = 54.0;
                        return;

                            
                        }

                            
                        }

                            
                        }
                        else {
                            
                                
                        if (x[59] < 0.13662253320217133) {
                            
                                
                        *classIdx = 3;
                        *classScore = 54.0;
                        return;

                            
                        }
                        else {
                            
                                
                        if (x[51] < 30.5) {
                            
                                
                        *classIdx = 1;
                        *classScore = 82.0;
                        return;

                            
                        }
                        else {
                            
                                
                        *classIdx = 3;
                        *classScore = 54.0;
                        return;

                            
                        }

                            
                        }

                            
                        }

                            
                        }

                            
                        }

                }
            
        
            
                /**
                 * Random forest's tree #17
                 */
                void tree17(float *x, uint8_t *classIdx, float *classScore) {
                    
                        if (x[6] < 0.007016309536993504) {
                            
                                
                        *classIdx = 0;
                        *classScore = 81.0;
                        return;

                            
                        }
                        else {
                            
                                
                        if (x[88] < 0.05985589325428009) {
                            
                                
                        *classIdx = 2;
                        *classScore = 64.0;
                        return;

                            
                        }
                        else {
                            
                                
                        if (x[42] < 0.7375634610652924) {
                            
                                
                        if (x[48] < 0.09182913601398468) {
                            
                                
                        if (x[27] < 0.0013350736116990447) {
                            
                                
                        *classIdx = 1;
                        *classScore = 71.0;
                        return;

                            
                        }
                        else {
                            
                                
                        if (x[83] < 0.1696428582072258) {
                            
                                
                        *classIdx = 2;
                        *classScore = 64.0;
                        return;

                            
                        }
                        else {
                            
                                
                        *classIdx = 3;
                        *classScore = 59.0;
                        return;

                            
                        }

                            
                        }

                            
                        }
                        else {
                            
                                
                        *classIdx = 1;
                        *classScore = 71.0;
                        return;

                            
                        }

                            
                        }
                        else {
                            
                                
                        *classIdx = 1;
                        *classScore = 71.0;
                        return;

                            
                        }

                            
                        }

                            
                        }

                }
            
        
            
                /**
                 * Random forest's tree #18
                 */
                void tree18(float *x, uint8_t *classIdx, float *classScore) {
                    
                        if (x[83] < 0.5178571343421936) {
                            
                                
                        if (x[6] < 0.026613366790115833) {
                            
                                
                        if (x[8] < 0.041351811960339546) {
                            
                                
                        if (x[111] < 22.5) {
                            
                                
                        if (x[57] < 0.15476638078689575) {
                            
                                
                        *classIdx = 3;
                        *classScore = 77.0;
                        return;

                            
                        }
                        else {
                            
                                
                        *classIdx = 1;
                        *classScore = 68.0;
                        return;

                            
                        }

                            
                        }
                        else {
                            
                                
                        *classIdx = 1;
                        *classScore = 68.0;
                        return;

                            
                        }

                            
                        }
                        else {
                            
                                
                        if (x[41] < 0.19492385513149202) {
                            
                                
                        *classIdx = 1;
                        *classScore = 68.0;
                        return;

                            
                        }
                        else {
                            
                                
                        *classIdx = 3;
                        *classScore = 77.0;
                        return;

                            
                        }

                            
                        }

                            
                        }
                        else {
                            
                                
                        *classIdx = 2;
                        *classScore = 64.0;
                        return;

                            
                        }

                            
                        }
                        else {
                            
                                
                        *classIdx = 0;
                        *classScore = 66.0;
                        return;

                            
                        }

                }
            
        
            
                /**
                 * Random forest's tree #19
                 */
                void tree19(float *x, uint8_t *classIdx, float *classScore) {
                    
                        if (x[86] < 0.010189669206738472) {
                            
                                
                        *classIdx = 0;
                        *classScore = 73.0;
                        return;

                            
                        }
                        else {
                            
                                
                        if (x[88] < 0.059859225526452065) {
                            
                                
                        *classIdx = 2;
                        *classScore = 67.0;
                        return;

                            
                        }
                        else {
                            
                                
                        if (x[41] < 0.3807106614112854) {
                            
                                
                        if (x[3] < 0.3064150884747505) {
                            
                                
                        *classIdx = 2;
                        *classScore = 67.0;
                        return;

                            
                        }
                        else {
                            
                                
                        *classIdx = 1;
                        *classScore = 78.0;
                        return;

                            
                        }

                            
                        }
                        else {
                            
                                
                        if (x[49] < 0.0067182492930442095) {
                            
                                
                        *classIdx = 3;
                        *classScore = 57.0;
                        return;

                            
                        }
                        else {
                            
                                
                        *classIdx = 1;
                        *classScore = 78.0;
                        return;

                            
                        }

                            
                        }

                            
                        }

                            
                        }

                }
            
        


            
};



static RandomForestClassifier forest;


#endif