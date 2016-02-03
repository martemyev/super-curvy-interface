#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <sstream>

template <typename T>
inline std::string d2s(T data)
{
  std::ostringstream o;
  if (!(o << data))
    throw std::runtime_error("Bad conversion of data to string!");
  return o.str();
}

void read_binary(const std::string &filename, int n_cols, int *n_rows,
                 float **values);
void write_binary(const std::string &filename, const float *values, int n_cols,
                  int n_rows);

std::string file_stem(const std::string &path);

void make_super_curvy(float *values, int n_cols, int n_rows, int width,
                      int jump);



int main(int argc, char **argv)
{
  if (argc != 5) {
    std::cout << "\nUsage:\n" << argv[0] << " file.bin ncols width jump\n"
              << "file.bin contains media properties with curvy interface\n"
              << "ncols is number of columns in binary file\n"
              << "width is size of part of interface for jump (in cells)\n"
              << "jump is heigth of jump of interface (in cells)\n\n";
    return 1;
  }

  try {
  const std::string filename = std::string(argv[1]);
  const int n_cols = atoi(argv[2]);
  const int width  = atoi(argv[3]);
  const int jump   = atoi(argv[4]);

  float *values = NULL;
  int n_rows;
  read_binary(filename, n_cols, &n_rows, &values);

  make_super_curvy(values, n_cols, n_rows, width, jump);

  write_binary(file_stem(filename) + "_curvy.bin", values, n_cols, n_rows);

  delete[] values;
  }
  catch (const std::exception& e) {
    std::cout << "\n" << e.what() << "\n" << std::endl;
    return 2;
  }
  catch (...) {
    std::cout << "\n\n\nUnknown exception!\n\n" << std::endl;
    return 3;
  }

  return 0;
}



void read_binary(const std::string &filename, int n_cols, int *n_rows,
                 float **values)
{
  std::ifstream in(filename.c_str(), std::ios::binary);
  if (!in)
    throw std::runtime_error("File '" + filename + "' can't be opened.");

  in.seekg(0, in.end); // jump to the end of the file
  int length = in.tellg(); // total length of the file in bytes
  int n_values = length / sizeof(float); // number of values
  *n_rows = n_values / n_cols; // number of rows of the matrix in the binary file

  if (length % sizeof(float) != 0)
    throw std::runtime_error("The number of bytes in the file '" + filename +
                             "' is not divisible by " + d2s(sizeof(float)));

  if (n_values % n_cols != 0)
    throw std::runtime_error("The number of values in the file '" + filename +
                             "' is not divisible by the number of columns " +
                             d2s(n_cols));

  in.seekg(0, in.beg); // jump to the beginning of the file

  *values = new float[n_values];
  in.read(reinterpret_cast<char*>(*values), length); // read all at once

  if(length != static_cast<int>(in.gcount()))
    throw std::runtime_error("The number of successfully read bytes (" +
                             d2s(in.gcount()) + ") is different from the "
                             "expected one (" + d2s(length) + ")");
}



void write_binary(const std::string &filename, const float *values, int n_cols,
                  int n_rows)
{
  std::ofstream out(filename.c_str(), std::ios::binary);
  if (!out)
    throw std::runtime_error("File '" + filename + "' can't be opened.");

  out.write((char*)values, n_cols*n_rows*sizeof(float));
}



std::string file_name(const std::string &path)
{
  if (path == "") return path;
  // extract a filename
  const std::string fname = path.substr(path.find_last_of('/') + 1);
  return fname;
}

std::string file_stem(const std::string &path)
{
  if (path == "") return path;
  // get a file name from the path
  const std::string fname = file_name(path);
  // extract a stem and return it
  return fname.substr(0, fname.find_last_of('.'));
}



void make_super_curvy(float *values, int n_cols, int n_rows, int width,
                      int jump)
{
  const double tol = 1.;
  int sign = fabs(jump) / jump;
  for (int col = 0; col < n_cols; ++col) {
    bool column_done = false;
    for (int row = 0; row < n_rows - jump && !column_done; ++row) {
      if (fabs(values[row*n_cols + col] - values[(row+1)*n_cols + col]) > tol) {
        if (col % width == 0)
          sign *= -1;
        for (int j = 0; j < jump; ++j) {
          values[(row+sign*j)*n_cols + col] = values[(row-sign)*n_cols + col];
        }
        column_done = true;
      }
    }
  }
}