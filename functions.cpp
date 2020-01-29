#include "functions.h"

void printToFile(std::string filename, const std::string &text){
  std::ofstream mfile;
  mfile.open(filename);
  for(long int i=0; i<text.length(); ++i){
    mfile<<text[i];
  }
  mfile.close();
}

static size_t writerF(void *ptr, size_t size, size_t nmemb, void *userdata)
{
 ((std::string*)userdata)->append((char*)ptr, size * nmemb);
 return size * nmemb;
}

void stockDataToFile(const std::string &tickerName, const std::string &quandl_auth_token){
  std::string mainLink="https://www.quandl.com/api/v3/datasets/WIKI";
  mainLink+="/"+tickerName;
  mainLink+=".csv?sort_order=desc&auth_token=";
  mainLink+=quandl_auth_token;
  CURL *curl;
  std::string quandlData;
  std::string fName="./quotes/";
  fName+=tickerName;
  fName+=".txt";
  curl = curl_easy_init();
  if(curl) {
    const char* linkArrayChar=mainLink.c_str();
    curl_easy_setopt(curl, CURLOPT_URL, linkArrayChar);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writerF);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &quandlData);
    curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    printToFile(fName,quandlData);
  }
}

void findExpectedReturn(std::string tickerName[], double _m[], double (*covariance_matrix)[3], const int &number_of_days){
  std::vector<std::vector<double>> adj_closing_values;
  adj_closing_values.resize(3);
  double total_return;
  double mean_values [3];
  double mean_value;
  for(int istock=0; istock<3; ++istock){
    std::string stockInfo = "./quotes/" + tickerName[istock] + ".txt";
    std::string LINE;
    std::ifstream infile;
    infile.open(stockInfo);
    getline(infile, LINE);
    for(int day=0; day<number_of_days; ++day){
      for(int column=1; column<=12; ++column){
        getline(infile, LINE, ',');
        if(column % 12 == 0){
          adj_closing_values[istock].push_back(stod(LINE));
        }
      }
      getline(infile, LINE);
    }
    infile.close();

    total_return = 0;
    mean_value = 0;
    for(int i=0; i<number_of_days-1; ++i){
      total_return += log(adj_closing_values[istock][i] / adj_closing_values[istock][i+1]);
      mean_value += adj_closing_values[istock][i];
    }
    mean_value += adj_closing_values[istock][number_of_days-1];
    mean_values [istock] = (mean_value / number_of_days);
    _m[istock] = total_return / (number_of_days-1);
  }

  findCovarianceMatrix(covariance_matrix, adj_closing_values, mean_values, number_of_days);
}

void findCovarianceMatrix(double (*covariance_matrix)[3], const std::vector<std::vector<double>> &adj_closing_values, const double mean_values[], const int &number_of_days){
  for(int row = 0; row < 3; ++row){
    for(int column = 0; column < 3; ++column){
      covariance_matrix[row][column] = 0;
      for(int i = 0; i < number_of_days; ++i){
        covariance_matrix[row][column] += (adj_closing_values[row][i] - mean_values[row]) * (adj_closing_values[column][i] - mean_values[column]);
      }
      covariance_matrix[row][column] /= (number_of_days-1);
    }
  }
}

void findInverseMatrix(double (*inverse_matrix)[3], double (*covariance_matrix)[3]){
  double determinant = 0;
  for(int i = 0; i < 3; i++){
    determinant = determinant + (covariance_matrix[0][i] * (covariance_matrix[1][(i+1)%3] * covariance_matrix[2][(i+2)%3] - covariance_matrix[1][(i+2)%3] * covariance_matrix[2][(i+1)%3]));
  }

  for(int i = 0; i < 3; i++){
  	for(int j = 0; j < 3; j++)
  		inverse_matrix[i][j] = ((covariance_matrix[(j+1)%3][(i+1)%3] * covariance_matrix[(j+2)%3][(i+2)%3]) - (covariance_matrix[(j+1)%3][(i+2)%3] * covariance_matrix[(j+2)%3][(i+1)%3]))/ determinant;
  }
}

void findMVPweights(double weights[3], double (*inverse_covariance)[3]){
  double u[3];
  for(int i = 0; i < 3; ++i){
    u[i] = 1;
  }

  //find numerator
  double numerator[3];
  for(int i = 0; i < 3; ++i){
    numerator[i] = 0;
  }
  for(int column = 0; column < 3; ++column){
    for(int row = 0; row < 3; ++row){
      numerator[column] += inverse_covariance[row][column];
    }
  }

  //find denominator
  double denominator = 0;
  for(int i = 0; i < 3; ++i){
    denominator += numerator[i];
  }

  //find MVPweights
  for(int i = 0; i < 3; ++i){
    weights[i] = numerator[i] / denominator;
  }
}

void findMarketPortfolioWeights(double weights[3], double (*inverse_covariance)[3], double m[3], double r){
  double Ru[3];
  for(int i = 0; i < 3; ++i){
    Ru[i] = r;
  }

  //find numerator
  double numerator[3];
  for(int i = 0; i < 3; ++i){
    numerator[i] = 0;
  }
  for(int column = 0; column < 3; ++column){
    for(int row = 0; row < 3; ++row){
      numerator[column] += (m[row] * inverse_covariance[row][column]);
    }
  }
  double numerator2[3];
  for(int i = 0; i < 3; ++i){
    numerator2[i] = 0;
  }
  for(int column = 0; column < 3; ++column){
    for(int row = 0; row < 3; ++row){
      numerator2[column] += (Ru[row] * inverse_covariance[row][column]);
    }
  }

  //find denominator
  double denominator = 0;
  for(int i = 0; i < 3; ++i){
    denominator += numerator[i];
  }
  double denominator2 = 0;
  for(int i = 0; i < 3; ++i){
    denominator2 += numerator2[i];
  }

  denominator -= denominator2;

  //numerator
  for(int i = 0; i < 3; ++i){
    numerator[i] -= numerator2[i];
  }

  //market portfolio weights
  for(int i = 0; i < 3; ++i){
    weights[i] = (numerator[i] / denominator);
  }
}
