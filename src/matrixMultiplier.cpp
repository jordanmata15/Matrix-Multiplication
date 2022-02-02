#include


int main(){
    int m = 5;
    int n = 4;
    matrixGenerator g = matrixGenerator(m, n);
    int** a = g.getMatrix();
    for (int i=0; i<m; i++){
      for (int j=0; j<n; j++){
        std::cout << a[i][j];
      }
      std::cout << std::endl;
    }
}
