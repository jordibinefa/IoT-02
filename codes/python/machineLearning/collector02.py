from everywhereml.data import Dataset
from everywhereml.data.collect import SerialCollector

"""
Create a SerialCollector object.
Each data line is marked by the 'IMU:' string
Collect 30 seconds of data for each gesture
Replace the port with your own!

If a imu.csv file already exists, skip collection
"""

try:
    imu_dataset = Dataset.from_csv(
        'imu.csv', 
        name='ContinuousMotion', 
        target_name_column='target_name'
    )
    """
    Print summary of dataset
    """
    print(imu_dataset.describe())
    """
    Plot features pairplot
    Since this is a time series dataset, the pairplot won't be very informative
    We will come back to the pairplot after feature pre-processing to see great improvements!
    """
    # imu_dataset.plot.features_pairplot(n=300)
    
except FileNotFoundError:
    imu_collector = SerialCollector(
        port='/dev/ttyUSB0', 
        baud=115200, 
        start_of_frame='IMU:', 
        feature_names=['ax', 'ay', 'az', 'gx', 'gy', 'gz']
    )
    imu_dataset = imu_collector.collect_many_classes(
        dataset_name='ContinuousMotion', 
        duration=30
    )
    
    # save dataset to file for later use
    imu_dataset.df.to_csv('imu.csv', index=False)
    

    
