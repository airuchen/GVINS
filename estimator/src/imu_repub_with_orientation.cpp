#include "ros/ros.h"
#include <sensor_msgs/Imu.h>
#include <geometry_msgs/Quaternion.h>

ros::Publisher sync_imu_pub;

sensor_msgs::Imu current_imu;

void realsense_imu_callback(const sensor_msgs::ImuConstPtr& msg)
{
    sensor_msgs::Imu imu_data;
    imu_data = *msg;
    imu_data.orientation = current_imu.orientation;
    imu_data.orientation_covariance =  current_imu.orientation_covariance;

    sync_imu_pub.publish(imu_data);

}

void xsense_imu_callback(const sensor_msgs::ImuConstPtr& msg)
{
    current_imu.orientation = msg->orientation;
    current_imu.orientation_covariance = msg->orientation_covariance;    
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "imu_fusion");
    ros::NodeHandle nh;
    ros::Subscriber realsense_imu_sub = nh.subscribe("camera/imu", 1, realsense_imu_callback);
    ros::Subscriber xsense_imu_sub = nh.subscribe("imu/data", 1, xsense_imu_callback);
    sync_imu_pub = nh.advertise<sensor_msgs::Imu>("sync_imu", 1);

    ros::spin();

    return 0;
}