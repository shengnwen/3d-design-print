//! \file examples/Minkowski_sum_2/sum_triangle_square.cpp
// Computing the Minkowski sum of a triangle and a square.

#include <CGAL/basic.h>
#include <CGAL/minkowski_sum_2.h>
#include <fstream>
#include "bops_linear.h"
#include <ctime>

int main(int argc, char *argv[]) {
    // Construct the triangle.
    std::clock_t start;
    double duration;
    start = std::clock();
    if (argc < 2) {
        std::cout << "You must enter one file name" << std::endl;
        return 0;
    }
//    std::string filename(argv[1]);
    std::string outfilename = "./ms_" + std::string(argv[1]);
    std::ifstream infile(argv[1]);
    std::ofstream outfile;
    outfile.open(outfilename, std::ios::out);
    std::string line;
    while (std::getline(infile, line)) {
        if (line.empty()) {
            break;
        }
        std::istringstream iss(line);
        int a, b;
        Polygon_2 P, Q;
        for (int i = 0; i < 3; i++) {
            iss >> a >> b;
            P.push_back(Point_2(a, b));
//            std::cout << P << std::endl;
        }
 //       std::cout << "P = " << P << std::endl;
        for (int i = 0; i < 3; i++) {
            iss >> a >> b;
            Q.push_back(Point_2(a, b));
 //           std::cout << Q << std::endl;
        }
//        std::cout << "Q = " << Q << std::endl;
        Polygon_with_holes_2 sum = CGAL::minkowski_sum_2(P, Q);
        CGAL_assertion(sum.number_of_holes() == 0);
//        std::cout << "P (+) Q = " << sum.outer_boundary() << std::endl;
        outfile << sum.outer_boundary() << std::endl;
    }
    infile.close();
    outfile.close();
    duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
    std::cout << "run time: " << duration << std::endl; 
   return 0;
}
