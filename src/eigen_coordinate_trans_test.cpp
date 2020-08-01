#include <iostream>
using namespace std;
#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Geometry>
//g++ eigen_test.cpp  -I/usr/include/eigen3 -L/opt/ros/kinetic/lib/ -leigen_conversions -std=c++11
int main()
{
  Eigen::Quaterniond q2 = Eigen::Quaterniond(0.99999, -0.000006, 0.000006, 0.0004).normalized();
  Eigen::Vector3d t2 = Eigen::Vector3d(2.645197, 0.000006, 0.000004);
  Eigen::Isometry3d T2 = Eigen::Isometry3d::Identity();
  T2.rotate(q2.toRotationMatrix());
  T2.pretranslate(t2);
  Eigen::Vector3d target_pos = T2.inverse() * Eigen::Vector3d(5.0, 0.5, 0);
  cout << target_pos.transpose() << endl;
  return 1;
}
