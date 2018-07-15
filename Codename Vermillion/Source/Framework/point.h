#pragma once


template<class T>
class Point {
public:
	Point() { x = y = 0; }
	Point(T x, T y) { this->x = x; this->y = y; }
	void Set(T x, T y) { this->x = x; this->y = y; }
	void Set(Point<T> &p) { x = p.x; y = p.y; }
	
	Point<T> operator- (const Point<T>& subtrahed) {
		return Point(p.x - subtrahed.x, p.y - subtrahed.y);
	}

	T x, y;
};

using Pointf = Point<float>;
using Pointd = Point<double>;
using Pointi = Point<int>;