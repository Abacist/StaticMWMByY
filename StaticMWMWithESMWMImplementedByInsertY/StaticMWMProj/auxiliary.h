#pragma once
#include"CBG.h"


int verifyBegin(CBG g);
void splitCBG(CBG g, X midX, CBG& gl, CBG& gr);
bool cmpXinESwithEnd(X x1, X x2);
bool cmpYInc(Y y1, Y y2);
bool cmpXinEEwithBegin(X x1, X x2);
bool cmpYDesc(Y y1, Y y2);
bool cmpXByWeight(X x1, X x2);
bool cmpXById(X x1, X x2);
bool cmpRandom(Y y1, Y y2);