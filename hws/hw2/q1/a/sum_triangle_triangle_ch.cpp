//! \file examples/Minkowski_sum_2/sum_triangle_square.cpp
// Computing the Minkowski sum of a triangle and a square.

#include <CGAL/basic.h>
#include <CGAL/minkowski_sum_2.h>
#include <fstream>
#include <CGAL/ch_graham_andrew.h>
#include "bops_linear.h"
#include <ctime>

int main(int argc, char *argv[]) {
    std::clock_t start;
    double duration;
    start = std::clock();
    if (argc < 2) {
        std::cout << "You must enter one file name" << std::endl;
        return 0;
    }
    std::string outfilename = "./ms_" + std::string(argv[1]);
    std::ifstream infile(argv[1]);
    std::ofstream outfile;
    outfile.open(outfilename, std::ios::out);
    std::ostream_iterator< Point_2 >  out( outfile , " ");
    std::string line;
    while (std::getline(infile, line)) {
        if (line.empty()) {
            break;
        }
        std::istringstream iss(line);
        int points[6][2];
        Polygon_2 res;
        for (int i = 0; i < 3; i++) {
            iss >> points[i][0] >> points[i][1];
        }
        for (int i = 0; i < 3; i++) {
            iss >> points[i + 3][0] >> points[i + 3][1];
        }
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                res.push_back(Point_2(points[i][0] + points[j + 3][0],
                                      points[i][1] + points[j + 3][1]));
            }
        }
        CGAL::ch_graham_andrew( res.vertices_begin(), res.vertices_end(), out );
        outfile << std::endl;
    }
    infile.close();
    outfile.close();
    duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
    std::cout << "run time: " << duration << std::endl;
    return 0;
}
