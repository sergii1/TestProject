#include <iostream>
#include "time.h"
#include "thread"

using namespace std;

class Matrix {
public:
    int **matrix;
    int rows;
    int cols;

    Matrix(int p_rows = 1, int p_cols = 1) {
        rows = p_rows;
        cols = p_cols;
        matrix = new int *[rows];
        for (int i = 0; i < cols; i++) {
            matrix[i] = new int[cols];
        }
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                matrix[i][j] = rand() % 10;
            }
        }
    }

    void print() const {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                cout << matrix[i][j] << "  ";
            }
            cout << endl;
        }
        cout << endl;
    }

    ~Matrix() {
        for (int i = 0; i < cols; i++) {
            delete[] matrix[i];
        }
        delete[] matrix;
    }
};

class Task2 : public Matrix {
public:
    Task2(int p_rows = 1, int p_cols = 1) : Matrix(p_rows, p_cols) {}

     void calc_sub_sum(const int left, const int top, const int right, const int bottom) {
        cout << "sub sum";
        int res = 0;
        for (int i = top; i <= bottom; i++) {
            for (int j = left; j <= right; j++) {
                cout << matrix[i][j] << "  ";
                res += matrix[i][j];
            }
            cout << endl;
        }
        cout << "res" << res;
    }
};

class Task3 : public Task2 {
public:
    Task3(int p_rows = 1, int p_cols = 1) : Task2(p_rows, p_cols) {}

    void calc_sub_sum_thread(const int left, const int top, const int right, const int bottom) {
        m_thread = thread(&Task3::calc_sub_sum, this, left, top, right, bottom);
        cout << endl << "id" << endl << m_thread.get_id();
        m_thread.join();
    }

private:
    thread m_thread;

};

class Task4: public Task2{
public:
    Task4(int p_rows = 1, int p_cols = 1, int p_sub_matrix_amount = 1) : Task2(p_rows, p_cols) {
        sub_matrix_amount = p_sub_matrix_amount;
        m_threads = new thread[sub_matrix_amount];
    }

    void calc_sub_sum(const int left, const int top, const int right, const int bottom, int* res ) {
        *res = 0;
        for (int i = top; i <= bottom; i++) {
            for (int j = left; j <= right; j++) {
                *res += matrix[i][j];
            }
        }
    }

    void calc_sub_sum_thread(const int* const coordinates){
        int **results = new int* [sub_matrix_amount];
        for(int i=0; i<sub_matrix_amount; i++){
            results[i] = new int;
            m_threads[i] = thread(&Task4::calc_sub_sum, this, coordinates[i], coordinates[i+1], coordinates[i+2], coordinates[i+3], results[i]);
            cout << endl << "id "  << m_threads[i].get_id()<<endl;
        }
        for(int i=0; i<sub_matrix_amount; i++){
            m_threads->join();
        }
        cout<<endl<<endl<<endl<<"RESULTS ";
        for(int i=0; i<sub_matrix_amount; i++){
            cout<<*(results[i])<<"  ";
        }
    }

    ~Task4(){
        delete m_threads;
    }


private:
    int sub_matrix_amount;
    thread* m_threads;
};

int main() {
    srand(time(NULL));
    Task4 a(2, 3, 1);
    a.print();
    int coords[4] =  {0,0,2,1};
    a.calc_sub_sum_thread(coords);
    return 0;
}
