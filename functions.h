#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <string>
#include <fstream>
#include <vector>
#include <fstream>
#include <cmath>
#include <curl/curl.h>
#include<iostream>

void printToFile(std::string, const std::string &);
static size_t writerF(void *ptr, size_t, size_t, void *userdata);
void stockDataToFile(const std::string &, const std::string &);
void findExpectedReturn(std::string [], double [], double (*) [3], const int &);
void findCovarianceMatrix(double (*) [3], const std::vector<std::vector<double>> &, const double [], const int &);
void findInverseMatrix(double (*) [3], double (*) [3]);
void findMVPweights(double [3], double (*) [3]);
void findMarketPortfolioWeights(double [3], double (*) [3], double [3], double);

#endif
