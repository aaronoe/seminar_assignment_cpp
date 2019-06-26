//
// Created by aaron on 21.06.19.
//

#ifndef SEMINAR_ASSIGNMENT_TOPTRADINGCYCLE_H
#define SEMINAR_ASSIGNMENT_TOPTRADINGCYCLE_H

#include "../model/graph.h"
#include "vector"

using namespace std;

set<long> getVertexInCycle(const graph &market);

vector<pair<long, long>> getTopTradingCycle(const vector<models::student> &students, long seminar_count, const vector<long> &initial_ownership);

#endif //SEMINAR_ASSIGNMENT_TOPTRADINGCYCLE_H
