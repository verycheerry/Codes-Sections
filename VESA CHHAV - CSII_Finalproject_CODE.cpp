// ==========================================
// Author: Vesa
// Student ID: 2024584
// Course: CSCI 221
// Project: Matrix Determinant Calculator
// ==========================================

#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <type_traits>
#include <new>
#include <cstddef>

using namespace std;

// Constants
const int MAX_MATRIX_ROWCOL_SIZE = 12;
int SET_WIDTH = 8;

// ==========================================
// RANDOM MATRIX GENERATOR
// ==========================================
template <class T>
void randomSquareMatrix(T* arr, size_t len, int maxVal)
{
    srand((unsigned)time(nullptr));
    for (size_t i = 0; i < len; i++)
        arr[i] = rand() % maxVal;
}

// ==========================================
// NAMESPACE
// ==========================================
namespace MatrixDetVesa
{
    // Forward declaration
    template <class T>
    class det;

    // ==========================================
    // MATRIX CLASS
    // ==========================================
    template <class T>
    class Matrix
    {
    protected:
        T* arr;
        size_t row, col;

    public:
        Matrix(const T* inputArr, size_t r, size_t c);
        virtual ~Matrix();

        void print() const;
        void print2dAs1DArr() const;

        friend ostream& operator<<(ostream& out, const Matrix<T>& m)
        {
            m.print();
            return out;
        }
    };

    template <class T>
    Matrix<T>::Matrix(const T* inputArr, size_t r, size_t c)
    {
        static_assert(is_same<T, long>::value || is_same<T, double>::value,
                      "Type must be long or double");

        if (!inputArr)
        {
            cout << "Null input\n";
            exit(-1);
        }

        if (r < 1 || c < 1 || r > MAX_MATRIX_ROWCOL_SIZE || c > MAX_MATRIX_ROWCOL_SIZE)
        {
            cout << "Invalid size\n";
            exit(-1);
        }

        row = r;
        col = c;

        arr = new (nothrow) T[row * col];
        if (!arr)
        {
            cout << "Allocation failed\n";
            exit(-1);
        }

        for (size_t i = 0; i < row * col; i++)
            arr[i] = inputArr[i];
    }

    template <class T>
    Matrix<T>::~Matrix()
    {
        delete[] arr;
        arr = nullptr;
    }

    template <class T>
    void Matrix<T>::print() const
    {
        print2dAs1DArr();
    }

    template <class T>
    void Matrix<T>::print2dAs1DArr() const
    {
        for (size_t i = 0; i < row; i++)
        {
            cout << "| ";
            for (size_t j = 0; j < col; j++)
                cout << setw(SET_WIDTH) << arr[i * col + j];
            cout << " |" << endl;
        }
    }

    // ==========================================
    // SQ MATRIX
    // ==========================================
    template <class T>
    class sqMatrix : public Matrix<T>
    {
    private:
        Matrix<T>* sqMat;

    public:
        sqMatrix(const T* inputArr, size_t size);
        virtual ~sqMatrix();

        template <class U>
        friend class det;
    };

    template <class T>
    sqMatrix<T>::sqMatrix(const T* inputArr, size_t size)
        : Matrix<T>(inputArr, size, size)
    {
        if (size < 2 || size > MAX_MATRIX_ROWCOL_SIZE)
        {
            cout << "Invalid square size\n";
            exit(-1);
        }

        sqMat = new (nothrow) Matrix<T>(inputArr, size, size);
        if (!sqMat)
        {
            cout << "Allocation failed\n";
            exit(-1);
        }
    }

    template <class T>
    sqMatrix<T>::~sqMatrix()
    {
        delete sqMat;
        sqMat = nullptr;
    }

    // ==========================================
    // DET CLASS
    // ==========================================
    template <class T>
    class det
    {
    private:
        T determinantValue;

        T determinantP(T** matrix, int size);
        T** retArr(T** matrix, int size, int excludeCol);

    public:
        det() : determinantValue(T{}) {}

        T determinant(const sqMatrix<T>& m);
    };

    template <class T>
    T** det<T>::retArr(T** matrix, int size, int excludeCol)
    {
        T** minor = new T*[size - 1];

        for (int i = 0; i < size - 1; i++)
            minor[i] = new T[size - 1];

        for (int i = 1; i < size; i++)
        {
            int colIndex = 0;
            for (int j = 0; j < size; j++)
            {
                if (j == excludeCol) continue;
                minor[i - 1][colIndex++] = matrix[i][j];
            }
        }

        return minor;
    }

    template <class T>
    T det<T>::determinantP(T** matrix, int size)
    {
        if (size == 2)
        {
            return matrix[0][0] * matrix[1][1] -
                   matrix[0][1] * matrix[1][0];
        }

        T result = T{};

        for (int col = 0; col < size; col++)
        {
            T** minor = retArr(matrix, size, col);

            T sign = (col % 2 == 0) ? 1 : -1;

            result += sign * matrix[0][col] *
                      determinantP(minor, size - 1);

            for (int i = 0; i < size - 1; i++)
                delete[] minor[i];
            delete[] minor;
        }

        return result;
    }

    template <class T>
    T det<T>::determinant(const sqMatrix<T>& m)
    {
        int size = m.row;

        T** matrix = new T*[size];

        for (int i = 0; i < size; i++)
        {
            matrix[i] = new T[size];
            for (int j = 0; j < size; j++)
                matrix[i][j] = m.arr[i * size + j];
        }

        determinantValue = determinantP(matrix, size);

        for (int i = 0; i < size; i++)
            delete[] matrix[i];
        delete[] matrix;

        return determinantValue;
    }
}

// ==========================================
// MAIN
// ==========================================
int main()
{
    double arr[9];

    randomSquareMatrix(arr, 9, 10);

    MatrixDetVesa::sqMatrix<double> m(arr, 3);

    cout << "Matrix:\n";
    cout << m;

    MatrixDetVesa::det<double> d;
    double result = d.determinant(m);

    cout << "\nDeterminant = " << result << endl;

    return 0;
}