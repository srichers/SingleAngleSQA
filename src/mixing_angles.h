#ifndef MIXINGANGLES_H
#define MIXINGANGLES_H


//=======//
// theta //
//=======//

double theta(const MATRIX<complex<double>,NF,NF>& Hf,
	     const array<double,NF> k){
  return atan2( sqrt(-(real(Hf[mu][mu])-k[1])),sqrt(real(Hf[e][e])-k[0]) );}

double thetabar(const MATRIX<complex<double>,NF,NF>& Hf,
		const array<double,NF> k){
  return atan2( sqrt(-(real(Hf[mu][mu])-k[1])),sqrt(real(Hf[e][e])-k[0]) );}

//==========//
// costheta //
//==========//

double cos2theta(const MATRIX<complex<double>,NF,NF>& Hf,
		 const array<double,NF> k,
		 const array<double,NF-1> dk){
  return (k[0]-real(Hf[mu][mu]))/dk[0];}

double cos2thetabar(const MATRIX<complex<double>,NF,NF>& Hf,
		    const array<double,NF> k,
		    const array<double,NF-1> dk){
  return (k[0]-real(Hf[mu][mu]))/dk[0];}

double costheta(const MATRIX<complex<double>,NF,NF>& Hf,
		const array<double,NF> k,
		const array<double,NF-1> dk){
  return sqrt(cos2theta(Hf,k,dk));}

double costhetabar(const MATRIX<complex<double>,NF,NF>& Hf,
		   const array<double,NF> k,
		   const array<double,NF-1> dk){
  return sqrt(cos2thetabar(Hf,k,dk));}

//====================//
// asymptoticcostheta //
//====================//

double asymptoticcos2theta(double x,int s){
  return ( -(s+a1) +s*g(x) )/(a2-a1);}

double asymptoticcos2thetabar(double x,int s){
  return ( -(s+a1) +s*g(x) )/(a2-a1);}

double asymptoticcostheta(double x,int s){
  return sqrt(asymptoticcos2theta(x,s));}

double asymptoticcosthetabar(double x,int s){
  return sqrt(asymptoticcos2thetabar(x,s));}

//==========//
// sintheta //
//==========//

double sin2theta(const MATRIX<complex<double>,NF,NF>& Hf,
		 const array<double,NF> k,
		 const array<double,NF-1> dk){
  return (k[0]-real(Hf[e][e]))/dk[0];}

double sin2thetabar(const MATRIX<complex<double>,NF,NF>& Hf,
		    const array<double,NF> k,
		    const array<double,NF-1> dk){
  return (k[0]-real(Hf[e][e]))/dk[0];}

double sintheta(const MATRIX<complex<double>,NF,NF>& Hf,
		const array<double,NF> k,
		const array<double,NF-1> dk){
  return sqrt(sin2theta(Hf,k,dk));}

double sinthetabar(const MATRIX<complex<double>,NF,NF>& Hf,
		   const array<double,NF> k,
		   const array<double,NF-1> dk){
  return sqrt(sin2thetabar(Hf,k,dk));}

//====================//
// asymptoticsintheta //
//====================//

double asymptoticsin2theta(double x,int s){
  return ( (s+a2) -s*g(x) )/(a2-a1);}

double asymptoticsin2thetabar(double x,int s){
  return ( (s+a2) -s*g(x) )/(a2-a1);}

double asymptoticsintheta(double x,int s){
  return sqrt(asymptoticsin2theta(x,s));}

double asymptoticsinthetabar(double x,int s){
  return sqrt(asymptoticsin2thetabar(x,s));}

//======//
// beta //
//======//

double beta(const MATRIX<complex<double>,NF,NF>& Hf,
	    const array<double,NF-1> dk){
  return atan2(-imag(Hf[mu][e])/dk[0],-real(Hf[mu][e])/dk[0]);}

double betabar(const MATRIX<complex<double>,NF,NF>& Hf,
	       const array<double,NF-1> dk){
  return atan2(-imag(Hf[mu][e])/dk[0],-real(Hf[mu][e])/dk[0]);}

//=======//
// alpha //
//=======//

double alpha1(void){ return alphaV[0];}
double alpha2(void){ return alphaV[1];}

//===//
// U //
//===//

MATRIX<complex<double>,NF,NF>
  U(const array<double,NF-1> dk, 
    const array<MATRIX<complex<double>,NF,NF>,NF> &C,
    const array<array<double,NF>,NF>& A){

  MATRIX<complex<double>,NF,NF> u;  
  double d;
  array<double,NF> r2;
  
  for(int j=0;j<=NF-1;j++){
    if(j==0) d = -dk[0]; // first column 
    else     d =  dk[0]; // second column
    
    r2[e]  = real(C[j][e ][e ])*d;
    r2[mu] = real(C[j][mu][mu])*d;
    
    if(r2[e]>=r2[mu]){
      u[e ][j] = A[j][e] * C[j][e][e ] / sqrt(r2[e]);
      u[mu][j] = A[j][e] * C[j][e][mu] / sqrt(r2[e]);
    }
    if(r2[mu]>=r2[e]){
      u[e ][j] = A[j][mu] * C[j][mu][e ] / sqrt(r2[mu]);
      u[mu][j] = A[j][mu] * C[j][mu][mu] / sqrt(r2[mu]);
    }
  }
  
  return u;
}

MATRIX<complex<double>,NF,NF>
  U(const MATRIX<complex<double>,NF,NF>& Hf,
    const array<double,NF> k,
    const array<double,NF-1> dk){
  
  //if(4.*norm(Hf[e][mu])/norm(Hf[e][e]-Hf[mu][mu])<0.0745){ return asymptoticU(Hf,dkk);}
  MATRIX<complex<double>,NF,NF> u;           
  double ctheta=costheta(Hf,k,dk);
  double stheta=sintheta(Hf,k,dk);
  double b=beta(Hf,dk);
  
  u[e ][0]= ctheta*exp(    - I*alphaV[0]);
  u[e ][1]= stheta*exp(    - I*alphaV[1]);
  u[mu][0]=-stheta*exp(I*b - I*alphaV[0]);
  u[mu][1]= ctheta*exp(I*b - I*alphaV[1]);
  
  return u;
}

MATRIX<complex<double>,NF,NF>
  asymptoticU(const MATRIX<complex<double>,NF,NF>& Hf, 
	      const array<double,NF-1> dk){

  MATRIX<complex<double>,NF,NF> u;
  double x = 4. * norm(Hf[e][mu]) / norm(Hf[e][e] - Hf[mu][mu]);
  int s = static_cast<int>(Sign(real(Hf[e][e] - Hf[mu][mu])));
  double ctheta = asymptoticcostheta(x,s);
  double stheta = asymptoticsintheta(x,s);
  double b = beta(Hf,dk);
  
  u[e ][0] = ctheta * exp(    - I*alphaV[0]);
  u[e ][1] = stheta * exp(    - I*alphaV[1]);
  u[mu][0] =-stheta * exp(I*b - I*alphaV[0]);
  u[mu][1] = ctheta * exp(I*b - I*alphaV[1]);
  
  return u;
}


//=======//
// Ustar //
//=======//

MATRIX<complex<double>,NF,NF> 
  Ustar(const MATRIX<complex<double>,NF,NF>& Hf,
	const array<double,NF> k,
	const array<double,NF-1> dk){
  
  MATRIX<complex<double>,NF,NF> ustar;         
  double ctheta = costhetabar(Hf,k,dk);
  double stheta = sinthetabar(Hf,k,dk);
  double b = betabar(Hf,dk);
  
  ustar[e ][0] = ctheta * exp(       I*alphaV[0]);
  ustar[e ][1] = stheta * exp(       I*alphaV[1]);
  ustar[mu][0] =-stheta * exp(-I*b + I*alphaV[0]);
  ustar[mu][1] = ctheta * exp(-I*b + I*alphaV[1]);
  
  return ustar;
}

MATRIX<complex<double>,NF,NF>
  asymptoticUstar(const MATRIX<complex<double>,NF,NF>& Hf,
		  const array<double,NF-1> dk){

  MATRIX<complex<double>,NF,NF> ustar;
  double x = 4.*norm(Hf[e][mu]) / norm(Hf[e][e] - Hf[mu][mu]);
  int s = static_cast<int>(Sign(real(Hf[e][e] - Hf[mu][mu])));
  double ctheta = asymptoticcosthetabar(x,s);
  double stheta = asymptoticsinthetabar(x,s);
  double b = betabar(Hf,dk);
  
  ustar[e ][0]= ctheta*exp(       I*alphaV[0]);
  ustar[e ][1]= stheta*exp(       I*alphaV[1]);
  ustar[mu][0]=-stheta*exp(-I*b + I*alphaV[0]);
  ustar[mu][1]= ctheta*exp(-I*b + I*alphaV[1]);
  
  return ustar;
}

//===//
// C //
//===//
template<flavour a,flavour b> complex<double> C(const MATRIX<complex<double>,NF,NF>& H,double k);
template<> complex<double> C<e,e>(const MATRIX<complex<double>,NF,NF>& H,double k){
  return H[mu][mu]-k;}
template<> complex<double> C<e,mu>(const MATRIX<complex<double>,NF,NF>& H,double k){
  return -H[mu][e];}
template<> complex<double> C<mu,e>(const MATRIX<complex<double>,NF,NF>& H,double k){
  return -H[e][mu];}
template<> complex<double> C<mu,mu>(const MATRIX<complex<double>,NF,NF>& H,double k){
  return H[e][e]-k;}
  
//======//
// dCdr //
//======//
template<flavour a,flavour b> complex<double> dCdr(const MATRIX<complex<double>,NF,NF>& dHdr,double dkdr);
template<> complex<double> dCdr<e,e>(const MATRIX<complex<double>,NF,NF>& dHdr,double dkdr){
  return dHdr[mu][mu]-dkdr;}
template<> complex<double> dCdr<e,mu>(const MATRIX<complex<double>,NF,NF>& dHdr,double dkdr){
  return -dHdr[mu][e];}
template<> complex<double> dCdr<mu,e>(const MATRIX<complex<double>,NF,NF>& dHdr,double dkdr){
  return -dHdr[e][mu];}
template<> complex<double> dCdr<mu,mu>(const MATRIX<complex<double>,NF,NF>& dHdr,double dkdr){
  return dHdr[e][e]-dkdr;}

//=====================//
// MixingMatrixFactors //
//=====================//
array<array<double,NF>,NF>
MixingMatrixFactors(const array<MATRIX<complex<double>,NF,NF>,NF>& C,
		    const array<MATRIX<complex<double>,NF,NF>,NF>& C0,
		    const array<array<double,NF>,NF> A0){

  array<array<double,NF>,NF> A = A0;  
  for(int j=0;j<=NF-1;j++){
    if(real(C[j][e][mu]*C0[j][e][mu])<0.) A[j][e ] *= -1.;
    if(real(C[j][mu][e]*C0[j][mu][e])<0.) A[j][mu] *= -1.;
  }
  
  return A;
}

//==================//
// CofactorMatrices //
//==================//
array<MATRIX<complex<double>,NF,NF>,NF>
  CofactorMatrices(const MATRIX<complex<double>,NF,NF>& H,
		   const array<double,NF> k){
  
  array<MATRIX<complex<double>,NF,NF>,NF> CC;
  for(int j=0;j<=NF-1;j++){
    CC[j][e ][e ] = (H[mu][mu]-k[j]);
    CC[j][e ][mu] = -H[mu][e];  
    CC[j][mu][e ] = conj(CC[j][e][mu]);   
    CC[j][mu][mu] = (H[e][e]-k[j]);
  }
  
  return CC;
}

//=============================//
// CofactorMatricesDerivatives //
//=============================//
array<MATRIX<complex<double>,NF,NF>,NF>
  CofactorMatricesDerivatives(const MATRIX<complex<double>,NF,NF>& dHdr,
			      const array<double,NF> dkdr){
  
  array<MATRIX<complex<double>,NF,NF>,NF> dCCdr;
  for(int j=0;j<=NF-1;j++){
    dCCdr[j][e ][e ] = (dHdr[mu][mu]-dkdr[j]);
    dCCdr[j][e ][mu] = -dHdr[mu][e];  
    dCCdr[j][mu][e ] = conj(dCCdr[j][e][mu]);   
    dCCdr[j][mu][mu] = (dHdr[e][e]-dkdr[j]);
  }
  
  return dCCdr;
}

//=============//
// Evaluate_UV //
//=============//
array<MATRIX<complex<double>,NF,NF>,NM> Evaluate_UV(void){
  array<MATRIX<complex<double>,NF,NF>,NM> UV;
  
  UV[matter][0][0] = c12V * exp(-I*alphaV[0]                  );
  UV[matter][0][1] = s12V * exp(-I*alphaV[1]                  );  
  UV[matter][1][0] =-s12V * exp( I* betaV[0])*exp(-I*alphaV[0]);
  UV[matter][1][1] = c12V * exp( I* betaV[0])*exp(-I*alphaV[1]);
  
  UV[antimatter]=Conjugate(UV[matter]);

  return UV;
}

//=============//
// Evaluate_CV //
//=============//
array<array<MATRIX<complex<double>,NF,NF>,NF>,NE>
  Evaluate_CV(const array<array<double,NF>,NE>& kV,
	      const array<array<MATRIX<complex<double>,NF,NF>,NE>,NM>& VfVac){

  array<array<MATRIX<complex<double>,NF,NF>,NF>,NE> CV;
  
  for(int i=0;i<=NE-1;i++){
    CV[i][0][e][mu]=C<e,mu>(VfVac[matter][i],kV[i][0]); CV[i][1][e][mu]=C<e,mu>(VfVac[matter][i],kV[i][1]);
    CV[i][0][mu][e]=C<mu,e>(VfVac[matter][i],kV[i][0]); CV[i][1][mu][e]=C<mu,e>(VfVac[matter][i],kV[i][1]);
  }

  return CV;
}

//=============//
// Evaluate_AV //
//=============//
array<array<array<double,NF>,NF>,NE>
  Evaluate_AV(const array<array<double,NF>,NE>& kV,
	      const array<array<MATRIX<complex<double>,NF,NF>,NE>,NM>& VfVac,
	      const array<MATRIX<complex<double>,NF,NF>,NM>& UV){
  
  array<array<array<double,NF>,NF>,NE> AV;

  double Delta;
  for(int i=0;i<=NE-1;i++){
    for(int j=0;j<=NF-1;j++){
      if(j==0) Delta=(kV[i][1]-kV[i][0]);
      else     Delta=(kV[i][0]-kV[i][1]);
      
      double re2=Delta*real(C<e,e>(VfVac[matter][i],kV[i][j]));
      double rmu2=Delta*real(C<mu,mu>(VfVac[matter][i],kV[i][j]));
      
      if(norm(UV[matter][e][j])>norm(UV[matter][mu][j]) ){
	AV[i][j][e ]=real( UV[matter][e][j] * sqrt(re2 ) / C<e ,e>(VfVac[matter][i],kV[i][j]) );
	AV[i][j][mu]=real( UV[matter][e][j] * sqrt(rmu2) / C<mu,e>(VfVac[matter][i],kV[i][j]) );
      }
      if(norm(UV[matter][mu][j])>norm(UV[matter][e][j]) ){
	AV[i][j][e ]=real( UV[matter][mu][j]*sqrt(re2 ) / C<e ,mu>(VfVac[matter][i],kV[i][j]) );
	AV[i][j][mu]=real( UV[matter][mu][j]*sqrt(rmu2) / C<mu,mu>(VfVac[matter][i],kV[i][j]) );
      }
    }
  }
  return AV;
} 

//===================//
// Vacuum Potentials //
//===================//
array<array<double,NF>,NE> set_kV(const array<double,NE>& E){
  assert(NF==2);
  array<array<double,NF>,NE> kV;
  for(int i=0;i<NE;i++){
    kV[i][0] = (m1*m1       ) * cgs::constants::c4 /2./E[i];
    kV[i][1] = (m1*m1 + dm21) * cgs::constants::c4 /2./E[i];
  }
  return kV;
}




#endif
