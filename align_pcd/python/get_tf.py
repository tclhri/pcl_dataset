#!/usr/bin/env python

import numpy as np
import os
import sys

def run():
    tffile1=sys.argv[1]
    tffile2=sys.argv[2]

    frame1=np.zeros((12,3))
    frame2=np.zeros((12,3))

    tf1=open(tffile1,"r")
    for i in range(0,24):
      line = tf1.readline()
      if i>11:
        frame1[i-12,0]=float(line.split(',')[0])
	frame1[i-12,1]=float(line.split(',')[1])
	frame1[i-12,2]=float(line.split(',')[2])
    print frame1

    tf2=open(tffile2,"r")
    for i in range(0,24):
      line = tf2.readline()
      if i>11:
        frame2[i-12,0]=float(line.split(',')[0])
	frame2[i-12,1]=float(line.split(',')[1])
	frame2[i-12,2]=float(line.split(',')[2])
    print frame2
 
    index=np.zeros(0)
    index = index.astype(int)

    for i in range(0,12):
      if ((frame1[i,0]!=0)and(frame2[i,0]!=0)):
        index=np.append(index,[i])


    frame1 = frame1[index,:]
    frame2 = frame2[index,:]

    print frame1
    print frame2
    p=frame1.T
    q=frame2.T

    n_effect = index.size

    weight =1./n_effect
    weight_matrix = np.zeros(n_effect)
    for i in range(0,n_effect):
      weight_matrix[i]=weight

    #print(weight_matrix)

    q_bar = np.dot(q,weight_matrix)
    q_mark = q-np.tile(q_bar,(n_effect,1)).T
    q_mark = q_mark/n_effect
    #print q_mark

    p_bar = np.dot(p,weight_matrix)
    p_mark = p-np.tile(p_bar,(n_effect,1)).T
    #print p_mark

    N = np.dot(p_mark,q_mark.T)
    #print N

    P, D, Q = np.linalg.svd(N, full_matrices=False)

    U=P
    V=Q.T

    #print U
    #print V

    tmpD = np.linalg.det(np.dot(U,V.T))
    minusD=np.array([1, 0, 0, 0, 1, 0, 0, 0, tmpD])
    minusD=minusD.reshape(3,3)
    R=np.dot(V,minusD)  #rotation matrix
    R=np.dot(R,U.T)

    print R   

    T=q_bar-np.dot(R,p_bar)
    print T

    result=open("tf.txt",'w')
    data="%f %f %f %f %f %f %f %f %f %f %f %f"%(R[0][0],R[0][1],R[0][2],R[1][0],R[1][1],R[1][2],R[2][0],R[2][1],R[2][2],T[0],T[1],T[2])
    result.write(data)
    result.close()








if __name__ == "__main__":
    if len(sys.argv)<2:
      print("Erorr: not enough arguments")
      print("Usage: python get_tf.py tffile1 tffile2")
    else:
      run()

