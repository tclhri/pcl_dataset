#ifndef __pcd_transformer__
#define __pcd_transformer__

#include <sensor_msgs/PointCloud2.h>
#include <geometry_msgs/TransformStamped.h>

#include <tf/transform_broadcaster.h>
#include <tf_conversions/tf_eigen.h>

#include <pcl/io/io.h>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <pcl_ros/publisher.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/common/transforms.h>


class pcd_transformer
{
public:
		pcd_transformer();
		~pcd_transformer();
		
public:


		sensor_msgs::PointCloud2 cloud;
		// create and destroy
		void init();
        sensor_msgs::PointCloud2 run(sensor_msgs::PointCloud2 in, double R[9], double T[3]);



};
#endif

