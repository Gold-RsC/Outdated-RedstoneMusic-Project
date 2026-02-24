#ifndef soma3_h
#define soma3_h
#include"geometry.h"
namespace soma3 {
	typedef std::pair<geometry::Point,int> Center;
	Center findStartCenter(geometry::Point p0,geometry::Point p1) {
		geometry::Line startLine;
		startLine.a=0,startLine.b=-1,startLine.c=p0.y;
		geometry::Line vbLine=geometry::Segment(p0,p1).vertical_bisector_line();
		geometry::Point p=startLine.intersection_point(vbLine);
		int mode;
		if(p.isnone) {
			p.y=p1.y,p.x=p.x-1;
			mode=0;
		} else {
			if(p0.x<p1.x) {
				mode=1;
			} else {
				mode=-1;
			}
		}
		return Center(p,mode);
	}
	Center findCenter(geometry::Point p1,geometry::Point p2,Center oldValue) {
		geometry::Point oldCenter=oldValue.first;
		geometry::Line exLine(oldCenter,p1);
		geometry::Line vbLine=geometry::Segment(p1,p2).vertical_bisector_line();
		geometry::Point center=exLine.intersection_point(vbLine);
		if(center.isnone) {
			geometry::Line vLine=geometry::Line(p1,p2).vertical_line(p2);
			geometry::Point dummyCenter;
			if(vLine.a!=0) {
				dummyCenter((-vLine.c-vLine.b*(p2.y-1))/vLine.a,p2.y-1);
			} else {
				dummyCenter(p2.x-1,(-vLine.c-vLine.a*(p2.x-1))/vLine.b);
			}
			return Center(dummyCenter,0);
		} else {
			int mode;
			if(exLine.a==0) {
				if((oldCenter.x<p1.x&&p1.x<center.x)||(oldCenter.x>p1.x&&p1.x>center.x)) {
					mode=-oldValue.second;
				} else {
					mode=oldValue.second;
				}
			} else {
				if((oldCenter.y<p1.y&&p1.y<center.y)||(oldCenter.y>p1.y&&p1.y>>center.y)) {
					mode=-oldValue.second;
				} else {
					mode=oldValue.second;
				}
			}
			return Center(center,mode);
		}
	}
}

#endif

