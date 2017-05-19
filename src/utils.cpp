#include "dip.hpp"

vector<Point2i> match(vector<Point2i> &point1,vector<Point2i> &point2)
{
	vector<Point2i> ans;
	for (int i=0;i<point1.size();++i)
	{
		int min=10000000,point=0,temp;
		for (int j=0;j<point2.size();++j)
		{
			temp=norm(point1[i]-point2[j]);
			if (min>temp)
			{
				min=temp;
				point=j;
			};
		};
		ans.push_back(Point2i(i,point));
	};
	return ans;
};