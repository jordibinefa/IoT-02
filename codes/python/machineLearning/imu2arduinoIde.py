# Inspired from https://eloquentarduino.com/gesture-classification/
# Code modified by Jordi Binefa
# 20231213
from everywhereml.data import Dataset
from everywhereml.preprocessing import Pipeline, MinMaxScaler, Window, SpectralFeatures
from pprint import pprint
from everywhereml.sklearn.ensemble import RandomForestClassifier

filesPath = "newArduinoIdeFiles/"

def writeFileFromString(fileName,stringText):
    f = open(fileName, 'w')
    f.write(stringText)
    f.close()
    # print("%s has been written"%fileName)

imu_dataset = Dataset.from_csv(
    'imu.csv',
    name='ContinuousMotion',
    target_name_column='target_name'
)

# print(imu_dataset.describe())

# this is the frequency of your sensor
# change according to your hardware
sampling_frequency = 60
mean_gesture_duration_in_millis = 1000
window_length = sampling_frequency * mean_gesture_duration_in_millis // 1000

imu_pipeline = Pipeline(name='ContinousMotionPipeline', steps=[
    MinMaxScaler(),
    # shift can be an integer (number of samples) or a float (percent)
    Window(length=window_length, shift=0.3),
    # order can either be 1 (first-order features) or 2 (add second-order features)
    SpectralFeatures(order=2)
])

"""
Enumerate features extracted from the SpectralFeatures step
"""

# pprint(imu_pipeline['SpectralFeatures'][0].feature_names)

"""
Apply feature pre-processing
"""
imu_dataset.apply(imu_pipeline)

print(imu_dataset.describe())

"""
Perform classification with a RandomForest
"""
imu_classifier = RandomForestClassifier(n_estimators=20, max_depth=20)
imu_train, imu_test = imu_dataset.split(test_size=0.3)
imu_classifier.fit(imu_train)

print('Score on test set: %.2f' % imu_classifier.score(imu_test))

pipelineH = imu_pipeline.to_arduino_file(
    '%sPipeline.h'%filesPath, 
    instance_name='pipeline'
)
# There is an error when compiling from Arduino IDE
# With this text replacement error is fixed
pipelineH = pipelineH.replace("transform(X, X)","transform(X)")

writeFileFromString('%sPipeline.h'%filesPath,pipelineH)
classifierH = imu_classifier.to_arduino_file(
    '%sClassifier.h'%filesPath,
    instance_name='forest',
    class_map=imu_dataset.class_map
)
# writeFileFromString('%sClassifier.h'%filesPath,classifierH)

