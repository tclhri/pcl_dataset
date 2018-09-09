This place is for sample database of point cloud used for training 3D Local Surface Descriptor. 

maintained by Ju-Hwan Seo (mail to: seojh1989@gmail.com)

ROS(Robot Operating System) based "alignment visualization" programs are included. 

Usage:


1. get transformation between two images  

rosrun align_pcd get_tf.py $tffile1 $tffile2

Ex) rosrun align_pcd get_tf.py ./data/hd/glove/glove_2_hd.txt ./data/hd/glove/glove_8_hd.txt

result) tf.txt file will be generated


2. align and publish.

rosrun align_pcd align_pcd $pcd1 $pcd2 tf.txt

Ex) rosrun align_pcd align_pcd ./data/hd/glove/glove_2_hd.pcd ./data/hd/glove/glove_8_hd.pcd tf.txt


3. Run Rviz and visualize point clouds for each topic name

cloud1: pcd1

cloud2: pcd2

cloud3: aligned pcd (=transformed pcd1)



