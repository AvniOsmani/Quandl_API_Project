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
  for(int i=0; i<3; ++i){
    std::cout<<m[i] <<"\n";
  }

  for(int i=0; i<3; ++i){
    for(int j=0; j<3; ++j){
      std::cout<<covariance_matrix[i][j] <<"\t";
    }
    std::cout<<"\n";
  }

  double inverse_covariance[3][3];
  findInverseMatrix(inverse_covariance, covariance_matrix);

  for(int i=0; i<3; ++i){
    for(int j=0; j<3; ++j){
      std::cout<<inverse_covariance[i][j] <<"\t";
    }
    std::cout<<"\n";
  }

  findMVPweights(MVPweights, inverse_covariance);
  std::cout<<"\n" <<"The weights for the minimum variance portolio are: \n";
  std::cout<<stocks[0] <<": " <<MVPweights[0] <<", " <<stocks[1] <<": " <<MVPweights[1] <<", " <<stocks[2] <<": " <<MVPweights[2] <<"\n";
  std::cout<<"The sum of the weights is: " <<MVPweights[0]+MVPweights[1]+MVPweights[2] <<"\n";

  findMarketPortfolioWeights(market_portfolio_weights, inverse_covariance, m, risk_free_rate);
  std::cout<<"\n" <<"The weights for the market portolio are: \n";
  std::cout<<stocks[0] <<": " <<market_portfolio_weights[0] <<", " <<stocks[1] <<": " <<market_portfolio_weights[1] <<", " <<stocks[2] <<": " <<market_portfolio_weights[2] <<"\n";
  std::cout<<"The sum of the weights is: " <<market_portfolio_weights[0]+market_portfolio_weights[1]+market_portfolio_weights[2] <<"\n";

  return 0;
}
