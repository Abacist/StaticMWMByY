#include<iostream>
#include<algorithm>
#include<fstream>
#include<Windows.h>
#include<set>
#include"CBG.h"
#include"auxiliary.h"

using namespace std;

void staticMWM(CBG g, vector<X> & z, vector<X> & t);
vector<MatchingEdge> ESMCM(CBG g);
vector<X> EEReplaceableSet(CBG g, vector<X> z, X x);
vector<X> ESReplaceableSet(CBG g, vector<X> z, X x);
vector<X> ReplaceXinEE(CBG g, X x, vector<X> z, vector<X> x1);
void ESMWM(CBG g, vector<X>& z, vector<X>& t);
void generator(char* fileName);
void readAll(char* fileName, CBG& g);



int main()
{
	ofstream out("outputStaticZ.txt");
	//cout << "hello" << endl;
	generator("input.txt");
	CBG g;
	readAll("input.txt", g);
	//cout << "Read!" << endl;
	vector<X> z, t;
	staticMWM(g, z, t);
	sort(z.begin(), z.end(), cmpXById);
	for (int i = 0; i < z.size(); i++)
	{
		out << z[i]._id << endl;
	}
	out.close();
	return 0;
}


void staticMWM(CBG g, vector<X> & z, vector<X> & t)
{
	int flag = verifyBegin(g);
	if (flag == -1)
	{
		ESMWM(g, z, t);
	}
	else
	{
		X selectedX = g._allX[flag];
		CBG gl, gr;
		splitCBG(g, selectedX, gl, gr);
		vector<X> zl, zr, tl, tr;
		staticMWM(gl, zl, tl);
		staticMWM(gr, zr, tr);
		sort(tl.begin(), tl.end(), cmpXinESwithEnd);
		for (int i = 0; i < tl.size(); i++)
		{
			CBG gr1;
			gr1._allX = zr;
			gr1._allX.push_back(tl[i]);
			gr1._allY = gr._allY;
			vector<MatchingEdge> tempM = ESMCM(gr1);
			if (tempM.size() > zr.size())
			{
				zr.push_back(tl[i]);
			}
			else
			{
				vector<X> rr = ESReplaceableSet(gr, zr, tl[i]);
				rr.push_back(tl[i]);
				sort(rr.begin(), rr.end(), cmpXinESwithEnd);
				X maxX = rr[rr.size() - 1];
				Y maxY = g._allY[0];
				for (int k = 0; k < g._allY.size(); k++)
				{
					if (maxY._value < g._allY[k]._value)
					{
						maxY = g._allY[k];
					}
				}
				if (maxX._end > maxY._value)
				{
					zr.push_back(tl[i]);
					vector<X>::iterator it = find(zr.begin(), zr.end(), maxX);
					zr.erase(it);
					t.push_back(maxX);
				}
				else
				{
					vector<X> tml;
					vector<X> allr = rr;
					for (int k = 0; k < rr.size(); k++)
					{
						if (rr[k]._begin < selectedX._begin)
						{
							tml.push_back(rr[k]);
						}
					}
					if (tml.size() != 0)
					{
						sort(tml.begin(), tml.end(), cmpXinEEwithBegin);
						X x1 = tml[tml.size() - 1];
						vector<X> rl = EEReplaceableSet(gl, zl, x1);
						for (int k = 0; k < rl.size(); k++)
						{
							allr.push_back(rl[k]);
						}
					}
					sort(allr.begin(), allr.end(), cmpXByWeight);
					X x2 = allr[0];
					if (x2 == tl[i])
					{
						continue;
					}
					else
					{
						vector<X>::iterator it = find(rr.begin(), rr.end(), x2);
						if (it != rr.end())
						{
							zr.push_back(tl[i]);
							it = find(zr.begin(), zr.end(), x2);
							zr.erase(it);
						}
						else
						{
							vector <X>::iterator it = find(zl.begin(), zl.end(), x2);
								vector<X> bx = ReplaceXinEE(gl, x2, zl, tml);
								sort(bx.begin(), bx.end(), cmpXinESwithEnd);
								X x3 = bx[0];
								zl.push_back(x3);
								it = find(zl.begin(), zl.end(), x2);
								zl.erase(it);
								zr.push_back(tl[i]);
								it = find(zr.begin(), zr.end(), x3);
								zr.erase(it);
						}
					}
				}
			}
		}
		z = zl;
		for (int i = 0; i < zr.size(); i++)
		{
			z.push_back(zr[i]);
		}
		for (int i = 0; i < tr.size(); i++)
		{
			t.push_back(tr[i]);
		}
	}
}

vector<MatchingEdge> ESMCM(CBG g)
{
	vector<MatchingEdge> m;
	sort(g._allX.begin(), g._allX.end(), cmpXinESwithEnd);
	int i = 0, j = 0;
	while (i != g._allX.size() && j != g._allY.size())
	{
		if (g._allX[i]._end >= g._allY[j]._value)
		{
			MatchingEdge me;
			me._x = g._allX[i];
			me._y = g._allY[j];
			m.push_back(me);
			j++;
		}
		i++;
	}
	return m;
}

vector<X> ESReplaceableSet(CBG g, vector<X> z, X x)
{
	vector<X> r;
	sort(z.begin(), z.end(), cmpXinESwithEnd);
	sort(g._allY.begin(), g._allY.end(), cmpYInc);
	if (x._end < g._allY[0]._value)
	{
		return r;
	}
	int i = 0;
	while (true)
	{
		r.push_back(z[i]);
		if (z[i]._end == g._allY[i]._value && x._end <= g._allY[i]._value)
		{
			break;
		}
		i++;
		if (i == z.size() || i == g._allY.size())
		{
			break;
		}
	}
	return r;
}

vector<X> EEReplaceableSet(CBG g, vector<X> z, X x)
{
	vector<X> r;
	sort(z.begin(), z.end(), cmpXinEEwithBegin);
	sort(g._allY.begin(), g._allY.end(), cmpYDesc);
	if (x._begin > g._allY[0]._value)
	{
		return r;
	}
	int i = 0;
	while (true)
	{
		r.push_back(z[i]);
		if (z[i]._begin == g._allY[i]._value && x._begin >= g._allY[i]._value)
		{
			break;
		}
		i++;
		if (i == z.size() || i == g._allY.size())
		{
			break;
		}
	}
	return r;
}

vector<X> ReplaceXinEE(CBG g, X x, vector<X> z, vector<X> x1)
{
	sort(z.begin(), z.end(), cmpXinEEwithBegin);
	sort(g._allY.begin(), g._allY.end(), cmpYDesc);
	vector<int> tightIndex;
	for (int i = 0; i < z.size() && i < g._allY.size(); i++)
	{
		if (z[i]._begin == g._allY[i]._value)
		{
			tightIndex.push_back(i);
		}
	}
	if (tightIndex.size() == 0 || g._allY[tightIndex[0]]._value <= x._begin)
	{
		return x1;
	}
	else 
	{
		int i = 0;
		while (g._allY[tightIndex[i]]._value>x._begin)
		{
			i++;
			if (i == tightIndex.size())
			{
				break;
			}
		}
		i--;
		int yValue = g._allY[tightIndex[i]]._value;
		vector<X> selectedX;
		for (int i = 0; i < x1.size(); i++)
		{
			if (x1[i]._begin < yValue)
			{
				selectedX.push_back(x1[i]);
			}
		}
		return selectedX;
	}
}

void ESMWM(CBG g, vector<X>& z, vector<X>& t)
{
	//initial
	vector<Y> insertedY;
	vector<Y> allY = g._allY;
	sort(allY.begin(), allY.end(), cmpRandom);
	int s = 50000;
	int e = -s;
	z.clear(); t.clear();
	vector<X> infeasible;
	t = g._allX;
	//iteration
	for (int i = 0; i < allY.size(); i++)
	{
		insertedY.push_back(allY[i]);
		if (allY[i]._value < s)
			s = allY[i]._value;
		if (allY[i]._value > e)
		{
			e = allY[i]._value;
			//reconstruct I and T
			vector<X> removedXinT;
			for (int j = 0; j < t.size(); j++)
			{
				if (t[j]._end <= e)
				{
					removedXinT.push_back(t[j]);
				}
			}
			for (int j = 0; j < removedXinT.size(); j++)
			{
				vector < X > ::iterator it = find(t.begin(), t.end(), removedXinT[j]);
				t.erase(it);
				infeasible.push_back(removedXinT[j]);
			}
		}


		sort(insertedY.begin(), insertedY.end(), cmpYInc);
		//if (z.empty() == false)
		{
			sort(z.begin(), z.end(), cmpXinESwithEnd);
		}
		int tight = -1000;
		for (int j = 0; j < z.size(); j++)
		{
			if (insertedY[j]._value == allY[i]._value)
			{
				break;
			}
				
			if (z[j]._end == insertedY[j]._value)
			{
				tight = insertedY[j]._value;
			}
		}
		//decide the X to be matched;
		vector<X> iback;
		for (int j = 0; j < infeasible.size(); j++)
		{
			if (infeasible[j]._end > tight)
			{
				iback.push_back(infeasible[j]);
			}
		}
		if (!iback.empty())
		{
			sort(iback.begin(), iback.end(), cmpXByWeight);
			X x1 = iback[iback.size() - 1];
			z.push_back(x1);
			vector<X>::iterator it = find(infeasible.begin(), infeasible.end(), x1);
			infeasible.erase(it);
		}
		else
		{
			//chose from T
			if (!t.empty())
			{
				sort(t.begin(), t.end(), cmpXinESwithEnd);
				z.push_back(t[0]);
				t.erase(t.begin());
			}
			
		}
	}

	/*z.clear(); t.clear();
	for (int i = 0; i < g._allX.size(); i++)
	{
		CBG tempg;
		tempg._allX = z;
		tempg._allX.push_back(g._allX[i]);
		tempg._allY = g._allY;
		vector<MatchingEdge> me = ESMCM(tempg);
		if (me.size() > z.size())
		{
			z.push_back(g._allX[i]);
		}
		else
		{
			tempg._allX.erase(tempg._allX.end() - 1);
			vector<X> r = ESReplaceableSet(tempg, z, g._allX[i]);
			r.push_back(g._allX[i]);
			sort(r.begin(), r.end(), cmpXinESwithEnd);
			int maxX = r[r.size() - 1]._end;
			int maxY = g._allY[0]._value;
			for (int j = 0; j < g._allY.size(); j++)
			{
				if (g._allY[j]._value > maxY)
				{
					maxY = g._allY[j]._value;
				}
			}
			if (maxX > maxY)
			{
				z.push_back(g._allX[i]);
				vector<X>::iterator it = find(z.begin(), z.end(), r[r.size() - 1]);
				z.erase(it);
				t.push_back(r[r.size() - 1]);
			}
			else
			{
				z.push_back(g._allX[i]);
				sort(r.begin(), r.end(), cmpXByWeight);
				vector<X>::iterator it = find(z.begin(), z.end(), r[0]);
				z.erase(it);
				
			}
		}
	}*/
}

void readAll(char* fileName, CBG& g)
{
	ifstream inf(fileName);
	int ynum;
	inf >> ynum;
	for (int i = 0; i < ynum; i++)
	{
		int temp;
		inf >> temp;
		Y y;
		y._value = temp;
		g._allY.push_back(y);
	}

	char command;
	while (!inf.eof())
	{
		inf >> command;
		if (command == '#')
		{
			char s[20];
			inf.get(s, 20, '\n');
			continue;
		}
		if (command == '$')
		{
			break;
		}

		switch (command)
		{
			//insert an X
		case '1':
		{
			X x;
			inf >> x._id >> x._begin >> x._end >> x._weight;
			g._allX.push_back(x);
		}break;


		case '2':
		{

		}break;
		case '3':
		{

		}break;
		case '4':
		{

		}break;
		case '5':
		{

		}break;
		case '6':
		{

		}break;
		case '7':
		{

		}break;
		case '8':
		{
		}break;
		}

	}
}

void generator(char* fileName)
{
	cout << "input range of Y" << endl;
	int range;
	cin >> range;
	int xnum;
	cout << "input the number of X" << endl;
	cin >> xnum;
	int maxw;
	cout << "input the max weight" << endl;
	cin >> maxw;

	/*int range = 80;
	int xnum = 300;
	int maxw = 500;*/

	ofstream of(fileName);

	of << range << endl;

	for (int i = 1; i <= range; i++)
	{
		of << i << " ";
	}
	//of << endl;


	vector<int> begin;
	vector<int> end;
	vector<int> weight;

	/*int seed;
	cout << "input seed of rand" << endl;
	cin >> seed;
	srand(seed);*/

	SYSTEMTIME lpsystime;
	GetLocalTime(&lpsystime);
	srand(lpsystime.wMinute*1000 + lpsystime.wMilliseconds);

	for (int i = 1; i <= xnum; i++)
	{
		int b = rand() % range + 1;
		int rest = range - b;
		int e;
		if (rest == 0)
			e = b;
		else
		{
			int diff = rand() % rest;
			e = b + diff;
		}
		begin.push_back(b);
		end.push_back(e);
		int w = rand() % maxw + 1;
		weight.push_back(w);
	}


	for (int i = 1; i <= xnum; i++)
	{
		of << endl;
		of << 1 << " ";
		of << i << " ";
		//of << 1 << " ";
		of << begin[i - 1] << " ";
		of << end[i - 1] << " ";
		of << weight[i - 1] << " ";
	}
	of << endl;
	of << '$' << endl;


	of.close();
}

