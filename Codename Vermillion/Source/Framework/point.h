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

template<class T>
class Color {
public:
	Color():r(0), b(0), g(0), a(1) { }
	Color(T color) :r(color), b(color), g(color), a(1) { }
	Color(T r, T g, T b) :r(r), g(g), b(b), a(1) {}
	Color(T r, T g, T b, T a) :r(r), g(g), b(b), a(a) {}

	void Set(Color<T> &c) { r = c.r; g = c.g; b = c.b; a = c.a; }

	T r, g, b, a;
};

using Colorf = Color<float>;
using Colord = Color<double>;