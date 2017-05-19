#include "dip.hpp"

int main(int argc, char *argv[]) {
  /*int device_id = 0;
  LivenessDetector detector(device_id);
  detector.Detect();*/
  string filename="007.jpg";
  Mat pic=imread(filename);
  int m[3],s[3];
  for (int k=0;k<3;++k) m[k]=0;
  for (int k=0;k<3;++k) s[k]=0;
  for (int i=0;i<pic.rows;++i)
	  for (int j=0;j<pic.cols;++j)
		for (int k=0;k<3;++k)
		{
			m[k]+=int(pic.at<Vec3b>(i,j)[k]);  
		};
  for (int k=0;k<3;++k) m[k]/=pic.rows*pic.cols;
  for (int i=0;i<pic.rows;++i)
	  for (int j=0;j<pic.cols;++j)
		for (int k=0;k<3;++k)
		{
			s[k]+=(int(pic.at<Vec3b>(i,j)[k])-m[k])*(int(pic.at<Vec3b>(i,j)[k])-m[k]);  
		};
  for (int k=0;k<3;++k) s[k]/=pic.rows*pic.cols;
  printf("%d %d %d",s[0],s[1],s[2]);
}