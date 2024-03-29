#ifndef EVOLVE_H
#define EVOLVE_H
#include <cmath>
#include <complex>

void evolve_oscillations(State& s,
		const State& s0,
		const State& sBlockStart,
		const double r_end,
		double& dr,
		const Profile& profile,
		const double accuracy,
		const double increase){
	array<array<array<array<array<double,NY>,NS>,NE>,NM>,NRK> dYdr;

	bool finish = false;
	double old_dr;
	do{
		old_dr = dr;
		if(s.r+dr>r_end){
			dr=r_end-s.r;
			finish=true;
		}

		// beginning of RK section
		const State sReset = s;
		bool repeat = false;
		double maxerror = 0;
		do{
			repeat=false;
			for(int k=0;k<=NRK-1;k++){
				s = sReset;
				s.r +=AA[k]* dr;
				for(state m = matter; m <= antimatter; m++)
					for(int i=0;i<=NE-1;i++)
						for(solution x=msw;x<=si;x++)
							for(int j=0;j<=NY-1;j++)
								for(int l=0;l<=k-1;l++)
									s.Y[m][i][x][j] += BB[k][l] * dYdr[l][m][i][x][j] * dr;

				s.update_background(profile);
				s.update_potential(profile,s0);
				dYdr[k] = Koscillate(s);
			}

			// increment all quantities and update C and A arrays
			s = sReset;
			s.r= sReset.r + dr;
			maxerror=0.;
			for(state m=matter;m<=antimatter;m++){
				for(int i=0;i<=NE-1;i++){
					for(solution x=msw;x<=si;x++){
						for(int j=0;j<=NY-1;j++){
							double Yerror = 0.;
							for(int k=0;k<=NRK-1;k++){
								assert(CC[k] == CC[k]);
								s.Y[m][i][x][j] += CC[k] * dYdr[k][m][i][x][j] * dr;
								Yerror += (CC[k]-DD[k]) * dYdr[k][m][i][x][j] * dr;
							}
							maxerror = max(maxerror, fabs(Yerror));
						}
					}
				}
			}
			s.update_background(profile);
			s.update_potential(profile,s0);

			// decide whether to accept step, if not adjust step size
			if(maxerror>accuracy){
				dr *= 0.9 * pow(accuracy/maxerror, 1./(NRKOrder-1.));
				if(dr > 4.*s.r*numeric_limits<double>::epsilon()) repeat=true;
			}

			// reset integration variables to those at beginning of step
			if(repeat==true) s = sReset;

		}while(repeat==true); // end of RK section

		// adjust step size based on RK error
		// could be moved up to RK section but better left here
		// in case adjustments are necessary based on new S matrices
		dr = min(dr*pow(accuracy/maxerror,1./max(1,NRKOrder)),increase*dr);
		double drmin = 4.*s.r*numeric_limits<double>::epsilon();
		dr = max(dr,drmin);
	} while(!finish);
	dr = old_dr; // reset dr to last one so its not controlled by block size

	// accumulate S and reset variables
	// need sBeforeBlock because we tack on to the end of Scumulative
	s.accumulate_S(dr, sBlockStart, s0);
}


void evolve_interactions(State& s,
		const State& sBlockStart,
		const double r_end,
		double& dr,
		const Profile& profile,
		const double accuracy,
		const double increase,
		const int do_interact_rotation,
		double& impact){

	// save old fmatrix
	array<array<MATRIX<complex<double>,NF,NF>,NE>,NM> old_fmatrixf = s.fmatrixf;

	// RK quantities
	array<array<array<MATRIX<complex<double>,NF,NF>,NE>,NM>,NRK> dfdr;

	// let neutrinos interact
	bool finish = false;
	double old_dr;
	do{
		old_dr = dr;
		if(s.r+dr>r_end){
			dr=r_end-s.r;
			finish=true;
		}

		// beginning of RK section
		const State sReset = s;
		bool repeat = false;
		double maxerror = 0;
		do{
			repeat=false;
			for(int k=0;k<=NRK-1;k++){
				s = sReset;
				s.r +=AA[k]* dr;

				for(state m = matter; m <= antimatter; m++)
					for(int i=0;i<=NE-1;i++)
						for(int l=0;l<=k-1;l++)
							s.fmatrixf[m][i] += dfdr[l][m][i] * BB[k][l] * dr;

				s.update_background(profile);
				eas.update(s.rho, s.T, s.Ye);
				dfdr[k] = Kinteract(s,sBlockStart,profile);
			}

			// increment all quantities and update C and A arrays
			s = sReset;
			s.r= sReset.r + dr;
			maxerror=0.;
			for(state m=matter;m<=antimatter;m++){
				for(int i=0;i<=NE-1;i++){
					MATRIX<complex<double>,NF,NF> ferror;
					for(int k=0;k<=NRK-1;k++){
						assert(CC[k] == CC[k]);
						s.fmatrixf[m][i] += dfdr[k][m][i] * CC[k] * dr;
						ferror += dfdr[k][m][i] * (CC[k]-DD[k]) * dr;
					}
					for(int f1=e; f1<=mu; f1++){
					  if(real(s.fmatrixf[m][i][f1][f1])<0. or real(s.fmatrixf[m][i][f1][f1])>1.){
					    maxerror = 1.;
					  }
					  for(int f2=e; f2<=mu; f2++){
					    maxerror = max(maxerror, std::abs(ferror[f1][f2]));
					  }
					}
				}
			}
			s.update_background(profile);

			// decide whether to accept step, if not adjust step size
			if(maxerror>accuracy){
			        dr /= increase;
				if(dr > 4.*s.r*numeric_limits<double>::epsilon()) repeat=true;
			}

			// reset integration variables to those at beginning of step
			if(repeat==true) s = sReset;

		}while(repeat==true); // end of RK section

		// adjust step size based on RK error
		// could be moved up to RK section but better left here
		// in case adjustments are necessary based on new S matrices
		dr = min(dr*pow(accuracy/maxerror,1./max(1,NRKOrder)),increase*dr);
		double drmin = 4.*s.r*numeric_limits<double>::epsilon();
		dr = max(dr,drmin);
	} while(!finish);

	// reset to last dr so its not determined by block size
	dr = old_dr;

	// loop through getting impact
	impact = 0;
        #pragma omp parallel for reduction(max:impact) collapse(4)
	for(int i=0; i<NE; i++)
	  for(int m=matter; m<=antimatter; m++)
	    for(int f1=e; f1<=mu; f1++)
	      for(int f2=e; f2<=mu; f2++)
		impact = max(impact, std::abs(s.fmatrixf[m][i][f1][f2] - old_fmatrixf[m][i][f1][f2]));

	// loop through getting rotation matrices
	if(do_interact_rotation)
          #pragma omp parallel for collapse(2)
	  for(int i=0; i<NE; i++){
		for(int m=matter; m<=antimatter; m++){
			array<double,4> hold = pauli_decompose(old_fmatrixf[m][i]);
			array<double,4> hnew = pauli_decompose(  s.fmatrixf[m][i]);

			// get the theta and phi contribution
			double oldmag2   = hold[0]*hold[0] + hold[1]*hold[1] + hold[2]*hold[2];
			double newmag2   = hnew[0]*hnew[0] + hnew[1]*hnew[1] + hnew[2]*hnew[2];
			double oldmag = sqrt(oldmag2);
			double newmag = sqrt(newmag2);
			if(oldmag2==0 or newmag2==0){
				continue;
			}
			else for(unsigned i=0; i<3; i++){ // to prevent problems near float minval
			    hold[i] /= oldmag;
			    hnew[i] /= newmag;
			}
			s.dtheta_dr_interact[i][m] = (acos(hnew[2]) - acos(hold[2])) / dr;
			s.dphi_dr_interact[i][m] = (atan2(hnew[1],hnew[0]) - atan2(hold[1],hold[0])) / dr;

			// get the axis of rotation
			// lrot = hold x hnew
			// alpha = angle between hold and hnew
			// http://www.vcpc.univie.ac.at/~ian/hotlist/qc/talks/bloch-sphere-rotations.pdf
			double lrot[3];
			lrot[0] =   hold[1]*hnew[2] - hold[2]*hnew[1] ;
			lrot[1] = -(hold[0]*hnew[2] - hold[2]*hnew[0]);
			lrot[2] =   hold[0]*hnew[1] - hold[1]*hnew[0] ;
			double lmag2 = lrot[0]*lrot[0] + lrot[1]*lrot[1] + lrot[2]*lrot[2];
			double lmag = sqrt(lmag2);
			double sinalpha = lmag;
			if(lmag > 0)
				for(unsigned i=0; i<3; i++) lrot[i] /= lmag;
			else continue;

			// get the rotation operator in the flavor basis
			double alpha = asin(sinalpha);
			array<complex<double>,4> Rcoeff;
			for(int i=0; i<3; i++) Rcoeff[i] = -I * sin(alpha/2.) * lrot[i];
			Rcoeff[3] = cos(alpha/2.);

			MATRIX<complex<double>,NF,NF> R = pauli_reconstruct(Rcoeff);
			assert( abs( Trace( R*Adjoint(R)) - (double)NF) < 1e-5);

			// apply to Scumulative
			s.Scumulative[m][i] = Adjoint(s.UU[m][i]) * R * s.UU[m][i] * s.Scumulative[m][i];

			// check that it makes sense
			assert( abs( Trace( s.Scumulative[m][i]*Adjoint(s.Scumulative[m][i])) - (double)NF) < 1e-5);

		}
	}
}
#endif
