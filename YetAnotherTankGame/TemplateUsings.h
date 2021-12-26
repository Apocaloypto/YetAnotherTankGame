#pragma once
#include <set>
#include "Point2D.h"
#include "Vector2D.h"
#include "Dim2D.h"
#include "Typedefs.h"

using CPixelPos = CPoint2D<Real>;
using CPixelDim = CDim2D<Real>;

using CTilePos = CPoint2D<Real>;
using CTileDim = CDim2D<Real>;


template<class T>
struct const_ptr_compare : std::less<T const*> {
  typedef void is_transparent;
};

template<class T>
using ptr_set = std::set<T *, const_ptr_compare<T>>;
