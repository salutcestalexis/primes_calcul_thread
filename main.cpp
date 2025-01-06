#include <iostream>
#include <chrono>
#include <thread>
#include <vector>

static int NB_THREAD = 8;       //max de mon cpu
static int PRIMES_TO = 1000000;

int** fragmentPacket(int c, int n);
void printMatrix(int** matrix, int rows, int cols);
void printPrimes(int s, int e);

int main()
{
  int** arr;
  std::vector<std::thread> threads;

  arr = fragmentPacket(PRIMES_TO, NB_THREAD);
  //printMatrix(arr, NB_THREAD, 2);

  auto start = std::chrono::high_resolution_clock::now();   //start the chrono

  for(int i = 0; i < NB_THREAD; i++)
  {
    int row = arr[i][0];
    int col = arr[i][1];
    threads.emplace_back(printPrimes, row, col);
  }

  for(auto& t : threads)
    t.join();

  auto end = std::chrono::high_resolution_clock::now(); //stop the chrono
  auto duration = std::chrono::duration_cast<std::chrono::seconds>(end-start);  //calcul difference

  std::cout << "Running time : " << duration.count() << " s" << std::endl;

  return 0;
}

int** fragmentPacket(int c, int n)    //Cuts a C number into N equal-sized packets
{
  int** fragment = new int*[n];

  int s = c / n;  //size of fragment
  int r = c % n;  //rest of division

  for(int i = 0; i < n; i++)
  {
    fragment[i] = new int[2];     //For each row, I initialize two columns
    for(int j = 0; j < 2; j++)
    {
      if(j == 0)
        fragment[i][j] = i * s;

      if(j == 1)
      {
        if(i == (n-1))
          fragment[i][j] = (i + j) * s + r;
        else
          fragment[i][j] = (i + j) * s - 1; //Adding the modulo for the last one
      }
    }
  }

  return fragment;
}

void printMatrix(int** matrix, int rows, int cols) 
{
    for (int i = 0; i < rows; ++i) 
    {
        for (int j = 0; j < cols; ++j) 
        {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

void printPrimes(int s, int e)    //To optimize
{
  int j;

  for(int i = s; i <= e; i++)
  {
    for(j = 2; j < i; j++)
      if(i % j == 0)
        break;

    if(j == i)
      std::cout << i << std::endl;

  }
}