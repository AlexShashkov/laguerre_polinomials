#include "headers/Polynomial.h"
#include "headers/PolynomialGenerator.h"

#include "headers/Laguerre18m.h"
#include "headers/L18_or.h"
#include "headers/Laguerre.h"
#include "framework/framework.h"

#define number double

int main(){
    using Laguerre::Polynomial;
    Laguerre::Original<double>* solver = new Laguerre::Original<double>();
    Laguerre::ModifiedLaguerre18<double>* solver18 = new Laguerre::ModifiedLaguerre18<double>();
    try{
        std::cout << "ORIGINAL LAGUERRE:\n";
        // Polynomial<number> genRoots = Laguerre::Generator<number>::createFromRoots(0.666082, 0.666085, 0.666077);
        Polynomial<number> genRoots = Laguerre::Generator<number>::createFromRoots(-0.663831, -0.663821, -0.663818);
        std::vector<number> coeffs = {0.292521, 1.32198, 1.99147, 1};
        genRoots.print();
        int deg = genRoots.degree();
        std::vector<std::complex<double>> roots(deg);
        std::vector<int> conv(deg);

        // Set Laguerre solver for Polynomial and solve it
        genRoots.setSolver(solver);
        genRoots.solve(roots, conv, 80);
        Laguerre::printVec(roots);
        Laguerre::printVec(conv);

        std::cout << "2018 LAGUERRE MODIFICATION:\n";

        deg = genRoots.degree(); 
        roots = std::vector<std::complex<double>>(deg);
        conv = std::vector<int>(deg);

        genRoots.print();
        genRoots.setSolver(solver18);
        genRoots.solve(roots, conv, 80);
        Laguerre::printVec(roots);
        Laguerre::printVec(conv);

        std::cout << "2018 LAGUERRE BAD MODIFICATION:\n";

        deg = genRoots.degree(); 
        roots = std::vector<std::complex<double>>(deg);
        conv = std::vector<int>(deg);

        genRoots.print();
        genRoots.setSolver(solver18);
        laguerre(coeffs, deg, roots, conv, 80);
        Laguerre::printVec(roots);
        Laguerre::printVec(conv);

    }
    catch (const std::invalid_argument &exc)
    {
        std::cerr << "Bad argument: " << exc.what();
    }
    catch (const std::exception &exc)
    {
        std::cerr << exc.what();
    }

    delete solver;
    delete solver18;
    return 0;
}