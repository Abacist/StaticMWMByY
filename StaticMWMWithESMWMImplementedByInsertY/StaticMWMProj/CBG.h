#pragma once

#include<vector>

using namespace std;


class X
{
public:
	int _id;
	int _begin;
	int _end;
	int _weight;
	bool operator==(const X& rhs)
	{
		return this->_id == rhs._id;
	}
};

class Y
{
public:
	int _value;
	//int _weight;
};

class CBG
{
public:
	vector<X> _allX;
	vector<Y> _allY;
};

class MatchingEdge
{
public:
	X _x;
	Y _y;
};

