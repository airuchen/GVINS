#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <geometry_msgs/PoseStamped.h>

void poseCallback(const geometry_msgs::PoseStampedPtr& msg) {
	static tf::TransformBroadcaster br;
	tf::Transform transform;
        transform.setOrigin( tf::Vector3(msg->pose.position.x, msg->pose.position.y, 0.0) );
        tf::Quaternion q;
	q[0] = msg->pose.orientation.x;
	q[1] = msg->pose.orientation.y;
	q[2] = msg->pose.orientation.z;
	q[3] = msg->pose.orientation.w;
        transform.setRotation(q);
        br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "odom", "base_link"));
}

int main(int argc, char** argv){
	ros::init(argc, argv, "global_tf_pub");
	ros::NodeHandle node;
	ros::Subscriber sub = node.subscribe("gvins/enu_pose", 5, &poseCallback);

	ros::spin();
	return 0;
};
