#include <ros/ros.h>
#include <sensor_msgs/PointCloud2.h>
#include "stdlib.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <pcl/io/io.h>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <pcl_ros/publisher.h>
#include <pcl_conversions/pcl_conversions.h>
#include <ros/publisher.h>

#include "pcd_transformer.h"


using namespace std;

vector<string> split(const char *str, char c = ' ')
{
    vector<string> result;

    do
    {
        const char *begin = str;

        while(*str != c && *str)
            str++;

        result.push_back(string(begin, str));
    } while (0 != *str++);

    return result;
}



int main(int argc, char** argv){
	//Init ROS-related things
	ros::init(argc, argv, "align_pcd");
	ros::NodeHandle n;

	double R[9]={1,0,0,0,1,0,0,0,1};
	double T[3]={0,0,0};

	ros::ServiceClient client;
	pcl_ros::Publisher<sensor_msgs::PointCloud2> pub1;
	pcl_ros::Publisher<sensor_msgs::PointCloud2> pub2;
	pcl_ros::Publisher<sensor_msgs::PointCloud2> pub3;
	pub1.advertise(n, "cloud1",1);
	pub2.advertise(n, "cloud2",1);
	pub3.advertise(n, "cloud3",1);
	sensor_msgs::PointCloud2 pcd1;
	sensor_msgs::PointCloud2 pcd2;
	sensor_msgs::PointCloud2 pcd3;

	//print the usage
	if(argc<4){
		cout<<"Error: Not enough arguments"<<endl;
		cout<<"Usage: ./align_pcd pcd1 pcd2 tf"<<endl;
		return 0;
	}

	//get argvs
	string pcdpath1=argv[1];
	string pcdpath2=argv[2];
	string tfpath=argv[3];

	//read tf
	ifstream openFile(argv[3]);
	if(openFile.is_open()){
		string line;
		getline(openFile,line);
		//cout<<line<<endl;
		vector<string> tok=split(line.c_str(),' ');
		//cout<<tok.size()<<endl;
		for(int m=0;m<9;m++){
			R[m]=atof(tok[m].c_str());		
			//cout<<R[m]<<endl;
		}
		T[0]=atof(tok[9].c_str());
		T[1]=atof(tok[10].c_str());
		T[2]=atof(tok[11].c_str());
	}
	



	//read pcds
	if (pcdpath1 == "" || pcl::io::loadPCDFile(pcdpath1, pcd1) == -1){
	      ROS_INFO("No Such File");

	}

	if (pcdpath2 == "" || pcl::io::loadPCDFile(pcdpath2, pcd2) == -1){
	      ROS_INFO("No Such File");
	}


	pcd_transformer pcd_tf;
	pcd3=pcd_tf.run(pcd1,R,T);
	


	ros::Rate r(10);
	while(ros::ok()){
	        pcd1.header.frame_id="/base_link";
		pcd2.header.frame_id="/base_link";
		pcd3.header.frame_id="/base_link";
		pcd1.header.stamp=ros::Time::now();
		pcd2.header.stamp=ros::Time::now();
		pcd3.header.stamp=ros::Time::now();
		pub1.publish(pcd1);
		pub2.publish(pcd2);
		pub3.publish(pcd3);
		r.sleep();
	}


	return 0;
}
