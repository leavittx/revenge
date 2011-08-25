#pragma once

#include "vector2.h"

template <class T>
class Rect
{
private:
		// the defined rectangle is

		T m_x1,  // the smaller x int the rect
		  m_y1,  // the smaller y in the rect
		  m_x2,  // the smaller x >= x1 and outside the rect (if x1 == x2, the width is 0)
		  m_y2;  // the smaller y >= y2 and outside the rect (if x1 == x2, the height is 0)

public:
		//////////////////
		// constructors //
		//////////////////

//		static rect opCall(T x1, T y1, T x2, T y2)
//		{
//			rect v = void;
//			v.m_x1 = min(x1, x2);
//			v.m_x2 = max(x1, x2);
//			v.m_y1 = min(y1, y2);
//			v.m_y2 = max(y1, y2);
//			return v;
//		}

		Rect(T x1, T y1, T x2, T y2)
		{
			m_x1 = min(x1, x2);
			m_x2 = max(x1, x2);
			m_y1 = min(y1, y2);
			m_y2 = max(y1, y2);
		}


		// construct from 2 point
		Rect(Vector2<T> x, Vector2<T> y)// : Rect(x.x, x.y, y.x, y.y)
		{
			m_x1 = x.x;
			m_x2 = y.x;
			m_y1 = x.y;
			m_y2 = y.y;
		}

		/////////////
		// getters //
		/////////////

		Vector2<T> a() { return Vector2<T>(x1, y1); }
		Vector2<T> b() { return Vector2<T>(x2, y2); }
		T x1() { return m_x1; }
		T x2() { return m_x2; }
		T y1() { return m_y1; }
		T y2() { return m_y2; }
		Vector2<T> size() { return Vector2<T>(width, height); }


		T width() { return m_x2 - m_x1; }
		T height() { return m_y2 - m_y1; }

		double ratio() { return width / static_cast<double>(height); }

//		char[] toString()
//		{
//			return format("( %s , %s ) - ( %s , %s )", m_x1, m_y1, m_x2, m_y2);
//		}

		// returns the largest rect within with asked ratio
//		rect subRectWithRatio(double asked_ratio)
//		{
//			rect r = *this;

//			if (ratio < asked_ratio) // crop because of ratio if needed
//			{
//				auto new_height = width * (1.0 / asked_ratio);
//				auto diff = height - new_height;
//				r.m_y1 += cast(T)round(diff * 0.5);
//				r.m_y2 -= cast(T)round(diff * 0.5);
//			}
//			else if (ratio > asked_ratio)
//			{
//				auto new_width = height * asked_ratio;
//				auto diff = width - new_width;
//				r.m_x1 += cast(T)round(diff * 0.5);
//				r.m_x2 -= cast(T)round(diff * 0.5);
//			}
//			return r;
//		}
};

typedef Rect<int>    Recti;
typedef Rect<float>  Rectf;
typedef Rect<double> Rectd;
