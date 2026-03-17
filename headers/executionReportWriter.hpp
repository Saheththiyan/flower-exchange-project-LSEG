#pragma once

#include <bits/stdc++.h>
#include "order.hpp"
using namespace std;

class ExecutionReportWriter {
private:
    string filePath;
    static string statustoString(ExecStatus status);
    static int sidetoInt(Side side);

public:
    explicit ExecutionReportWriter(const string& filePath);
    void writeHeader();
    void writeRows(const vector<ExecutionReport> &rows);
};

