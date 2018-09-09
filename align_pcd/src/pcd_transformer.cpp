#include "pcd_transformer.h"

pcd_transformer::pcd_transformer()
{
	
}

pcd_transformer::~pcd_transformer()
{
	
}

void pcd_transformer::init(){


}

sensor_msgs::PointCloud2 pcd_transformer::run(sensor_msgs::PointCloud2 in, double R[9], double T[3]){

    pcl::PCLPointCloud2 pcl_pc;
    pcl_conversions::toPCL(in, pcl_pc);
    pcl::PointCloud<pcl::PointXYZRGBA>::Ptr clouddata(new pcl::PointCloud<pcl::PointXYZRGBA>);
    pcl::PointCloud<pcl::PointXYZRGBA>::Ptr clouddata_transformed(new pcl::PointCloud<pcl::PointXYZRGBA>);
    pcl::fromPCLPointCloud2(pcl_pc,*clouddata);

    double px, py, pz, qw, qx, qy, qz;
    px = T[0];
    py = T[1];
    pz =  T[2];


    Eigen::Matrix3f mat;
    mat(0,0)=R[0];
    mat(0,1)=R[1];
    mat(0,2)=R[2];
    mat(1,0)=R[3];
    mat(1,1)=R[4];
    mat(1,2)=R[5];
    mat(2,0)=R[6];
    mat(2,1)=R[7];
    mat(2,2)=R[8];

    Eigen::Quaternionf q(mat);
    //std::cout<<q.x()<<" " <<q.y()<<" " <<q.z()<<" "<<q.w()<<std::endl;

    qx=q.x();
    qy=q.y();
    qz=q.z();
    qw=q.w();



    tf::Transform transform;
    transform.setOrigin(tf::Vector3(px,py,pz));
    transform.setRotation(tf::Quaternion(qx,qy,qz,qw));

    tf::Transform inv_transform;
    inv_transform = transform.inverse();
    Eigen::Affine3d tf_final;
    //tf::transformTFToEigen(inv_transform,tf_final);
    tf::transformTFToEigen(transform,tf_final);

    pcl::transformPointCloud (*clouddata, *clouddata_transformed, tf_final);

    pcl::PCLPointCloud2 pcl_pc2;
    pcl::toPCLPointCloud2(*clouddata_transformed,pcl_pc2);

    sensor_msgs::PointCloud2 output;
    pcl_conversions::fromPCL(pcl_pc2,output);

    return output;



}

