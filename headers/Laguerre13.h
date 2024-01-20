#ifndef LAGUERRE13
#define LAGUERRE13

#include <vector>
#include <limits>
#include <numbers>
#include <complex>
#include <iostream>
#include <algorithm>

#include "BaseSolver.h"
#include "ExtendedFunctions.h"

namespace Laguerre{
using std::complex;
using std::vector;

template<typename T>
class Laguerre13 : public BaseSolver<T>{
private:
    //
    static constexpr T eps   = std::numeric_limits<T>::epsilon();
    

    /**
     * \brief Laguerre's method to find a root of a polynomial.
        * \param a   Polynomial object.
        * \param x   Initial guess for the root.
     */
     inline void laguer13(const std::vector<std::complex<T>>& a, std::complex<T>& x, int& converged, int itmax, int& lam){
        converged = 1;
        std::complex<T> dx, x1, b, d, f, g, h, sq, gp, gm, g2;
        T err, abx, abp, abm;
        int m = a.size() - 1;

        

        for (int iter = 1; iter <= 1; iter++) {
            b = std::complex<T>(1.0, 0.0);
            err = std::abs(b);
            d = f = std::complex<T>(0.0, 0.0);
            abx = std::abs(x);

            for (int j = m - 1; j >= 0; j--) {
                f = fma(x, f, d);
                d = fma(x, d, b);
                b = fma(x, b, a[j]);
                err = fma(err, abx, std::abs(b));
            }

            if (std::abs(b) <= err * eps)
                return;  // We are on the root.

            g = static_cast<T>(lam)* b / d; 
            h = static_cast<T>(2)*f / d;
            sq = std::sqrt(fma(-g, h, static_cast<T>(lam - 1)));
            gp = static_cast<T>(1) + sq;
            abp = std::abs(gp);
            
           
            dx =g / gp;
            x1 = x - dx;
            std::cout<<x1<<"\n";
            if (x == x1)
                return;  // Converged.
            x = x1;
            
        }
    }


public:
    Laguerre13(){
        // 
    }

    /**
     * \brief Find the roots of a polynomial using Laguerre's method.
         * \param poly Polynomial object.
         * \param roots Vector to store the roots.
         * \param conv Vector to store convergence status of each root.
         * \param itmax Maximum number of iterations.
     */
     void operator()(std::vector<T>& poly, std::vector<std::complex<T>>& roots, std::vector<int>& conv, int itmax=80) override{
        std::complex<T> x, _b, _c;
        int m = poly.size() - 1;
        std::vector<std::complex<T>> ad(m + 1);
        std::vector<std::complex<T>> ad_v;
        int lambda;
        int m1;

        // Copy of coefficients for successive deflation.
        for (int i = 0; i <= m; i++)
            ad[i] = poly[i];
        
        auto diff = Laguerre::diff(poly, 1);
        std::vector<T> quotient, remainder;
        Laguerre::divide(poly, diff, quotient, remainder);

        bool zeros = std::all_of(remainder.begin(), remainder.end(), [](int i) { return i==0; });
        if (zeros)
        {
            lambda = 2*m;
            m1 = 1;
        }
        else
        {
            std::vector<T> quotient1, remainder1;
            Laguerre::divide(diff, remainder, quotient1, remainder1);
            zeros = std::all_of(remainder1.begin(), remainder1.end(), [](int i) { return i==0; });
            if (zeros){
                m1 = m - remainder.size()+1;
                lambda = 2*m/m1;
            } 
        }

 

        int j = m - 1;
        for (int i = 0; i < m1; ++i){
            x = std::complex<T>(0.0, 0.0);

            // Start at zero to favor convergence to the smallest remaining root.
            ad_v = std::vector<std::complex<T>>(ad.cbegin(), ad.cbegin() + j + 2);
            laguer13(ad_v, x, conv[j], itmax, lambda);
            if (std::abs(imag(x)) <= std::abs(real(x)) * eps)
                x.imag(static_cast<T>(0));

            for (int k = 0; k < lambda/2; ++k){
                roots[j] = x;
                _b = ad[j + 1];
                for (int jj = j; jj >= 0; jj--) {
                    _c = ad[jj];
                    ad[jj] = _b;
                    _b = fma(x, _b, _c);
                }
                --j;
                
            }     
        }
 }

};

};
#endif