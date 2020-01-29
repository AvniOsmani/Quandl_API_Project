// c++ -o main main.cpp functions.cpp -lcurl -std=c++14 && ./main

#include <iostream>
#include <string>
#include "functions.h"

std::string myQuandlToken = "GG97_55eBx6TswWFZDfL";

int main(){

  double MVPweights[3];
  double market_portfolio_weights[3];

  std::cout<<"Enter the " <<3 <<" stocks you want in your portfolio: \n";
  std::string stocks[3];
  for(int i=0; i<3; ++i){
    std::cin>>stocks[i];
  }
  std::cout<<"What is the current risk free rate in decimal form? \n";
  double risk_free_rate;
  std::cin>>risk_free_rate;

  std::cout<<"Enter the number of days of stock history: \n";
  int number_of_days;
  std::cin>>number_of_days;

  for(int i=0; i<3; ++i){
    stockDataToFile(stocks[i], myQuandlToken);
  }

  double m[3];
  double covariance_matrix[3][3];
  findExpectedReturn(stocks, m, covariance_matrix, number_of_days);

  double inverse_covariance[3][3];
  findInverseMatrix(inverse_covariance, covariance_matrix);

  findMVPweights(MVPweights, inverse_covariance);
  std::cout<<"\n" <<"The weights for the minimum variance portolio are: \n";
  std::cout<<stocks[0] <<": \t" <<MVPweights[0] <<"\n" <<stocks[1] <<": \t" <<MVPweights[1] <<"\n" <<stocks[2] <<": \t" <<MVPweights[2] <<"\n\n";

  findMarketPortfolioWeights(market_portfolio_weights, inverse_covariance, m, risk_free_rate);
  std::cout<<"\n" <<"The weights for the market portolio are: \n";
  std::cout<<stocks[0] <<": \t" <<market_portfolio_weights[0] <<"\n" <<stocks[1] <<": \t" <<market_portfolio_weights[1] <<"\n" <<stocks[2] <<": \t" <<market_portfolio_weights[2] <<"\n";

  return 0;
}
