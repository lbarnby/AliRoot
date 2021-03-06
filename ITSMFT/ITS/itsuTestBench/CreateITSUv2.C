#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TSystem.h>
#include <TMath.h>
#endif

//---------------------------------------
double radii2Turbo(double rMin,double rMid,double rMax, double sensW)
{
  // compute turbo angle from radii and sensor width
  return TMath::ASin((rMax*rMax-rMin*rMin)/(2*rMid*sensW))*TMath::RadToDeg();
}

double radii2Phi(double rMin,double rMid,double rMax, double sensW)
{
  // compute phi coverage
  return 2*TMath::ACos((rMax+rMin)*
		       (rMid*rMid+rMin*rMax-sensW*sensW/4.)/
		       (4.*rMid*rMax*rMin));
}

void CreateITSUv2()
{
  //
  gSystem->Load("libITSUpgradeBase");
  gSystem->Load("libITSUpgradeSim");
  //
  // build ITS upgrade detector
  // sensitive area 13x15mm (X,Z) with 20x20 micron pitch, 2mm dead zone on readout side and 50 micron guardring
  const double kSensThick = 18e-4;
  const double kPitchX = 20e-4;
  const double kPitchZ = 20e-4;
  const int    kNRow   = 650; 
  const int    kNCol   = 1500;
  const double kSiThickIB = 50e-4;
  const double kSiThickOB = 50e-4;
  //  const double kSensThick = 120e-4;   // -> sensor Si thickness
  //
  const double kReadOutEdge = 0.2;   // width of the readout edge (passive bottom)
  const double kGuardRing   = 50e-4; // width of passive area on left/right/top of the sensor
  //
  const int kNLr = 7;
  const int kNLrInner = 3;
  const int kBuildLevel = 0;
  enum {kRmn,kRmd,kRmx,kNModPerStave,kPhi0,kNStave,kNPar};
  // Radii are from last TDR (ALICE-TDR-017.pdf Tab. 1.1, rMid is mean value)
  const double tdr5dat[kNLr][kNPar] = { 
    {2.24, 2.34, 2.67,  9., 16.42, 12}, // for each inner layer: rMin,rMid,rMax,NChip/Stave, phi0, nStaves
    {3.01, 3.15, 3.46,  9., 12.18, 16},
    {3.78, 3.93, 4.21,  9.,  9.55, 20},
    {-1,  19.6 ,   -1,  4.,  0.  , 24},  // for others: -, rMid, -, NMod/HStave, phi0, nStaves // 24 was 49
    {-1,  24.55, -1,    4.,  0.  , 30},  // 30 was 61
    {-1,  34.39, -1,    7.,  0.  , 42},  // 42 was 88
    {-1,  39.34, -1,    7.,  0.  , 48}   // 48 was 100
  };
  const int nChipsPerModule = 7; // For OB: how many chips in a row

  // create segmentations:
  AliITSMFTSegmentationPix* seg0 = new AliITSMFTSegmentationPix(0,        // segID (0:9)
							    1,  // chips per module
							    kNCol,    // ncols (total for module)
							    kNRow,    // nrows
							    kPitchX,  // default row pitch in cm
							    kPitchZ,  // default col pitch in cm
							    kSensThick,  // sensor thickness in cm
							    -1,     // no special left col between chips
							    -1,     // no special right col between chips
							    kGuardRing, // left
							    kGuardRing, // right
							    kGuardRing, // top
							    kReadOutEdge  // bottom
							    );    // see AliITSMFTSegmentationPix.h for extra options
  seg0->Store(AliITSUGeomTGeo::GetITSsegmentationFileName());
  //
  seg0->Print();
  //
  double dzLr,rLr,phi0,turbo;
  int nStaveLr,nModPerStaveLr,idLr;
  //
  AliITSUv2 *ITS  = new AliITSUv2("ITS Upgrade",kNLr);
  ITS->SetStaveModelIB(AliITSUv2::kIBModel4);
  ITS->SetStaveModelOB(AliITSUv2::kOBModel2);
  //
  const int kNWrapVol = 3;
  const double wrpRMin[kNWrapVol]  = { 2.1, 15.0, 32.0};
  const double wrpRMax[kNWrapVol]  = {14.0, 30.0, 46.0};
  const double wrpZSpan[kNWrapVol] = {70., 95., 200.};
  //
  ITS->SetNWrapVolumes(kNWrapVol); // define wrapper volumes for layers
  for (int iw=0;iw<kNWrapVol;iw++) ITS->DefineWrapVolume(iw,wrpRMin[iw],wrpRMax[iw],wrpZSpan[iw]);
  //
  for (int idLr=0;idLr<kNLr;idLr++) {
    rLr   = tdr5dat[idLr][kRmd];
    phi0  = tdr5dat[idLr][kPhi0]; 
    //
    nStaveLr = TMath::Nint(tdr5dat[idLr][kNStave]);
    nModPerStaveLr =  TMath::Nint(tdr5dat[idLr][kNModPerStave]);
    int nChipsPerStaveLr = nModPerStaveLr;
    //
    if (idLr>=kNLrInner) {
      nChipsPerStaveLr *= nChipsPerModule;
      ITS->DefineLayer(idLr, phi0, rLr, nChipsPerStaveLr*seg0->Dz(), nStaveLr, nModPerStaveLr, 
		       kSiThickOB, seg0->Dy(), seg0->GetChipTypeID(),kBuildLevel);
//      ITS->AddGammaConversionRods(diameter,number_of_rods);
//                                   [cm]       [integer]
        ITS->AddGammaConversionRods(0.1,1); // Example
    } else {
      turbo = radii2Turbo(tdr5dat[idLr][kRmn],rLr,tdr5dat[idLr][kRmx],seg0->Dx());	
      ITS->DefineLayerTurbo(idLr, phi0, rLr, nChipsPerStaveLr*seg0->Dz(), nStaveLr, nChipsPerStaveLr, 
			    seg0->Dx(), turbo, kSiThickIB, seg0->Dy(), seg0->GetChipTypeID(),kBuildLevel);
    }

  }

}
