#include <message_filters/subscriber.h>
#include <message_filters/time_synchronizer.h>
#include <message_filters/synchronizer.h>
#include <message_filters/sync_policies/exact_time.h>
#include <message_filters/sync_policies/approximate_time.h>
#include <sensor_msgs/Imu.h>
#include "ros/ros.h"

using namespace sensor_msgs;
using namespace message_filters;

ros::Publisher sync_imu_pub;

void callback(const ImuConstPtr& xsense_imu_data, const ImuConstPtr& realsense_imu_data)
{
    Imu sync_imu_data;
    sync_imu_data = *realsense_imu_data;
    sync_imu_data.orientation = xsense_imu_data->orientation;
    sync_imu_data.orientation_covariance = xsense_imu_data->orientation_covariance;

    sync_imu_pub.publish(sync_imu_data);
  // Solve all of perception here...
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "imu_sync_pub");

  ros::NodeHandle nh;
  message_filters::Subscriber<Imu> xsense_imu_sub(nh, "imu/data", 1);
  message_filters::Subscriber<Imu> realsense_imu_sub(nh, "camera/imu", 1);
  sync_imu_pub = nh.advertise<Imu>("sync_imu", 1);

  typedef sync_policies::ApproximateTime<Imu, Imu> MySyncPolicy;
  // typedef sync_policies::ExactTime<Imu, Imu> MySyncPolicy;
  Synchronizer<MySyncPolicy> sync(MySyncPolicy(100),xsense_imu_sub, realsense_imu_sub);
  sync.registerCallback(boost::bind(&callback, _1, _2));

  ros::spin();

  return 0;
}