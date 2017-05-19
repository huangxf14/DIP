#include "dip.hpp"
//#include <algorithm>
//#include <functional>

bool cmp(Point3i &x,Point3i &y)
{
	return x.x<y.x;
};

vector<Point2i> Match(vector<Point2i> &point1,vector<Point2i> &point2,int value)
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

