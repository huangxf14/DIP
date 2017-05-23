#include "dip.hpp"
//#include <algorithm>
//#include <functional>

bool cmp(Point3i &x,Point3i &y)
{
	return x.x<y.x;
};

// 对模板关键点和根据轮廓取出的物体关键点进行匹配
vector<Point2i> Match(vector<Point2i> &point1,vector<Point2i> &point2,double value)
{
	vector<Point2i> ans;
	vector<Point3i> graph;
	//int ;
	double temp;
	for (int i=0;i<point1.size();++i)
	{
		for (int j=0;j<point2.size();++j)
		{
			temp=norm(point1[i]-point2[j]);
			graph.push_back(Point3i(floor(temp),i,j));
		};
	};
	sort(graph.begin(),graph.end(),cmp);
	bool *flag1=new bool[point1.size()],*flag2=new bool[point2.size()];
	for (int i=0;i<point1.size();++i) flag1[i]=false;
	for (int i=0;i<point2.size();++i) flag2[i]=false;
	int num=0;
	for (int i=0;i<graph.size();++i)
	{
		if (graph[i].x>value) break;
		if (flag1[graph[i].y]||(flag2[graph[i].z])) continue;
		ans.push_back(Point2i(graph[i].y,graph[i].z));
		flag1[graph[i].y]=true;flag2[graph[i].z]=true;
		num++;
		if (num>=point1.size()) break;
	};
	return ans;
};

// 根据关键点绘制手的轮廓
void DrawKeypoints(Mat &img, vector<Point2i> &keypoints) {

  Mat mask;
  KeypointsMask(img, keypoints, mask);

  Mat tmp;
  img.copyTo(tmp);
  tmp.setTo(Scalar(255, 255, 255), mask);
  addWeighted(img, 0.8, tmp, 0.2, 0., img);
}

// 根据关键点取出手掌形状的 mask
void KeypointsMask(Mat &img, vector<Point2i> &keypoints, Mat &mask) {

  mask = Mat(img.rows, img.cols, CV_8UC1);

  Scalar color(255);
  int thickness = 30;

  // palm
  vector<Point2i> palm_boundary;
  palm_boundary.push_back(keypoints.at(0));
  palm_boundary.push_back(keypoints.at(1));
  palm_boundary.push_back(keypoints.at(3));
  palm_boundary.push_back(keypoints.at(5));
  palm_boundary.push_back(keypoints.at(7));
  palm_boundary.push_back(keypoints.at(9));
  palm_boundary.push_back(keypoints.at(11));

  for (int i = 0; i != palm_boundary.size(); ++i) {
    line(mask, palm_boundary.at(i), palm_boundary.at((i + 1) % palm_boundary.size()), color, thickness);
  }
  fillConvexPoly(mask, palm_boundary, color);

  // finger 1
  Point2i finger1root(
    (keypoints.at(1).x + keypoints.at(3).x) / 2,
    (keypoints.at(1).y + keypoints.at(3).y) / 2);
  line(mask, finger1root, keypoints.at(2), color, thickness);

  // finger 2
  Point2i finger2root(
    (keypoints.at(3).x + keypoints.at(5).x) / 2,
    (keypoints.at(3).y + keypoints.at(5).y) / 2);
  line(mask, finger2root, keypoints.at(4), color, thickness);

  // finger 3
  Point2i finger3root(
    (keypoints.at(5).x + keypoints.at(7).x) / 2,
    (keypoints.at(5).y + keypoints.at(7).y) / 2);
  line(mask, finger3root, keypoints.at(6), color, thickness);

  // finger 4
  Point2i finger4root(
    (keypoints.at(7).x + keypoints.at(9).x) / 2,
    (keypoints.at(7).y + keypoints.at(9).y) / 2);
  line(mask, finger4root, keypoints.at(8), color, thickness);

  // finger 5
  Point2i finger5root(
    (keypoints.at(9).x + keypoints.at(11).x) / 2,
    (keypoints.at(9).y + keypoints.at(11).y) / 2);
  line(mask, finger5root, keypoints.at(10), color, thickness);

  threshold(mask, mask, 0, 255, THRESH_BINARY);
}

// 在画面上绘制关键点匹配的结果
void DrawMatch(Mat &img, vector<pair<Point2i, Point2i>> &match) {
  for (int i = 0; i != match.size(); ++i) {
    circle(img, match[i].second, 2, Scalar(0, 0, 255), 2);
    line(img, match[i].first, match[i].second, Scalar(255, 0, 0), 2);
  }
}

// 在画面上绘制提示信息
void DrawInfo(Mat &img, bool is_palm, bool is_living, int stage) {
  Scalar red(0, 0, 255), green(0, 255, 0), white(255, 255, 255);
  if (is_living) {
    putText(img, "Living", Point2i(20, 30), FONT_HERSHEY_SIMPLEX, 1, red, 2);
  } else {
    putText(img, "Not Living", Point2i(20, 30), FONT_HERSHEY_SIMPLEX, 1, white, 2);
  }
  if (is_palm) {
    putText(img, "Move Fingers Follow Instructions", Point2i(20, 60), FONT_HERSHEY_SIMPLEX, 1, green, 2);
    putText(img, string("Step: ") + to_string(stage), Point2i(20, 90), FONT_HERSHEY_SIMPLEX, 1, green, 2);
  } else {
    putText(img, "Put Right Hand In Area", Point2i(20, 60), FONT_HERSHEY_SIMPLEX, 1, white, 2);
  }
}