/*
//  Copyright (c) 2018, James Kneller and Sherwood Richers
//
//  This file is part of IsotropicSQA.
//
//  IsotropicSQA is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  IsotropicSQA is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with IsotropicSQA.  If not, see <http://www.gnu.org/licenses/>.
//
*/

#ifndef MATRIX_H
#define MATRIX_H

#include <cassert>
#include <iostream>
using std::ostream;

template<typename T, unsigned a, unsigned b>
  class MATRIX{
    T m[a][b];

 public:
    const T* operator[](const size_t i) const{
      return m[i];
    }
    T* operator[](const size_t i){
      return m[i];
    }

    MATRIX<T,a,b>(){
      for(unsigned i=0; i<a; i++)
	for(unsigned j=0; j<b; j++)
	  m[i][j] = 0;
    }
    
    template<typename T1, unsigned c>
    MATRIX<T,a,b> operator*(const MATRIX<T1,b,c>& right) const{
      MATRIX<T,a,c> result;

      for(unsigned i=0; i<a; i++)
	for(unsigned j=0; j<c; j++){
	  for(unsigned k=0; k<b; k++){
	    result[i][j] += m[i][k]*right[k][j];
	  }
	}
      return result;
    }

    MATRIX<T,a,b> operator*(const T right) const{
      MATRIX<T,a,b> result;

      for(unsigned i=0; i<a; i++)
	for(unsigned j=0; j<b; j++)
	  result[i][j] = m[i][j]*right;
      return result;
    }

    MATRIX<T,a,b> operator/(const T right) const{
      MATRIX<T,a,b> result;

      for(unsigned i=0; i<a; i++)
	for(unsigned j=0; j<b; j++)
	  result[i][j] = m[i][j]/right;
      return result;
    }

    template<typename T1>
    MATRIX<T,a,b> operator+(const MATRIX<T1,a,b>& right) const{
      MATRIX<T,a,b> result;
      for(unsigned i=0; i<a; i++)
	for(unsigned j=0; j<b; j++)
	  result[i][j] = m[i][j] + right[i][j];
      return result;
    }
    template<typename T1>
    MATRIX<T,a,b> operator+=(const MATRIX<T1,a,b>& right){
      *this = *this + right;
      return *this;
    }
    template<typename T1>
    MATRIX<T,a,b> operator-=(const MATRIX<T1,a,b>& right){
      *this = *this - right;
      return *this;
    }
    MATRIX<T,a,b> operator+(const T right) const{
      assert(a==b);
      MATRIX<T,a,b> result = *this;
      for(unsigned i=0; i<a; i++)
	result[i][i] += right;
      return result;
    }
    
    template<typename T1>
    MATRIX<T,a,b> operator-(const MATRIX<T1,a,b>& right) const{
      MATRIX<T,a,b> result;
      for(unsigned i=0; i<a; i++)
	for(unsigned j=0; j<b; j++)
	  result[i][j] = m[i][j] - right[i][j];
      return result;
    }

    MATRIX<T,a,b> operator-() const{
      MATRIX<T,a,b> result;
      for(unsigned i=0; i<a; i++)
	for(unsigned j=0; j<b; j++)
	  result[i][j] = -m[i][j];
      return result;
    }

    template<typename T1>
    MATRIX<T,a,b> operator*=(const T1 input){
      for(unsigned i=0; i<a; i++)
	for(unsigned j=0; j<b; j++)
	  m[i][j] *= input;
      return *this;
    }

};

template<typename T, typename T2, unsigned a>
  MATRIX<T,a,a> operator+(const T2 left, const MATRIX<T,a,a> right){
  return right + left;
}

template<typename T, typename T2, unsigned a>
  MATRIX<T,a,a> operator-(const T2 left, const MATRIX<T,a,a> right){
  return (-right) + left;
}

template<typename T, unsigned a, unsigned b>
ostream& operator<<(ostream& os, const MATRIX<T,a,b>& m){
  for(unsigned i=0; i<a; i++){
    os << "[";
    for(unsigned j=0; j<b; j++){
      os << m[i][j] << (j==b-1 ? "]" : ",");
    }
    os << endl;
  }
  return os;  
}

template<typename T, unsigned a, unsigned b>
MATRIX<T,a,b> Adjoint(const MATRIX<T,a,b>& input){
  assert(a==b);
  MATRIX<T,a,b> result;
  for(unsigned i=0; i<a; i++)
    for(unsigned j=0; j<b; j++)
      result[i][j] = conj(input[j][i]);
  return result;
}

template<typename T, unsigned a, unsigned b>
MATRIX<T,a,b> Conjugate(const MATRIX<T,a,b>& input){
  MATRIX<T,a,b> result;
  for(unsigned i=0; i<a; i++)
    for(unsigned j=0; j<b; j++)
      result[i][j] = conj(input[i][j]);
  return result;
}

template<typename T, unsigned a, unsigned b>
T Trace(const MATRIX<T,a,b>& input){
  assert(a==b);
  T result = 0;
  for(unsigned i=0; i<a; i++)
    result += input[i][i];
  return result;
}

double IsospinL(const MATRIX<complex<double>,2,2>& input){
  double x = real(input[0][1]);
  double y = imag(input[0][1]);
  double z = abs(input[0][0]-input[1][1])/2.;
  return sqrt(x*x + y*y + z*z);
}

#endif
